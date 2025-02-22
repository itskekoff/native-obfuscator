package ru.itskekoff.j2c.translator.processor.cpp.reference;

import lombok.experimental.UtilityClass;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;

@UtilityClass
public class ReferenceSnippetGenerator {

    public String generateJavaClassReference(MethodContext methodContext,
                                                    MethodNode method,
                                                    String className) {
        if (methodContext.notClinit(method))
            return "classes[%s].applyDecryption()".formatted(methodContext.output().pushJavaClass(className));
         else
             return "env->FindClass(\"%s\")".formatted(className);
    }

}
