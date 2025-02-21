
#include "table.h"
#include "jni.h"
#include <string>
#include <stdexcept>
#include <cmath>

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

template <int sort>
jarray create_multidim_array_value(JNIEnv *env, jint count, jint required_count,
    const char *name, int line, std::initializer_list<jint> sizes, int dim_index = 0) {
    if (required_count == 0) {
        env->FatalError("required_count == 0");
        return nullptr;
    }
    jint current_size = sizes.begin()[dim_index];
    if (current_size < 0) {
        throw_re(env, "java/lang/NegativeArraySizeException", "MULTIANEWARRAY size < 0", line);
        return nullptr;
    }
    if (count == 1) {
        return create_array_value<sort>(env, current_size);
    }
    jobjectArray result_array = nullptr;
    if (jclass clazz = env->FindClass((std::string(count - 1, '[') + std::string(name)).c_str())) {
        result_array = env->NewObjectArray(current_size, clazz, nullptr);
        if (env->ExceptionCheck()) {
            return nullptr;
        }
        env->DeleteLocalRef(clazz);
    }
    else
        return nullptr;

    if (required_count == 1) {
        return result_array;
    }

    for (jint i = 0; i < current_size; i++) {
        jarray inner_array = create_multidim_array_value<sort>(env, count - 1, required_count - 1,
            name, line, sizes, dim_index + 1);
        if (env->ExceptionCheck()) {
            env->DeleteLocalRef(result_array);
            return nullptr;
        }
        env->SetObjectArrayElement(result_array, i, inner_array);
        env->DeleteLocalRef(inner_array);
        if (env->ExceptionCheck()) {
            env->DeleteLocalRef(result_array);
            return nullptr;
        }
    }
    return result_array;
}

