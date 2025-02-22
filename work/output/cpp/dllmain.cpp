
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
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[0]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc));
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
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(I)V"), cstack1.i);
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
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[0]) ^ -7412164112488922400) ^ -6235009369104149489) ^ -2580298173230781880) ^ -7378348891237852194) ^ 5881354903917515326) ^ -801747174796244499 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L7:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[1]) ^ 6688316052440567113) ^ -3524017591875167742) ^ 4010190531838376576) ^ -4100354187364869717) ^ -4480527914952727081) ^ -2597713052603474505 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L8:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[2]) ^ 7781957994122325002) ^ 3939318318134146517) ^ 5592181479473475615) ^ -8419285186768546469) ^ 5715624610544720339) ^ 2732642162628204948 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L9:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[3]) ^ -7487020049815320187) ^ -2319311483425300569) ^ -5945817999465310545) ^ 889524146005969696) ^ 4951814368820363489) ^ -4796475810496138218 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L10:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[4]) ^ 8038479317783907645) ^ 7258337527310604052) ^ -9091222905874538587) ^ 4686418120617937934) ^ 4807697817160774530) ^ 8784302713888999378 ^ rtdsc));
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
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[1]) ^ -8288537523892522805) ^ -8895737890573223965) ^ 7544126799501677406) ^ -2549931883253357940) ^ -7074600913177047493) ^ 1427811973859851430 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование математических операций...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L14:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[2]) ^ -69799555977171428) ^ -3992603977628267752) ^ -2659862145656183631) ^ -4274366528962105888) ^ -1129849259870085792) ^ -8591831189969225665 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[3]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[5]) ^ 76262677825106207) ^ 5674222814952170783) ^ 4876105731939827160) ^ 1069811991413684078) ^ -2044980633825100276) ^ 6408862346679384694 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal0.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L15:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[4]) ^ 8839996791861110263) ^ -4611980143545991464) ^ -4134423631131974922) ^ 8125874145582298275) ^ -1612630978846333400) ^ 2560014929533009730 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[5]) ^ 6834378290537812044) ^ 6142809196410303498) ^ -6900585668570131545) ^ 1243824333010920249) ^ -5395659293202708862) ^ -6043763589664415762 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[6]) ^ -2702569435333457881) ^ 5523433030492579786) ^ -8375147154045922836) ^ -4802679061859259908) ^ -5878441012178956471) ^ 5108082529837819632 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal1.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L16:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[6]) ^ -2264382729631657678) ^ -2369574743480175136) ^ -4213987603557376672) ^ -7216887561556539739) ^ -8623835142633934518) ^ -5230069081345004140 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[7]) ^ -8363393461218920593) ^ -4980040004894375045) ^ 4178606973267148742) ^ 5717941153990463950) ^ -8004141448274199330) ^ -2566916657545074994 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[7]) ^ 5224044060682370929) ^ -8907086633133406480) ^ 3242416301613232369) ^ 1231273321094996533) ^ 1896573393333001496) ^ 1980540444425592348 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal2.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L17:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[8]) ^ -361924032171661291) ^ -4879513484784625911) ^ 714946177156313700) ^ 3484020499700898500) ^ 700413496629877770) ^ -1699730115847343150 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[9]) ^ 621258828240726182) ^ 8931630012923636644) ^ 6069565314025886115) ^ -8596435278123538097) ^ -7557382632153295102) ^ -5867085980347464792 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ 7781957994122325002) ^ 3939318318134146517) ^ 5592181479473475615) ^ -8419285186768546469) ^ 5715624610544720339) ^ 2732642162628204948 ^ rtdsc), cstack0.i, cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal3.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L18:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[10]) ^ 2484464736612264705) ^ 7905279468162837300) ^ 494831257331013077) ^ 7954999562332752153) ^ -85010491102652404) ^ -828493193632888257 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение суммы: %d + %d = %d (ожидалось 15)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[11]) ^ 1355219642397599596) ^ 2874053255878655122) ^ 6050988320869948783) ^ -3755467258223120127) ^ 9087917015930502249) ^ 6784913207928174642 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[12]) ^ -507986265973938927) ^ 3900403800639454466) ^ -6821021696144729795) ^ -1860158029264826057) ^ 1615544870584892255) ^ 1746320421213598108 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L19:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[13]) ^ -4088335848914738337) ^ -2826812390968059927) ^ -6582329778868524545) ^ 7274638961912453937) ^ 4202413283943176695) ^ 6669828384433055398 ^ rtdsc));
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[12]) ^ -109052345065629292) ^ -2509015789674550913) ^ 5283214086349837280) ^ 1576160250236746122) ^ -2981725753788288086) ^ -7083577368752310121 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L20:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[14]) ^ -728904437102614350) ^ -1850725101967167854) ^ 724234678029249662) ^ 1063536487603205868) ^ 1601135709442754903) ^ 1197642326869612941 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение произведения: %d * %d = %d (ожидалось 50)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[15]) ^ -7412164112488922400) ^ -6235009369104149489) ^ -2580298173230781880) ^ -7378348891237852194) ^ 5881354903917515326) ^ -801747174796244499 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[16]) ^ 5117234615968551157) ^ -1610757539958010239) ^ 6210116265720752283) ^ 8483312095229906173) ^ 4663581265501185571) ^ 3918337160269618062 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L21:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[17]) ^ 5077981826880093293) ^ -127169352004293400) ^ -4293551575982778422) ^ -4112905199280793433) ^ 2811704767288015981) ^ 5426590981486533606 ^ rtdsc));
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal1.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[16]) ^ 3948736728370460888) ^ -5158395568583442874) ^ 1262605491851124580) ^ 2623372049577918607) ^ -6462111799388380982) ^ -5406746690661955111 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L22:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[18]) ^ 8220144704119092501) ^ -3953689455838608405) ^ -8693403043747529837) ^ 7613250387243725316) ^ 2970230484384709588) ^ -7605509448554618824 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение разности: %d - %d = %d (ожидалось -5)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[19]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[20]) ^ -4455316258140658692) ^ 201975991849398130) ^ -6573041282290555879) ^ 4854154954109728600) ^ 5103135501051021123) ^ -8879543246559540127 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L23:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[21]) ^ 1244760556833261884) ^ 9221860839282636122) ^ -8623127567900096752) ^ 6929752032770704347) ^ 9080712431064466277) ^ -2712797876098593749 ^ rtdsc));
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal2.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[20]) ^ -9018848701493845575) ^ -1889639619461859906) ^ 6757775576120595868) ^ 7622663645106926279) ^ -2498944030517073181) ^ 211320585455006101 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L24:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[22]) ^ 5444962231811046352) ^ -3155957730526784161) ^ -4302840072560747088) ^ -1692421191478068096) ^ 1910982554475138849) ^ 2529218538769577515 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение частного: %d / %d = %d (ожидалось 2)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[23]) ^ -4198794938774043345) ^ 3520995196730888369) ^ -2809701589634051169) ^ -486885816508305909) ^ 4195208703372108019) ^ -2827882699593712993 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[24]) ^ -4964709260318338742) ^ -5487540908142167107) ^ 3542095193863934742) ^ -6343688103812603425) ^ -8753542533151386124) ^ 8899387537384118622 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack5.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L25:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[25]) ^ 76262677825106207) ^ 5674222814952170783) ^ 4876105731939827160) ^ 1069811991413684078) ^ -2044980633825100276) ^ 6408862346679384694 ^ rtdsc));
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 2
    cstack2.i = clocal3.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[24]) ^ -3247565585749077855) ^ 2266025828226146629) ^ 8878137333282602529) ^ 4863568216267896859) ^ -366039013850761645) ^ -1062713216844882498 ^ rtdsc), cstack2.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L26:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[26]) ^ -2481651264204782457) ^ -7172182140610349510) ^ 2526340549784616619) ^ -7726373574432324537) ^ -5864031851036819119) ^ 5656760624181804798 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Математические операции пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
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
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[27]) ^ 8074082470316814865) ^ 4826227825290504676) ^ 2217373156660978284) ^ 2269071862572968055) ^ 3885361862634691369) ^ -4159458907198710271 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование манипуляций со строками...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
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
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[25]) ^ 2815841997600245136) ^ 7308600783070511323) ^ 7623690776222046452) ^ -5653914245529104246) ^ -63396749389446376) ^ 9217895984737865300 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L32:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[28]) ^ 8733187347147290491) ^ 2684348949629404777) ^ -1166723667024455008) ^ -3068831153992343701) ^ 1154376893321850675) ^ 4497811649672002739 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение реверсирования строки: '%s' (ожидалось '!dlroW ,olleH')\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 2
    cstack2.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
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
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L33:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[29]) ^ -3539690057648600423) ^ 1379116321069788471) ^ -6193798413319484461) ^ -5824788833073617665) ^ 1464223742649201916) ^ 5829387857277000016 ^ rtdsc));
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
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[6]) ^ 1355219642397599596) ^ 2874053255878655122) ^ 6050988320869948783) ^ -3755467258223120127) ^ 9087917015930502249) ^ 6784913207928174642 ^ rtdsc), "equals", "(Ljava/lang/Object;)Z"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L34;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[26]) ^ -7633082283617597823) ^ 6460605797703812512) ^ 4964958196648230280) ^ -4454654378664787565) ^ 5866945742775377974) ^ -1350425269140229665 ^ rtdsc), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L34:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[30]) ^ -6681852934887599630) ^ 5205636433494038068) ^ -1793946945554733046) ^ 895799654111415202) ^ 1305698025552508309) ^ 414744213608600830 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Манипуляции со строками пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
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
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[31]) ^ -8035665841081458102) ^ -6525240195463148967) ^ -6334349365014350629) ^ -4457792128422543022) ^ 7690003914409305563) ^ -3956035283340082837 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование сортировки массива...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
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
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[27]) ^ -290717731400814148) ^ 8703011193474661544) ^ 4885394224222828530) ^ -1350672016389041259) ^ -1144258416717255848) ^ -9140509284313210831 ^ rtdsc), cstack0.l);
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
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[32]) ^ -4419713109902718768) ^ -2230133705875733950) ^ 4735554775949993696) ^ 2436808696064758720) ^ 4180799546524937962) ^ -3376560793937698160 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение сортировки массива: %s (ожидалось %s)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_2; Stack pointer: 2
    cstack2.i = 2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[7]) ^ 2922651446609032204) ^ 12271694190082378) ^ 4655990803524591946) ^ 5540791058340505026) ^ -2830404617262663155) ^ 7280099264598872290 ^ rtdsc), (jmethodID)(((((((__int64)(methods[28]) ^ -3286818374837535719) ^ 3749614016179863469) ^ -1625530504125960880) ^ -7732649078242802747) ^ -2217915507768963939) ^ 445540604372033046 ^ rtdsc), cstack5.l);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[7]) ^ 2922651446609032204) ^ 12271694190082378) ^ 4655990803524591946) ^ 5540791058340505026) ^ -2830404617262663155) ^ 7280099264598872290 ^ rtdsc), (jmethodID)(((((((__int64)(methods[28]) ^ -3286818374837535719) ^ 3749614016179863469) ^ -1625530504125960880) ^ -7732649078242802747) ^ -2217915507768963939) ^ 445540604372033046 ^ rtdsc), cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L44:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L45:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[33]) ^ -7048833339818552688) ^ 8234424812016528829) ^ -1784658453271731676) ^ -1524684357986277431) ^ 2206420242660352738) ^ 3312116652030589626 ^ rtdsc));
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
    cstack0.i = (jint) env->CallStaticBooleanMethod((jclass)(((((((__int64)(classes[7]) ^ 2922651446609032204) ^ 12271694190082378) ^ 4655990803524591946) ^ 5540791058340505026) ^ -2830404617262663155) ^ 7280099264598872290 ^ rtdsc), (jmethodID)(((((((__int64)(methods[30]) ^ 877780147607341529) ^ -6196094855904424733) ^ -8613839071322128087) ^ 4509268024967979010) ^ -8465309429832208207) ^ 184574566618362342 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L46;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = env->NewStringUTF("Ошибка: Массив не отсортирован правильно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L46:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[34]) ^ 5224044060682370929) ^ -8907086633133406480) ^ 3242416301613232369) ^ 1231273321094996533) ^ 1896573393333001496) ^ 1980540444425592348 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сортировка массива пройдена успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
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
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[35]) ^ 3325235004072892482) ^ -5448626386352507760) ^ -2491445704227411465) ^ 5543928812393227779) ^ -4653462793191558040) ^ -8561034794910826153 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование операций с float...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L50:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[36]) ^ -215861798369383656) ^ 4787313307795812624) ^ 8250914054752324491) ^ 8828199020076688466) ^ -214717877325136715) ^ -5145780648613317111 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[37]) ^ -2446048111671875237) ^ 8842452235374070025) ^ -4611807465684385422) ^ 8303024245527224495) ^ -6786367809857869576) ^ -7287000996905904850 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[31]) ^ 4785857354980570726) ^ -1014078859160651558) ^ -918743253170281092) ^ 3645481825087243660) ^ 4641967523787979543) ^ -6128052018101135495 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal0.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L51:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[38]) ^ -6575043490173779858) ^ -2090692663976325469) ^ -4761646913957220256) ^ -6356239115728527141) ^ -1461309846615675765) ^ -1523052506530392179 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[39]) ^ 5883148937512846555) ^ 7397778564915045237) ^ -141680522072200923) ^ -4106629695470315223) ^ -834411575979839198) ^ -7808933076708213554 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[32]) ^ 3400090941399290270) ^ 9082419801678194936) ^ 874074126302084496) ^ -2723944224850594112) ^ -3723922253799438907) ^ -4566306163505899730 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal1.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L52:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[40]) ^ 5883148937512846555) ^ 7397778564915045237) ^ -141680522072200923) ^ -4106629695470315223) ^ -834411575979839198) ^ -7808933076708213554 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[41]) ^ -3432880608639813355) ^ -5917212772105607771) ^ -9161498381721971671) ^ 5369916470795991608) ^ -1302784133813949455) ^ 3891591141432974303 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[33]) ^ -1021028909002136917) ^ -2737634609123526013) ^ 4099043000841746992) ^ 8821923511971242960) ^ 3431398461647751169) ^ 8089743400991495456 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal2.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L53:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[42]) ^ 5918752085750786479) ^ 4965668862894945861) ^ -7279828537541202964) ^ -6523975953515285103) ^ -1756747530505922359) ^ -2305950619791404290 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[43]) ^ 1900215797108186866) ^ 6131460453850120982) ^ 7244447902956007738) ^ 5025029537359274722) ^ 3575515013307340127) ^ -5491035119098674844 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[34]) ^ 1939468586196644730) ^ 4647872265896404143) ^ -698628324755045877) ^ -825497241839577289) ^ 5427391507225542421) ^ -6999288940315590388 ^ rtdsc), cstack0.f, cstack1.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FSTORE; Stack pointer: 1
    clocal3.f = cstack0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L54:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[44]) ^ 8587125113345012855) ^ -6982477838656066462) ^ -8702691540325498502) ^ -8413009674368133667) ^ 2069508267276865159) ^ 7943862186732943997 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение суммы: %.2f + %.2f = %.2f (ожидалось 8.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[45]) ^ -4163191786241136125) ^ 1088885499005756289) ^ 8498894468606498407) ^ -2904232074553275789) ^ 3272872748846024858) ^ 2675099753028128974 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
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
    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[46]) ^ -7850350818190722602) ^ 1657998404868605433) ^ -6741457723719328045) ^ -4964140387245605067) ^ 8626749034372493372) ^ -8910339637322972342 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L55:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[47]) ^ -5769876375246059300) ^ 5434255252943013168) ^ -609775860046642757) ^ -6349963611918048931) ^ -5107426189883530945) ^ 3688167513279379573 ^ rtdsc));
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
    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), (jmethodID)(((((((__int64)(methods[38]) ^ -5402895970315106241) ^ 2405466870125555111) ^ -619064352329644127) ^ -3929479604115323595) ^ -6008148402696408077) ^ 790795074857390778 ^ rtdsc), cstack0.f);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 2
    cstack2.f = clocal0.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[39]) ^ 4347670644983803227) ^ 6878928919107070660) ^ -5079902803658827256) ^ 6059690329079490787) ^ 7387361649947990294) ^ 4210099593081688277 ^ rtdsc), cstack2.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L56:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[48]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение разности: %.2f - %.2f = %.2f (ожидалось 3.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[49]) ^ -8363393461218920593) ^ -4980040004894375045) ^ 4178606973267148742) ^ 5717941153990463950) ^ -8004141448274199330) ^ -2566916657545074994 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
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
    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[50]) ^ -7266101878686644764) ^ 3431817414886354455) ^ 4955669700070261614) ^ -2034170366567094932) ^ 4966223529962500841) ^ -4247797716152153052 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L57:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[51]) ^ 2085530819998922366) ^ -4132045015232708938) ^ 6837339544251030322) ^ 4518681282831179973) ^ 4512260133270527936) ^ 8001404596333019971 ^ rtdsc));
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
    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), (jmethodID)(((((((__int64)(methods[38]) ^ -5402895970315106241) ^ 2405466870125555111) ^ -619064352329644127) ^ -3929479604115323595) ^ -6008148402696408077) ^ 790795074857390778 ^ rtdsc), cstack0.f);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 2
    cstack2.f = clocal2.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[44]) ^ -1861799167872830103) ^ -7830472824022765273) ^ 7085319962400171534) ^ -7213749807503816986) ^ 7999850763736656805) ^ -2624459071440118264 ^ rtdsc), cstack2.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L58:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[52]) ^ -1751340082308492391) ^ 4268463661987838046) ^ 3312691773165698157) ^ 547774970916942860) ^ 8007055340012758185) ^ 6873252012586650127 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение частного: %.2f / %.2f = %.2f (ожидалось 2.20)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[53]) ^ 8622728261582952779) ^ 9032156537328353074) ^ 2605904522210018369) ^ 7616388141296448069) ^ 1147172312750781998) ^ -4999899434354765652 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
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
    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[54]) ^ -728904437102614350) ^ -1850725101967167854) ^ 724234678029249662) ^ 1063536487603205868) ^ 1601135709442754903) ^ 1197642326869612941 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 6
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), cstack5.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L59:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[55]) ^ -3176359289273198007) ^ -2598193567224117531) ^ -5398158693360434256) ^ 28875700177957099) ^ -2210710927197895263) ^ -8503492385310750179 ^ rtdsc));
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
    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), (jmethodID)(((((((__int64)(methods[38]) ^ -5402895970315106241) ^ 2405466870125555111) ^ -619064352329644127) ^ -3929479604115323595) ^ -6008148402696408077) ^ 790795074857390778 ^ rtdsc), cstack0.f);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // FLOAD; Stack pointer: 2
    cstack2.f = clocal3.f;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[49]) ^ -582842203300336715) ^ 7816101690613270681) ^ 8260202551330293156) ^ 6407715007978995833) ^ 686004331192773122) ^ -2248408210191328316 ^ rtdsc), cstack2.f);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L60:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[56]) ^ -1131487994566474629) ^ 3610172978575422283) ^ 7871671185781253072) ^ 1060398733550483115) ^ 3424193881076682493) ^ -1407967683035272935 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Операции с float пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
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
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[57]) ^ 7963623384752477153) ^ -7272708660720098644) ^ 5990001341600484365) ^ -5492452915847791791) ^ 3878157277768655397) ^ 4789574082484072954 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Тестирование операций с double...");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L64:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[58]) ^ 4055546177379247956) ^ 5992019416245679797) ^ -1705094480846329926) ^ -4628666715967056441) ^ 9217624402152986559) ^ -7344543406505980824 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[59]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[50]) ^ -8912039256780025803) ^ -9185968716932223443) ^ 3790075607718108657) ^ 370624875266983935) ^ -5265951902685257256) ^ -1726476130389019613 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal0.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L65:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[60]) ^ -547239055062396791) ^ 5383991988593171305) ^ 1122054535861291116) ^ 3990368754228993249) ^ -236331623333310039) ^ 3254574242430513652 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[61]) ^ 2887048294076124984) ^ 2444381387620247162) ^ -6652605254715957629) ^ 7958137312090507610) ^ -1908068662736579994) ^ 1777116816271997619 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[51]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal2.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L66:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[62]) ^ 1059445533942526385) ^ 1038622238950881722) ^ -8216019209195119336) ^ 7436100291593766392) ^ 8143967311101278468) ^ 2241506482179263052 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[63]) ^ -5150024283209074242) ^ 4775964565235630109) ^ 3949203552568912158) ^ -5837339849284508677) ^ 8756456420594977683) ^ -4593052182342543489 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[52]) ^ -5548958204117383877) ^ -7261359918159916129) ^ -8155032229925654918) ^ 9173085949218438056) ^ -5093017024446426297) ^ 4236845607623364740 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal4.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L67:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[64]) ^ 1572488176970724375) ^ 7676660648713862200) ^ -689339832472044507) ^ -3245981249642302625) ^ 6328113720038419554) ^ -4101916497598634297 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 2
    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[65]) ^ -7013230191580612764) ^ 5802315109996429453) ^ -8922806464445766421) ^ -3942030611736280015) ^ 1284084283839302281) ^ 8815099108947398889 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[53]) ^ -6827915168689877265) ^ -4461190359086400467) ^ 9116829254853775075) ^ -4448378870559342059) ^ 2220829399507522794) ^ 3860794746374574792 ^ rtdsc), cstack0.d, cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DSTORE; Stack pointer: 2
    clocal6.d = cstack0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L68:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[66]) ^ 3435694089637230194) ^ 6650310099658095560) ^ -6264073889166917545) ^ -5141290482895563992) ^ -4646258212620489364) ^ 936676289115942238 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение суммы: %.2f + %.2f = %.2f (ожидалось 13.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[67]) ^ 1501281876199877231) ^ -5905864029545425255) ^ -4859787879538559338) ^ 1588711262152669838) ^ 8172785637680520468) ^ 3338862670867233384 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[68]) ^ 1462029091406386664) ^ -4422275841591708416) ^ 3083288352467461573) ^ 7439238045646489145) ^ 6320909143762318174) ^ 4847116492084148928 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L69:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[69]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc));
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
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 3727818557241785465) ^ 7537219606814453719) ^ 8807861861730136741) ^ 5547066566445950532) ^ -6476520960530518334) ^ -5955424785005940277 ^ rtdsc), cstack0.d);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal0.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[58]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L70:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[70]) ^ -5221230579684954090) ^ -8806560113023657346) ^ -221244494497602673) ^ -1002647333194568917) ^ -7845615735472473019) ^ 2847726986123324192 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение произведения: %.2f * %.2f = %.2f (ожидалось 30.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[71]) ^ 6834378290537812044) ^ 6142809196410303498) ^ -6900585668570131545) ^ 1243824333010920249) ^ -5395659293202708862) ^ -6043763589664415762 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[72]) ^ 2630526970414542340) ^ -874637817261243077) ^ 8030799126337089276) ^ -5147565986706042201) ^ -1000141869352634185) ^ -4274543730693829515 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L71:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[73]) ^ 2338402502809987069) ^ -1761547324417601236) ^ -7041136620264997714) ^ 2610821037661994891) ^ 830120887147329377) ^ 2617557343428053000 ^ rtdsc));
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
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 3727818557241785465) ^ 7537219606814453719) ^ 8807861861730136741) ^ 5547066566445950532) ^ -6476520960530518334) ^ -5955424785005940277 ^ rtdsc), cstack0.d);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal2.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[63]) ^ -6208063080947859503) ^ -5119481046793783526) ^ -4770935410535188922) ^ -3935755107925801805) ^ -2362032059428552898) ^ -4420424949247348270 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L72:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[74]) ^ 8658331409820892703) ^ 6600046839603220993) ^ -4532243493258983672) ^ 5199041883251478189) ^ 224836353929731542) ^ 503083013972109020 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение разности: %.2f - %.2f = %.2f (ожидалось 7.00)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[75]) ^ -3871067318636580853) ^ 1975795006162114447) ^ 5124086145794001076) ^ 7784124974788238735) ^ 1442609996641028592) ^ -4217001321093753541 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[76]) ^ -255114583162874224) ^ 6270901495749529464) ^ -2252753786951206214) ^ -3768018274434011139) ^ -2066594375538306305) ^ -3637526831691368863 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L73:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[77]) ^ -5734273227008119376) ^ 3002145550922913791) ^ -7747923871220677502) ^ -8767309865668051515) ^ -6029762144409614106) ^ 9191149970196188837 ^ rtdsc));
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
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 3727818557241785465) ^ 7537219606814453719) ^ 8807861861730136741) ^ 5547066566445950532) ^ -6476520960530518334) ^ -5955424785005940277 ^ rtdsc), cstack0.d);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal4.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[68]) ^ -2592110349769120169) ^ -824374552911401214) ^ 6298968734724122699) ^ 2958845720856467234) ^ -5871236431607887795) ^ -3840950459844963593 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L74:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[78]) ^ -8142475285795277874) ^ 771088897712247275) ^ -3366649396611863419) ^ 2794246637122432025) ^ -7989732287132061978) ^ -2018238563201089828 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Сравнение частного: %.2f / %.2f = %.2f (ожидалось 3.33)\n");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_3; Stack pointer: 2
    cstack2.i = 3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, "java/lang/NegativeArraySizeException", "ARRAYLENGTH negative", __LINE__); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 4
    cstack4.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 5
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[79]) ^ -2994693902938013152) ^ 4636523523336221627) ^ -5000338826938458210) ^ 2955707966803744481) ^ -4048178259973960205) ^ -6446560465454882173 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[80]) ^ 8148938403348245357) ^ 910529939611655756) ^ 5582892982895506949) ^ -5998801174670853836) ^ 4814902397731843206) ^ -164730275793783847 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 7
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
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
    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ -5256833732217861310) ^ -6374450415298525266) ^ 6916903516676432072) ^ 1414698924850400963) ^ -6923279776651422562) ^ -2655255466498517775 ^ rtdsc), cstack5.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // AASTORE; Stack pointer: 6
    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "printf", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;"), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L75:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jfieldID)(((((((__int64)(fields[81]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc));
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
    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 3727818557241785465) ^ 7537219606814453719) ^ 8807861861730136741) ^ 5547066566445950532) ^ -6476520960530518334) ^ -5955424785005940277 ^ rtdsc), cstack0.d);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DLOAD; Stack pointer: 2
    cstack2.d = clocal6.d;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), (jmethodID)(((((((__int64)(methods[73]) ^ -5477751907641504029) ^ 6321164755804404030) ^ -3984584182859140088) ^ 4338393437423465592) ^ -6937688937793559915) ^ -3203933560842502941 ^ rtdsc), cstack2.d);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5588210993205841740) ^ -5777771730206199289) ^ -211956002214601303) ^ -3423131345292261550) ^ -6944893522659595887) ^ 5745099428840280284 ^ rtdsc), "<init>", "(Ljava/lang/Object;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L76:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), (jfieldID)(((((((__int64)(fields[82]) ^ 8658331409820892703) ^ 6600046839603220993) ^ -4532243493258983672) ^ 5199041883251478189) ^ 224836353929731542) ^ 503083013972109020 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF("Операции с double пройдены успешно.");
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 6392541943985493902) ^ -5359448608802941141) ^ 8189927075482860072) ^ 7091213362452016802) ^ -5424477615486983567) ^ -7141119782647353391 ^ rtdsc), "println", "(Ljava/lang/String;)V"), cstack1.l);
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
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[11]) ^ 7272564991944644951) ^ -1750198581857418720) ^ -2739426118081585381) ^ -1170384166686359582) ^ -8141053419362719613) ^ 2064828868567344785 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[11]) ^ 7272564991944644951) ^ -1750198581857418720) ^ -2739426118081585381) ^ -1170384166686359582) ^ -8141053419362719613) ^ 2064828868567344785 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 7272564991944644951) ^ -1750198581857418720) ^ -2739426118081585381) ^ -1170384166686359582) ^ -8141053419362719613) ^ 2064828868567344785 ^ rtdsc), "<init>", "(Ljava/lang/String;)V"), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 7272564991944644951) ^ -1750198581857418720) ^ -2739426118081585381) ^ -1170384166686359582) ^ -8141053419362719613) ^ 2064828868567344785 ^ rtdsc), "reverse", "()Ljava/lang/StringBuilder;"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 7272564991944644951) ^ -1750198581857418720) ^ -2739426118081585381) ^ -1170384166686359582) ^ -8141053419362719613) ^ 2064828868567344785 ^ rtdsc), "toString", "()Ljava/lang/String;"));
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
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[7]) ^ 2922651446609032204) ^ 12271694190082378) ^ 4655990803524591946) ^ 5540791058340505026) ^ -2830404617262663155) ^ 7280099264598872290 ^ rtdsc), (jmethodID)(((((((__int64)(methods[74]) ^ 4785857354980570726) ^ -1014078859160651558) ^ -918743253170281092) ^ 3645481825087243660) ^ 4641967523787979543) ^ -6128052018101135495 ^ rtdsc), cstack0.l);
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
    classes[0] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=197&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/System")) ^ 1786243946462463404), rtdsc)));
    classes[1] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=155&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/io/PrintStream")) ^ 1908527760331663727), rtdsc)));
    classes[2] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=49&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("ru/kotopushka/Main")) ^ 1660655170454862121), rtdsc)));
    classes[3] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=142&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/Object")) ^ 1877130566329763406), rtdsc)));
    classes[4] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=356&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/Integer")) ^ 1100463115185806843), rtdsc)));
    classes[5] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=192&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/AssertionError")) ^ 1794506371135818636), rtdsc)));
    classes[6] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=361&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/String")) ^ 1092200690512451611), rtdsc)));
    classes[7] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=163&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/util/Arrays")) ^ 1842428401599527533), rtdsc)));
    classes[8] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=384&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/Float")) ^ 1477229451798545284), rtdsc)));
    classes[9] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=87&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/Math")) ^ 1597860782451061479), rtdsc)));
    classes[10] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=13&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/Double")) ^ 1667265107616565929), rtdsc)));
    classes[11] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=330&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass("java/lang/StringBuilder")) ^ 1143427695999463356), rtdsc)));

    /* METHOD REFERENCE TABLE */
    methods[0] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=308&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "testMathOperations", "()V") ^ 1232661828355111934), rtdsc)));
    methods[1] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=358&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "testStringManipulation", "()V") ^ 1097158144457471291), rtdsc)));
    methods[2] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=85&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "testArraySorting", "()V") ^ 1601165748884429735), rtdsc)));
    methods[3] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=59&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "testFloatOperations", "()V") ^ 1644130329698086248), rtdsc)));
    methods[4] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=423&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "testDoubleOperations", "()V") ^ 1412782576283093218), rtdsc)));
    methods[5] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=296&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "add", "(II)I") ^ 1199612151136527485), rtdsc)));
    methods[6] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=212&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "multiply", "(II)I") ^ 1814336178325962764), rtdsc)));
    methods[7] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=211&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "subtract", "(II)I") ^ 1815988661542646892), rtdsc)));
    methods[8] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=85&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "divide", "(II)I") ^ 1601165748884429735), rtdsc)));
    methods[9] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=89&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[4]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc), "valueOf", "(I)Ljava/lang/Integer;") ^ 1594555811722725927), rtdsc)));
    methods[12] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=214&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello0_proxy1", "(I)Ljava/lang/String;") ^ 1811031207597627212), rtdsc)));
    methods[16] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=363&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello1_proxy2", "(I)Ljava/lang/String;") ^ 1088895728374050651), rtdsc)));
    methods[20] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=353&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello2_proxy3", "(I)Ljava/lang/String;") ^ 1105420564835859227), rtdsc)));
    methods[24] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=329&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello3_proxy4", "(I)Ljava/lang/String;") ^ 1145080179216147484), rtdsc)));
    methods[25] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=291&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "reverseString", "(Ljava/lang/String;)Ljava/lang/String;") ^ 1207874567219948125), rtdsc)));
    methods[26] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=34&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello4_proxy5", "(Ljava/lang/String;)Ljava/lang/String;") ^ 1632562942886330056), rtdsc)));
    methods[27] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=93&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "sortArray", "([I)[I") ^ 1587945874561022119), rtdsc)));
    methods[28] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=240&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[7]) ^ 2922651446609032204) ^ 12271694190082378) ^ 4655990803524591946) ^ 5540791058340505026) ^ -2830404617262663155) ^ 7280099264598872290 ^ rtdsc), "toString", "([I)Ljava/lang/String;") ^ 1768066626783970699), rtdsc)));
    methods[30] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=5&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[7]) ^ 2922651446609032204) ^ 12271694190082378) ^ 4655990803524591946) ^ 5540791058340505026) ^ -2830404617262663155) ^ 7280099264598872290 ^ rtdsc), "equals", "([I[I)Z") ^ 1680484977645006249), rtdsc)));
    methods[31] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=200&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "add", "(FF)F") ^ 1781286496812411020), rtdsc)));
    methods[32] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=263&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "multiply", "(FF)F") ^ 1254144123056907487), rtdsc)));
    methods[33] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=96&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "subtract", "(FF)F") ^ 1530108940502241157), rtdsc)));
    methods[34] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=333&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "divide", "(FF)F") ^ 1138470246349410972), rtdsc)));
    methods[35] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=142&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[8]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), "valueOf", "(F)Ljava/lang/Float;") ^ 1877130566329763406), rtdsc)));
    methods[38] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=338&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), "abs", "(F)F") ^ 1183087310379751613), rtdsc)));
    methods[39] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=253&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello5_proxy6", "(F)Ljava/lang/String;") ^ 1746584336377142443), rtdsc)));
    methods[44] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=10&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello6_proxy7", "(F)Ljava/lang/String;") ^ 1672222557266618313), rtdsc)));
    methods[49] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=75&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello7_proxy8", "(F)Ljava/lang/String;") ^ 1564811100937509734), rtdsc)));
    methods[50] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=225&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "add", "(DD)D") ^ 1739974399215438635), rtdsc)));
    methods[51] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=217&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "multiply", "(DD)D") ^ 1806073753652607532), rtdsc)));
    methods[52] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=345&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "subtract", "(DD)D") ^ 1171519923567995421), rtdsc)));
    methods[53] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=280&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "divide", "(DD)D") ^ 1278931379897103999), rtdsc)));
    methods[54] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=299&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[10]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc), "valueOf", "(D)Ljava/lang/Double;") ^ 1194654697191507805), rtdsc)));
    methods[57] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=409&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[9]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), "abs", "(D)D") ^ 1488796838610301476), rtdsc)));
    methods[58] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=120&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello8_proxy9", "(D)Ljava/lang/String;") ^ 1543328810530681478), rtdsc)));
    methods[63] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=124&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello9_proxy10", "(D)Ljava/lang/String;") ^ 1536718877663944966), rtdsc)));
    methods[68] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=45&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello10_proxy11", "(D)Ljava/lang/String;") ^ 1614385618912870055), rtdsc)));
    methods[73] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=89&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$hello11_proxy12", "(D)Ljava/lang/String;") ^ 1594555811722725927), rtdsc)));
    methods[74] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=200&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[7]) ^ 2922651446609032204) ^ 12271694190082378) ^ 4655990803524591946) ^ 5540791058340505026) ^ -2830404617262663155) ^ 7280099264598872290 ^ rtdsc), "sort", "([I)V") ^ 1781286496812411020), rtdsc)));

    /* METHOD REFERENCE TABLE */
    fields[0] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=125&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1535066390152293541), rtdsc)));
    fields[1] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=350&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1163257503189607484), rtdsc)));
    fields[2] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=303&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1188044764324771293), rtdsc)));
    fields[3] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=126&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1533413906935609413), rtdsc)));
    fields[4] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=132&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1893655407086539278), rtdsc)));
    fields[5] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=383&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1108725531269227483), rtdsc)));
    fields[6] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=191&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1849038338761231341), rtdsc)));
    fields[7] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=69&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1574726004532581798), rtdsc)));
    fields[8] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=349&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1164909986406291612), rtdsc)));
    fields[9] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=179&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1868868145951375470), rtdsc)));
    fields[10] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=216&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1807726241164258956), rtdsc)));
    fields[11] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=361&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1092200690512451611), rtdsc)));
    fields[12] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=324&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1153342599594535420), rtdsc)));
    fields[13] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=275&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1287193800275491935), rtdsc)));
    fields[14] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=114&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1553243714125753542), rtdsc)));
    fields[15] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=308&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1232661828355111934), rtdsc)));
    fields[16] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=339&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1181434827163067485), rtdsc)));
    fields[17] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=218&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1804421270435923404), rtdsc)));
    fields[18] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=32&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1635867913614665608), rtdsc)));
    fields[19] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=384&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1477229451798545284), rtdsc)));
    fields[20] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=8&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1675527523699986569), rtdsc)));
    fields[21] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=272&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1292151249925544320), rtdsc)));
    fields[22] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=421&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1416087547011428771), rtdsc)));
    fields[23] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=410&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intB", "I") ^ 1487144359688584644), rtdsc)));
    fields[24] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=317&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "intA", "I") ^ 1217789475109987486), rtdsc)));
    fields[25] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=296&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1199612151136527485), rtdsc)));
    fields[26] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=422&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1414435063794744643), rtdsc)));
    fields[27] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=39&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1624300522507942120), rtdsc)));
    fields[28] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=260&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1259101568411992575), rtdsc)));
    fields[29] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=375&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1121945405592635099), rtdsc)));
    fields[30] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=273&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1290498766708860191), rtdsc)));
    fields[31] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=215&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1809378724380943084), rtdsc)));
    fields[32] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=136&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1887045469924835470), rtdsc)));
    fields[33] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=6&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1678832494428322121), rtdsc)));
    fields[34] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=211&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1815988661542646892), rtdsc)));
    fields[35] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=14&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1665612624399881801), rtdsc)));
    fields[36] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=342&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatA", "F") ^ 1176477373218047805), rtdsc)));
    fields[37] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=38&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatB", "F") ^ 1625953005724626248), rtdsc)));
    fields[38] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=145&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatA", "F") ^ 1925052601088439599), rtdsc)));
    fields[39] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=432&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatB", "F") ^ 1450789711741664643), rtdsc)));
    fields[40] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=432&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatA", "F") ^ 1450789711741664643), rtdsc)));
    fields[41] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=247&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatB", "F") ^ 1756499235677247211), rtdsc)));
    fields[42] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=48&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatA", "F") ^ 1662307653671546249), rtdsc)));
    fields[43] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=244&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatB", "F") ^ 1761456689622266891), rtdsc)));
    fields[44] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=267&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1247534185895203678), rtdsc)));
    fields[45] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=26&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatA", "F") ^ 1698662301618466250), rtdsc)));
    fields[46] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=297&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatB", "F") ^ 1197959663624876061), rtdsc)));
    fields[47] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=71&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1571421033804246246), rtdsc)));
    fields[48] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=195&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1789548917190798956), rtdsc)));
    fields[49] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=69&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatA", "F") ^ 1574726004532581798), rtdsc)));
    fields[50] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=269&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatB", "F") ^ 1244229215166868126), rtdsc)));
    fields[51] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=326&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1150037633161167164), rtdsc)));
    fields[52] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=387&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1472271997853525604), rtdsc)));
    fields[53] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=395&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatA", "F") ^ 1459052127825085284), rtdsc)));
    fields[54] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=114&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "floatB", "F") ^ 1553243714125753542), rtdsc)));
    fields[55] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=73&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1568116071665845286), rtdsc)));
    fields[56] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=231&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1730059495620366570), rtdsc)));
    fields[57] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=174&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1824251077626067533), rtdsc)));
    fields[58] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=235&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1723449558458662762), rtdsc)));
    fields[59] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=187&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1855648275922935149), rtdsc)));
    fields[60] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=203&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1776329047162358635), rtdsc)));
    fields[61] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=35&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1630910459669645928), rtdsc)));
    fields[62] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=158&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1903570306386644047), rtdsc)));
    fields[63] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=171&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1829208531571087213), rtdsc)));
    fields[64] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=98&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1526803969773905605), rtdsc)));
    fields[65] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=134&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1890350436358203726), rtdsc)));
    fields[66] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=391&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1465662064986789092), rtdsc)));
    fields[67] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=354&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1103768081619175099), rtdsc)));
    fields[68] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=233&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1726754529186998314), rtdsc)));
    fields[69] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=120&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1543328810530681478), rtdsc)));
    fields[70] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=427&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1406172639121389410), rtdsc)));
    fields[71] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=383&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1108725531269227483), rtdsc)));
    fields[72] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=209&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1819293627976015148), rtdsc)));
    fields[73] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=223&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1796158854352502764), rtdsc)));
    fields[74] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=11&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1670570069754966889), rtdsc)));
    fields[75] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=236&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1721797075241978634), rtdsc)));
    fields[76] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=221&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1799463820785871020), rtdsc)));
    fields[77] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=199&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1782938984324062444), rtdsc)));
    fields[78] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=343&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1174824890001363677), rtdsc)));
    fields[79] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=194&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleA", "D") ^ 1791201400407483084), rtdsc)));
    fields[80] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=288&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "doubleB", "D") ^ 1212832021164967806), rtdsc)));
    fields[81] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=222&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -2007861405970075035) ^ 949444457106347807) ^ -450647915195839257) ^ 5888815741534977368) ^ 8914982137691671290) ^ 821591465620822994 ^ rtdsc), "$assertionsDisabled", "Z") ^ 1797811337569186892), rtdsc)));
    fields[82] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=11&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -8327790312980980669) ^ -7412149702619507125) ^ -2959541042201853299) ^ 3300594895945494070) ^ -8926477407095249787) ^ 2936065790781799678 ^ rtdsc), "out", "Ljava/io/PrintStream;") ^ 1670570069754966889), rtdsc)));

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
