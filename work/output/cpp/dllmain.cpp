
#include <string>
#include <stdexcept>
#include <cmath>

#include "table.h"
#include "jni.h"
#include <string>
#include <stdexcept>
#include <cmath>
#include <conio.h>
#include <fstream>
#include <wtypes.h>
#include <winioctl.h>
#include <sstream>
#include <thread>
#include <filesystem>
#include <random>
#include <wininet.h>
#include "xorstr.h"
#pragma comment(lib, "WinINet.lib")
#define _CRT_SECURE_NO_WARNINGS

long rtdsc = (long)__rdtsc();

std::string request(std::string url) {
    HINTERNET interwebs = InternetOpenA(("Mozilla/5.0"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
    HINTERNET urlFile;
    std::string rtn;
    if (interwebs) {
        urlFile = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);
        if (urlFile) {
            char buffer[2000];
            DWORD bytesRead;
            do {
                InternetReadFile(urlFile, buffer, 2000, &bytesRead);
                rtn.append(buffer, bytesRead);
                memset(buffer, 0, 2000);
            } while (bytesRead);
            InternetCloseHandle(interwebs);
            InternetCloseHandle(urlFile);
            return rtn;
        }
    }
    InternetCloseHandle(interwebs);
    return rtn;
}

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
    jarray create_array_value(JNIEnv* env, jint size);

    template <>
    jarray create_array_value<1>(JNIEnv* env, jint size) {
        return env->NewBooleanArray(size);
    }

    template <>
    jarray create_array_value<2>(JNIEnv* env, jint size) {
        return env->NewCharArray(size);
    }

    template <>
    jarray create_array_value<3>(JNIEnv* env, jint size) {
        return env->NewByteArray(size);
    }

    template <>
    jarray create_array_value<4>(JNIEnv* env, jint size) {
        return env->NewShortArray(size);
    }

    template <>
    jarray create_array_value<5>(JNIEnv* env, jint size) {
        return env->NewIntArray(size);
    }

    template <>
    jarray create_array_value<6>(JNIEnv* env, jint size) {
        return env->NewFloatArray(size);
    }

    template <>
    jarray create_array_value<7>(JNIEnv* env, jint size) {
        return env->NewLongArray(size);
    }

    template <>
    jarray create_array_value<8>(JNIEnv* env, jint size) {
        return env->NewDoubleArray(size);
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

// ru/kotopushka/Main
void JNICALL  Java_ru_kotopushka_Main_main0(JNIEnv *env, jclass clazz, jarray arg0) {
    // stack count: 4, locals count: 3, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {};

    clocal0.l = arg0; 

L1:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 0
    cstack0.i = (jint) 13;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEWARRAY; Stack pointer: 1
        cstack0.l = env->NewIntArray(cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 3
    cstack3.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 2
    cstack2.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 3
    cstack3.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 2
    cstack2.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 3
    cstack3.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_4; Stack pointer: 3
    cstack3.i = 4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_4; Stack pointer: 2
    cstack2.i = 4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_5; Stack pointer: 3
    cstack3.i = 5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_5; Stack pointer: 2
    cstack2.i = 5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 6;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 2
    cstack2.i = (jint) 6;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 7;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 2
    cstack2.i = (jint) 7;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 8;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 2
    cstack2.i = (jint) 8;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 9;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 2
    cstack2.i = (jint) 9;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 10;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 2
    cstack2.i = (jint) 10;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 11;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 2
    cstack2.i = (jint) 11;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 12;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 2
    cstack2.i = (jint) 12;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 13;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L2:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 0
    cstack0.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal2.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L3:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal2.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARRAYLENGTH; Stack pointer: 2
    cstack1.i = (env)->GetArrayLength((jarray)cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IF_ICMPGE; Stack pointer: 2
    if (cstack0.i >= cstack1.i) goto L4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L5:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal2.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IALOAD; Stack pointer: 3
    env->GetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(I)V"), cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IINC; Stack pointer: 0
    clocal2.i += 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[0]) ^ 3214775914213587475) ^ 899181197051473240) ^ 1281182480712094616) ^ -2217595970322499363) ^ -4660667373762626716) ^ 387998194771957072 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L7:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[1]) ^ 3435694089637230194) ^ 6650310099658095560) ^ -6264073889166917545) ^ -5141290482895563992) ^ -4646258212620489364) ^ 936676289115942238 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L8:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[2]) ^ -8509455695021198228) ^ 3799877280529705332) ^ -3357360904328862049) ^ 373762629319706688) ^ -7089010074319184845) ^ 879133875220898968 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L9:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[3]) ^ -5880335460810397012) ^ -6664681237362557448) ^ 3162852324892863323) ^ 4335255683370742839) ^ -5114630770454599621) ^ -5809543570747388818 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L10:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[4]) ^ 8295000641445490288) ^ -7869387345812424621) ^ -5327883213218033876) ^ -654622650000096574) ^ 3899771019481861425) ^ -3610780812854725105 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L11:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L12:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
