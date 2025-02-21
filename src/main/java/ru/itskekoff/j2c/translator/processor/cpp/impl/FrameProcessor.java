package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.Type;
import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.MethodProcessor;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;


public class FrameProcessor extends BaseProcessor {

    public List<Integer> stacks = new ArrayList<>();

    public FrameProcessor() {
        super(F_NEW);
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof FrameNode frameNode) {
            Consumer<Object> appendLocal = local -> writer.locals.add(
                    local instanceof String || local instanceof LabelNode
                            ? MethodProcessor.TYPE_TO_STACK[Type.OBJECT]
                            : MethodProcessor.STACK_TO_STACK[(int) local]
            );

            Consumer<Object> appendStack = stack -> stacks.add(
                    stack instanceof String || stack instanceof LabelNode
                            ? MethodProcessor.TYPE_TO_STACK[Type.OBJECT]
                            : MethodProcessor.STACK_TO_STACK[(int) stack]
            );

            int frameType = frameNode.type;
            switch (frameType) {
                case F_APPEND:
                    frameNode.local.forEach(appendLocal);
                    stacks.clear();
                    break;

                case F_CHOP:
                    frameNode.local.forEach(_ -> writer.locals.removeLast());
                    stacks.clear();
                    break;

                case F_NEW:
                case F_FULL:
                    writer.locals.clear();
                    stacks.clear();
                    frameNode.local.forEach(appendLocal);
                    frameNode.stack.forEach(appendStack);
                    break;

                case F_SAME:
                    stacks.clear();
                    break;

                case F_SAME1:
                    stacks.clear();
                    appendStack.accept(frameNode.stack.getFirst());
                    break;
            }

            int newStackPointer = 0;
            if (frameType == F_NEW || frameType == F_FULL) {
                newStackPointer = frameNode.stack.stream()
                        .mapToInt(argument -> Math.max(1, argument instanceof Integer
                                ? MethodProcessor.STACK_TO_STACK[(int) argument]
                                : MethodProcessor.TYPE_TO_STACK[Type.OBJECT])).sum();
            } else if (frameType == F_SAME1) {
                newStackPointer = frameNode.stack.stream().limit(1)
                        .mapToInt(argument -> Math.max(1, argument instanceof Integer
                                ? MethodProcessor.STACK_TO_STACK[(int) argument]
                                : MethodProcessor.TYPE_TO_STACK[Type.OBJECT])).sum();
            }
            writer.getStackPointer().set(newStackPointer);
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
