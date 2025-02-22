package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.FieldInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceSnippetGenerator;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.MethodProcessor;

import static ru.itskekoff.j2c.translator.processor.cpp.utils.SnippetGenerator.reinterpretArray;

public class FieldProcessor extends BaseProcessor {
    public static String[] TYPES = new String[]{"Void", "Boolean", "Char", "Byte", "Short", "Int", "Float", "Long", "Double", "Array", "Object", "Object"};

    public FieldProcessor() {
        super(GETFIELD, PUTFIELD, GETSTATIC, PUTSTATIC);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof FieldInsnNode fieldInsnNode) {
            boolean isStatic = insnNode.getOpcode() == GETSTATIC || insnNode.getOpcode() == PUTSTATIC;

            String fieldIdAddition = ReferenceSnippetGenerator.generateJavaFieldReference(context, method, fieldInsnNode, isStatic);

            switch (insnNode.getOpcode()) {
                case GETSTATIC -> {
                    switch (Type.getType(((FieldInsnNode) insnNode).desc).getSort()) {
                        case 1, 2, 4, 5 -> context.output().pushMethodLine("cstack%s.i = %s env->GetStatic%sField(%s, %s);".formatted(
                                context.getStackPointer().peek(),
                                Type.getType(((FieldInsnNode) insnNode).desc).getSort() == 5 ? "(jint)" : "",
                                reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                fieldIdAddition
                        ));
                        case 6 ->
                                context.output().pushMethodLine("cstack%s.f = env->GetStatic%sField(%s, %s);".formatted(
                                        context.getStackPointer().peek(),
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition
                                ));
                        case 7 ->
                                context.output().pushMethodLine("cstack%s.j = env->GetStatic%sField(%s, %s);".formatted(
                                        context.getStackPointer().peek(),
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition
                                ));
                        case 8 ->
                                context.output().pushMethodLine("cstack%s.d = env->GetStatic%sField(%s, %s);".formatted(
                                        context.getStackPointer().peek(),
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition
                                ));
                        case 9, 10, 11 ->
                                context.output().pushMethodLine("cstack%s.l = env->GetStatic%sField(%s, %s);".formatted(
                                        context.getStackPointer().peek(),
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition
                                ));
                    }
                }
                case GETFIELD -> {
                    switch (Type.getType(((FieldInsnNode) insnNode).desc).getSort()) {
                        case 1, 2, 4, 5 -> context.output().pushMethodLine("cstack%s.i = %s env->Get%sField(cstack%s.l, %s);".formatted(
                                context.getStackPointer().peek() - 1,
                                Type.getType(((FieldInsnNode) insnNode).desc).getSort() == 5 ? "(jint)" : "",
                                reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                context.getStackPointer().peek() - 1,
                                fieldIdAddition
                        ));
                        case 6 ->
                                context.output().pushMethodLine("cstack%s.f = env->Get%sField(cstack%s.l, %s);".formatted(
                                        context.getStackPointer().peek() - 1,
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 1,
                                        fieldIdAddition
                                ));
                        case 7 ->
                                context.output().pushMethodLine("cstack%s.j = env->Get%sField(cstack%s.l, %s);".formatted(
                                        context.getStackPointer().peek() - 1,
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 1,
                                        fieldIdAddition
                                ));
                        case 8 ->
                                context.output().pushMethodLine("cstack%s.d = env->Get%sField(cstack%s.l, %s);".formatted(
                                        context.getStackPointer().peek() - 1,
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 1,
                                        fieldIdAddition
                                ));
                        case 9, 10, 11 ->
                                context.output().pushMethodLine("cstack%s.l = env->Get%sField(cstack%s.l, %s);".formatted(
                                        context.getStackPointer().peek() - 1,
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 1,
                                        fieldIdAddition
                                ));
                    }
                }
                case PUTSTATIC -> {
                    switch (Type.getType(((FieldInsnNode) insnNode).desc).getSort()) {
                        case 1, 2, 4, 5 -> context.output().pushMethodLine("env->SetStatic%sField(%s, %s, %s cstack%s.i);".formatted(
                                reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                fieldIdAddition,
                                Type.getType(((FieldInsnNode) insnNode).desc).getSort() == 5 ? "(%s)".formatted(MethodProcessor.CPP_TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]) : "",
                                context.getStackPointer().peek() - 1
                        ));
                        case 6 ->
                                context.output().pushMethodLine("env->SetStatic%sField(%s, %s, cstack%s.f);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 1
                                ));
                        case 7 ->
                                context.output().pushMethodLine("env->SetStatic%sField(%s, %s, cstack%s.j);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 2
                                ));
                        case 8 ->
                                context.output().pushMethodLine("env->SetStatic%sField(%s, %s, cstack%s.d);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 2
                                ));
                        case 9, 10, 11 ->
                                context.output().pushMethodLine("env->SetStatic%sField(%s, %s, cstack%s.l);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        ReferenceSnippetGenerator.generateJavaClassReference(context, method, fieldInsnNode.owner),
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 1
                                ));
                    }
                }
                case PUTFIELD -> {
                    switch (Type.getType(((FieldInsnNode) insnNode).desc).getSort()) {
                        case 1, 2, 4, 5 -> context.output().pushMethodLine("env->Set%sField(cstack%s.l, %s, %s cstack%s.i);".formatted(
                                reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                context.getStackPointer().peek() - 2,
                                fieldIdAddition,
                                Type.getType(((FieldInsnNode) insnNode).desc).getSort() == 5 ? "(%s)".formatted(MethodProcessor.CPP_TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]) : "",
                                context.getStackPointer().peek() - 1
                        ));
                        case 6 ->
                                context.output().pushMethodLine("env->Set%sField(cstack%s.l, %s, cstack%s.f);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 2,
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 1
                                ));
                        case 7 ->
                                context.output().pushMethodLine("env->Set%sField(cstack%s.l, %s, cstack%s.j);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 3,
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 2
                                ));
                        case 8 ->
                                context.output().pushMethodLine("env->Set%sField(cstack%s.l, %s, cstack%s.d);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 3,
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 2
                                ));
                        case 9, 10, 11 ->
                                context.output().pushMethodLine("env->Set%sField(cstack%s.l, %s, cstack%s.l);".formatted(
                                        reinterpretArray(TYPES[Type.getType(((FieldInsnNode) insnNode).desc).getSort()]),
                                        context.getStackPointer().peek() - 2,
                                        fieldIdAddition,
                                        context.getStackPointer().peek() - 1
                                ));
                    }
                }
            }

            if (insnNode.getOpcode() == Opcodes.GETFIELD || insnNode.getOpcode() == Opcodes.PUTFIELD) {
                context.getStackPointer().pop(1);
            }
            if (insnNode.getOpcode() == Opcodes.GETSTATIC || insnNode.getOpcode() == Opcodes.GETFIELD) {
                context.getStackPointer().push(Type.getType(fieldInsnNode.desc).getSize());
            }
            if (insnNode.getOpcode() == Opcodes.PUTSTATIC || insnNode.getOpcode() == Opcodes.PUTFIELD) {
                context.getStackPointer().pop(Type.getType(fieldInsnNode.desc).getSize());
            }
        }
    }


    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}