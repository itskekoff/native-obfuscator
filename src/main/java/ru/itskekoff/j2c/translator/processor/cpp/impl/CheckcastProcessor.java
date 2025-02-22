package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.TypeInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class CheckcastProcessor extends BaseProcessor {

    public CheckcastProcessor() {
        super(CHECKCAST);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {

        if (context.notClinit(method)) {

            int classId = context.output().pushJavaClass(((TypeInsnNode) insn).desc);

            context.output().pushMethodLine("if (cstack%s.l != nullptr && !env->IsInstanceOf(cstack%s.l, classes[%s].applyDecryption())) {}"
                    .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1, classId));

        } else {

            context.output().pushMethodLine("if (cstack%s.l != nullptr && !env->IsInstanceOf(cstack%s.l, env->FindClass(\"%s\"))) {}"
                    .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1, ((TypeInsnNode) insn).desc));

        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
