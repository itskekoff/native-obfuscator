package ru.itskekoff.j2c.translator.processor.cpp.impl.condition;

import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class SwitchProcessor extends BaseProcessor {
    private int keyIndex = 0;

    public SwitchProcessor() {
        super(LOOKUPSWITCH, TABLESWITCH);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof LookupSwitchInsnNode lookupSwitchInsn) {
            processSwitch(context, lookupSwitchInsn.keys, lookupSwitchInsn.labels, lookupSwitchInsn.dflt);
        } else if (insnNode instanceof TableSwitchInsnNode tableSwitchInsn) {
            List<Integer> keys = IntStream.rangeClosed(tableSwitchInsn.min, tableSwitchInsn.max)
                    .boxed()
                    .collect(Collectors.toList());
            processSwitch(context, keys, tableSwitchInsn.labels, tableSwitchInsn.dflt);
        }
    }

    private void processSwitch(MethodContext writer, List<Integer> keys, List<LabelNode> labels, LabelNode defaultLabel) {
        MethodContext.ContextBuilder contextBuilder = writer.output();
        contextBuilder.pushMethodLine("switch (cstack%s.i) {".formatted(writer.getStackPointer().peek() - 1));

        for (int i = 0; i < keys.size(); i++) {
            int key = keys.get(i);
            if (i < labels.size()) {
                LabelNode labelNode = labels.get(i);
                String label = writer.getLabelPool().getName(labelNode.getLabel());
                contextBuilder.pushLineWithTab(2, "case " + key + ": goto " + label + "; break;");
            }
        }

        String defaultLabelName = writer.getLabelPool().getName(defaultLabel.getLabel());
        contextBuilder.pushLineWithTab(2, "default: goto " + defaultLabelName + "; break;");
        contextBuilder.pushMethodLine("}");
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer - 1;
    }
}
