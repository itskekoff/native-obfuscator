package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class MutexProcessor extends BaseProcessor {

    public MutexProcessor() {
        super(MONITOREXIT, MONITORENTER);
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        switch (insn.getOpcode()) {
            case MONITORENTER ->
                    writer.output().pushMethodLine("env->MonitorEnter(cstack%s.l);".formatted(writer.getStackPointer().peek()));
            case MONITOREXIT ->
                    writer.output().pushMethodLine("env->MonitorExit(cstack%s.l);".formatted(writer.getStackPointer().peek()));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer - 1;
    }
}
