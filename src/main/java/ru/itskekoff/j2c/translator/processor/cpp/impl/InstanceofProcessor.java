package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.TypeInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceSnippetGenerator;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class InstanceofProcessor extends BaseProcessor {

    public InstanceofProcessor() {
        super(INSTANCEOF);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof TypeInsnNode) {
            context.output().pushMethodLine("cstack%s.i = cstack%s.l == nullptr ? false : env->IsInstanceOf(cstack%s.l, %s);"
                    .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1,
                            context.getStackPointer().peek() - 1, ReferenceSnippetGenerator.generateJavaClassReference(context, method, (((TypeInsnNode) insn).desc)))
            );
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
