package ru.itskekoff.j2c.translator.processor.cpp.impl.stack;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.IntInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class PushCompiler extends BaseProcessor {
    public PushCompiler() {
        super(BIPUSH, SIPUSH);
    }

    @Override
    public void translate(ClassContext classContext, AbstractInsnNode insnNode, MethodNode method) {
        String val = String.valueOf(((IntInsnNode) insnNode).operand);
        classContext.output().pushMethodLine("cstack%d.i = (jint) %s;".formatted(classContext.getStackPointer().peek(), val));
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer + 1;
    }
}
