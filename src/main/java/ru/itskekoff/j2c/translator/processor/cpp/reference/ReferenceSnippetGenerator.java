package ru.itskekoff.j2c.translator.processor.cpp.reference;

import lombok.experimental.UtilityClass;
import org.objectweb.asm.tree.FieldInsnNode;
import org.objectweb.asm.tree.MethodInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;

@UtilityClass
public class ReferenceSnippetGenerator {

    public String generateJavaClassReference(MethodContext methodContext,
                                                    MethodNode method,
                                                    String className) {

        ReferenceNode referenceNode = methodContext.output().pushJavaClass(className);


        if (methodContext.notClinit(method))
            return "(jclass)(((((((__int64)(classes[%s]) ^ %s) ^ %s) ^ %s) ^ %s) ^ %s) ^ %s ^ rtdsc)"
                    .formatted(referenceNode.getId(), referenceNode.getKluch2(), referenceNode.getKluch3(), referenceNode.getKluch4(), referenceNode.getKluch5(), referenceNode.getKluch6(), referenceNode.getKluch());
        else
             return "env->FindClass((\"%s\"))".formatted(className);
    }

    /*
        TODO: SUPPORT ONLY STATIC METHOD'S
     */

    public String generateJavaMethodReference(MethodContext methodContext,
                                             MethodNode method,
                                             MethodInsnNode mh, boolean isStatic) {

        ReferenceNode referenceNode = methodContext.output().allocateOrGetMethodNode(methodContext, mh.owner, mh.name, mh.desc, isStatic, method);

        if (methodContext.notClinit(method))
            return "(jmethodID)(((((((__int64)(methods[%s]) ^ %s) ^ %s) ^ %s) ^ %s) ^ %s) ^ %s ^ rtdsc)"
                    .formatted(referenceNode.getId(), referenceNode.getKluch2(), referenceNode.getKluch3(), referenceNode.getKluch4(), referenceNode.getKluch5(), referenceNode.getKluch6(), referenceNode.getKluch());
        else
            return "env->GetStaticMethodID(%s, (\"%s\"), (\"%s\"))".formatted(generateJavaClassReference(methodContext, method, mh.owner),mh.name, mh.desc);
    }

    public String generateJavaFieldReference(MethodContext methodContext,
                                             MethodNode method,
                                             FieldInsnNode fn, boolean isStatic) {

        ReferenceNode referenceNode = methodContext.output().allocateOrGetFieldNode(methodContext,fn.owner, fn.name, fn.desc, isStatic, method);

        if (methodContext.notClinit(method))
            return "(jfieldID)(((((((__int64)(fields[%s]) ^ %s) ^ %s) ^ %s) ^ %s) ^ %s) ^ %s ^ rtdsc)"
                    .formatted(referenceNode.getId(), referenceNode.getKluch2(), referenceNode.getKluch3(), referenceNode.getKluch4(), referenceNode.getKluch5(), referenceNode.getKluch6(), referenceNode.getKluch());
        else
            return "env->Get%sFieldID(%s, (\"%s\"), (\"%s\"))".formatted(referenceNode.isStatic(),generateJavaClassReference(methodContext, method, fn.owner), fn.name, fn.desc);
    }

}
