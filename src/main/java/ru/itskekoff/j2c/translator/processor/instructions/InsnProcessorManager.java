package ru.itskekoff.j2c.translator.processor.instructions;

import org.objectweb.asm.tree.ClassNode;
import ru.itskekoff.j2c.translator.TranslatorMain;
import ru.itskekoff.j2c.translator.processor.instructions.interfaces.InstructionProcessor;
import ru.itskekoff.j2c.translator.utils.ReflectionUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * @author itskekoff
 * @since 16:24 of 19.02.2025
 */
public class InsnProcessorManager {
    private final static List<InstructionProcessor> PROCESSORS = new ArrayList<>();

    static {
        PROCESSORS.addAll(ReflectionUtils.getClasses("ru.itskekoff.j2c.translator.processor.instructions.impl", InstructionProcessor.class));
        TranslatorMain.LOGGER.info("Loaded {} instruction processors before compilation", PROCESSORS.size());
    }

    public static void process(ClassNode classNode) {
        PROCESSORS.forEach(processor -> processor.process(classNode));
    }
}
