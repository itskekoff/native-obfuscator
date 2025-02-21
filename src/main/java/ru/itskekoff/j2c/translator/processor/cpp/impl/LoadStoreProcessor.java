package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.VarInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class LoadStoreProcessor extends BaseProcessor {

    public LoadStoreProcessor() {
        super(ILOAD, FLOAD, ALOAD, LLOAD, DLOAD, ISTORE, FSTORE, ASTORE, LSTORE, DSTORE);
    }

    @Override
    public void translate(ClassContext classContext, AbstractInsnNode insnNode, MethodNode method) {
        switch (insnNode.getOpcode()) {
            case ILOAD -> {
                classContext.output().pushMethodLine("cstack%s.i = clocal%s.i;".formatted(classContext.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                classContext.getStackPointer().push();
            }
            case FLOAD -> {
                classContext.output().pushMethodLine("cstack%s.f = clocal%s.f;".formatted(classContext.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                classContext.getStackPointer().push();
            }
            case ALOAD -> {
                classContext.output().pushMethodLine("cstack%s.l = clocal%s.l;".formatted(classContext.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                classContext.getStackPointer().push();
            }
            case LLOAD -> {
                classContext.output().pushMethodLine("cstack%s.j = clocal%s.j;".formatted(classContext.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                classContext.getStackPointer().push(2);
            }
            case DLOAD -> {
                classContext.output().pushMethodLine("cstack%s.d = clocal%s.d;".formatted(classContext.getStackPointer().peek(), ((VarInsnNode) insnNode).var));
                classContext.getStackPointer().push(2);
            }
            case ISTORE -> {
                classContext.getStackPointer().pop();
                classContext.output().pushMethodLine("clocal%s.i = cstack%s.i;".formatted(((VarInsnNode) insnNode).var, classContext.getStackPointer().peek()));
            }
            case FSTORE -> {
                classContext.getStackPointer().pop();
                classContext.output().pushMethodLine("clocal%s.f = cstack%s.f;".formatted(((VarInsnNode) insnNode).var, classContext.getStackPointer().peek()));
            }
            case ASTORE -> {
                classContext.output().pushMethodLine("clocal%s.l = cstack%s.l;".formatted(((VarInsnNode) insnNode).var, classContext.getStackPointer().peek() - 1));
                classContext.getStackPointer().pop();
            }
            case LSTORE -> {
                classContext.getStackPointer().pop(2);
                classContext.output().pushMethodLine("clocal%s.j = cstack%s.j;".formatted(((VarInsnNode) insnNode).var, classContext.getStackPointer().peek()));
            }
            case DSTORE -> {
                classContext.getStackPointer().pop(2);
                classContext.output().pushMethodLine("clocal%s.d = cstack%s.d;".formatted(((VarInsnNode) insnNode).var, classContext.getStackPointer().peek()));
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
