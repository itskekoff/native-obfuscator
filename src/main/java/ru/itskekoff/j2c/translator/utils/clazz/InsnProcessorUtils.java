package ru.itskekoff.j2c.translator.utils.clazz;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodInsnNode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;
import java.util.function.Supplier;

public class InsnProcessorUtils {
    private static final String MAGIC_CONST = String.valueOf(Math.random());

    public static final Supplier<AbstractInsnNode> LOOKUP_LOCAL = () -> new MethodInsnNode(Opcodes.INVOKESTATIC,
            "native/magic/1/lookup/obfuscator" + MAGIC_CONST, "a",
            "()Ljava/lang/invoke/MethodHandles$Lookup;");
    public static final Supplier<AbstractInsnNode> CLASSLOADER_LOCAL = () -> new MethodInsnNode(Opcodes.INVOKESTATIC,
            "native/magic/1/classloader/obfuscator" + MAGIC_CONST, "a",
            "()Ljava/lang/ClassLoader;");
    public static final Supplier<AbstractInsnNode> CLASS_LOCAL = () -> new MethodInsnNode(Opcodes.INVOKESTATIC,
            "native/magic/1/class/obfuscator" + MAGIC_CONST, "a",
            "()Ljava/lang/Class;");
    public static final Function<String, AbstractInsnNode> INVOKE_REVERSE =
            desc -> {
                List<Type> argumentTypes = new ArrayList<>(Arrays.asList(Type.getArgumentTypes(desc)));
                argumentTypes.add(Type.getObjectType("java/lang/invoke/MethodHandle"));
                return new MethodInsnNode(Opcodes.INVOKESTATIC, "native/magic/1/invoke/obfuscator" + MAGIC_CONST, "a",
                        Type.getMethodDescriptor(Type.getReturnType(desc), argumentTypes.toArray(new Type[0])));
            };
    public static final Supplier<AbstractInsnNode> LINK_CALL_SITE_METHOD = () -> new MethodInsnNode(Opcodes.INVOKESTATIC,
            "native/magic/1/linkcallsite/obfuscator" + MAGIC_CONST, "a", "(Ljava/lang/Object;Ljava/lang/Object;" +
                                                                         "Ljava/lang/Object;Ljava/lang/Object;" +
                                                                         "Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/invoke/MemberName;");

}
