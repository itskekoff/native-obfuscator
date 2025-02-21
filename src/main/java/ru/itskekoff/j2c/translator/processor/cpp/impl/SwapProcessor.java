package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class SwapProcessor extends BaseProcessor {
    public SwapProcessor() {
        super(SWAP);
    }

    @Override
    public void translate(ClassContext classContext, AbstractInsnNode insnNode, MethodNode method) {
        classContext.output().pushMethodLine("std::swap(cstack%d, cstack%d);"
                .formatted(classContext.getStackPointer().peek() - 1, classContext.getStackPointer().peek() - 2));
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
