package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class GotoProcessor extends BaseProcessor {

    public GotoProcessor() {
        super(GOTO);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof JumpInsnNode) {
            context.output().pushMethodLine("goto %s;".formatted(context.getLabelPool().getName(((JumpInsnNode) insn).label.getLabel())));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}