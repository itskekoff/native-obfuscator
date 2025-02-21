package ru.itskekoff.j2c.translator.processor.instructions;

import org.objectweb.asm.tree.ClassNode;
import ru.itskekoff.j2c.translator.processor.instructions.impl.IndyPreprocessor;
import ru.itskekoff.j2c.translator.processor.instructions.impl.LdcPreprocessor;
import ru.itskekoff.j2c.translator.processor.instructions.impl.NativePreprocessor;
import ru.itskekoff.j2c.translator.processor.instructions.interfaces.InstructionProcessor;

import java.util.ArrayList;
import java.util.List;

/**
 * @author itskekoff
 * @since 16:24 of 19.02.2025
 */
public class InsnProcessorManager {
    private final static List<InstructionProcessor> PROCESSORS = new ArrayList<>();

    static {
        PROCESSORS.add(new IndyPreprocessor());
        PROCESSORS.add(new LdcPreprocessor());
        PROCESSORS.add(new NativePreprocessor());
    }

    public static void process(ClassNode classNode) {
        PROCESSORS.forEach(processor -> processor.process(classNode));
    }
}
