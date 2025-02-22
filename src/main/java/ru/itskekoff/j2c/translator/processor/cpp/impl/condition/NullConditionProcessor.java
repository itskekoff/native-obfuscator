package ru.itskekoff.j2c.translator.processor.cpp.impl.condition;


import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.JumpInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class NullConditionProcessor extends BaseProcessor {
    public NullConditionProcessor() {
        super(IFNULL, IFNONNULL);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof JumpInsnNode) {
            context.output().pushMethodLine("if (%senv->IsSameObject(cstack%s.l, nullptr)) goto %s;"
                    .formatted(insnNode.getOpcode() == IFNULL ? "" : "!",
                            context.getStackPointer().peek() - 1,
                            context.getLabelPool().getName(((JumpInsnNode) insnNode).label.getLabel())
                    ));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer - 1;
    }
}
