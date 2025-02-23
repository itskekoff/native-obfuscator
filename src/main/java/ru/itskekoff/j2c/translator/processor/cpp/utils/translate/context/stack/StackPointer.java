package ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.stack;

/**
 * @author itskekoff
 * @since 15:25 of 23.02.2025
 */
public class StackPointer {
    private int pointer;

    public StackPointer() {
        this.pointer = 0;
    }

    public StackPointer push(int... count) {
        pointer += (count.length > 0) ? count[0] : 1;
        return this;
    }

    public StackPointer pop(int... count) {
        pointer -= (count.length > 0) ? count[0] : 1;
        return this;
    }

    public int peek() {
        return this.pointer;
    }

    public void set(int pointer) {
        this.pointer = pointer;
    }
}
