package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.TypeInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class NewProcessor extends BaseProcessor {

    public NewProcessor() {
        super(NEW);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof TypeInsnNode node) {

            if (context.notClinit(method)) {
                context.output().pushMethodLine("if (jobject obj = env->AllocObject(classes[%s].applyDecryption())) { cstack%s.l = obj; }"
                        .formatted(context.output().pushJavaClass(node.desc), context.getStackPointer().peek()));
            } else {
                context.output().pushMethodLine("if (jobject obj = env->AllocObject(env->FindClass(\"%s\"))) { cstack%s.l = obj; }"
                        .formatted(node.desc, context.getStackPointer().peek()));
            }

        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer + 1;
    }
}
