package ru.itskekoff.j2c.translator.processor.instructions.impl;

import org.objectweb.asm.Handle;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.LdcInsnNode;
import ru.itskekoff.j2c.translator.TranslatorMain;
import ru.itskekoff.j2c.translator.processor.instructions.interfaces.InstructionProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.MainJarProcessor;
import ru.itskekoff.j2c.translator.utils.BaseUtils;
import ru.itskekoff.j2c.translator.utils.clazz.MethodHandleUtils;
import ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter;

public class LdcPreprocessor implements InstructionProcessor {
    @Override
    public void process(ClassNode classNode) {
        if (!BaseUtils.hasFlag(classNode.access, Opcodes.ACC_INTERFACE)) {
            MainJarProcessor jarProcessor = TranslatorMain.getProcessor();

            classNode.methods.stream()
                    .filter(ClassFilter::shouldProcess)
                    .filter(methodNode -> jarProcessor.getFilter().shouldProcess(classNode, methodNode))
                    .forEach(methodNode -> {
                        AbstractInsnNode insnNode = methodNode.instructions.getFirst();
                        while (insnNode != null) {
                            if (insnNode instanceof LdcInsnNode ldcInsnNode) {

                                if (ldcInsnNode.cst instanceof Handle) {
                                    methodNode.instructions.insertBefore(ldcInsnNode,
                                            MethodHandleUtils.generateMethodHandleLdcInsn((Handle) ldcInsnNode.cst));
                                    AbstractInsnNode nextInsnNode = insnNode.getNext();
                                    methodNode.instructions.remove(insnNode);
                                    insnNode = nextInsnNode;
                                    continue;
                                }

                                if (ldcInsnNode.cst instanceof Type type) {

                                    if (type.getSort() == Type.METHOD) {
                                        methodNode.instructions.insertBefore(ldcInsnNode,
                                                MethodHandleUtils.generateMethodTypeLdcInsn(type, classNode));
                                        AbstractInsnNode nextInsnNode = insnNode.getNext();
                                        methodNode.instructions.remove(insnNode);
                                        insnNode = nextInsnNode;
                                    }
                                }
                            }

                            insnNode = insnNode.getNext();
                        }
                    });
        }
    }
}
