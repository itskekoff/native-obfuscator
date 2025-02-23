package ru.itskekoff.j2c.translator.processor.cpp.reference;

import lombok.experimental.UtilityClass;
import org.objectweb.asm.tree.FieldInsnNode;
import org.objectweb.asm.tree.MethodInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter;

@UtilityClass
public class ReferenceSnippetGenerator {
    public String generateJavaClassReference(MethodContext methodContext, MethodNode method, String className) {
        ReferenceNode referenceNode = methodContext.output().pushJavaClass(className);

        if (!ClassFilter.isClinit(method)) {
            return generateObfuscatedReference("classes", referenceNode);
        } else {
            return "env->FindClass(\"%s\")".formatted(className);
        }
    }

    public String generateJavaMethodReference(MethodContext methodContext, MethodNode method, MethodInsnNode mh, boolean isStatic) {
        ReferenceNode referenceNode = methodContext.output().allocateOrGetMethodNode(methodContext, mh.owner, mh.name, mh.desc, isStatic, method);

        if (!ClassFilter.isClinit(method)) {
            return generateObfuscatedReference("methods", referenceNode);
        } else {
            return "env->GetStaticMethodID(%s, \"%s\", \"%s\")"
                    .formatted(generateJavaClassReference(methodContext, method, mh.owner), mh.name, mh.desc);
        }
    }

    public String generateJavaFieldReference(MethodContext methodContext, MethodNode method, FieldInsnNode fn, boolean isStatic) {
        ReferenceNode referenceNode = methodContext.output().allocateOrGetFieldNode(methodContext, fn.owner, fn.name, fn.desc, isStatic, method);

        if (!ClassFilter.isClinit(method)) {
            return generateObfuscatedReference("fields", referenceNode);
        } else {
            return "env->Get%sFieldID(%s, \"%s\", \"%s\")"
                    .formatted(referenceNode.isStatic(), generateJavaClassReference(methodContext, method, fn.owner), fn.name, fn.desc);
        }
    }

    /*
        Server code:

        // random source multiplier: 6364136222846791005L

        randomSource := customRandomSource{seed: SeedConst}
        seedAsLong := initialSeed ^ (XorConstant ^ randomSource.Int63())

        random := newConsistentRandom(seedAsLong)
        key := random.Int63()
        address ^= key

        for i := 0; i < 6; i++ {
            kluch := random.Int63()
            address ^= kluch
            if i == 5 {
                address ^= rtdsc
            }
        }
        _, _ = w.Write([]byte(strconv.FormatInt(address, 10)))
     */

    private String generateObfuscatedReference(String arrayName, ReferenceNode referenceNode) {
        long[] referenceKeys = referenceNode.getReferenceKeys();

        return "(j%s)(((((((__int64)(%s[%s]) ^ %d) ^ %d) ^ %d) ^ %d) ^ %d) ^ %d ^ rtdsc)"
                .formatted(getReferenceType(arrayName), arrayName, referenceNode.getId(),
                        referenceKeys[1],
                        referenceKeys[2],
                        referenceKeys[3],
                        referenceKeys[4],
                        referenceKeys[5],
                        referenceKeys[0]);
    }

    private String getReferenceType(String arrayName) {
        return switch (arrayName) {
            case "classes" -> "class";
            case "methods" -> "methodID";
            case "fields" -> "fieldID";
            default -> throw new IllegalArgumentException("Unknown array name: " + arrayName);
        };
    }

}
