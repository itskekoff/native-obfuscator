package ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context;

import lombok.Getter;
import org.objectweb.asm.Label;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AnnotationNode;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.annotations.vmp.VMProtectType;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceSnippetGenerator;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceTable;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.stack.StackPointer;
import ru.itskekoff.j2c.translator.utils.BaseUtils;

import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;

import static ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter.*;

@Getter
public class MethodContext {
    private final LabelPool labelPool = new LabelPool();
    private final StackPointer stackPointer = new StackPointer();
    private final List<Integer> locals = new ArrayList<>();
    private final ClassNode classNode;
    private final ContextBuilder contextBuilder;

    public MethodContext(ClassNode classNode) {
        this.classNode = classNode;
        this.contextBuilder = new ContextBuilder(this.classNode);
    }

    public ContextBuilder output() {
        return this.contextBuilder;
    }

    public static class LabelPool {

        private final WeakHashMap<Label, Long> labels = new WeakHashMap<>();
        private long currentIndex = 0;

        public String getName(Label label) {
            return "L" + this.labels.computeIfAbsent(label, _ -> ++currentIndex);
        }

        public void clear() {
            this.labels.clear();
            this.currentIndex = 0;
        }
    }
}