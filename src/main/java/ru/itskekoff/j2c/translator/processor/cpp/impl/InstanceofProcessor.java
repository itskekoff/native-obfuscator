package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.TypeInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class InstanceofProcessor extends BaseProcessor {

    public InstanceofProcessor() {
        super(INSTANCEOF);
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof TypeInsnNode) {

            if (writer.isNotClinit(method)) {
                writer.output().pushMethodLine("cstack%s.i = cstack%s.l == nullptr ? false : env->IsInstanceOf(cstack%s.l, classes[%s].applyDecryption());"
                        .formatted(writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 1,
                                writer.getStackPointer().peek() - 1, writer.output().pushJavaClass(((TypeInsnNode) insn).desc))
                );
            } else
                writer.output().pushMethodLine("cstack%s.i = cstack%s.l == nullptr ? false : env->IsInstanceOf(cstack%s.l, env->FindClass(\"%s\"));"
                        .formatted(writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 1,
                                writer.getStackPointer().peek() - 1, ((TypeInsnNode) insn).desc)
                );

        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
