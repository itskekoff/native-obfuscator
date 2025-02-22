package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class MutexProcessor extends BaseProcessor {

    public MutexProcessor() {
        super(MONITOREXIT, MONITORENTER);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        switch (insn.getOpcode()) {
            case MONITORENTER ->
                    context.output().pushMethodLine("env->MonitorEnter(cstack%s.l);".formatted(context.getStackPointer().peek()));
            case MONITOREXIT ->
                    context.output().pushMethodLine("env->MonitorExit(cstack%s.l);".formatted(context.getStackPointer().peek()));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer - 1;
    }
}
