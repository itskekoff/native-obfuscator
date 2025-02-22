package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class ThrowProcessor extends BaseProcessor {
    public ThrowProcessor() {
        super(ATHROW);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            if (context.notClinit(method)) {
                int classId = context.output().pushJavaClass(context.getClassNode().name);

                context.output().pushMethodLine("if (cstack%s.l == nullptr) env->ThrowNew(classes[%s].applyDecryption(), \"\"); else env->Throw((jthrowable) cstack%s.l);"
                        .formatted(context.getStackPointer().peek() - 1, classId, context.getStackPointer().peek() - 1));
            } else {

                String fieldAddition = "env->FindClass(\"%s\")".formatted(context.getClassNode().name);

                context.output().pushMethodLine("if (cstack%s.l == nullptr) env->ThrowNew(%s, \"\"); else env->Throw((jthrowable) cstack%s.l);"
                        .formatted(context.getStackPointer().peek() - 1, fieldAddition, context.getStackPointer().peek() - 1));
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer - 1;
    }
}
