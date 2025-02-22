package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceSnippetGenerator;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class InvokeProcessor extends BaseProcessor {

    public InvokeProcessor() {
        super(INVOKEINTERFACE, INVOKESPECIAL, INVOKESTATIC, INVOKEVIRTUAL);
    }

    @Override
    public void translate(MethodContext classContext, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof MethodInsnNode mh) {
            Type[] args = Type.getArgumentTypes(mh.desc);
            List<Integer> argOffsets = new ArrayList<>();

            int stackOffset = classContext.getStackPointer().peek();
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

            int invokeStackPointer = stackOffset - objectOffset;

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
            }

            if (insnNode.getOpcode() == INVOKEVIRTUAL || insnNode.getOpcode() == INVOKEINTERFACE) {
                String returnType = Type.getReturnType(mh.desc).getDescriptor();
                switch (returnType) {
                    case "V" -> classContext.output().pushMethodLine("env->CallVoidMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "Z" ->
                            classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallBooleanMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "C" ->
                            classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallCharMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "B" ->
                            classContext.output().pushMethodLine("cstack%s.b = (jint) env->CallByteMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "S" ->
                            classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallShortMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "I" ->
                            classContext.output().pushMethodLine("cstack%s.i = env->CallIntMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "F" ->
                            classContext.output().pushMethodLine("cstack%s.f = env->CallFloatMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "J" ->
                            classContext.output().pushMethodLine("cstack%s.j = env->CallLongMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "D" ->
                            classContext.output().pushMethodLine("cstack%s.d = env->CallDoubleMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    default ->
                            classContext.output().pushMethodLine("cstack%s.l = env->CallObjectMethod(cstack%s.l, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                }
            }
            if (insnNode.getOpcode() == INVOKESTATIC) {
                String returnType = Type.getReturnType(mh.desc).getDescriptor();

                switch (returnType) {
                    case "V" -> classContext.output().pushMethodLine("env->CallStaticVoidMethod(%s, %s%s);"
                            .formatted(ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "Z" -> classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticBooleanMethod(%s, %s%s);"
                                .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "C" -> classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticCharMethod(%s, %s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "B" -> classContext.output().pushMethodLine("cstack%s.b = (jint) env->CallStaticByteMethod(%s,%s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "S" -> classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallStaticShortMethod(%s, %s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "I" -> classContext.output().pushMethodLine("cstack%s.i = env->CallStaticIntMethod(%s, %s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "F" -> classContext.output().pushMethodLine("cstack%s.f = env->CallStaticFloatMethod(%s, %s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "J" -> classContext.output().pushMethodLine("cstack%s.j = env->CallStaticLongMethod(%s, %s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    case "D" -> classContext.output().pushMethodLine("cstack%s.d = env->CallStaticDoubleMethod(%s, %s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                    default -> classContext.output().pushMethodLine("cstack%s.l = env->CallStaticObjectMethod(%s, %s%s);"
                            .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaMethodReference(classContext, method, mh, true), arg4Call));
                }
            }
            if (insnNode.getOpcode() == INVOKESPECIAL) {
                String returnType = Type.getReturnType(mh.desc).getDescriptor();

                switch (returnType) {
                    case "V" ->
                            classContext.output().pushMethodLine("env->CallNonvirtualVoidMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "Z" ->
                            classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallNonvirtualBooleanMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "C" ->
                            classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallNonvirtualCharMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "B" ->
                            classContext.output().pushMethodLine("cstack%s.b = (jint) env->CallNonvirtualByteMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "S" ->
                            classContext.output().pushMethodLine("cstack%s.i = (jint) env->CallNonvirtualShortMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "I" ->
                            classContext.output().pushMethodLine("cstack%s.i = env->CallNonvirtualIntMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "F" ->
                            classContext.output().pushMethodLine("cstack%s.f = env->CallNonvirtualFloatMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "J" ->
                            classContext.output().pushMethodLine("cstack%s.j = env->CallNonvirtualLongMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    case "D" ->
                            classContext.output().pushMethodLine("cstack%s.d = env->CallNonvirtualDoubleMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                    default ->
                            classContext.output().pushMethodLine("cstack%s.l = env->CallNonvirtualObjectMethod(cstack%s.l, %s, env->GetMethodID(%s, \"%s\", \"%s\")%s);"
                                    .formatted(invokeStackPointer, invokeStackPointer, ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), ReferenceSnippetGenerator.generateJavaClassReference(classContext, method, mh.owner), mh.name, mh.desc, arg4Call));
                }
            }

            if (insnNode.getOpcode() != INVOKESTATIC) {
                classContext.getStackPointer().pop();
            }
            classContext.getStackPointer().pop(Arrays.stream(Type.getArgumentTypes(mh.desc)).mapToInt(Type::getSize).sum()).push(Type.getReturnType(mh.desc).getSize());
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}