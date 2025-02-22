package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.FieldNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.utils.SnippetGenerator;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class InvokeProcessor extends BaseProcessor {
    private int currentIndex = 0;

    public InvokeProcessor() {
        super(INVOKEINTERFACE, INVOKESPECIAL, INVOKESTATIC, INVOKEVIRTUAL);
    }

    static int id = 0;

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof MethodInsnNode mh) {
            int length = Type.getArgumentTypes(mh.desc).length + 1; // +1 for the 'this' reference
            int invokeStackPointer = context.getStackPointer().peek();
            Type[] args = Type.getArgumentTypes(mh.desc);
//            instructionName += "_" + returnType.getSort();

            StringBuilder argsBuilder = new StringBuilder();
            List<Integer> argOffsets = new ArrayList<>();

            int stackOffset = context.getStackPointer().peek();
            for (Type argType : args) {
                stackOffset -= argType.getSize();
            }
            int argumentOffset = stackOffset;
            for (Type argType : args) {
                argOffsets.add(argumentOffset);
                argumentOffset += argType.getSize();
            }

            boolean isStatic = insnNode.getOpcode() == Opcodes.INVOKESTATIC;
            int objectOffset = isStatic ? 0 : 1;

            invokeStackPointer = stackOffset - objectOffset;

            StringBuilder arg4Call = new StringBuilder();
            for (int i = 0; i < argOffsets.size(); i++) {
                Type arg = args[i];
                StringBuilder appender = new StringBuilder();
                appender.append(", cstack").append(argOffsets.get(i));
                switch (arg.getDescriptor()) {
                    case "I" -> appender.append(".i");
                    case "J" -> appender.append(".j");
                    case "F" -> appender.append(".f");
                    case "D" -> appender.append(".d");
                    default -> appender.append(".l");
                }
                arg4Call.append(appender);
//                argsBuilder.append(", ").append(context.getSnippets().getSnippet("INVOKE_ARG_" + args[i].getSort(),
//                        Util.createMap("index", argOffsets.get(i))));
            }

            // Handle INVOKEVIRTUAL and INVOKEINTERFACE
            int index = currentIndex++;
            String tempClassAddition = "";

            boolean clinit = method.name.contains("$Clinit");

            if (clinit) {
                tempClassAddition += "\njclass tempClass_%d = env->FindClass(\"%s\");".formatted(index, mh.owner);
            } else {
                tempClassAddition = "classes[%s].applyDecryption()".formatted(context.output().pushJavaClass(mh.owner));
            }

            if (clinit) {
                context.output().pushString(tempClassAddition);
                tempClassAddition = "tempClass_%d".formatted(index);
            }

            if (insnNode.getOpcode() == INVOKEVIRTUAL || insnNode.getOpcode() == INVOKEINTERFACE) {
                // String arg4Call = getArg(mh.desc, classContext);
                String returnType = Type.getReturnType(mh.desc).getDescriptor();
//                classContext.output().pushMethodLine("jclass %s = env->FindClass(\"%s\");"
//                        .formatted(tempClassAddition, mh.owner));
                switch (returnType) {
                    case "V" -> {
                        context.output().pushMethodLine("env->CallVoidMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                .formatted(invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    }
                    case "Z" ->
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallBooleanMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "C" ->
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallCharMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "B" ->
                            context.output().pushMethodLine("cstack%s.b = (jint) env->CallByteMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "S" ->
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallShortMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "I" ->
                            context.output().pushMethodLine("cstack%s.i = env->CallIntMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "F" ->
                            context.output().pushMethodLine("cstack%s.f = env->CallFloatMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "J" ->
                            context.output().pushMethodLine("cstack%s.j = env->CallLongMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "D" ->
                            context.output().pushMethodLine("cstack%s.d = env->CallDoubleMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                    default ->
                            context.output().pushMethodLine("cstack%s.l = env->CallObjectMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, mh.name, mh.desc, arg4Call));
                }
            }
            if (insnNode.getOpcode() == INVOKESTATIC) {
                String returnType = Type.getReturnType(mh.desc).getDescriptor();

                switch (returnType) {
                    case "V" -> {
                        if (context.notClinit(method)) {
                            FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);
//                            classContext.output().pushMethodLine("env->CallVoidMethod(cstack%s.l,methods[%s]%s);"
//                                    .formatted(invokeStackPointer, fieldNode.getId(), arg4Call));

                            context.output().pushMethodLine("env->CallStaticVoidMethod(%s, methods[%s]%s);"
                                    .formatted(tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("env->CallStaticVoidMethod(%s, %s%s);"
                                    .formatted(tempClassAddition, SnippetGenerator.getMethodID(true, mh.name, mh.desc, tempClassAddition), arg4Call));
                        }
                    }
                    case "Z" -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);

                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticBooleanMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticBooleanMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }

                    }
                    case "C" -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);
                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticCharMethod(%s, methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticCharMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }
                    }
                    case "B" -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);
                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.b = (jint) env->CallStaticByteMethod(%s,methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.b = (jint) env->CallStaticByteMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }
                        }
                    case "S" -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);
                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticShortMethod(%s, methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticShortMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }
                    }
                    case "I" -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);
                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.i = env->CallStaticIntMethod(%s, methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.i = env->CallStaticIntMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }
                    }
                    case "F" -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);

                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.f = env->CallStaticFloatMethod(%s, methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.f = env->CallStaticFloatMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }

                    }
                    case "J" -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);

                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.j = env->CallStaticLongMethod(%s, methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.j = env->CallStaticLongMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }

                    }
                    case "D" -> {
                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);

                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.d = env->CallStaticDoubleMethod(%s, methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.d = env->CallStaticDoubleMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));

                        }
                    }
                    default -> {

                        FieldNode fieldNode = context.output().allocateOrGetFieldNode(mh.owner, mh.name, mh.desc, true);

                        if (context.notClinit(method)) {
                            context.output().pushMethodLine("cstack%s.l = env->CallStaticObjectMethod(%s, methods[%s]%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, fieldNode.getId(), arg4Call));
                        } else {
                            context.output().pushMethodLine("cstack%s.l = env->CallStaticObjectMethod(%s, env->GetStaticMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                        }

                    }
                }
            }
            if (insnNode.getOpcode() == INVOKESPECIAL) {
                String returnType = Type.getReturnType(mh.desc).getDescriptor();

                switch (returnType) {
                    case "V" ->
                            context.output().pushMethodLine("env->CallNonvirtualVoidMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "Z" ->
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallNonvirtualBooleanMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "C" ->
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallNonvirtualCharMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "B" ->
                            context.output().pushMethodLine("cstack%s.b = (jint) env->CallNonvirtualByteMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "S" ->
                            context.output().pushMethodLine("cstack%s.i = (jint) env->CallNonvirtualShortMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "I" ->
                            context.output().pushMethodLine("cstack%s.i = env->CallNonvirtualIntMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "F" ->
                            context.output().pushMethodLine("cstack%s.f = env->CallNonvirtualFloatMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "J" ->
                            context.output().pushMethodLine("cstack%s.j = env->CallNonvirtualLongMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    case "D" ->
                            context.output().pushMethodLine("cstack%s.d = env->CallNonvirtualDoubleMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                    default ->
                            context.output().pushMethodLine("cstack%s.l = env->CallNonvirtualObjectMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, tempClassAddition, tempClassAddition, mh.name, mh.desc, arg4Call));
                }
            }

            if (insnNode.getOpcode() != INVOKESTATIC) {
                context.getStackPointer().pop();
            }
            context.getStackPointer().pop(Arrays.stream(Type.getArgumentTypes(mh.desc)).mapToInt(Type::getSize).sum()).push(Type.getReturnType(mh.desc).getSize());
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}