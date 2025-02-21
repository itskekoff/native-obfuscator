package ru.itskekoff.j2c.translator.processor.cpp.impl.condition;


import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.JumpInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class NullConditionProcessor extends BaseProcessor {
    public NullConditionProcessor() {
        super(IFNULL, IFNONNULL);
    }

    @Override
    public void translate(ClassContext classContext, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof JumpInsnNode) {
            classContext.output().pushMethodLine("if (%senv->IsSameObject(cstack%s.l, nullptr)) goto %s;"
                    .formatted(insnNode.getOpcode() == IFNULL ? "" : "!",
                            classContext.getStackPointer().peek() - 1,
                            classContext.getLabelPool().getName(((JumpInsnNode) insnNode).label.getLabel())
                    ));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer - 1;
    }
}
