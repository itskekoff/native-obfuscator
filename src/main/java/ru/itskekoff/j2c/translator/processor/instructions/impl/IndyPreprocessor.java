package ru.itskekoff.j2c.translator.processor.instructions.impl;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.TranslatorMain;
import ru.itskekoff.j2c.translator.processor.instructions.interfaces.InstructionProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.MainJarProcessor;
import ru.itskekoff.j2c.translator.utils.BaseUtils;
import ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.objectweb.asm.Opcodes.*;

public class IndyPreprocessor implements InstructionProcessor {
    private static final List<MethodNode> methodNodes = new ArrayList<>();
    private static int id = 0;

    private static String getName() {
        return "$hello%s_proxy%s".formatted((id++), id);
    }

    private static void processIndy(ClassNode classNode, MethodNode methodNode,
                                    InvokeDynamicInsnNode invokeDynamicInsnNode) {
        MethodNode proxyMethod = new MethodNode(ACC_PUBLIC | ACC_STATIC, getName(), invokeDynamicInsnNode.desc, null, null);
        InsnList instructions = new InsnList();

        Type returnType = Type.getReturnType(invokeDynamicInsnNode.desc);


        Type[] args = Type.getArgumentTypes(invokeDynamicInsnNode.desc);
        int pos = 0;

        for (int i = 0; i < args.length; i++) {
            int opcode = Opcodes.ALOAD;
            boolean isArray = args[i].getDescriptor().startsWith("[");
            boolean isWide = args[i].getSort() == Type.LONG || args[i].getSort() == Type.DOUBLE;

            int varIndex = pos;
            if (!isArray && isWide) {
                pos++;
            }

            switch (args[i].getSort()) {
                case Type.LONG:
                    opcode = isArray ? Opcodes.ALOAD : Opcodes.LLOAD;
                    break;
                case Type.FLOAT:
                    opcode = isArray ? Opcodes.ALOAD : Opcodes.FLOAD;
                    break;
                case Type.DOUBLE:
                    opcode = isArray ? Opcodes.ALOAD : Opcodes.DLOAD;
                    break;
                case Type.BOOLEAN:
                case Type.BYTE:
                case Type.SHORT:
                case Type.CHAR:
                case Type.INT:
                    opcode = isArray ? Opcodes.ALOAD : Opcodes.ILOAD;
                    break;
                default:
                    break;
            }

            instructions.add(new VarInsnNode(opcode, varIndex));

//            pos += isWide ? 2 : 1;
            pos+=1;
        }

        instructions.add(new InvokeDynamicInsnNode(invokeDynamicInsnNode.name, invokeDynamicInsnNode.desc, invokeDynamicInsnNode.bsm, invokeDynamicInsnNode.bsmArgs));

        int opcode = ARETURN;
        switch (returnType.getSort()){
            case Type.BOOLEAN:
            case Type.BYTE:
            case Type.SHORT:
            case Type.CHAR:
            case Type.INT:
                opcode = IRETURN;
                break;
            case Type.LONG:
                opcode = LRETURN;
                break;
            case Type.FLOAT:
                opcode = FRETURN;
                break;
            case Type.DOUBLE:
                opcode = DRETURN;
                break;
            default:
                break;
        }
        instructions.add(new InsnNode(opcode));

        proxyMethod.instructions = instructions;
        methodNode.instructions.set(invokeDynamicInsnNode, new MethodInsnNode(Opcodes.INVOKESTATIC, classNode.name, proxyMethod.name, invokeDynamicInsnNode.desc, false));
        methodNodes.add(proxyMethod);
    }


    @Override
    public void process(ClassNode classNode) {
        if (!BaseUtils.hasFlag(classNode.access, Opcodes.ACC_INTERFACE)) {
            MainJarProcessor jarProcessor = TranslatorMain.getProcessor();
            classNode.methods.stream()
                    .filter(methodNode -> ClassFilter.shouldProcess(methodNode) || methodNode.name.contains("linit"))
                    .filter(methodNode -> jarProcessor.getFilter().shouldProcess(classNode, methodNode))
                    .forEach(methodNode -> Arrays.stream(methodNode.instructions.toArray())
                            .filter(abstractInsnNode -> abstractInsnNode instanceof InvokeDynamicInsnNode)
                            .forEach(abstractInsnNode -> processIndy(classNode, methodNode, (InvokeDynamicInsnNode) abstractInsnNode)));
        }
        classNode.methods.addAll(methodNodes);
    }

}