void JNICALL  Java_ru_kotopushka_Main_testMathOperations1(JNIEnv *env, jclass clazz) {
    // stack count: 6, locals count: 4, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack4 = {}, cstack5 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

L13:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование математических операций...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L14:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[5]) ^ 6538604177787771537) ^ 4307378179482530098) ^ -2720849124925648049) ^ -6011352186586777552) ^ -6339608989441998051) ^ 7859573754001256968 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal0.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L15:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[6]) ^ 6431794728778984469) ^ -6843036796756657981) ^ 246850839181871865) ^ 1240686578958197496) ^ -3572601117273813977) ^ -8649373603864268935 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal1.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L16:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[7]) ^ -8726724229594323008) ^ -1002730116600469043) ^ 3382967249013131241) ^ -135723379261110813) ^ -4329206782722069447) ^ -6680780492961843710 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal2.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L17:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[8]) ^ 5772689851948508843) ^ -4701157921095558082) ^ 3630947662867305157) ^ 6578589599818476548) ^ -841616156550907874) ^ 1140099912974569671 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal3.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L18:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение суммы: %d + %d = %d (ожидалось 15)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 5
    cstack5.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L19:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L20;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 1
    cstack1.i = (jint) 15;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IF_ICMPEQ; Stack pointer: 2
    if (cstack0.i == cstack1.i) goto L20;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[10]) ^ -7779144521714842754) ^ -3206220990581658728) ^ -2571009676652813215) ^ 8647911174668974085) ^ 6782077121025359754) ^ 2095625263625744297 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L20:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение произведения: %d * %d = %d (ожидалось 50)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 5
    cstack5.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L21:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L22;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 1
    cstack1.i = (jint) 50;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IF_ICMPEQ; Stack pointer: 2
    if (cstack0.i == cstack1.i) goto L22;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[11]) ^ -6026397698907641943) ^ 2115236043766555633) ^ -4373115548408180172) ^ -1008922837005047127) ^ -4199499392204617840) ^ -2363493033686447560 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L22:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение разности: %d - %d = %d (ожидалось -5)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 5
    cstack5.i = clocal2.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L23:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L24;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal2.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 1
    cstack1.i = (jint) -5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IF_ICMPEQ; Stack pointer: 2
    if (cstack0.i == cstack1.i) goto L24;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal2.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[12]) ^ -1751340082308492391) ^ 4268463661987838046) ^ 3312691773165698157) ^ 547774970916942860) ^ 8007055340012758185) ^ 6873252012586650127 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L24:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение частного: %d / %d = %d (ожидалось 2)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intB", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "intA", "I"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 5
    cstack5.i = clocal3.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[4].applyDecryption(), (jmethodID)(((((((__int64)(methods[9]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L25:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L26;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal3.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 1
    cstack1.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IF_ICMPEQ; Stack pointer: 2
    if (cstack0.i == cstack1.i) goto L26;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal3.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[13]) ^ 6431794728778984469) ^ -6843036796756657981) ^ 246850839181871865) ^ 1240686578958197496) ^ -3572601117273813977) ^ -8649373603864268935 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L26:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Математические операции пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L27:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L28:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
void JNICALL  Java_ru_kotopushka_Main_testStringManipulation2(JNIEnv *env, jclass clazz) {
    // stack count: 6, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack4 = {}, cstack5 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {};

L29:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование манипуляций со строками...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L30:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF("Hello, World!");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal0.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L31:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[14]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L32:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение реверсирования строки: '%s' (ожидалось '!dlroW ,olleH')\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 2
    cstack2.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L33:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L34;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("!dlroW ,olleH");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(classes[6].applyDecryption(), "equals", "(Ljava/lang/Object;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L34;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[15]) ^ 6795125501449354181) ^ 7626397384364020337) ^ 1042490563435889366) ^ 7094351116504739555) ^ -7247535787120911156) ^ -4535509768447500218 ^ rtdsc), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L34:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Манипуляции со строками пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L35:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L36:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
void JNICALL  Java_ru_kotopushka_Main_testArraySorting3(JNIEnv *env, jclass clazz) {
    // stack count: 6, locals count: 3, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack4 = {}, cstack5 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {};

L37:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование сортировки массива...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L38:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_5; Stack pointer: 0
    cstack0.i = 5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEWARRAY; Stack pointer: 1
        cstack0.l = env->NewIntArray(cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_5; Stack pointer: 3
    cstack3.i = 5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 2
    cstack2.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 3
    cstack3.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 2
    cstack2.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 8;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 3
    cstack3.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_4; Stack pointer: 2
    cstack2.i = 4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 3
    cstack3.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal0.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L39:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[16]) ^ 8804393643623170339) ^ -2179870445820859384) ^ 3003724380042059823) ^ -7903523670082283461) ^ -690295020025282943) ^ -2942967518793864942 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L40:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_5; Stack pointer: 0
    cstack0.i = 5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEWARRAY; Stack pointer: 1
        cstack0.l = env->NewIntArray(cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 3
    cstack3.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 2
    cstack2.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 3
    cstack3.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 2
    cstack2.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 3
    cstack3.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_5; Stack pointer: 3
    cstack3.i = 5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_4; Stack pointer: 2
    cstack2.i = 4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 3
    cstack3.i = (jint) 8;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IASTORE; Stack pointer: 4
    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal2.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L41:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение сортировки массива: %s (ожидалось %s)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 2
    cstack2.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L42:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[7].applyDecryption(), (jmethodID)(((((((__int64)(methods[17]) ^ 8547872324256554992) ^ -5498889650702349623) ^ -759615308319477592) ^ -2562482895169281656) ^ 217631773358662865) ^ -8994628070054659371 ^ rtdsc), cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal2.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L43:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[7].applyDecryption(), (jmethodID)(((((((__int64)(methods[17]) ^ 8547872324256554992) ^ -5498889650702349623) ^ -759615308319477592) ^ -2562482895169281656) ^ 217631773358662865) ^ -8994628070054659371 ^ rtdsc), cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L44:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L45:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L46;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal2.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = (jint) env->CallStaticBooleanMethod(classes[7].applyDecryption(), env->GetStaticMethodID(classes[7].applyDecryption(), "equals", "([I[I)Z"), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L46;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = env->NewStringUTF("Ошибка: Массив не отсортирован правильно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L46:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сортировка массива пройдена успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L47:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L48:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
void JNICALL  Java_ru_kotopushka_Main_testFloatOperations4(JNIEnv *env, jclass clazz) {
    // stack count: 6, locals count: 4, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack4 = {}, cstack5 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

L49:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование операций с float...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L50:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatA", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatB", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[19]) ^ 2776589208511787272) ^ 8792188975319195458) ^ -2879977069776451549) ^ 196612533669747764) ^ -1915273243307648670) ^ -7720594267754770772 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal0.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L51:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatA", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatB", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[20]) ^ 6503001029549831613) ^ 6739487881502629474) ^ 4417298890543353992) ^ -3594005928541807672) ^ -5417273030620947594) ^ 2356591301379415000 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal1.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L52:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatA", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatB", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[21]) ^ -8509455695021198228) ^ 3799877280529705332) ^ -3357360904328862049) ^ 373762629319706688) ^ -7089010074319184845) ^ 879133875220898968 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal2.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L53:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatA", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatB", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[22]) ^ -6500187552847382070) ^ -6006390549655174389) ^ -1396127087722691592) ^ 3822631916442235288) ^ -531769307223556632) ^ 2471676124874534244 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal3.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L54:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение суммы: %.2f + %.2f = %.2f (ожидалось 8.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatA", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatB", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 5
    cstack5.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L55:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L56;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 0
    cstack0.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.f = 8.0f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSUB; Stack pointer: 2
    cstack0.f = cstack0.f - cstack1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.f = env->CallStaticFloatMethod(classes[9].applyDecryption(), (jmethodID)(((((((__int64)(methods[24]) ^ 5808293000186448767) ^ -7133267623115657459) ^ -3507200348306729587) ^ 4161243341773506668) ^ -1763952111076991035) ^ 6643082369891378935 ^ rtdsc), cstack0.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // F2D; Stack pointer: 1
    cstack0.d = (jdouble) cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 1.0E-4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DCMPG; Stack pointer: 4
    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFLT; Stack pointer: 1
    if (cstack0.i < 0) goto L56;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 2
    cstack2.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[25]) ^ 6649063263352109249) ^ -2040429403921450902) ^ -6493477309865154129) ^ 1750172591833982294) ^ -6332404413165896671) ^ -1089459235681526257 ^ rtdsc), cstack2.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L56:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение разности: %.2f - %.2f = %.2f (ожидалось 3.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatA", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatB", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 5
    cstack5.f = clocal2.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L57:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L58;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 0
    cstack0.f = clocal2.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.f = 3.0f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSUB; Stack pointer: 2
    cstack0.f = cstack0.f - cstack1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.f = env->CallStaticFloatMethod(classes[9].applyDecryption(), (jmethodID)(((((((__int64)(methods[24]) ^ 5808293000186448767) ^ -7133267623115657459) ^ -3507200348306729587) ^ 4161243341773506668) ^ -1763952111076991035) ^ 6643082369891378935 ^ rtdsc), cstack0.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // F2D; Stack pointer: 1
    cstack0.d = (jdouble) cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 1.0E-4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DCMPG; Stack pointer: 4
    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFLT; Stack pointer: 1
    if (cstack0.i < 0) goto L58;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 2
    cstack2.f = clocal2.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[26]) ^ 8441062875247767924) ^ 1797439446768013915) ^ 2208084655788042323) ^ 4689555870375693391) ^ 2984639645526846940) ^ -7056831354210633658 ^ rtdsc), cstack2.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L58:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение частного: %.2f / %.2f = %.2f (ожидалось 2.20)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatA", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "floatB", "F"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 5
    cstack5.f = clocal3.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod(classes[8].applyDecryption(), (jmethodID)(((((((__int64)(methods[23]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L59:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L60;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 0
    cstack0.f = clocal3.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.f = 2.2f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSUB; Stack pointer: 2
    cstack0.f = cstack0.f - cstack1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.f = env->CallStaticFloatMethod(classes[9].applyDecryption(), (jmethodID)(((((((__int64)(methods[24]) ^ 5808293000186448767) ^ -7133267623115657459) ^ -3507200348306729587) ^ 4161243341773506668) ^ -1763952111076991035) ^ 6643082369891378935 ^ rtdsc), cstack0.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // F2D; Stack pointer: 1
    cstack0.d = (jdouble) cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 1.0E-4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DCMPG; Stack pointer: 4
    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFLT; Stack pointer: 1
    if (cstack0.i < 0) goto L60;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 2
    cstack2.f = clocal3.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[27]) ^ -8035665841081458102) ^ -6525240195463148967) ^ -6334349365014350629) ^ -4457792128422543022) ^ 7690003914409305563) ^ -3956035283340082837 ^ rtdsc), cstack2.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L60:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Операции с float пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L61:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L62:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
void JNICALL  Java_ru_kotopushka_Main_testDoubleOperations5(JNIEnv *env, jclass clazz) {
    // stack count: 7, locals count: 8, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack4 = {}, cstack5 = {}, cstack6 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {}, clocal4 = {}, clocal5 = {}, clocal6 = {}, clocal7 = {};

L63:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование операций с double...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L64:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[28]) ^ 5077981826880093293) ^ -127169352004293400) ^ -4293551575982778422) ^ -4112905199280793433) ^ 2811704767288015981) ^ 5426590981486533606 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal0.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L65:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[29]) ^ 5263296849770828793) ^ 8056069252622428296) ^ -4700659934687755838) ^ -4619253458103855478) ^ 3748449887251203790) ^ 472286623208676804 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal2.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L66:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[30]) ^ 6613460115114169325) ^ 391680293803681178) ^ 644670705603847912) ^ 4167518849878952174) ^ -5410068454344846214) ^ -6592441684008400929 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal4.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L67:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[31]) ^ 3948736728370460888) ^ -5158395568583442874) ^ 1262605491851124580) ^ 2623372049577918607) ^ -6462111799388380982) ^ -5406746690661955111 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal6.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L68:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение суммы: %.2f + %.2f = %.2f (ожидалось 13.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 5
    cstack5.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L69:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L70;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 13.0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSUB; Stack pointer: 4
    cstack0.d = cstack0.d - cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.d = env->CallStaticDoubleMethod(classes[9].applyDecryption(), (jmethodID)(((((((__int64)(methods[33]) ^ 3143569617737707627) ^ 5763400592501737401) ^ -2889265562059452919) ^ 2617096545767440397) ^ -2815995456120525803) ^ 7828777358942857457 ^ rtdsc), cstack0.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 1.0E-4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DCMPG; Stack pointer: 4
    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFLT; Stack pointer: 1
    if (cstack0.i < 0) goto L70;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[34]) ^ 111865826063046131) ^ 3242113112932071406) ^ -2262042283529174880) ^ -1347534262336318506) ^ -2967316592646150733) ^ -6534899274408324955 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L70:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение произведения: %.2f * %.2f = %.2f (ожидалось 30.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 5
    cstack5.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L71:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L72;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 30.0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSUB; Stack pointer: 4
    cstack0.d = cstack0.d - cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.d = env->CallStaticDoubleMethod(classes[9].applyDecryption(), (jmethodID)(((((((__int64)(methods[33]) ^ 3143569617737707627) ^ 5763400592501737401) ^ -2889265562059452919) ^ 2617096545767440397) ^ -2815995456120525803) ^ 7828777358942857457 ^ rtdsc), cstack0.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 1.0E-4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DCMPG; Stack pointer: 4
    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFLT; Stack pointer: 1
    if (cstack0.i < 0) goto L72;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[35]) ^ 6356938795747553978) ^ -2927338911077809061) ^ -3118668982757689503) ^ -8938184457507532230) ^ -4502141656665933110) ^ 5802641842735323554 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L72:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение разности: %.2f - %.2f = %.2f (ожидалось 7.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 5
    cstack5.d = clocal4.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L73:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L74;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal4.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 7.0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSUB; Stack pointer: 4
    cstack0.d = cstack0.d - cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.d = env->CallStaticDoubleMethod(classes[9].applyDecryption(), (jmethodID)(((((((__int64)(methods[33]) ^ 3143569617737707627) ^ 5763400592501737401) ^ -2889265562059452919) ^ 2617096545767440397) ^ -2815995456120525803) ^ 7828777358942857457 ^ rtdsc), cstack0.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 1.0E-4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DCMPG; Stack pointer: 4
    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFLT; Stack pointer: 1
    if (cstack0.i < 0) goto L74;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal4.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[36]) ^ -7412164112488922400) ^ -6235009369104149489) ^ -2580298173230781880) ^ -7378348891237852194) ^ 5881354903917515326) ^ -801747174796244499 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L74:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение частного: %.2f / %.2f = %.2f (ожидалось 3.33)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, classes[3].applyDecryption(), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleA", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 4
    cstack4.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "doubleB", "D"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 4
    cstack4.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 5
    cstack5.d = clocal6.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod(classes[10].applyDecryption(), (jmethodID)(((((((__int64)(methods[32]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L75:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField(classes[2].applyDecryption(), env->GetStaticFieldID(classes[2].applyDecryption(), "$assertionsDisabled", "Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L76;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal6.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 3.33;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSUB; Stack pointer: 4
    cstack0.d = cstack0.d - cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.d = env->CallStaticDoubleMethod(classes[9].applyDecryption(), (jmethodID)(((((((__int64)(methods[33]) ^ 3143569617737707627) ^ 5763400592501737401) ^ -2889265562059452919) ^ 2617096545767440397) ^ -2815995456120525803) ^ 7828777358942857457 ^ rtdsc), cstack0.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.d = 1.0E-4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DCMPG; Stack pointer: 4
    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFLT; Stack pointer: 1
    if (cstack0.i < 0) goto L76;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[5].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal6.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod(classes[2].applyDecryption(), (jmethodID)(((((((__int64)(methods[37]) ^ 4201608411181525592) ^ -2787897869178400580) ^ 5830873396749680865) ^ 715511804408733525) ^ 8302493023903004778) ^ 7656150125847662238 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[5].applyDecryption(), env->GetMethodID(classes[5].applyDecryption(), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew(classes[2].applyDecryption(), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L76:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField(classes[0].applyDecryption(), env->GetStaticFieldID(classes[0].applyDecryption(), "out", "Ljava/io/PrintStream;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Операции с double пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID(classes[1].applyDecryption(), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L77:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L78:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
jint JNICALL  Java_ru_kotopushka_Main_add6(JNIEnv *env, jclass clazz, jint arg0, jint arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.i = arg0; clocal1.i = arg1; 

L79:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 1
    cstack1.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IADD; Stack pointer: 2
    cstack0.i = cstack0.i + cstack1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IRETURN; Stack pointer: 1
    return (jint) cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L80:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jint) 0; }

    return (jint) 0;
}
jint JNICALL  Java_ru_kotopushka_Main_multiply7(JNIEnv *env, jclass clazz, jint arg0, jint arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.i = arg0; clocal1.i = arg1; 

L81:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 1
    cstack1.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IMUL; Stack pointer: 2
    cstack0.i = cstack0.i * cstack1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IRETURN; Stack pointer: 1
    return (jint) cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L82:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jint) 0; }

    return (jint) 0;
}
jint JNICALL  Java_ru_kotopushka_Main_subtract8(JNIEnv *env, jclass clazz, jint arg0, jint arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.i = arg0; clocal1.i = arg1; 

L83:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 1
    cstack1.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISUB; Stack pointer: 2
    cstack0.i = cstack0.i - cstack1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IRETURN; Stack pointer: 1
    return (jint) cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L84:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jint) 0; }

    return (jint) 0;
}
jint JNICALL  Java_ru_kotopushka_Main_divide9(JNIEnv *env, jclass clazz, jint arg0, jint arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.i = arg0; clocal1.i = arg1; 

L85:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 0
    cstack0.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 1
    cstack1.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IDIV; Stack pointer: 2
    cstack0.i = cstack0.i / cstack1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IRETURN; Stack pointer: 1
    return (jint) cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L86:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jint) 0; }

    return (jint) 0;
}
jfloat JNICALL  Java_ru_kotopushka_Main_add10(JNIEnv *env, jclass clazz, jfloat arg0, jfloat arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.f = arg0; clocal1.f = arg1; 

L87:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 0
    cstack0.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 1
    cstack1.f = clocal1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FADD; Stack pointer: 2
    cstack0.f = cstack0.f + cstack1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FRETURN; Stack pointer: 1
    return (jfloat) cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L88:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jfloat) 0; }

    return (jfloat) 0;
}
jfloat JNICALL  Java_ru_kotopushka_Main_multiply11(JNIEnv *env, jclass clazz, jfloat arg0, jfloat arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.f = arg0; clocal1.f = arg1; 

L89:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 0
    cstack0.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 1
    cstack1.f = clocal1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FMUL; Stack pointer: 2
    cstack0.f = cstack0.f * cstack1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FRETURN; Stack pointer: 1
    return (jfloat) cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L90:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jfloat) 0; }

    return (jfloat) 0;
}
jfloat JNICALL  Java_ru_kotopushka_Main_subtract12(JNIEnv *env, jclass clazz, jfloat arg0, jfloat arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.f = arg0; clocal1.f = arg1; 

L91:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 0
    cstack0.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 1
    cstack1.f = clocal1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSUB; Stack pointer: 2
    cstack0.f = cstack0.f - cstack1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FRETURN; Stack pointer: 1
    return (jfloat) cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L92:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jfloat) 0; }

    return (jfloat) 0;
}
jfloat JNICALL  Java_ru_kotopushka_Main_divide13(JNIEnv *env, jclass clazz, jfloat arg0, jfloat arg1) {
    // stack count: 2, locals count: 2, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {};

    clocal0.f = arg0; clocal1.f = arg1; 

L93:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 0
    cstack0.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 1
    cstack1.f = clocal1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FDIV; Stack pointer: 2
    cstack0.f = cstack0.f / cstack1.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FRETURN; Stack pointer: 1
    return (jfloat) cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L94:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jfloat) 0; }

    return (jfloat) 0;
}
jdouble JNICALL  Java_ru_kotopushka_Main_add14(JNIEnv *env, jclass clazz, jdouble arg0, jdouble arg1) {
    // stack count: 4, locals count: 4, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {}, clocal4 = {}, clocal5 = {};

    clocal0.d = arg0; clocal2.d = arg1; 

L95:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DADD; Stack pointer: 4
    cstack0.d = cstack0.d + cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DRETURN; Stack pointer: 2
    return (jdouble) cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L96:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jdouble) 0; }

    return (jdouble) 0;
}
jdouble JNICALL  Java_ru_kotopushka_Main_multiply15(JNIEnv *env, jclass clazz, jdouble arg0, jdouble arg1) {
    // stack count: 4, locals count: 4, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {}, clocal4 = {}, clocal5 = {};

    clocal0.d = arg0; clocal2.d = arg1; 

L97:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DMUL; Stack pointer: 4
    cstack0.d = cstack0.d * cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DRETURN; Stack pointer: 2
    return (jdouble) cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L98:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jdouble) 0; }

    return (jdouble) 0;
}
jdouble JNICALL  Java_ru_kotopushka_Main_subtract16(JNIEnv *env, jclass clazz, jdouble arg0, jdouble arg1) {
    // stack count: 4, locals count: 4, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {}, clocal4 = {}, clocal5 = {};

    clocal0.d = arg0; clocal2.d = arg1; 

L99:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSUB; Stack pointer: 4
    cstack0.d = cstack0.d - cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DRETURN; Stack pointer: 2
    return (jdouble) cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L100:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jdouble) 0; }

    return (jdouble) 0;
}
jdouble JNICALL  Java_ru_kotopushka_Main_divide17(JNIEnv *env, jclass clazz, jdouble arg0, jdouble arg1) {
    // stack count: 4, locals count: 4, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack_exception = {};
    jvalue clocal0 = {}, clocal1 = {}, clocal2 = {}, clocal3 = {}, clocal4 = {}, clocal5 = {};

    clocal0.d = arg0; clocal2.d = arg1; 

L101:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 0
    cstack0.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DDIV; Stack pointer: 4
    cstack0.d = cstack0.d / cstack2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DRETURN; Stack pointer: 2
    return (jdouble) cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L102:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jdouble) 0; }

    return (jdouble) 0;
}
jobject JNICALL  Java_ru_kotopushka_Main_reverseString18(JNIEnv *env, jclass clazz, jobject arg0) {
    // stack count: 3, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = arg0; 

L103:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject(classes[11].applyDecryption())) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, classes[11].applyDecryption(), env->GetMethodID(classes[11].applyDecryption(), "<init>", "(Ljava/lang/String;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "reverse", "()Ljava/lang/StringBuilder;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID(classes[11].applyDecryption(), "toString", "()Ljava/lang/String;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L104:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
jarray JNICALL  Java_ru_kotopushka_Main_sortArray19(JNIEnv *env, jclass clazz, jarray arg0) {
    // stack count: 1, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = arg0; 

L105:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod(classes[7].applyDecryption(), (jmethodID)(((((((__int64)(methods[38]) ^ 8330603789683430212) ^ 8145247030171994915) ^ 5980712845022515699) ^ -3071968908045066454) ^ 2977435060660810968) ^ 1892201635472149567 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L106:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jarray) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L107:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jarray) 0; }

    return (jarray) 0;
}
extern "C" JNIEXPORT void JNICALL  Java_ru_kotopushka_Main__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 2, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    JNINativeMethod jniMethods[] = {
        (char*)"main", (char*)"([Ljava/lang/String;)V", &Java_ru_kotopushka_Main_main0,
        (char*)"testMathOperations", (char*)"()V", &Java_ru_kotopushka_Main_testMathOperations1,
        (char*)"testStringManipulation", (char*)"()V", &Java_ru_kotopushka_Main_testStringManipulation2,
        (char*)"testArraySorting", (char*)"()V", &Java_ru_kotopushka_Main_testArraySorting3,
        (char*)"testFloatOperations", (char*)"()V", &Java_ru_kotopushka_Main_testFloatOperations4,
        (char*)"testDoubleOperations", (char*)"()V", &Java_ru_kotopushka_Main_testDoubleOperations5,
        (char*)"add", (char*)"(II)I", &Java_ru_kotopushka_Main_add6,
        (char*)"multiply", (char*)"(II)I", &Java_ru_kotopushka_Main_multiply7,
        (char*)"subtract", (char*)"(II)I", &Java_ru_kotopushka_Main_subtract8,
        (char*)"divide", (char*)"(II)I", &Java_ru_kotopushka_Main_divide9,
        (char*)"add", (char*)"(FF)F", &Java_ru_kotopushka_Main_add10,
        (char*)"multiply", (char*)"(FF)F", &Java_ru_kotopushka_Main_multiply11,
        (char*)"subtract", (char*)"(FF)F", &Java_ru_kotopushka_Main_subtract12,
        (char*)"divide", (char*)"(FF)F", &Java_ru_kotopushka_Main_divide13,
        (char*)"add", (char*)"(DD)D", &Java_ru_kotopushka_Main_add14,
        (char*)"multiply", (char*)"(DD)D", &Java_ru_kotopushka_Main_multiply15,
        (char*)"subtract", (char*)"(DD)D", &Java_ru_kotopushka_Main_subtract16,
        (char*)"divide", (char*)"(DD)D", &Java_ru_kotopushka_Main_divide17,
        (char*)"reverseString", (char*)"(Ljava/lang/String;)Ljava/lang/String;", &Java_ru_kotopushka_Main_reverseString18,
        (char*)"sortArray", (char*)"([I)[I", &Java_ru_kotopushka_Main_sortArray19,
    };
    env->RegisterNatives(env->FindClass("ru/kotopushka/Main"), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /* CLASS REFERENCE TABLE */
    classes[0] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/System"))))));
    classes[1] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/io/PrintStream"))))));
    classes[2] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("ru/kotopushka/Main"))))));
    classes[3] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Object"))))));
    classes[4] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Integer"))))));
    classes[5] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/AssertionError"))))));
    classes[6] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/String"))))));
    classes[7] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/util/Arrays"))))));
    classes[8] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Float"))))));
    classes[9] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Math"))))));
    classes[10] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/Double"))))));
    classes[11] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass("java/lang/StringBuilder"))))));

    /* METHOD REFERENCE TABLE */
    methods[0] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=181&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "testMathOperations", "()V") ^ 1865563179518007214), rtdsc)));
    methods[1] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=391&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "testStringManipulation", "()V") ^ 1465662064986789092), rtdsc)));
    methods[2] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=76&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "testArraySorting", "()V") ^ 1563158617720825606), rtdsc)));
    methods[3] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=206&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "testFloatOperations", "()V") ^ 1771371593217338955), rtdsc)));
    methods[4] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=313&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "testDoubleOperations", "()V") ^ 1224399407976723998), rtdsc)));
    methods[5] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=148&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "add", "(II)I") ^ 1920095147143419919), rtdsc)));
    methods[6] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=276&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "multiply", "(II)I") ^ 1285541317058807807), rtdsc)));
    methods[7] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=371&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "subtract", "(II)I") ^ 1128555338459371611), rtdsc)));
    methods[8] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=55&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "divide", "(II)I") ^ 1650740266859790056), rtdsc)));
    methods[9] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=150&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/lang/Integer"), "valueOf", "(I)Ljava/lang/Integer;") ^ 1916790180710051663), rtdsc)));
    methods[10] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=41&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello0_proxy1", "(I)Ljava/lang/String;") ^ 1620995551779606568), rtdsc)));
    methods[11] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=245&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello1_proxy2", "(I)Ljava/lang/String;") ^ 1759804206405582763), rtdsc)));
    methods[12] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=387&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello2_proxy3", "(I)Ljava/lang/String;") ^ 1472271997853525604), rtdsc)));
    methods[13] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=276&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello3_proxy4", "(I)Ljava/lang/String;") ^ 1285541317058807807), rtdsc)));
    methods[14] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=120&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "reverseString", "(Ljava/lang/String;)Ljava/lang/String;") ^ 1543328810530681478), rtdsc)));
    methods[15] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=230&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello4_proxy5", "(Ljava/lang/String;)Ljava/lang/String;") ^ 1731711978837050698), rtdsc)));
    methods[16] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=4&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "sortArray", "([I)[I") ^ 1682137460861690377), rtdsc)));
    methods[17] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=178&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/util/Arrays"), "toString", "([I)Ljava/lang/String;") ^ 1870520629168059598), rtdsc)));
    methods[18] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=238&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/util/Arrays"), "equals", "([I[I)Z") ^ 1718492108808610378), rtdsc)));
    methods[19] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=170&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "add", "(FF)F") ^ 1830861014787771341), rtdsc)));
    methods[20] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=20&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "multiply", "(FF)F") ^ 1708577205213538314), rtdsc)));
    methods[21] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=76&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "subtract", "(FF)F") ^ 1563158617720825606), rtdsc)));
    methods[22] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=106&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "divide", "(FF)F") ^ 1513584099745465285), rtdsc)));
    methods[23] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=179&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/lang/Float"), "valueOf", "(F)Ljava/lang/Float;") ^ 1868868145951375470), rtdsc)));
    methods[24] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=183&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/lang/Math"), "abs", "(F)F") ^ 1862258213084638958), rtdsc)));
    methods[25] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=237&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello5_proxy6", "(F)Ljava/lang/String;") ^ 1720144587730327210), rtdsc)));
    methods[26] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=306&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello6_proxy7", "(F)Ljava/lang/String;") ^ 1235966799083447486), rtdsc)));
    methods[27] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=215&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello7_proxy8", "(F)Ljava/lang/String;") ^ 1809378724380943084), rtdsc)));
    methods[28] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=218&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "add", "(DD)D") ^ 1804421270435923404), rtdsc)));
    methods[29] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=300&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "multiply", "(DD)D") ^ 1193002213974823677), rtdsc)));
    methods[30] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=109&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "subtract", "(DD)D") ^ 1508626645800445605), rtdsc)));
    methods[31] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=363&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "divide", "(DD)D") ^ 1088895728374050651), rtdsc)));
    methods[32] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=125&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/lang/Double"), "valueOf", "(D)Ljava/lang/Double;") ^ 1535066390152293541), rtdsc)));
    methods[33] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=437&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/lang/Math"), "abs", "(D)D") ^ 1442527287068309411), rtdsc)));
    methods[34] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=424&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello8_proxy9", "(D)Ljava/lang/String;") ^ 1411130093066409090), rtdsc)));
    methods[35] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=27&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello9_proxy10", "(D)Ljava/lang/String;") ^ 1697009818401782121), rtdsc)));
    methods[36] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=308&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello10_proxy11", "(D)Ljava/lang/String;") ^ 1232661828355111934), rtdsc)));
    methods[37] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=228&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("ru/kotopushka/Main"), "$hello11_proxy12", "(D)Ljava/lang/String;") ^ 1735016949565386250), rtdsc)));
    methods[38] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=441&rtdsc={}", ((__int64)env->GetStaticMethodID(env->FindClass("java/util/Arrays"), "sort", "([I)V") ^ 1435917354201572899), rtdsc)));

L108:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->FindClass("ru/kotopushka/Main");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(env->FindClass("java/lang/Class"), "desiredAssertionStatus", "()Z"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L109;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 0
    cstack0.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L110;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L109:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 0
    cstack0.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L110:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticBooleanField(env->FindClass("ru/kotopushka/Main"), env->GetStaticFieldID(env->FindClass("ru/kotopushka/Main"), "$assertionsDisabled", "Z"),  cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L111:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_5; Stack pointer: 0
    cstack0.i = 5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticIntField(env->FindClass("ru/kotopushka/Main"), env->GetStaticFieldID(env->FindClass("ru/kotopushka/Main"), "intA", "I"), (jint) cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L112:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // BIPUSH; Stack pointer: 0
    cstack0.i = (jint) 10;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticIntField(env->FindClass("ru/kotopushka/Main"), env->GetStaticFieldID(env->FindClass("ru/kotopushka/Main"), "intB", "I"), (jint) cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L113:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.f = 5.5f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticFloatField(env->FindClass("ru/kotopushka/Main"), env->GetStaticFieldID(env->FindClass("ru/kotopushka/Main"), "floatA", "F"), cstack0.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L114:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.f = 2.5f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticFloatField(env->FindClass("ru/kotopushka/Main"), env->GetStaticFieldID(env->FindClass("ru/kotopushka/Main"), "floatB", "F"), cstack0.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L115:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.d = 10.0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 2
    env->SetStaticDoubleField(env->FindClass("ru/kotopushka/Main"), env->GetStaticFieldID(env->FindClass("ru/kotopushka/Main"), "doubleA", "D"), cstack0.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L116:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.d = 3.0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 2
    env->SetStaticDoubleField(env->FindClass("ru/kotopushka/Main"), env->GetStaticFieldID(env->FindClass("ru/kotopushka/Main"), "doubleB", "D"), cstack0.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
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