// net/minecraft/client/main/Main$1
jobject JNICALL  Java_net_minecraft_client_main_Main_000241_getPasswordAuthentication0(JNIEnv *env, jobject obj) {
    // stack count: 4, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = obj; 

L1:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[0].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 3
    jfieldID fieldId_0 = env->GetFieldID(classes[1].applyDecryption(), "val$s1", "Ljava/lang/String;");
    cstack2.l = env->GetObjectField(cstack2.l, fieldId_0);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 4
    jfieldID fieldId_1 = env->GetFieldID(classes[1].applyDecryption(), "val$s2", "Ljava/lang/String;");
    cstack3.l = env->GetObjectField(cstack3.l, fieldId_1);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 4
    cstack3.l = env->CallObjectMethod(cstack3.l, env->GetMethodID(classes[2].applyDecryption(), "toCharArray", "()[C"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, classes[0].applyDecryption(), env->GetMethodID(classes[0].applyDecryption(), "<init>", "(Ljava/lang/String;[C)V"), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L2:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main_000241__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 0, locals count: 0, try-catches: 0
    jvalue cstack_exception = {};
    JNINativeMethod jniMethods[] = {
        "getPasswordAuthentication", "()Ljava/net/PasswordAuthentication;", &Java_net_minecraft_client_main_Main_000241_getPasswordAuthentication0,
    };
    env->RegisterNatives(env->FindClass("net/minecraft/client/main/Main$1"), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /*PROTECTION TABLE*/
    classes[0] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/net/PasswordAuthentication"))))));
    classes[1] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/main/Main$1"))))));
    classes[2] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/String"))))));

    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
// net/minecraft/client/main/Main$2
void JNICALL  Java_net_minecraft_client_main_Main_000242_run1(JNIEnv *env, jobject obj) {
    // stack count: 2, locals count: 3, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {};

    clocal0.l = obj; 

L1:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod(classes[3].applyDecryption(), env->GetStaticMethodID(classes[3].applyDecryption(), "getInstance", "()Lnet/minecraft/client/Minecraft;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L2:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L3;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[3].applyDecryption(), "getIntegratedServer", "()Lnet/minecraft/server/integrated/IntegratedServer;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal2.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L5:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal2.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L3;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal2.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 1
    cstack1.i = 1;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[4].applyDecryption(), "initiateShutdown", "(Z)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L3:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L7:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main_000242__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 0, locals count: 0, try-catches: 0
    jvalue cstack_exception = {};
    JNINativeMethod jniMethods[] = {
        "run", "()V", &Java_net_minecraft_client_main_Main_000242_run1,
    };
    env->RegisterNatives(env->FindClass("net/minecraft/client/main/Main$2"), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /*PROTECTION TABLE*/
    classes[3] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/Minecraft"))))));
    classes[4] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/server/integrated/IntegratedServer"))))));

    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
// net/minecraft/client/main/Main$3
void JNICALL  Java_net_minecraft_client_main_Main_000243_run2(JNIEnv *env, jobject obj) {
    // stack count: 3, locals count: 2, try-catches: 1
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {};

    clocal0.l = obj; 

L2:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // ICONST_1; Stack pointer: 0
    cstack0.i = 1;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod(classes[5].applyDecryption(), env->GetStaticMethodID(classes[5].applyDecryption(), "initGameThread", "(Z)V"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
L3:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 1
    jfieldID fieldId_2 = env->GetFieldID(classes[6].applyDecryption(), "val$minecraft", "Lnet/minecraft/client/Minecraft;");
    cstack0.l = env->GetObjectField(cstack0.l, fieldId_2);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[7].applyDecryption(), "run", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L5;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L1:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_3 = env->GetStaticFieldID(classes[8].applyDecryption(), "LOGGER", "Lorg/apache/logging/log4j/Logger;");
    cstack0.l = env->GetStaticObjectField(classes[8].applyDecryption(), fieldId_3);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Exception in client thread");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[9].applyDecryption(), "error", "(Ljava/lang/String;Ljava/lang/Throwable;)V"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L5:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L7:
CATCH_0:
    if (env->IsInstanceOf(cstack_exception.l, classes[10].applyDecryption())) { env->ExceptionClear(); goto L1; } 
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main_000243__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 0, locals count: 0, try-catches: 0
    jvalue cstack_exception = {};
    JNINativeMethod jniMethods[] = {
        "run", "()V", &Java_net_minecraft_client_main_Main_000243_run2,
    };
    env->RegisterNatives(env->FindClass("net/minecraft/client/main/Main$3"), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /*PROTECTION TABLE*/
    classes[5] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("com/mojang/blaze3d/systems/RenderSystem"))))));
    classes[6] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/main/Main$3"))))));
    classes[7] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/Minecraft"))))));
    classes[8] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/main/Main"))))));
    classes[9] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("org/apache/logging/log4j/Logger"))))));
    classes[10] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Throwable"))))));

    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
// net/minecraft/client/main/Main
void JNICALL  Java_net_minecraft_client_main_Main_main3(JNIEnv *env, jclass clazz, jarray arg0) {
    // stack count: 12, locals count: 59, try-catches: 8
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack4 = {}, cstack5 = {}, cstack6 = {}, cstack7 = {}, cstack8 = {}, cstack9 = {}, cstack10 = {}, cstack11 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {}, clocal4 = {}, clocal5 = {}, clocal6 = {}, clocal7 = {}, clocal8 = {}, clocal9 = {}, clocal10 = {}, clocal11 = {}, clocal12 = {}, clocal13 = {}, clocal14 = {}, clocal15 = {}, clocal16 = {}, clocal17 = {}, clocal18 = {}, clocal19 = {}, clocal20 = {}, clocal21 = {}, clocal22 = {}, clocal23 = {}, clocal24 = {}, clocal25 = {}, clocal26 = {}, clocal27 = {}, clocal28 = {}, clocal29 = {}, clocal30 = {}, clocal31 = {}, clocal32 = {}, clocal33 = {}, clocal34 = {}, clocal35 = {}, clocal36 = {}, clocal37 = {}, clocal38 = {}, clocal39 = {}, clocal40 = {}, clocal41 = {}, clocal42 = {}, clocal43 = {}, clocal44 = {}, clocal45 = {}, clocal46 = {}, clocal47 = {}, clocal48 = {}, clocal49 = {}, clocal50 = {}, clocal51 = {}, clocal52 = {}, clocal53 = {}, clocal54 = {}, clocal55 = {}, clocal56 = {}, clocal57 = {}, clocal58 = {};

    clocal0.l = arg0; 

L7:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[11].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 2
    env->CallNonvirtualVoidMethod(cstack1.l, classes[11].applyDecryption(), env->GetMethodID(classes[11].applyDecryption(), "<init>", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L8:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "allowsUnrecognizedOptions", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L9:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("demo");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L10:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("disableMultiplayer");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L11:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("disableChat");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L12:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("fullscreen");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L13:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("checkGlErrors");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L14:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("server");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal2.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L15:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("port");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[13].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // SIPUSH; Stack pointer: 1
    cstack1.i = (jint) 25565;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod(classes[13].applyDecryption(), env->GetStaticMethodID(classes[13].applyDecryption(), "valueOf", "(I)Ljava/lang/Integer;"), cstack1.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[13].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal3.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L16:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("gameDir");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[15].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 1
    if (jobject obj = env->AllocObject(classes[15].applyDecryption())) { cstack1.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 2
    cstack2 = cstack1;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.l = env->NewStringUTF(".");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack2.l, classes[15].applyDecryption(), env->GetMethodID(classes[15].applyDecryption(), "<init>", "(Ljava/lang/String;)V"), cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[15].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal4.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L17:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("assetsDir");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[15].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal5.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L18:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("resourcePackDir");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[15].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal6.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L19:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("dataPackDir");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[15].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal7.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L20:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("proxyHost");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal8.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L21:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("proxyPort");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("8080");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[16].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[13].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal9.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L22:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("proxyUser");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal10.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L23:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("proxyPass");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal11.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L24:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("username");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack1.j = env->CallStaticLongMethod(classes[17].applyDecryption(), env->GetStaticMethodID(classes[17].applyDecryption(), "milliTime", "()J"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.j = 1000LL;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LREM; Stack pointer: 5
    cstack1.j = (cstack1.j % cstack3.j);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack1.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "$hello0_proxy1", "(J)Ljava/lang/String;"), cstack1.j);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[16].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal12.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L25:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("uuid");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal13.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L26:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("accessToken");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "required", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal14.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L27:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("version");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "required", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal15.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L28:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("width");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[13].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // SIPUSH; Stack pointer: 1
    cstack1.i = (jint) 854;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod(classes[13].applyDecryption(), env->GetStaticMethodID(classes[13].applyDecryption(), "valueOf", "(I)Ljava/lang/Integer;"), cstack1.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[13].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal16.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L29:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("height");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[13].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // SIPUSH; Stack pointer: 1
    cstack1.i = (jint) 480;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod(classes[13].applyDecryption(), env->GetStaticMethodID(classes[13].applyDecryption(), "valueOf", "(I)Ljava/lang/Integer;"), cstack1.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[13].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal17.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L30:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("fullscreenWidth");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[13].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal18.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L31:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("fullscreenHeight");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[13].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "ofType", "(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal19.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L32:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("userProperties");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("{}");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[16].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal20.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L33:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("profileProperties");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("{}");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[16].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal21.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L34:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("assetIndex");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal22.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L35:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("userType");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("legacy");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[16].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal23.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L36:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("versionType");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "accepts", "(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[12].applyDecryption(), "withRequiredArg", "()Ljoptsimple/ArgumentAcceptingOptionSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("release");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[16].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultsTo", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal24.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L37:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "nonOptions", "()Ljoptsimple/NonOptionArgumentSpec;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal25.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L38:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "parse", "([Ljava/lang/String;)Ljoptsimple/OptionSet;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal26.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L39:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal25.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "valuesOf", "(Ljoptsimple/OptionSpec;)Ljava/util/List;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal27.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L40:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal27.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[20].applyDecryption(), "isEmpty", "()Z"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L41;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L42:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_4 = env->GetStaticFieldID(classes[21].applyDecryption(), "out", "Ljava/io/PrintStream;");
    cstack0.l = env->GetStaticObjectField(classes[21].applyDecryption(), fieldId_4);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal27.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "$hello1_proxy2", "(Ljava/util/List;)Ljava/lang/String;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[22].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L41:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal8.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal28.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L43:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_5 = env->GetStaticFieldID(classes[23].applyDecryption(), "NO_PROXY", "Ljava/net/Proxy;");
    cstack0.l = env->GetStaticObjectField(classes[23].applyDecryption(), fieldId_5);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal29.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L44:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal28.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L45;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L46:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[23].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // GETSTATIC; Stack pointer: 2
    jfieldID fieldId_6 = env->GetStaticFieldID(classes[24].applyDecryption(), "SOCKS", "Ljava/net/Proxy$Type;");
    cstack2.l = env->GetStaticObjectField(classes[24].applyDecryption(), fieldId_6);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // NEW; Stack pointer: 3
    if (jobject obj = env->AllocObject(classes[25].applyDecryption())) { cstack3.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // DUP; Stack pointer: 4
    cstack4 = cstack3;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal28.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal9.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // INVOKESTATIC; Stack pointer: 8
    cstack6.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack6.l, cstack7.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // CHECKCAST; Stack pointer: 7
    if (cstack6.l != nullptr && !env->IsInstanceOf(cstack6.l, classes[13].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // INVOKEVIRTUAL; Stack pointer: 7
    cstack6.i = env->CallIntMethod(cstack6.l, env->GetMethodID(classes[13].applyDecryption(), "intValue", "()I"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // INVOKESPECIAL; Stack pointer: 7
    env->CallNonvirtualVoidMethod(cstack4.l, classes[25].applyDecryption(), env->GetMethodID(classes[25].applyDecryption(), "<init>", "(Ljava/lang/String;I)V"), cstack5.l, cstack6.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, classes[23].applyDecryption(), env->GetMethodID(classes[23].applyDecryption(), "<init>", "(Ljava/net/Proxy$Type;Ljava/net/SocketAddress;)V"), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // ASTORE; Stack pointer: 1
    clocal29.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
L47:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L45;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L1:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal30.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L45:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal10.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal30.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L48:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal11.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal31.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L49:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal29.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    jfieldID fieldId_7 = env->GetStaticFieldID(classes[23].applyDecryption(), "NO_PROXY", "Ljava/net/Proxy;");
    cstack1.l = env->GetStaticObjectField(classes[23].applyDecryption(), fieldId_7);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[23].applyDecryption(), "equals", "(Ljava/lang/Object;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L50;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal30.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.i = (jint) env->CallStaticBooleanMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "isNotEmpty", "(Ljava/lang/String;)Z"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L50;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal31.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.i = (jint) env->CallStaticBooleanMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "isNotEmpty", "(Ljava/lang/String;)Z"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L50;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L51:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[26].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal30.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal31.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, classes[26].applyDecryption(), env->GetMethodID(classes[26].applyDecryption(), "<init>", "(Ljava/lang/String;Ljava/lang/String;)V"), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod(classes[27].applyDecryption(), env->GetStaticMethodID(classes[27].applyDecryption(), "setDefault", "(Ljava/net/Authenticator;)V"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L50:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal16.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[13].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = env->CallIntMethod(cstack0.l, env->GetMethodID(classes[13].applyDecryption(), "intValue", "()I"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal32.i = cstack0.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L52:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal17.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[13].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = env->CallIntMethod(cstack0.l, env->GetMethodID(classes[13].applyDecryption(), "intValue", "()I"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal33.i = cstack0.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L53:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal18.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[13].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "toOptionalInt", "(Ljava/lang/Integer;)Ljava/util/OptionalInt;"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal34.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L54:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal19.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[13].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "toOptionalInt", "(Ljava/lang/Integer;)Ljava/util/OptionalInt;"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal35.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L55:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("fullscreen");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljava/lang/String;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal36.i = cstack0.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L56:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("demo");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljava/lang/String;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal37.i = cstack0.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L57:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("disableMultiplayer");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljava/lang/String;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal38.i = cstack0.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L58:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("disableChat");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljava/lang/String;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal39.i = cstack0.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L59:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal15.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal40.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L60:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[28].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 2
    env->CallNonvirtualVoidMethod(cstack1.l, classes[28].applyDecryption(), env->GetMethodID(classes[28].applyDecryption(), "<init>", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = classes[29].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 2
    if (jobject obj = env->AllocObject(classes[30].applyDecryption())) { cstack2.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack3.l, classes[30].applyDecryption(), env->GetMethodID(classes[30].applyDecryption(), "<init>", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[28].applyDecryption(), "registerTypeAdapter", "(Ljava/lang/reflect/Type;Ljava/lang/Object;)Lcom/google/gson/GsonBuilder;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[28].applyDecryption(), "create", "()Lcom/google/gson/Gson;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal41.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L61:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal41.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal20.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack1.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 2
    if (cstack1.l != nullptr && !env->IsInstanceOf(cstack1.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = classes[29].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack0.l = env->CallStaticObjectMethod(classes[31].applyDecryption(), env->GetStaticMethodID(classes[31].applyDecryption(), "fromJson", "(Lcom/google/gson/Gson;Ljava/lang/String;Ljava/lang/Class;)Ljava/lang/Object;"), cstack0.l, cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[29].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal42.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L62:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal41.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal21.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack1.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 2
    if (cstack1.l != nullptr && !env->IsInstanceOf(cstack1.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = classes[29].applyDecryption();
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack0.l = env->CallStaticObjectMethod(classes[31].applyDecryption(), env->GetStaticMethodID(classes[31].applyDecryption(), "fromJson", "(Lcom/google/gson/Gson;Ljava/lang/String;Ljava/lang/Class;)Ljava/lang/Object;"), cstack0.l, cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[29].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal43.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L63:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal24.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal44.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L64:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal4.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[15].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal45.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L65:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal5.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljoptsimple/OptionSpec;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L66;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal5.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[15].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L67;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L66:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[15].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal45.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.l = env->NewStringUTF("assets/");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, classes[15].applyDecryption(), env->GetMethodID(classes[15].applyDecryption(), "<init>", "(Ljava/io/File;Ljava/lang/String;)V"), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L67:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal46.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L68:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal6.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljoptsimple/OptionSpec;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L69;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal6.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[15].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L70;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L69:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[15].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal45.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.l = env->NewStringUTF("resourcepacks/");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, classes[15].applyDecryption(), env->GetMethodID(classes[15].applyDecryption(), "<init>", "(Ljava/io/File;Ljava/lang/String;)V"), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L70:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal47.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L71:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal13.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljoptsimple/OptionSpec;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L72;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal13.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[32].applyDecryption(), "value", "(Ljoptsimple/OptionSet;)Ljava/lang/Object;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L73;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L72:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal12.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[32].applyDecryption(), "value", "(Ljoptsimple/OptionSet;)Ljava/lang/Object;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod(classes[33].applyDecryption(), env->GetStaticMethodID(classes[33].applyDecryption(), "getOfflineUUID", "(Ljava/lang/String;)Ljava/util/UUID;"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[34].applyDecryption(), "toString", "()Ljava/lang/String;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L73:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal48.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L74:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal22.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "has", "(Ljoptsimple/OptionSpec;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L75;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal22.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[32].applyDecryption(), "value", "(Ljoptsimple/OptionSet;)Ljava/lang/Object;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L76;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L75:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 0
    cstack0.l = nullptr;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L76:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal49.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L77:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal2.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal50.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L78:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal3.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[18].applyDecryption(), env->GetStaticMethodID(classes[18].applyDecryption(), "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[13].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal51.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L79:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[35].applyDecryption(), env->GetStaticMethodID(classes[35].applyDecryption(), "crash", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L80:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[36].applyDecryption(), env->GetStaticMethodID(classes[36].applyDecryption(), "register", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L81:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[36].applyDecryption(), env->GetStaticMethodID(classes[36].applyDecryption(), "checkTranslations", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L82:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[17].applyDecryption(), env->GetStaticMethodID(classes[17].applyDecryption(), "func_240994_l_", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L83:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[37].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal12.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 4
    cstack2.l = env->CallObjectMethod(cstack2.l, env->GetMethodID(classes[32].applyDecryption(), "value", "(Ljoptsimple/OptionSet;)Ljava/lang/Object;"), cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 3
    if (cstack2.l != nullptr && !env->IsInstanceOf(cstack2.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal48.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 4
    cstack4.l = clocal14.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 6
    cstack4.l = env->CallObjectMethod(cstack4.l, env->GetMethodID(classes[32].applyDecryption(), "value", "(Ljoptsimple/OptionSet;)Ljava/lang/Object;"), cstack5.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 5
    if (cstack4.l != nullptr && !env->IsInstanceOf(cstack4.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal23.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal26.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 7
    cstack5.l = env->CallObjectMethod(cstack5.l, env->GetMethodID(classes[32].applyDecryption(), "value", "(Ljoptsimple/OptionSet;)Ljava/lang/Object;"), cstack6.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 6
    if (cstack5.l != nullptr && !env->IsInstanceOf(cstack5.l, classes[16].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 6
    env->CallNonvirtualVoidMethod(cstack1.l, classes[37].applyDecryption(), env->GetMethodID(classes[37].applyDecryption(), "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"), cstack2.l, cstack3.l, cstack4.l, cstack5.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal52.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L84:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[38].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 2
    if (jobject obj = env->AllocObject(classes[39].applyDecryption())) { cstack2.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 4
    cstack4.l = clocal52.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal42.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal43.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal29.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 8
    env->CallNonvirtualVoidMethod(cstack3.l, classes[39].applyDecryption(), env->GetMethodID(classes[39].applyDecryption(), "<init>", "(Lnet/minecraft/util/Session;Lcom/mojang/authlib/properties/PropertyMap;Lcom/mojang/authlib/properties/PropertyMap;Ljava/net/Proxy;)V"), cstack4.l, cstack5.l, cstack6.l, cstack7.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 3
    if (jobject obj = env->AllocObject(classes[40].applyDecryption())) { cstack3.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 4
    cstack4 = cstack3;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 5
    cstack5.i = clocal32.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 6
    cstack6.i = clocal33.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal34.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal35.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 9
    cstack9.i = clocal36.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 10
    env->CallNonvirtualVoidMethod(cstack4.l, classes[40].applyDecryption(), env->GetMethodID(classes[40].applyDecryption(), "<init>", "(IILjava/util/OptionalInt;Ljava/util/OptionalInt;Z)V"), cstack5.i, cstack6.i, cstack7.l, cstack8.l, cstack9.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 4
    if (jobject obj = env->AllocObject(classes[41].applyDecryption())) { cstack4.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 5
    cstack5 = cstack4;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal45.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal47.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal46.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 9
    cstack9.l = clocal49.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 10
    env->CallNonvirtualVoidMethod(cstack5.l, classes[41].applyDecryption(), env->GetMethodID(classes[41].applyDecryption(), "<init>", "(Ljava/io/File;Ljava/io/File;Ljava/io/File;Ljava/lang/String;)V"), cstack6.l, cstack7.l, cstack8.l, cstack9.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 5
    if (jobject obj = env->AllocObject(classes[42].applyDecryption())) { cstack5.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 6
    cstack6 = cstack5;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 7
    cstack7.i = clocal37.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal40.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 9
    cstack9.l = clocal44.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 10
    cstack10.i = clocal38.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 11
    cstack11.i = clocal39.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 12
    env->CallNonvirtualVoidMethod(cstack6.l, classes[42].applyDecryption(), env->GetMethodID(classes[42].applyDecryption(), "<init>", "(ZLjava/lang/String;Ljava/lang/String;ZZ)V"), cstack7.l, cstack8.l, cstack9.l, cstack10.l, cstack11.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 6
    if (jobject obj = env->AllocObject(classes[43].applyDecryption())) { cstack6.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 7
    cstack7 = cstack6;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal50.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 9
    cstack9.l = clocal51.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 10
    cstack9.i = env->CallIntMethod(cstack9.l, env->GetMethodID(classes[13].applyDecryption(), "intValue", "()I"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 10
    env->CallNonvirtualVoidMethod(cstack7.l, classes[43].applyDecryption(), env->GetMethodID(classes[43].applyDecryption(), "<init>", "(Ljava/lang/String;I)V"), cstack8.l, cstack9.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 7
    env->CallNonvirtualVoidMethod(cstack1.l, classes[38].applyDecryption(), env->GetMethodID(classes[38].applyDecryption(), "<init>", "(Lnet/minecraft/client/GameConfiguration$UserInformation;Lnet/minecraft/client/renderer/ScreenSize;Lnet/minecraft/client/GameConfiguration$FolderInformation;Lnet/minecraft/client/GameConfiguration$GameInformation;Lnet/minecraft/client/GameConfiguration$ServerInformation;)V"), cstack2.l, cstack3.l, cstack4.l, cstack5.l, cstack6.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal53.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L85:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[44].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = env->NewStringUTF("Client Shutdown Thread");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[44].applyDecryption(), env->GetMethodID(classes[44].applyDecryption(), "<init>", "(Ljava/lang/String;)V"), cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal54.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L86:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal54.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 1
    if (jobject obj = env->AllocObject(classes[45].applyDecryption())) { cstack1.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 2
    cstack2 = cstack1;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 3
    jfieldID fieldId_8 = env->GetStaticFieldID(classes[18].applyDecryption(), "LOGGER", "Lorg/apache/logging/log4j/Logger;");
    cstack3.l = env->GetStaticObjectField(classes[18].applyDecryption(), fieldId_8);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack2.l, classes[45].applyDecryption(), env->GetMethodID(classes[45].applyDecryption(), "<init>", "(Lorg/apache/logging/log4j/Logger;)V"), cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[46].applyDecryption(), "setUncaughtExceptionHandler", "(Ljava/lang/Thread$UncaughtExceptionHandler;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L87:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod(classes[47].applyDecryption(), env->GetStaticMethodID(classes[47].applyDecryption(), "getRuntime", "()Ljava/lang/Runtime;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal54.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[47].applyDecryption(), "addShutdownHook", "(Ljava/lang/Thread;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L88:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[48].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 2
    env->CallNonvirtualVoidMethod(cstack1.l, classes[48].applyDecryption(), env->GetMethodID(classes[48].applyDecryption(), "<init>", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L89:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_1; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod(classes[46].applyDecryption(), env->GetStaticMethodID(classes[46].applyDecryption(), "currentThread", "()Ljava/lang/Thread;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_1; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Render thread");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_1; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[46].applyDecryption(), "setName", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_1; }
L90:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[49].applyDecryption(), env->GetStaticMethodID(classes[49].applyDecryption(), "initRenderThread", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L91:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[49].applyDecryption(), env->GetStaticMethodID(classes[49].applyDecryption(), "beginInitialization", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L92:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[50].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal53.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[50].applyDecryption(), env->GetMethodID(classes[50].applyDecryption(), "<init>", "(Lnet/minecraft/client/GameConfiguration;)V"), cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal55.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L93:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[49].applyDecryption(), env->GetStaticMethodID(classes[49].applyDecryption(), "finishInitialization", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L94:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L95;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L2:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L96:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_9 = env->GetStaticFieldID(classes[18].applyDecryption(), "LOGGER", "Lorg/apache/logging/log4j/Logger;");
    cstack0.l = env->GetStaticObjectField(classes[18].applyDecryption(), fieldId_9);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Failed to create window: ");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal56.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[51].applyDecryption(), "warn", "(Ljava/lang/String;Ljava/lang/Throwable;)V"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L97:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L3:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L98:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Initializing game");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(classes[35].applyDecryption(), env->GetStaticMethodID(classes[35].applyDecryption(), "makeCrashReport", "(Ljava/lang/Throwable;Ljava/lang/String;)Lnet/minecraft/crash/CrashReport;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal57.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L99:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal57.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Initialization");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[35].applyDecryption(), "makeCategory", "(Ljava/lang/String;)Lnet/minecraft/crash/CrashReportCategory;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L100:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 0
    cstack0.l = nullptr;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[52].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal53.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 2
    jfieldID fieldId_10 = env->GetFieldID(classes[38].applyDecryption(), "gameInfo", "Lnet/minecraft/client/GameConfiguration$GameInformation;");
    cstack1.l = env->GetObjectField(cstack1.l, fieldId_10);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 2
    jfieldID fieldId_11 = env->GetFieldID(classes[42].applyDecryption(), "version", "Ljava/lang/String;");
    cstack1.l = env->GetObjectField(cstack1.l, fieldId_11);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 2
    cstack2.l = nullptr;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 3
    if (cstack2.l != nullptr && !env->IsInstanceOf(cstack2.l, classes[53].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal57.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    env->CallStaticVoidMethod(classes[50].applyDecryption(), env->GetStaticMethodID(classes[50].applyDecryption(), "fillCrashReport", "(Lnet/minecraft/client/resources/LanguageManager;Ljava/lang/String;Lnet/minecraft/client/GameSettings;Lnet/minecraft/crash/CrashReport;)V"), cstack0.l, cstack1.l, cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L101:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal57.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod(classes[50].applyDecryption(), env->GetStaticMethodID(classes[50].applyDecryption(), "displayCrashReport", "(Lnet/minecraft/crash/CrashReport;)V"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L102:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L95:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[50].applyDecryption(), "isRenderOnThread", "()Z"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L103;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L104:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[54].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = env->NewStringUTF("Game thread");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, classes[54].applyDecryption(), env->GetMethodID(classes[54].applyDecryption(), "<init>", "(Ljava/lang/String;Lnet/minecraft/client/Minecraft;)V"), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L105:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[46].applyDecryption(), "start", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L106:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[50].applyDecryption(), "isRunning", "()Z"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L107;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L106;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L103:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 0
    cstack0.l = nullptr;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L108:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_3; }
    // ICONST_0; Stack pointer: 0
    cstack0.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_3; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod(classes[49].applyDecryption(), env->GetStaticMethodID(classes[49].applyDecryption(), "initGameThread", "(Z)V"), cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_3; }
L109:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[50].applyDecryption(), "run", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L110:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L107;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal57.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L111:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_12 = env->GetStaticFieldID(classes[18].applyDecryption(), "LOGGER", "Lorg/apache/logging/log4j/Logger;");
    cstack0.l = env->GetStaticObjectField(classes[18].applyDecryption(), fieldId_12);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Unhandled game exception");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal57.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[51].applyDecryption(), "error", "(Ljava/lang/String;Ljava/lang/Throwable;)V"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L107:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_4; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_4; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_4; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[50].applyDecryption(), "shutdown", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_4; }
L112:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L113;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L114:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[46].applyDecryption(), "join", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L113:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[50].applyDecryption(), "shutdownMinecraftApplet", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L115:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L116;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L5:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_7; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_7; }
    // ASTORE; Stack pointer: 1
    clocal57.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_7; }
L117:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    jfieldID fieldId_13 = env->GetStaticFieldID(classes[18].applyDecryption(), "LOGGER", "Lorg/apache/logging/log4j/Logger;");
    cstack0.l = env->GetStaticObjectField(classes[18].applyDecryption(), fieldId_13);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Exception during client thread shutdown");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal57.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[51].applyDecryption(), "error", "(Ljava/lang/String;Ljava/lang/Throwable;)V"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L118:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[50].applyDecryption(), "shutdownMinecraftApplet", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L119:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L116;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_7; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_7; }
    // ASTORE; Stack pointer: 1
    clocal58.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_7; }
L120:
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[50].applyDecryption(), "shutdownMinecraftApplet", "()V"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L121:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal58.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[18].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L116:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L122:
CATCH_3:
    if (env->IsInstanceOf(cstack_exception.l, classes[55].applyDecryption())) { env->ExceptionClear(); goto L4; } 
CATCH_2:
    if (env->IsInstanceOf(cstack_exception.l, classes[55].applyDecryption())) { env->ExceptionClear(); goto L3; } 
CATCH_1:
    if (env->IsInstanceOf(cstack_exception.l, classes[56].applyDecryption())) { env->ExceptionClear(); goto L2; } 
CATCH_0:
    if (env->IsInstanceOf(cstack_exception.l, classes[57].applyDecryption())) { env->ExceptionClear(); goto L1; } 
CATCH_7:
    goto L6;
CATCH_6:
    goto L6;
CATCH_5:
    goto L6;
CATCH_4:
    if (env->IsInstanceOf(cstack_exception.l, classes[58].applyDecryption())) { env->ExceptionClear(); goto L5; } 
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
jobject JNICALL  Java_net_minecraft_client_main_Main_toOptionalInt4(JNIEnv *env, jclass clazz, jobject arg0) {
    // stack count: 1, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = arg0; 

L123:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L124;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = env->CallIntMethod(cstack0.l, env->GetMethodID(classes[13].applyDecryption(), "intValue", "()I"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod(classes[59].applyDecryption(), env->GetStaticMethodID(classes[59].applyDecryption(), "of", "(I)Ljava/util/OptionalInt;"), cstack0.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L125;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L124:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod(classes[59].applyDecryption(), env->GetStaticMethodID(classes[59].applyDecryption(), "empty", "()Ljava/util/OptionalInt;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L125:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L126:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
jobject JNICALL  Java_net_minecraft_client_main_Main_getValue5(JNIEnv *env, jclass clazz, jobject arg0, jobject arg1) {
    // stack count: 2, locals count: 5, try-catches: 1
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {}, clocal4 = {}, clocal5 = {}, clocal6 = {};

    clocal0.l = arg0; clocal1.l = arg1; 

L128:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[19].applyDecryption(), "valueOf", "(Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto CATCH_0; }
L129:
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L127:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal2.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L130:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INSTANCEOF; Stack pointer: 1
    cstack0.i = cstack0.l == nullptr ? false : env->IsInstanceOf(cstack0.l, classes[14].applyDecryption());
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L131;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L132:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, classes[14].applyDecryption())) {}
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal3.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L133:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal3.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[14].applyDecryption(), "defaultValues", "()Ljava/util/List;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal4.l = cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L134:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal4.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[20].applyDecryption(), "isEmpty", "()Z"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L131;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L135:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal4.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 1
    cstack1.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[20].applyDecryption(), "get", "(I)Ljava/lang/Object;"), cstack1.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L131:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal2.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[18].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L136:
CATCH_0:
    if (env->IsInstanceOf(cstack_exception.l, classes[55].applyDecryption())) { env->ExceptionClear(); goto L127; } 
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
jboolean JNICALL  Java_net_minecraft_client_main_Main_isNotEmpty6(JNIEnv *env, jclass clazz, jobject arg0) {
    // stack count: 1, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = arg0; 

L137:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L138;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[16].applyDecryption(), "isEmpty", "()Z"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L138;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 0
    cstack0.i = 1;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L139;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L138:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 0
    cstack0.i = 0;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L139:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // IRETURN; Stack pointer: 1
    return (jboolean) cstack0.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L140:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jboolean) 0; }

    return (jboolean) 0;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 3, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    JNINativeMethod jniMethods[] = {
        "main", "([Ljava/lang/String;)V", &Java_net_minecraft_client_main_Main_main3,
        "toOptionalInt", "(Ljava/lang/Integer;)Ljava/util/OptionalInt;", &Java_net_minecraft_client_main_Main_toOptionalInt4,
        "getValue", "(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;", &Java_net_minecraft_client_main_Main_getValue5,
        "isNotEmpty", "(Ljava/lang/String;)Z", &Java_net_minecraft_client_main_Main_isNotEmpty6,
    };
    env->RegisterNatives(env->FindClass("net/minecraft/client/main/Main"), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /*PROTECTION TABLE*/
    classes[11] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("joptsimple/OptionParser"))))));
    classes[12] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("joptsimple/OptionSpecBuilder"))))));
    classes[13] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Integer"))))));
    classes[14] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("joptsimple/ArgumentAcceptingOptionSpec"))))));
    classes[15] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/io/File"))))));
    classes[16] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/String"))))));
    classes[17] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/util/Util"))))));
    classes[18] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/main/Main"))))));
    classes[19] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("joptsimple/OptionSet"))))));
    classes[20] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/util/List"))))));
    classes[21] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/System"))))));
    classes[22] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/io/PrintStream"))))));
    classes[23] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/net/Proxy"))))));
    classes[24] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/net/Proxy$Type"))))));
    classes[25] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/net/InetSocketAddress"))))));
    classes[26] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/main/Main$1"))))));
    classes[27] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/net/Authenticator"))))));
    classes[28] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("com/google/gson/GsonBuilder"))))));
    classes[29] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("com/mojang/authlib/properties/PropertyMap"))))));
    classes[30] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("com/mojang/authlib/properties/PropertyMap$Serializer"))))));
    classes[31] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/util/JSONUtils"))))));
    classes[32] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("joptsimple/OptionSpec"))))));
    classes[33] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/entity/player/PlayerEntity"))))));
    classes[34] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/util/UUID"))))));
    classes[35] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/crash/CrashReport"))))));
    classes[36] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/util/registry/Bootstrap"))))));
    classes[37] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/util/Session"))))));
    classes[38] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/GameConfiguration"))))));
    classes[39] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/GameConfiguration$UserInformation"))))));
    classes[40] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/renderer/ScreenSize"))))));
    classes[41] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/GameConfiguration$FolderInformation"))))));
    classes[42] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/GameConfiguration$GameInformation"))))));
    classes[43] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/GameConfiguration$ServerInformation"))))));
    classes[44] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/main/Main$2"))))));
    classes[45] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/util/DefaultUncaughtExceptionHandler"))))));
    classes[46] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Thread"))))));
    classes[47] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Runtime"))))));
    classes[48] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("com/mojang/blaze3d/Empty3i"))))));
    classes[49] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("com/mojang/blaze3d/systems/RenderSystem"))))));
    classes[50] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/Minecraft"))))));
    classes[51] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("org/apache/logging/log4j/Logger"))))));
    classes[52] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/resources/LanguageManager"))))));
    classes[53] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/GameSettings"))))));
    classes[54] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/main/Main$3"))))));
    classes[55] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Throwable"))))));
    classes[56] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("net/minecraft/client/util/UndeclaredException"))))));
    classes[57] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Exception"))))));
    classes[58] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/InterruptedException"))))));
    classes[59] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/util/OptionalInt"))))));

L141:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF("java.awt.headless");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("true");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
jclass tempClass_198 = env->FindClass("java/lang/System");
    cstack0.l = env->CallStaticObjectMethod(tempClass_198, env->GetStaticMethodID(tempClass_198, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L142:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF("file.encoding");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("UTF-8");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
jclass tempClass_199 = env->FindClass("java/lang/System");
    cstack0.l = env->CallStaticObjectMethod(tempClass_199, env->GetStaticMethodID(tempClass_199, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L143:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF("java.net.preferIPv4Stack");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("true");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
jclass tempClass_200 = env->FindClass("java/lang/System");
    cstack0.l = env->CallStaticObjectMethod(tempClass_200, env->GetStaticMethodID(tempClass_200, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L144:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF("java.util.concurrent.ForkJoinPool.common.parallelism");
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
jclass tempClass_201 = env->FindClass("java/lang/Runtime");
    cstack1.l = env->CallStaticObjectMethod(tempClass_201, env->GetStaticMethodID(tempClass_201, "getRuntime", "()Ljava/lang/Runtime;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
jclass tempClass_202 = env->FindClass("java/lang/Runtime");
    cstack1.i = env->CallIntMethod(cstack1.l, env->GetMethodID(tempClass_202, "availableProcessors", "()I"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 2
    cstack2.i = 1;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // ISUB; Stack pointer: 3
    cstack1.i = cstack1.i - cstack2.i;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
jclass tempClass_203 = env->FindClass("java/lang/String");
    cstack1.l = env->CallStaticObjectMethod(tempClass_203, env->GetStaticMethodID(tempClass_203, "valueOf", "(I)Ljava/lang/String;"), cstack1.i);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
jclass tempClass_204 = env->FindClass("java/lang/System");
    cstack0.l = env->CallStaticObjectMethod(tempClass_204, env->GetStaticMethodID(tempClass_204, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L145:
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
jclass tempClass_205 = env->FindClass("org/apache/logging/log4j/LogManager");
    cstack0.l = env->CallStaticObjectMethod(tempClass_205, env->GetStaticMethodID(tempClass_205, "getLogger", "()Lorg/apache/logging/log4j/Logger;"));
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    jfieldID fieldId_14 = env->GetStaticFieldID(env->FindClass("net/minecraft/client/main/Main"), "LOGGER", "Lorg/apache/logging/log4j/Logger;");
    env->SetStaticObjectField(env->FindClass("net/minecraft/client/main/Main"), fieldId_14, cstack0.l);
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { jthrowable exception = env->ExceptionOccurred(); env->ExceptionClear(); cstack_exception.l = exception; goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
#ifndef _Included_ru_itskekoff_j2c_loader_Protection
#define _Included_ru_itskekoff_j2c_loader_Protection
#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL Java_ru_itskekoff_j2c_loader_Protection_initialize(JNIEnv* env, jclass klass) {
        jclass clazz = env->FindClass("[Z");
        boolean_array_class = (jclass)env->NewGlobalRef(clazz);


    }
#ifdef __cplusplus
}
#endif
#endif
