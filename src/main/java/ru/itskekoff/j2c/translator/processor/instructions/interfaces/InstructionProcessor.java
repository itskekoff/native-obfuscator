package ru.itskekoff.j2c.translator.processor.instructions.interfaces;

import org.objectweb.asm.tree.ClassNode;

/**
 * @author itskekoff
 * @since 16:22 of 19.02.2025
 */
public interface InstructionProcessor {
    void process(ClassNode classNode);
}
