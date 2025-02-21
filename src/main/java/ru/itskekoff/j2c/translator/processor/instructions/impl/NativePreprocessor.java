package ru.itskekoff.j2c.translator.processor.instructions.impl;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.processor.instructions.interfaces.InstructionProcessor;
import ru.itskekoff.j2c.translator.utils.BaseUtils;

import static ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor.RETURN;

public class NativePreprocessor implements InstructionProcessor {
    private MethodNode proxyMethod;

    private void processClinit(MethodNode method, ClassNode clazz) {
        proxyMethod.instructions.clear();
        proxyMethod.instructions.insert(method.instructions);
        proxyMethod.visitMaxs(method.maxStack, method.maxLocals);
        method.instructions.clear();
        InsnList list = new InsnList();
        list.add(new MethodInsnNode(Opcodes.INVOKESTATIC, "ru/itskekoff/j2c/loader/Protection", "init", "()V", false));
        list.add(new MethodInsnNode(Opcodes.INVOKESTATIC, clazz.name, "$Clinit", "()V", false));
        list.add(new InsnNode(RETURN));
        method.instructions.insert(list);
    }

    @Override
    public void process(ClassNode clazz) {
        if (!BaseUtils.hasFlag(clazz.access, Opcodes.ACC_INTERFACE)) {
            if (proxyMethod == null) {
                proxyMethod = new MethodNode(Opcodes.ACC_PUBLIC + Opcodes.ACC_STATIC, "$Clinit", "()V", null, new String[0]);
                proxyMethod.instructions.add(new InsnNode(RETURN));
            }
            MethodNode clinitNode = clazz.methods.stream().filter(methodNode -> methodNode.name.equals("<clinit>")).findFirst().get();
            processClinit(clinitNode, clazz);
            clazz.methods.addLast(proxyMethod);
        }
    }
}
