package ru.itskekoff.j2c.translator.processor.cpp;

import org.objectweb.asm.Label;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.TranslatorMain;
import ru.itskekoff.j2c.translator.configuration.TranslatorConfiguration;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;
import ru.itskekoff.j2c.translator.utils.BaseUtils;
import ru.itskekoff.j2c.translator.processor.cpp.utils.NativeLinker;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.utils.ReflectionUtils;

import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class MethodProcessor {
    private static final List<BaseProcessor> processors = new ArrayList<>();
    public static final String[] CPP_TYPES = new String[]{
            "void", "jboolean", "jchar",
            "jbyte", "jshort", "jint",
            "jfloat", "jlong", "jdouble",
            "jarray", "jobject", "jobject"
    };
    public static final int[] TYPE_TO_STACK = new int[]{1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0};
    public static final int[] STACK_TO_STACK = new int[]{1, 1, 1, 2, 2, 0, 0, 0, 0};

    private static int index = 0;

    static {
        processors.addAll(ReflectionUtils.getClasses("ru.itskekoff.j2c.translator.processor.cpp.impl", BaseProcessor.class));

        var supportedInsns = processors.stream()
                .flatMap(processor -> processor.supportedOpcodes.stream())
                .collect(Collectors.toSet());

        int totalInstructions = BaseUtils.OPCODE_NAME_MAP.size();
        double supportPercentage = (supportedInsns.size() / (double) totalInstructions) * 100;
        TranslatorMain.COMPILE_LOGGER.info("Loaded {} processors. [{}/{}] ({} instructions supported)",
                processors.size(), supportedInsns.size(), totalInstructions, "%.2f%%".formatted(supportPercentage));

        var unsupportedInsns = BaseUtils.OPCODE_NAME_MAP.keySet().stream()
                .filter(opcode -> !supportedInsns.contains(opcode))
                .collect(Collectors.toSet());

        if (!unsupportedInsns.isEmpty()) {
            var unsupportedInsnNames = unsupportedInsns.stream()
                    .map(BaseUtils.OPCODE_NAME_MAP::get)
                    .toList();

            TranslatorMain.COMPILE_LOGGER.warn("Unsupported instructions: {}", unsupportedInsnNames);
        }
    }

    public void process(MethodContext context, MethodNode method, NativeLinker linker) {
        String className = context.getClassNode().name;
        String methodName = BaseUtils.getJNICompatibleName(method.name);
        String nativeName = generateNativeName(className, methodName);

        if (!isClinit(method)) linker.pushMethod(method, nativeName);
        Type[] args = Type.getArgumentTypes(method.desc);
        boolean isStatic = BaseUtils.hasFlag(method.access, Opcodes.ACC_STATIC);

        List<Type> argTypes = new ArrayList<>(Arrays.asList(args));
        List<String> argNames = new ArrayList<>();
        initializeArguments(args, argTypes, argNames, isStatic);
        context.output().pushMethod(method, nativeName, argNames, CPP_TYPES, args, isClinit(method), isStatic);
        context.output().pushMethodLine("// stack count: %d, locals count: %d, try-catches: %d"
                .formatted(method.maxStack, method.maxLocals, method.tryCatchBlocks.size()));

        if (method.maxStack == 0) {
            context.output().pushMethodLine("jvalue cstack_exception = {};");
        } else {
            StringBuilder stackBuilder = new StringBuilder();
            StringBuilder localBuilder = new StringBuilder("jvalue clocal0 = {}");
            initializeLocals(method, argTypes, stackBuilder, localBuilder);
            context.output().pushMethodLine(stackBuilder.toString());
            context.output().pushMethodLine(localBuilder.toString());
            writeLocals(context, argTypes, argNames);
        }

        if (isClinit(method)) {
            linker.end();
            context.output().pushMethodLine(linker.getMethods().toString());
            context.output().pushMethodLine(context.output().getClassReferences());
            context.output().pushMethodLine(context.output().getMethodReferences());
        }

        Map<String, List<CatchInfo>> catchHandlers = new HashMap<>();
        Map<LabelNode, String> catchLabels = new HashMap<>();
        AtomicInteger tryCatchIndex = new AtomicInteger(0);
        for (TryCatchBlockNode tryCatchBlock : method.tryCatchBlocks) {
            String catchLabel = "CATCH_" + tryCatchIndex.get();
            tryCatchIndex.incrementAndGet();
            catchLabels.put(tryCatchBlock.handler, catchLabel);
            String targetLabel = context.getLabelPool().getName(tryCatchBlock.handler.getLabel());
            catchHandlers.computeIfAbsent(catchLabel, _ -> new ArrayList<>())
                    .add(new CatchInfo(tryCatchBlock.type, targetLabel));
        }

        processInstructions(method, context, catchLabels);
        writeCatchHandlers(context, method, catchHandlers);

        finalizeMethod(method, context);
    }

    private boolean isClinit(MethodNode method) {
        return method.name.equals("$Clinit");
    }

    public static String generateNativeName(String className, String methodName) {
        boolean clinit = methodName.contains("Clinit");
        String sanitizedClassName = className.replaceAll("_", "_1").replaceAll("/", "_")
                .replaceAll("\\$", "_00024")
                .replaceAll(" ", "_00020");
        String sanitizedMethodName = methodName.replaceAll("\\$", "_00024")
                .replaceAll(" ", "_00020");

        return sanitizedClassName + "_" + sanitizedMethodName + (clinit ? "" : index++);
    }


    private void initializeArguments(Type[] args, List<Type> argTypes, List<String> argNames, boolean isStatic) {
        if (!isStatic) {
            argTypes.addFirst(Type.getType(Object.class));
        }

        if (!isStatic) argNames.add("obj");
        for (int i = 0; i < args.length; ++i) {
            argNames.add("arg" + i);
        }
    }

    private void initializeLocals(MethodNode method, List<Type> argTypes, StringBuilder stackBuilder, StringBuilder localBuilder) {
        AtomicInteger cLocalIndex = new AtomicInteger(1);
        if (method.maxStack > 0) {
            stackBuilder.append("jvalue ");
            for (int i = 0; i < method.maxStack; i++) {
                stackBuilder.append(String.format("cstack%s = {}", i));
                if (i != method.maxStack - 1) {
                    stackBuilder.append(", ");
                }
            }
        }

        IntStream.range(1, method.maxLocals).forEach(_ ->
                localBuilder.append(", ").append("clocal%d = {}".formatted(cLocalIndex.getAndIncrement())));

        if (argTypes.size() > 1) {
            for (int i = 0; i < argTypes.size(); i++) {
                localBuilder.append(", ").append("clocal%d = {}".formatted(cLocalIndex.getAndIncrement()));
            }
        }

        stackBuilder.append(", cstack_exception = {}");

        stackBuilder.append(";");
        localBuilder.append(";").append("\n");
    }

    private void writeLocals(MethodContext writer, List<Type> argTypes, List<String> argNames) {
        if (!argTypes.isEmpty()) {
            writer.output().pushLine().pushTab();
        }

        int localIndex = 0;
        for (int i = 0; i < argTypes.size(); ++i) {
            Type current = argTypes.get(i);

            String stackType = switch (current.getDescriptor()) {
                case "I" -> ".i";  // jint
                case "J" -> ".j";  // jlong
                case "F" -> ".f";  // jfloat
                case "D" -> ".d";  // jdouble
                case "C" -> ".i";  // jchar
                case "Z" -> ".i";  // jboolean
                case "B" -> ".b";  // jbyte
                case "S" -> ".i";  // jshort
                default -> ".l";   // pointer
            };

            writer.output().pushString("clocal%s%s = ".formatted(localIndex, stackType));
            writer.output().pushString(argNames.get(i)).pushString("; ");
            localIndex += current.getSize();
        }

        if (!argTypes.isEmpty()) {
            writer.output().pushLine();
        }

        argTypes.forEach(t -> writer.getLocals().add(TYPE_TO_STACK[t.getSort()]));
    }

    private void processInstructions(MethodNode method, MethodContext writer, Map<LabelNode, String> catchLabels) {
        if (TranslatorConfiguration.IMP.MAIN.DEBUG_ENABLED) TranslatorMain.COMPILE_LOGGER.info("Processing method {}{}", method.name, method.desc);
        AtomicReference<Label> label = new AtomicReference<>(new Label());
        method.instructions.forEach(insn -> {
            if (insn instanceof LabelNode labelNode) {
                label.set(labelNode.getLabel());
                String labelName = writer.getLabelPool().getName(label.get());
                writer.output().pushLine().pushString("%s:".formatted(labelName));
                if (TranslatorConfiguration.IMP.MAIN.DEBUG_ENABLED)
                    TranslatorMain.LOGGER.info("Label start (name={})", labelName);
                return;
            }

            if (insn.getOpcode() != -1) {
                String opcodeName = BaseUtils.getOpcodeString(insn.getOpcode()).equals("UNKNOWN")
                        ? String.valueOf(insn.getOpcode())
                        : BaseUtils.getOpcodeString(insn.getOpcode());
                writer.output().pushMethodLine("// %s; Stack pointer: %d".formatted(opcodeName, writer.getStackPointer().peek()));
            }

            processors.stream()
                    .filter(compiler -> compiler.supports(insn.getOpcode()))
                    .findFirst()
                    .ifPresentOrElse(
                            compiler -> {
                                int currentPointer = writer.getStackPointer().peek();
                                compiler.translate(writer, insn, method);
                                writer.getStackPointer().set(compiler.updateStackPointer(insn, writer.getStackPointer().peek()));
                                int updatedPointer = writer.getStackPointer().peek();
                                int diff = updatedPointer - currentPointer;
                                if (TranslatorConfiguration.IMP.MAIN.DEBUG_ENABLED)
                                    TranslatorMain.COMPILE_LOGGER.info("Updated pointer from {} to {} (diff: {}) after instruction {} [{}]",
                                            currentPointer, updatedPointer, diff, BaseUtils.getOpcodeString(insn.getOpcode()), insn.getOpcode());
                            },
                            () -> TranslatorMain.COMPILE_LOGGER.error("Unsupported insn {} [{}]",
                                    BaseUtils.getOpcodeString(insn.getOpcode()),
                                    insn.getOpcode())
                    );

            boolean inTryCatchBlock = false;
            String currentCatchLabel = null;

            for (TryCatchBlockNode tryCatchBlock : method.tryCatchBlocks) {
                if (tryCatchBlock.start.getLabel().equals(label.get())) {
                    inTryCatchBlock = true;
                    currentCatchLabel = catchLabels.get(tryCatchBlock.handler);
                    break;
                }
            }

            if (inTryCatchBlock) {
                writer.output().pushMethodLine("if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto %s; }"
                        .formatted(currentCatchLabel));
            } else {
                writer.output().pushMethodLine("if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }");
            }
        });
    }

    private void writeCatchHandlers(MethodContext context, MethodNode method, Map<String, List<CatchInfo>> catchHandlers) {
        catchHandlers.forEach((catchLabel, handlers) -> {
            context.output().pushLine().pushString("%s:".formatted(catchLabel));

            handlers.forEach(handler -> {
                if (handler.exceptionType == null) {
                    context.output().pushMethodLine("goto %s;".formatted(handler.targetLabel));
                } else {

                    if (context.notClinit(method)) {
                        int classId = context.output().pushJavaClass(handler.exceptionType);

                        context.output().pushMethodLine("if (env->IsInstanceOf(cstack_exception.l, classes[%s].applyDecryption())) { env->ExceptionClear(); goto %s; } "
                                .formatted(classId, handler.targetLabel));
                    }
                    else
                        context.output().pushMethodLine("if (env->IsInstanceOf(cstack_exception.l, env->FindClass(\"%s\"))) { env->ExceptionClear(); goto %s; } "
                                .formatted(handler.exceptionType, handler.targetLabel));
                }
            });
        });

        context.output().pushLine().pushString("L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); %s }\n"
                .formatted(Type.getReturnType(method.desc).getSort() != Type.VOID
                        ? "return (%s) 0;".formatted(CPP_TYPES[Type.getReturnType(method.desc).getSort()])
                        : "return;"
                ));
    }

    private void finalizeMethod(MethodNode method, MethodContext writer) {
        writer.output().pushMethodLine(this.getReturn(method));
        writer.output().pushLine().pushString("}");
        writer.getStackPointer().set(0);

        method.instructions.clear();
        method.access |= Opcodes.ACC_NATIVE;
    }

    private String getReturn(MethodNode method) {
        int sort = Type.getReturnType(method.desc).getSort();
        StringBuilder builder = new StringBuilder("return");
        if (sort != Type.VOID) {
            builder.append(" ");
            builder.append("(%s) 0".formatted(CPP_TYPES[sort]));
        }
        builder.append(";");
        return builder.toString();
    }

    static class CatchInfo {
        String exceptionType;
        String targetLabel;

        public CatchInfo(String exceptionType, String targetLabel) {
            this.exceptionType = exceptionType;
            this.targetLabel = targetLabel;
        }
    }
}