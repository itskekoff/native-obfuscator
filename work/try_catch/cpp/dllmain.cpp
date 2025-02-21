#include "jni.h"
#include <type_traits>
#include <string>
#include <iostream>

jclass boolean_array_class;

    void bastore(JNIEnv *env, jarray array, jint index, jint value) {
        if (env->IsInstanceOf(array, boolean_array_class))
            env->SetBooleanArrayRegion((jbooleanArray) array, index, 1, (jboolean*) (&value));
        else
            env->SetByteArrayRegion((jbyteArray) array, index, 1, (jbyte*) (&value));
    }

    jbyte baload(JNIEnv *env, jarray array, jint index) {
        jbyte ret_value;
        if (env->IsInstanceOf(array, boolean_array_class))
            env->GetBooleanArrayRegion((jbooleanArray) array, index, 1, (jboolean*) (&ret_value));
        else
            env->GetByteArrayRegion((jbyteArray) array, index, 1, (jbyte*) (&ret_value));
        return ret_value;
    }

    void throw_re(JNIEnv *env, const char *exception_class, const char *error, int line) {
        jclass exception_class_ptr = env->FindClass(exception_class);
        if (env->ExceptionCheck()) {
            return;
        }
        env->ThrowNew(exception_class_ptr, ("\"" + std::string(error) + "\" on " + std::to_string(line)).c_str());
        env->DeleteLocalRef(exception_class_ptr);
    }


// Main
void JNICALL  Java_Main_main0(JNIEnv *env, jclass clazz, jarray arg0 ) {
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {};
    clocal0.l = arg0;

    // try-catches count 2
L1:
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_0 = env->GetStaticFieldID(env->FindClass("java/lang/System"), "out", "Ljava/io/PrintStream;");
    cstack0.l = env->GetStaticObjectField(env->FindClass("java/lang/System"), fieldId_0);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Hello World from inside try catch ");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    jclass tempClass_0 = env->FindClass("java/io/PrintStream");
    env->CallVoidMethod(cstack0.l, env->GetMethodID(tempClass_0, "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L2:
    // GOTO; Stack pointer: 0
    goto L3;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    jclass tempClass_1 = env->FindClass("java/lang/Exception");
    env->CallVoidMethod(cstack0.l, env->GetMethodID(tempClass_1, "printStackTrace", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L3:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_1 = env->GetStaticFieldID(env->FindClass("java/lang/System"), "out", "Ljava/io/PrintStream;");
    cstack0.l = env->GetStaticObjectField(env->FindClass("java/lang/System"), fieldId_1);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Hello World from outside try catch!");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    jclass tempClass_2 = env->FindClass("java/io/PrintStream");
    env->CallVoidMethod(cstack0.l, env->GetMethodID(tempClass_2, "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L5:
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_2 = env->GetStaticFieldID(env->FindClass("java/lang/System"), "out", "Ljava/io/PrintStream;");
    cstack0.l = env->GetStaticObjectField(env->FindClass("java/lang/System"), fieldId_2);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Hello World from inside try catch 2!");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    jclass tempClass_3 = env->FindClass("java/io/PrintStream");
    env->CallVoidMethod(cstack0.l, env->GetMethodID(tempClass_3, "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(env->FindClass("java/lang/RuntimeException"))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = env->NewStringUTF("e");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    jclass tempClass_4 = env->FindClass("java/lang/RuntimeException");
    env->CallNonvirtualVoidMethod(cstack1.l, tempClass_4, env->GetMethodID(tempClass_4, "<init>", "(Ljava/lang/String;)V"), cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    jclass tempThrow_0 = env->FindClass("Main");
    if (cstack0.l == nullptr) env->ThrowNew(tempThrow_0, ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_3 = env->GetStaticFieldID(env->FindClass("java/lang/System"), "out", "Ljava/io/PrintStream;");
    cstack0.l = env->GetStaticObjectField(env->FindClass("java/lang/System"), fieldId_3);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Hello world from catch block 2!");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    jclass tempClass_5 = env->FindClass("java/io/PrintStream");
    env->CallVoidMethod(cstack0.l, env->GetMethodID(tempClass_5, "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_4 = env->GetStaticFieldID(env->FindClass("java/lang/System"), "out", "Ljava/io/PrintStream;");
    cstack0.l = env->GetStaticObjectField(env->FindClass("java/lang/System"), fieldId_4);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("yupieee!!!!");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    jclass tempClass_6 = env->FindClass("java/io/PrintStream");
    env->CallVoidMethod(cstack0.l, env->GetMethodID(tempClass_6, "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
CATCH_0:
    if (env->IsInstanceOf(cstack_exception.l, env->FindClass("java/lang/Exception"))) { env->ExceptionClear(); goto L4; }
CATCH_1:
    if (env->IsInstanceOf(cstack_exception.l, env->FindClass("java/lang/RuntimeException"))) { env->ExceptionClear(); goto L6; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); }

TRY_0:
TRY_1:
}
extern "C" JNIEXPORT void JNICALL  Java_Main__00024Clinit(JNIEnv *env, jclass clazz ) {
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    JNINativeMethod jniMethods[] = {
        "main", "([Ljava/lang/String;)V", &Java_Main_main0,
    };
    env->RegisterNatives(env->FindClass("Main"), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); }

}
                    #ifndef _Included_ru_kotopushka_protection_Protection
#define _Included_ru_kotopushka_protection_Protection
#ifdef __cplusplus
    extern "C" {
#endif
JNIEXPORT void JNICALL Java_ru_kotopushka_protection_Protection_initialize
(JNIEnv* env, jclass klass) {
jclass clazz = env->FindClass("[Z");
boolean_array_class = (jclass)env->NewGlobalRef(clazz);
}

#ifdef __cplusplus
}
#endif
#endif
