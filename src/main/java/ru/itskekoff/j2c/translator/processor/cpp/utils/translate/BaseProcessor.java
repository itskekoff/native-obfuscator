package ru.itskekoff.j2c.translator.processor.cpp.utils.translate;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;

import java.util.*;
import java.util.stream.Collectors;

public abstract class BaseProcessor implements Opcodes {
    public final Set<Integer> supportedOpcodes = new HashSet<>();

    public BaseProcessor(int... opcodes) {
        this.supportedOpcodes.addAll(Arrays.stream(opcodes).boxed().collect(Collectors.toSet()));
    }

    public boolean supports(int opcode) {
        return supportedOpcodes.contains(opcode);
    }

    public abstract void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method);
    public abstract int updateStackPointer(AbstractInsnNode insnNode, int currentPointer);
}