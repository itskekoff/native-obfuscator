package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class GotoProcessor extends BaseProcessor {

    public GotoProcessor() {
        super(GOTO);
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof JumpInsnNode) {
            writer.output().pushMethodLine("goto %s;".formatted(writer.getLabelPool().getName(((JumpInsnNode) insn).label.getLabel())));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}