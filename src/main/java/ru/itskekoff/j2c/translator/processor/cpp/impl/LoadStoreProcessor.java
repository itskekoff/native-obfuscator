package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.VarInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class LoadStoreProcessor extends BaseProcessor {

    public LoadStoreProcessor() {
        super(ILOAD, FLOAD, ALOAD, LLOAD, DLOAD, ISTORE, FSTORE, ASTORE, LSTORE, DSTORE);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        switch (insnNode.getOpcode()) {
            case ILOAD -> {
                context.output().pushMethodLine("cstack%s.i = clocal%s.i;".formatted(context.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                context.getStackPointer().push();
            }
            case FLOAD -> {
                context.output().pushMethodLine("cstack%s.f = clocal%s.f;".formatted(context.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                context.getStackPointer().push();
            }
            case ALOAD -> {
                context.output().pushMethodLine("cstack%s.l = clocal%s.l;".formatted(context.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                context.getStackPointer().push();
            }
            case LLOAD -> {
                context.output().pushMethodLine("cstack%s.j = clocal%s.j;".formatted(context.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                context.getStackPointer().push(2);
            }
            case DLOAD -> {
                context.output().pushMethodLine("cstack%s.d = clocal%s.d;".formatted(context.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                context.getStackPointer().push(2);
            }
            case ISTORE -> {
                context.getStackPointer().pop();
                context.output().pushMethodLine("clocal%s.i = cstack%s.i;".formatted(((VarInsnNode) insnNode).var, context.getStackPointer().peek()));
            }
            case FSTORE -> {
                context.getStackPointer().pop();
                context.output().pushMethodLine("clocal%s.f = cstack%s.f;".formatted(((VarInsnNode) insnNode).var, context.getStackPointer().peek()));
            }
            case ASTORE -> {
                context.output().pushMethodLine("clocal%s.l = cstack%s.l;".formatted(((VarInsnNode) insnNode).var, context.getStackPointer().peek() - 1));
                context.getStackPointer().pop();
            }
            case LSTORE -> {
                context.getStackPointer().pop(2);
                context.output().pushMethodLine("clocal%s.j = cstack%s.j;".formatted(((VarInsnNode) insnNode).var, context.getStackPointer().peek()));
            }
            case DSTORE -> {
                context.getStackPointer().pop(2);
                context.output().pushMethodLine("clocal%s.d = cstack%s.d;".formatted(((VarInsnNode) insnNode).var, context.getStackPointer().peek()));
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
