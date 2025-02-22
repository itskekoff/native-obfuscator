package ru.itskekoff.j2c.translator.processor.cpp.impl.stack;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.IntInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class PushCompiler extends BaseProcessor {
    public PushCompiler() {
        super(BIPUSH, SIPUSH);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        String val = String.valueOf(((IntInsnNode) insnNode).operand);
        context.output().pushMethodLine("cstack%d.i = (jint) %s;".formatted(context.getStackPointer().peek(), val));
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer + 1;
    }
}
