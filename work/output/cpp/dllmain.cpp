
#include <string>
#include <stdexcept>
#include <cmath>

#include "table.h"
#include "VMProtectSDK.h"
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
    HINTERNET interwebs = InternetOpenA(xorstr_("Mozilla/5.0"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 0

    cstack0.i = (jint) 13;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEWARRAY; Stack pointer: 1

        cstack0.l = env->NewIntArray(cstack0.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 2

    cstack2.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 3

    cstack3.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 2

    cstack2.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 3

    cstack3.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 2

    cstack2.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 3

    cstack3.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_4; Stack pointer: 3

    cstack3.i = 4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_4; Stack pointer: 2

    cstack2.i = 4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_5; Stack pointer: 3

    cstack3.i = 5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_5; Stack pointer: 2

    cstack2.i = 5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 6;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 2

    cstack2.i = (jint) 6;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 7;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 2

    cstack2.i = (jint) 7;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 8;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 2

    cstack2.i = (jint) 8;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 9;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 2

    cstack2.i = (jint) 9;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 10;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 2

    cstack2.i = (jint) 10;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 11;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 2

    cstack2.i = (jint) 11;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 12;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 2

    cstack2.i = (jint) 12;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 13;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ASTORE; Stack pointer: 1

    clocal1.l = cstack0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L2:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 0

    cstack0.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ISTORE; Stack pointer: 1

    clocal2.i = cstack0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L3:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal2.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 1

    cstack1.l = clocal1.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ARRAYLENGTH; Stack pointer: 2

    cstack1.i = (env)->GetArrayLength((jarray)cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IF_ICMPGE; Stack pointer: 2

    if (cstack0.i >= cstack1.i) goto L4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L5:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    VMProtectBeginUltra("-2346535656150022953");
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[0]) ^ 8804393643623170339) ^ -2179870445820859384) ^ 3003724380042059823) ^ -7903523670082283461) ^ -690295020025282943) ^ -2942967518793864942 ^ rtdsc));
    VMProtectEnd();
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 1

    cstack1.l = clocal1.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 2

    cstack2.i = clocal2.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IALOAD; Stack pointer: 3

    env->GetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack1.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    VMProtectBeginUltra("-3503399538247601123");
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(I)V")), cstack1.i);
    VMProtectEnd();
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L6:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IINC; Stack pointer: 0

    clocal2.i += 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GOTO; Stack pointer: 0

    goto L3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L4:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 0

    VMProtectBeginUltra("-7290478346217999342");
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[0]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc));
    VMProtectEnd();
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L7:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 0

    VMProtectBeginUltra("1560556295251895358");
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[1]) ^ 7892417079686662713) ^ -2408489265269834483) ^ 1819553294533969534) ^ -657760404052819327) ^ 5722829191115789015) ^ -6216390827054578277 ^ rtdsc));
    VMProtectEnd();
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L8:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 0

    VMProtectBeginUltra("6790053037904344183");
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[2]) ^ -255114583162874224) ^ 6270901495749529464) ^ -2252753786951206214) ^ -3768018274434011139) ^ -2066594375538306305) ^ -3637526831691368863 ^ rtdsc));
    VMProtectEnd();
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L9:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 0

    VMProtectBeginUltra("3443218930701175987");
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[3]) ^ 8255747852357032425) ^ -6385799157858707781) ^ 2615193014493019739) ^ 5195904129198755436) ^ 2047894525563659131) ^ -2102526991637809561 ^ rtdsc));
    VMProtectEnd();
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L10:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 0

    VMProtectBeginUltra("-6175214473444563568");
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[4]) ^ -109052345065629292) ^ -2509015789674550913) ^ 5283214086349837280) ^ 1576160250236746122) ^ -2981725753788288086) ^ -7083577368752310121 ^ rtdsc));
    VMProtectEnd();
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L11:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // RETURN; Stack pointer: 0

    return;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[1]) ^ -1715736934070552467) ^ 1836353964262705966) ^ -3825456242303303883) ^ -1869571287128027020) ^ 7084719385486675024) ^ -6070509608501059521 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Тестирование математических операций..."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L14:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[2]) ^ 222324911627383843) ^ -3105694466176942298) ^ -6034670472763648257) ^ 6413990520379408635) ^ -2960112007780114761) ^ 2962811809618443437 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[3]) ^ 4639795121178293091) ^ 7765838426263428818) ^ -8454711130766291883) ^ -1698696699583513602) ^ 5557098897742994028) ^ -2682001481040194238 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[5]) ^ -4672584788418816175) ^ -4600631400985808949) ^ 167286871051437411) ^ 4344668941233943802) ^ 7862938792648136522) ^ 2007286463262236107 ^ rtdsc), cstack0.i, cstack1.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ISTORE; Stack pointer: 1

    clocal0.i = cstack0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L15:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[4]) ^ 9132121263760632830) ^ -3725070636389633305) ^ -7509231958239439548) ^ 367487121214261183) ^ -3442893731051329666) ^ -4332086144588872785 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[5]) ^ -2153923644067319966) ^ -8717382331179123432) ^ -7986615788496882752) ^ 544637216864220107) ^ -8616630557767898545) ^ 4267642002681764251 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[6]) ^ -255114583162874224) ^ 6270901495749529464) ^ -2252753786951206214) ^ -3768018274434011139) ^ -2066594375538306305) ^ -3637526831691368863 ^ rtdsc), cstack0.i, cstack1.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ISTORE; Stack pointer: 1

    clocal1.i = cstack0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L16:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[6]) ^ 5847545789274906631) ^ -8616855811069374298) ^ 6996467489101833822) ^ -1689283437425345343) ^ 87924382841211259) ^ 5134828548674463391 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[7]) ^ 7710751697646445154) ^ 8803537717879377974) ^ 1421733432406960784) ^ -3584592670678606709) ^ 7560296523891853957) ^ -8273322742615478987 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[7]) ^ -290717731400814148) ^ 8703011193474661544) ^ 4885394224222828530) ^ -1350672016389041259) ^ -1144258416717255848) ^ -9140509284313210831 ^ rtdsc), cstack0.i, cstack1.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ISTORE; Stack pointer: 1

    clocal2.i = cstack0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L17:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[8]) ^ -4526522554616538540) ^ 5066195387299662291) ^ 7703254744352480906) ^ -8757896607804850552) ^ 6947807414398154741) ^ -1438764078093672446 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[9]) ^ 1244760556833261884) ^ 9221860839282636122) ^ -8623127567900096752) ^ 6929752032770704347) ^ 9080712431064466277) ^ -2712797876098593749 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.i = env->CallStaticIntMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ 4825110139774061294) ^ -2497667047114368398) ^ -8861819485176302003) ^ -2205044954111608351) ^ 6493844017706181837) ^ -7636305839318051039 ^ rtdsc), cstack0.i, cstack1.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ISTORE; Stack pointer: 1

    clocal3.i = cstack0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L18:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[10]) ^ 9096518115522692906) ^ -1292960938664501225) ^ -371083942770437507) ^ 2784833379259231062) ^ -2520557776525246505) ^ 8611675480793804160 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение суммы: %d + %d = %d (ожидалось 15)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[11]) ^ -7412164112488922400) ^ -6235009369104149489) ^ -2580298173230781880) ^ -7378348891237852194) ^ 5881354903917515326) ^ -801747174796244499 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[12]) ^ -8580661995792045372) ^ 8664096675979969493) ^ -7527808951395376880) ^ 5208455141114679152) ^ -5244338156677083931) ^ 8319913047981733945 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 5

    cstack5.i = clocal0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L19:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[13]) ^ 660511617329184046) ^ 7448041824969919804) ^ -1873510917980134796) ^ 3999782016387161508) ^ -5705506133940125512) ^ -7375339801564380336 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L20;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 1

    cstack1.i = (jint) 15;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IF_ICMPEQ; Stack pointer: 2

    if (cstack0.i == cstack1.i) goto L20;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 2

    cstack2.i = clocal0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[12]) ^ 8255747852357032425) ^ -6385799157858707781) ^ 2615193014493019739) ^ 5195904129198755436) ^ 2047894525563659131) ^ -2102526991637809561 ^ rtdsc), cstack2.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L20:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[14]) ^ -5075168345882676454) ^ 860266675261813893) ^ 7314723383098408118) ^ 4341531187181221049) ^ -8760747113722454800) ^ -598323550937617065 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение произведения: %d * %d = %d (ожидалось 50)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[15]) ^ -3101503351946800220) ^ -6513891452902966451) ^ -2032638862830938296) ^ -8238997337065864792) ^ -1281170387805776130) ^ -4508763753905823756 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[16]) ^ 8476666023485707848) ^ -634670255252085462) ^ -4930063355385992422) ^ 2272209612330723512) ^ 2062303686705796483) ^ -1553848897293824394 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 5

    cstack5.i = clocal1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L21:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[17]) ^ -1933005472938644543) ^ -2966253428572501112) ^ 2914871915333656703) ^ -2379057300003811818) ^ -8602221396625761193) ^ 4816320097025749417 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L22;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 1

    cstack1.i = (jint) 50;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IF_ICMPEQ; Stack pointer: 2

    if (cstack0.i == cstack1.i) goto L22;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 2

    cstack2.i = clocal1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[16]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), cstack2.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L22:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[18]) ^ 9135770900316183474) ^ -2776549126618218064) ^ -8314160174776458418) ^ -3065693399939620948) ^ -668681278312076915) ^ 7103421659576888616 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение разности: %d - %d = %d (ожидалось -5)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[19]) ^ 1754153563305909231) ^ -3535366334435350257) ^ -291519970345035757) ^ -319148983016515244) ^ 4490646387262354612) ^ -2044984586332700882 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[20]) ^ -7194895582210764916) ^ -1432401980563909706) ^ 9126117747136776445) ^ -6868862882657034692) ^ 3121551616615367223) ^ 6758167189091530883 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 5

    cstack5.i = clocal2.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L23:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[21]) ^ 6246479710183216266) ^ 3420468672326171939) ^ 653959197886849281) ^ 1747034837781259541) ^ -4509346237237001786) ^ -3695069241291444837 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L24;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal2.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 1

    cstack1.i = (jint) -5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IF_ICMPEQ; Stack pointer: 2

    if (cstack0.i == cstack1.i) goto L24;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 2

    cstack2.i = clocal2.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[20]) ^ 5701483555472628995) ^ 163061474354706078) ^ -539500384199209673) ^ -7033461962096102605) ^ 1003055761091193040) ^ 8580879085735404648 ^ rtdsc), cstack2.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L24:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[22]) ^ 3360838152310832406) ^ -7880736088372607136) ^ 8817150358308105407) ^ 3126582554348257899) ^ -5575798747717641201) ^ -3058052346583951482 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение частного: %d / %d = %d (ожидалось 2)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[23]) ^ 8148938403348245357) ^ 910529939611655756) ^ 5582892982895506949) ^ -5998801174670853836) ^ 4814902397731843206) ^ -164730275793783847 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[24]) ^ -507986265973938927) ^ 3900403800639454466) ^ -6821021696144729795) ^ -1860158029264826057) ^ 1615544870584892255) ^ 1746320421213598108 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 5

    cstack5.i = clocal3.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), (jmethodID)(((((((__int64)(methods[9]) ^ 4166005262943585668) ^ -355788171453268499) ^ -5477722665785836006) ^ 3132858062453703405) ^ -9221915090985496381) ^ 2153167677520787567 ^ rtdsc), cstack5.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L25:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[25]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L26;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal3.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 1

    cstack1.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IF_ICMPEQ; Stack pointer: 2

    if (cstack0.i == cstack1.i) goto L26;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 2

    cstack2.i = clocal3.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[24]) ^ 4493732883081048159) ^ -1900988362022042421) ^ 2456065073937183535) ^ -7042875219959303568) ^ 6472230275992975809) ^ 764049056020747019 ^ rtdsc), cstack2.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L26:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[26]) ^ 5808293000186448767) ^ -7133267623115657459) ^ -3507200348306729587) ^ 4161243341773506668) ^ -1763952111076991035) ^ 6643082369891378935 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Математические операции пройдены успешно."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L27:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // RETURN; Stack pointer: 0

    return;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[27]) ^ 7928020232219569933) ^ -4840598962994966564) ^ -5318594720935032506) ^ -3075106662097789207) ^ 4800493236589705854) ^ -713408370137769013 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Тестирование манипуляций со строками..."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L30:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 0

    cstack0.l = env->NewStringUTF(("Hello, World!"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ASTORE; Stack pointer: 1

    clocal0.l = cstack0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L31:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 0

    cstack0.l = clocal0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 1

    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[25]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ASTORE; Stack pointer: 1

    clocal1.l = cstack0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L32:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[28]) ^ 1355219642397599596) ^ 2874053255878655122) ^ 6050988320869948783) ^ -3755467258223120127) ^ 9087917015930502249) ^ 6784913207928174642 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение реверсирования строки: '%s' (ожидалось '!dlroW ,olleH')\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 2

    cstack2.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 5

    cstack5.l = clocal1.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L33:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[29]) ^ -6318522166512197214) ^ 1228326540905164770) ^ -998307225595682842) ^ 6749464187362989966) ^ -2369236639999621574) ^ 4528608040435434955 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L34;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 0

    cstack0.l = clocal1.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("!dlroW ,olleH"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[6]) ^ -9164910935296123211) ^ 6890277661667253175) ^ -778192301475414923) ^ 2278485124731136314) ^ -1583812656562058696) ^ 3657371118220980062 ^ rtdsc), ("equals"), ("(Ljava/lang/Object;)Z")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L34;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 2

    cstack2.l = clocal1.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[26]) ^ -6867167957778335129) ^ -2977602171132683628) ^ -1386838586849755630) ^ 1402147908639509951) ^ 368952909884287796) ^ 5369048567591490336 ^ rtdsc), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L34:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[30]) ^ -4088335848914738337) ^ -2826812390968059927) ^ -6582329778868524545) ^ 7274638961912453937) ^ 4202413283943176695) ^ 6669828384433055398 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Манипуляции со строками пройдены успешно."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L35:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // RETURN; Stack pointer: 0

    return;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[31]) ^ 7635895760320047366) ^ -5727508470151324722) ^ -1943786393827567880) ^ 4683280366565215181) ^ 6630755988794702120) ^ 6178692703984113502 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Тестирование сортировки массива..."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L38:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_5; Stack pointer: 0

    cstack0.i = 5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEWARRAY; Stack pointer: 1

        cstack0.l = env->NewIntArray(cstack0.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 2

    cstack2.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_5; Stack pointer: 3

    cstack3.i = 5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 2

    cstack2.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 3

    cstack3.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 2

    cstack2.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 8;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 3

    cstack3.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_4; Stack pointer: 2

    cstack2.i = 4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 3

    cstack3.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ASTORE; Stack pointer: 1

    clocal0.l = cstack0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L39:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 0

    cstack0.l = clocal0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 1

    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[27]) ^ 2776589208511787272) ^ 8792188975319195458) ^ -2879977069776451549) ^ 196612533669747764) ^ -1915273243307648670) ^ -7720594267754770772 ^ rtdsc), cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ASTORE; Stack pointer: 1

    clocal1.l = cstack0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L40:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_5; Stack pointer: 0

    cstack0.i = 5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEWARRAY; Stack pointer: 1

        cstack0.l = env->NewIntArray(cstack0.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 2

    cstack2.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 3

    cstack3.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 2

    cstack2.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 3

    cstack3.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 2

    cstack2.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 3

    cstack3.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_5; Stack pointer: 3

    cstack3.i = 5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_4; Stack pointer: 2

    cstack2.i = 4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 3

    cstack3.i = (jint) 8;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IASTORE; Stack pointer: 4

    env->SetIntArrayRegion((jintArray) cstack1.l, cstack2.i, 1, &cstack3.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ASTORE; Stack pointer: 1

    clocal2.l = cstack0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L41:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[32]) ^ -4163191786241136125) ^ 1088885499005756289) ^ 8498894468606498407) ^ -2904232074553275789) ^ 3272872748846024858) ^ 2675099753028128974 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение сортировки массива: %s (ожидалось %s)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 2

    cstack2.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 5

    cstack5.l = clocal1.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L42:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[7]) ^ -1972258257732135111) ^ -1482665240618784273) ^ -7588795930664841298) ^ 3471469483490007489) ^ 7992646183165588129) ^ 6324573918242664961 ^ rtdsc), (jmethodID)(((((((__int64)(methods[28]) ^ 3506900386113110042) ^ 1786090704207831399) ^ -2093625846395370010) ^ 8470761079019015161) ^ -6490930125967622982) ^ -6504102879349925443 ^ rtdsc), cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 5

    cstack5.l = clocal2.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L43:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[7]) ^ -1972258257732135111) ^ -1482665240618784273) ^ -7588795930664841298) ^ 3471469483490007489) ^ 7992646183165588129) ^ 6324573918242664961 ^ rtdsc), (jmethodID)(((((((__int64)(methods[28]) ^ 3506900386113110042) ^ 1786090704207831399) ^ -2093625846395370010) ^ 8470761079019015161) ^ -6490930125967622982) ^ -6504102879349925443 ^ rtdsc), cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L44:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L45:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[33]) ^ 2815841997600245136) ^ 7308600783070511323) ^ 7623690776222046452) ^ -5653914245529104246) ^ -63396749389446376) ^ 9217895984737865300 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L46;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 0

    cstack0.l = clocal1.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 1

    cstack1.l = clocal2.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.i = (jint) env->CallStaticBooleanMethod((jclass)(((((((__int64)(classes[7]) ^ -1972258257732135111) ^ -1482665240618784273) ^ -7588795930664841298) ^ 3471469483490007489) ^ 7992646183165588129) ^ 6324573918242664961 ^ rtdsc), (jmethodID)(((((((__int64)(methods[30]) ^ 6100417467791004039) ^ -6246358111664332004) ^ -6882008671119226918) ^ -3597143682594530425) ^ -3594214863281987301) ^ -249018708525470876 ^ rtdsc), cstack0.l, cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L46;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.l = env->NewStringUTF(("Ошибка: Массив не отсортирован правильно."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L46:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[34]) ^ 6246479710183216266) ^ 3420468672326171939) ^ 653959197886849281) ^ 1747034837781259541) ^ -4509346237237001786) ^ -3695069241291444837 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сортировка массива пройдена успешно."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L47:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // RETURN; Stack pointer: 0

    return;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[35]) ^ 988239233171679241) ^ 5902841634401145883) ^ 6060276817447917449) ^ -6175951270320812760) ^ -8458104849261139531) ^ -8764458423064420883 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Тестирование операций с float..."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L50:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[36]) ^ -4273650876100441132) ^ 7436693082409737289) ^ -6175221420163547129) ^ 7780987220735515982) ^ 3265668163979988886) ^ -6822611330998639417 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[37]) ^ -5221230579684954090) ^ -8806560113023657346) ^ -221244494497602673) ^ -1002647333194568917) ^ -7845615735472473019) ^ 2847726986123324192 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[31]) ^ -985425760764196993) ^ -5169744306848658093) ^ -3039105010332287753) ^ 6404577258221240376) ^ 2509062502826700712) ^ -4854018224391181488 ^ rtdsc), cstack0.f, cstack1.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSTORE; Stack pointer: 1

    clocal0.f = cstack0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L51:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[38]) ^ 3506900386113110042) ^ 1786090704207831399) ^ -2093625846395370010) ^ 8470761079019015161) ^ -6490930125967622982) ^ -6504102879349925443 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[39]) ^ -6208063080947859503) ^ -5119481046793783526) ^ -4770935410535188922) ^ -3935755107925801805) ^ -2362032059428552898) ^ -4420424949247348270 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[32]) ^ 7489833526517769731) ^ 3052408815272755654) ^ 8966989806580940241) ^ -660898158105542080) ^ 7545887362749716604) ^ -8822000836959464153 ^ rtdsc), cstack0.f, cstack1.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSTORE; Stack pointer: 1

    clocal1.f = cstack0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L52:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[40]) ^ -5915938609048336936) ^ -4232571539637425368) ^ -8145743737642653548) ^ 6752601941415712719) ^ -4192294815928516460) ^ 7134218050340320831 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[41]) ^ -8580661995792045372) ^ 8664096675979969493) ^ -7527808951395376880) ^ 5208455141114679152) ^ -5244338156677083931) ^ 8319913047981733945 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[33]) ^ 4532985672169506023) ^ -3384576549975759260) ^ -5487011158068837376) ^ 5553342070256428742) ^ 8324106769911178103) ^ -744204769491135821 ^ rtdsc), cstack0.f, cstack1.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSTORE; Stack pointer: 1

    clocal2.f = cstack0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L53:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[42]) ^ 8547872324256554992) ^ -5498889650702349623) ^ -759615308319477592) ^ -2562482895169281656) ^ 217631773358662865) ^ -8994628070054659371 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 1

    cstack1.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[43]) ^ -8872786467691567940) ^ 7777187168823611334) ^ -4153000628582879549) ^ -5479901908226835371) ^ -3414075404472087666) ^ -3234729951605935157 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[34]) ^ -4526522554616538540) ^ 5066195387299662291) ^ 7703254744352480906) ^ -8757896607804850552) ^ 6947807414398154741) ^ -1438764078093672446 ^ rtdsc), cstack0.f, cstack1.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSTORE; Stack pointer: 1

    clocal3.f = cstack0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L54:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[44]) ^ -7779144521714842754) ^ -3206220990581658728) ^ -2571009676652813215) ^ 8647911174668974085) ^ 6782077121025359754) ^ 2095625263625744297 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение суммы: %.2f + %.2f = %.2f (ожидалось 8.00)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[45]) ^ -3871067318636580853) ^ 1975795006162114447) ^ 5124086145794001076) ^ 7784124974788238735) ^ 1442609996641028592) ^ -4217001321093753541 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[46]) ^ -5296086517011351877) ^ -4890862227344808427) ^ -3586764325027098633) ^ 7265225704049252974) ^ -8775156270569624856) ^ -1147001645281602231 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 5

    cstack5.f = clocal0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L55:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[47]) ^ -4198794938774043345) ^ 3520995196730888369) ^ -2809701589634051169) ^ -486885816508305909) ^ 4195208703372108019) ^ -2827882699593712993 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L56;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 0

    cstack0.f = clocal0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.f = 8.0f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSUB; Stack pointer: 2

    cstack0.f = cstack0.f - cstack1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 1

    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), (jmethodID)(((((((__int64)(methods[38]) ^ 5701483555472628995) ^ 163061474354706078) ^ -539500384199209673) ^ -7033461962096102605) ^ 1003055761091193040) ^ 8580879085735404648 ^ rtdsc), cstack0.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // F2D; Stack pointer: 1

    cstack0.d = (jdouble) cstack0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 1.0E-4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DCMPG; Stack pointer: 4

    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFLT; Stack pointer: 1

    if (cstack0.i < 0) goto L56;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 2

    cstack2.f = clocal0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[39]) ^ -1826196019634890179) ^ 8184161547666686966) ^ -52828053068830507) ^ 8815648008160764750) ^ 7077514804915606348) ^ 2878523381181723704 ^ rtdsc), cstack2.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L56:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[48]) ^ 8255747852357032425) ^ -6385799157858707781) ^ 2615193014493019739) ^ 5195904129198755436) ^ 2047894525563659131) ^ -2102526991637809561 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение разности: %.2f - %.2f = %.2f (ожидалось 3.00)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[49]) ^ 111865826063046131) ^ 3242113112932071406) ^ -2262042283529174880) ^ -1347534262336318506) ^ -2967316592646150733) ^ -6534899274408324955 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[50]) ^ 1900215797108186866) ^ 6131460453850120982) ^ 7244447902956007738) ^ 5025029537359274722) ^ 3575515013307340127) ^ -5491035119098674844 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 5

    cstack5.f = clocal2.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L57:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[51]) ^ 3325235004072892482) ^ -5448626386352507760) ^ -2491445704227411465) ^ 5543928812393227779) ^ -4653462793191558040) ^ -8561034794910826153 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L58;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 0

    cstack0.f = clocal2.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.f = 3.0f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSUB; Stack pointer: 2

    cstack0.f = cstack0.f - cstack1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 1

    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), (jmethodID)(((((((__int64)(methods[38]) ^ 5701483555472628995) ^ 163061474354706078) ^ -539500384199209673) ^ -7033461962096102605) ^ 1003055761091193040) ^ 8580879085735404648 ^ rtdsc), cstack0.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // F2D; Stack pointer: 1

    cstack0.d = (jdouble) cstack0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 1.0E-4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DCMPG; Stack pointer: 4

    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFLT; Stack pointer: 1

    if (cstack0.i < 0) goto L58;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 2

    cstack2.f = clocal2.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[44]) ^ 9025311814751845762) ^ 3571258456785762936) ^ -4541531989836952338) ^ 7619525891054203526) ^ -675885858883145591) ^ -2394289424449879776 ^ rtdsc), cstack2.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L58:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[52]) ^ 8401810086159310060) ^ 3281027634721730754) ^ -8295583181620521086) ^ -7906661419840038918) ^ 1132763147313677350) ^ -5548577532993718114 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение частного: %.2f / %.2f = %.2f (ожидалось 2.20)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[53]) ^ 5772689851948508843) ^ -4701157921095558082) ^ 3630947662867305157) ^ 6578589599818476548) ^ -841616156550907874) ^ 1140099912974569671 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.f = env->GetStaticFloatField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[54]) ^ -7889603607279180466) ^ 3141586592822322273) ^ 1201618508286692866) ^ 886386391953246943) ^ 6774872540454291078) ^ -7402085820401024094 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 5

    cstack5.f = clocal3.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 6

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7451416901577380263) ^ -4751421185445399945) ^ 5362778058775239030) ^ -1527822107744032888) ^ 4029478409999313032) ^ 706506642125703749 ^ rtdsc), cstack5.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L59:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[55]) ^ -7013230191580612764) ^ 5802315109996429453) ^ -8922806464445766421) ^ -3942030611736280015) ^ 1284084283839302281) ^ 8815099108947398889 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L60;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 0

    cstack0.f = clocal3.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.f = 2.2f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSUB; Stack pointer: 2

    cstack0.f = cstack0.f - cstack1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 1

    cstack0.f = env->CallStaticFloatMethod((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), (jmethodID)(((((((__int64)(methods[38]) ^ 5701483555472628995) ^ 163061474354706078) ^ -539500384199209673) ^ -7033461962096102605) ^ 1003055761091193040) ^ 8580879085735404648 ^ rtdsc), cstack0.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // F2D; Stack pointer: 1

    cstack0.d = (jdouble) cstack0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 1.0E-4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DCMPG; Stack pointer: 4

    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFLT; Stack pointer: 1

    if (cstack0.i < 0) goto L60;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 2

    cstack2.f = clocal3.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 3

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[49]) ^ 2630526970414542340) ^ -874637817261243077) ^ 8030799126337089276) ^ -5147565986706042201) ^ -1000141869352634185) ^ -4274543730693829515 ^ rtdsc), cstack2.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L60:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[56]) ^ 1244760556833261884) ^ 9221860839282636122) ^ -8623127567900096752) ^ 6929752032770704347) ^ 9080712431064466277) ^ -2712797876098593749 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Операции с float пройдены успешно."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L61:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // RETURN; Stack pointer: 0

    return;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[57]) ^ 7379374440953432019) ^ -9046527675032814962) ^ -5707126086484072591) ^ -8422422940821269222) ^ 7538682786473615224) ^ 127032152723319072 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Тестирование операций с double..."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L64:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[58]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 2

    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[59]) ^ 1647344118592089459) ^ 3760962758740045984) ^ 2676179998057451453) ^ 6932889791118394396) ^ 7257654259430538687) ^ -107187866193707873 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[50]) ^ 5516168528286926200) ^ -8020177130272015618) ^ -132392025494232257) ^ -6527113703273040560) ^ 66310641128005231) ^ -4911560629696290166 ^ rtdsc), cstack0.d, cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSTORE; Stack pointer: 2

    clocal0.d = cstack0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L65:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[60]) ^ 9206977201087030618) ^ -7640768522068482225) ^ -4143712127709943588) ^ -7900385916029560708) ^ -2513353195954177829) ^ -337357508888979065 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 2

    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[61]) ^ 6431794728778984469) ^ -6843036796756657981) ^ 246850839181871865) ^ 1240686578958197496) ^ -3572601117273813977) ^ -8649373603864268935 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[51]) ^ -1826196019634890179) ^ 8184161547666686966) ^ -52828053068830507) ^ 8815648008160764750) ^ 7077514804915606348) ^ 2878523381181723704 ^ rtdsc), cstack0.d, cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSTORE; Stack pointer: 2

    clocal2.d = cstack0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L66:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[62]) ^ 2851445145838185060) ^ 4876491089640346539) ^ 485542756458077115) ^ -8071260503574074126) ^ -985732703915529537) ^ -3725865636349844349 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 2

    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[63]) ^ -874966675199859282) ^ 6929192183456912523) ^ -6811733199566761129) ^ -4280642037067551394) ^ 2516267083397769388) ^ 4643692863930554199 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[52]) ^ 1098698323030984248) ^ -444965953297802414) ^ 2287648628213444073) ^ 1585573512394914381) ^ -8450900264395103558) ^ 733252660962347508 ^ rtdsc), cstack0.d, cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSTORE; Stack pointer: 2

    clocal4.d = cstack0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L67:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[64]) ^ -3286818374837535719) ^ 3749614016179863469) ^ -1625530504125960880) ^ -7732649078242802747) ^ -2217915507768963939) ^ 445540604372033046 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 2

    cstack2.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[65]) ^ -4234398087011983269) ^ 5953104898750987746) ^ 4328446421539983576) ^ 1930460441536663971) ^ 5117544662193158476) ^ -8330865147920587665 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[53]) ^ -6792312020451937341) ^ -6893300061106499844) ^ 1978681235089805738) ^ -6865725128604311939) ^ 1298493440686472337) ^ -9082966870418167561 ^ rtdsc), cstack0.d, cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSTORE; Stack pointer: 2

    clocal6.d = cstack0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L68:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[66]) ^ 8622728261582952779) ^ 9032156537328353074) ^ 2605904522210018369) ^ 7616388141296448069) ^ 1147172312750781998) ^ -4999899434354765652 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение суммы: %.2f + %.2f = %.2f (ожидалось 13.00)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[67]) ^ 3107966465204800407) ^ 8195510290226869482) ^ 4248882449114581826) ^ 5034442803812410277) ^ -1893659497299475346) ^ 2325794910615982785 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[68]) ^ -215861798369383656) ^ 4787313307795812624) ^ 8250914054752324491) ^ 8828199020076688466) ^ -214717877325136715) ^ -5145780648613317111 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 5

    cstack5.d = clocal0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L69:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[69]) ^ -7120039644884367128) ^ -5348099866242758626) ^ -5955106496043279211) ^ 3310008153808695033) ^ 4051092156007486356) ^ -7693848248918127013 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L70;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 13.0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSUB; Stack pointer: 4

    cstack0.d = cstack0.d - cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 8295000641445490288) ^ -7869387345812424621) ^ -5327883213218033876) ^ -654622650000096574) ^ 3899771019481861425) ^ -3610780812854725105 ^ rtdsc), cstack0.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 1.0E-4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DCMPG; Stack pointer: 4

    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFLT; Stack pointer: 1

    if (cstack0.i < 0) goto L70;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[58]) ^ -1897402320405737323) ^ -5398363130592600489) ^ -4223276100135345338) ^ -4796403553753814402) ^ 8922186718262739966) ^ -8127441524061960231 ^ rtdsc), cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L70:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[70]) ^ -5039565197644736530) ^ -1571843018168350891) ^ 176575367629406077) ^ 1924184933431218465) ^ 8763661005461013655) ^ 4904658901684224902 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение произведения: %.2f * %.2f = %.2f (ожидалось 30.00)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[71]) ^ 5847545789274906631) ^ -8616855811069374298) ^ 6996467489101833822) ^ -1689283437425345343) ^ 87924382841211259) ^ 5134828548674463391 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[72]) ^ -6646249786649659706) ^ 2773526731473938692) ^ -8932094961023735087) ^ -1521546603933554678) ^ 383362066731457852) ^ 5917726661935475502 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 5

    cstack5.d = clocal2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L71:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[73]) ^ 329134356341203615) ^ 8044720510062245781) ^ -9002370436871168171) ^ -838048253755501004) ^ -5727119879948298836) ^ 1025015093774417723 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L72;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 30.0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSUB; Stack pointer: 4

    cstack0.d = cstack0.d - cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 8295000641445490288) ^ -7869387345812424621) ^ -5327883213218033876) ^ -654622650000096574) ^ 3899771019481861425) ^ -3610780812854725105 ^ rtdsc), cstack0.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 1.0E-4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DCMPG; Stack pointer: 4

    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFLT; Stack pointer: 1

    if (cstack0.i < 0) goto L72;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[63]) ^ 3179172765975647551) ^ 3331290894776605321) ^ 8419330491886129361) ^ 199750287722470517) ^ -3738331410646608964) ^ -5114984257849884896 ^ rtdsc), cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L72:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[74]) ^ -4672584788418816175) ^ -4600631400985808949) ^ 167286871051437411) ^ 4344668941233943802) ^ 7862938792648136522) ^ 2007286463262236107 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение разности: %.2f - %.2f = %.2f (ожидалось 7.00)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[75]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[76]) ^ -5769876375246059300) ^ 5434255252943013168) ^ -609775860046642757) ^ -6349963611918048931) ^ -5107426189883530945) ^ 3688167513279379573 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 5

    cstack5.d = clocal4.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L73:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[77]) ^ 8148938403348245357) ^ 910529939611655756) ^ 5582892982895506949) ^ -5998801174670853836) ^ 4814902397731843206) ^ -164730275793783847 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L74;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal4.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 7.0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSUB; Stack pointer: 4

    cstack0.d = cstack0.d - cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 8295000641445490288) ^ -7869387345812424621) ^ -5327883213218033876) ^ -654622650000096574) ^ 3899771019481861425) ^ -3610780812854725105 ^ rtdsc), cstack0.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 1.0E-4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DCMPG; Stack pointer: 4

    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFLT; Stack pointer: 1

    if (cstack0.i < 0) goto L74;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal4.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[68]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L74:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[78]) ^ 2010674882672524578) ^ -216347133848827314) ^ 3471819718016501657) ^ -5660189749339582456) ^ 3582719593878408803) ^ 4006675964928093547 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Сравнение частного: %.2f / %.2f = %.2f (ожидалось 3.33)\n"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_3; Stack pointer: 2

    cstack2.i = 3;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ANEWARRAY; Stack pointer: 3

    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[3]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), nullptr); }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 4

    cstack4.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[79]) ^ 8658331409820892703) ^ 6600046839603220993) ^ -4532243493258983672) ^ 5199041883251478189) ^ 224836353929731542) ^ 503083013972109020 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 4

    cstack4.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 5

    cstack5.d = env->GetStaticDoubleField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[80]) ^ -4783043878278121183) ^ 1747176186713139348) ^ 3939915055990943492) ^ -3416855837186816044) ^ 7855734207782100550) ^ -7490424620764532284 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 3

    cstack3 = cstack2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_2; Stack pointer: 4

    cstack4.i = 2;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 5

    cstack5.d = clocal6.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 7

    cstack5.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), (jmethodID)(((((((__int64)(methods[54]) ^ 257928059865323767) ^ -5537804168197041674) ^ 5273925589771868615) ^ 3996644262334438755) ^ -3882447966601165218) ^ 8465794262240285404 ^ rtdsc), cstack5.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // AASTORE; Stack pointer: 6

    env->SetObjectArrayElement((jobjectArray) cstack3.l, cstack4.i, cstack5.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 3

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("printf"), ("(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;")), cstack1.l, cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // POP; Stack pointer: 1

    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L75:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.i =  env->GetStaticBooleanField((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jfieldID)(((((((__int64)(fields[81]) ^ 3179172765975647551) ^ 3331290894776605321) ^ 8419330491886129361) ^ 199750287722470517) ^ -3738331410646608964) ^ -5114984257849884896 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L76;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal6.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 3.33;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSUB; Stack pointer: 4

    cstack0.d = cstack0.d - cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 2

    cstack0.d = env->CallStaticDoubleMethod((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), (jmethodID)(((((((__int64)(methods[57]) ^ 8295000641445490288) ^ -7869387345812424621) ^ -5327883213218033876) ^ -654622650000096574) ^ 3899771019481861425) ^ -3610780812854725105 ^ rtdsc), cstack0.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 2

    cstack2.d = 1.0E-4;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DCMPG; Stack pointer: 4

    {
        jdouble value1 = cstack0.d;
        jdouble value2 = cstack2.d;
        cstack0.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));
    }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFLT; Stack pointer: 1

    if (cstack0.i < 0) goto L76;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal6.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 4

    cstack2.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), (jmethodID)(((((((__int64)(methods[73]) ^ 4639795121178293091) ^ 7765838426263428818) ^ -8454711130766291883) ^ -1698696699583513602) ^ 5557098897742994028) ^ -2682001481040194238 ^ rtdsc), cstack2.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[5]) ^ -5185627431447014166) ^ 7208074262960762189) ^ -7359392509966604714) ^ -3419993591239538797) ^ -8767951689998556180) ^ 8350709438745166160 ^ rtdsc), ("<init>"), ("(Ljava/lang/Object;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ATHROW; Stack pointer: 1

    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L76:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GETSTATIC; Stack pointer: 0

    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jfieldID)(((((((__int64)(fields[82]) ^ -7814747669952782678) ^ -774111292856526647) ^ 4567138338816188826) ^ -7381486645290574947) ^ 7704413079846410211) ^ -3407357188996097671 ^ rtdsc));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 1

    cstack1.l = env->NewStringUTF(("Операции с double пройдены успешно."));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 2

    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[1]) ^ 2705382907740940128) ^ -4790335702940091997) ^ -7050425116842966379) ^ 5031305049759687524) ^ -70601329960515052) ^ -279815099288903091 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L77:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // RETURN; Stack pointer: 0

    return;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 1

    cstack1.i = clocal1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IADD; Stack pointer: 2

    cstack0.i = cstack0.i + cstack1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IRETURN; Stack pointer: 1

    return (jint) cstack0.i;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 1

    cstack1.i = clocal1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IMUL; Stack pointer: 2

    cstack0.i = cstack0.i * cstack1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IRETURN; Stack pointer: 1

    return (jint) cstack0.i;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 1

    cstack1.i = clocal1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ISUB; Stack pointer: 2

    cstack0.i = cstack0.i - cstack1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IRETURN; Stack pointer: 1

    return (jint) cstack0.i;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 0

    cstack0.i = clocal0.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ILOAD; Stack pointer: 1

    cstack1.i = clocal1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IDIV; Stack pointer: 2

    cstack0.i = cstack0.i / cstack1.i;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IRETURN; Stack pointer: 1

    return (jint) cstack0.i;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 0

    cstack0.f = clocal0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 1

    cstack1.f = clocal1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FADD; Stack pointer: 2

    cstack0.f = cstack0.f + cstack1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FRETURN; Stack pointer: 1

    return (jfloat) cstack0.f;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 0

    cstack0.f = clocal0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 1

    cstack1.f = clocal1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FMUL; Stack pointer: 2

    cstack0.f = cstack0.f * cstack1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FRETURN; Stack pointer: 1

    return (jfloat) cstack0.f;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 0

    cstack0.f = clocal0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 1

    cstack1.f = clocal1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FSUB; Stack pointer: 2

    cstack0.f = cstack0.f - cstack1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FRETURN; Stack pointer: 1

    return (jfloat) cstack0.f;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 0

    cstack0.f = clocal0.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FLOAD; Stack pointer: 1

    cstack1.f = clocal1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FDIV; Stack pointer: 2

    cstack0.f = cstack0.f / cstack1.f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // FRETURN; Stack pointer: 1

    return (jfloat) cstack0.f;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DADD; Stack pointer: 4

    cstack0.d = cstack0.d + cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DRETURN; Stack pointer: 2

    return (jdouble) cstack0.d;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DMUL; Stack pointer: 4

    cstack0.d = cstack0.d * cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DRETURN; Stack pointer: 2

    return (jdouble) cstack0.d;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DSUB; Stack pointer: 4

    cstack0.d = cstack0.d - cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DRETURN; Stack pointer: 2

    return (jdouble) cstack0.d;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 0

    cstack0.d = clocal0.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DLOAD; Stack pointer: 2

    cstack2.d = clocal2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DDIV; Stack pointer: 4

    cstack0.d = cstack0.d / cstack2.d;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DRETURN; Stack pointer: 2

    return (jdouble) cstack0.d;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // NEW; Stack pointer: 0

    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[11]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc))) { cstack0.l = obj; }
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // DUP; Stack pointer: 1

    cstack1 = cstack0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 2

    cstack2.l = clocal0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESPECIAL; Stack pointer: 3

    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[11]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;)V")), cstack2.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 1

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), ("reverse"), ("()Ljava/lang/StringBuilder;")));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 1

    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), ("toString"), ("()Ljava/lang/String;")));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ARETURN; Stack pointer: 1

    return (jobject) cstack0.l;
    
    }

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
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 0

    cstack0.l = clocal0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKESTATIC; Stack pointer: 1

    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[7]) ^ -1972258257732135111) ^ -1482665240618784273) ^ -7588795930664841298) ^ 3471469483490007489) ^ 7992646183165588129) ^ 6324573918242664961 ^ rtdsc), (jmethodID)(((((((__int64)(methods[74]) ^ 4312067496745863303) ^ -9135705452582381580) ^ 2058245211810174784) ^ 8477036582829493371) ^ 8309697604474073455) ^ -1292882863835120987 ^ rtdsc), cstack0.l);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L106:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ALOAD; Stack pointer: 0

    cstack0.l = clocal0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ARETURN; Stack pointer: 1

    return (jarray) cstack0.l;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L107:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jarray) 0; }

    return (jarray) 0;
}
extern "C" JNIEXPORT void JNICALL  Java_ru_kotopushka_Main__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 2, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack_exception = {};
    jvalue clocal0 = {};

          VMProtectBeginUltra("clinit_740444269656754756");
JNINativeMethod jniMethods[] = {
        (char*)xorstr_("main"), (char*)xorstr_("([Ljava/lang/String;)V"), &Java_ru_kotopushka_Main_main0,
        (char*)xorstr_("testMathOperations"), (char*)xorstr_("()V"), &Java_ru_kotopushka_Main_testMathOperations1,
        (char*)xorstr_("testStringManipulation"), (char*)xorstr_("()V"), &Java_ru_kotopushka_Main_testStringManipulation2,
        (char*)xorstr_("testArraySorting"), (char*)xorstr_("()V"), &Java_ru_kotopushka_Main_testArraySorting3,
        (char*)xorstr_("testFloatOperations"), (char*)xorstr_("()V"), &Java_ru_kotopushka_Main_testFloatOperations4,
        (char*)xorstr_("testDoubleOperations"), (char*)xorstr_("()V"), &Java_ru_kotopushka_Main_testDoubleOperations5,
        (char*)xorstr_("add"), (char*)xorstr_("(II)I"), &Java_ru_kotopushka_Main_add6,
        (char*)xorstr_("multiply"), (char*)xorstr_("(II)I"), &Java_ru_kotopushka_Main_multiply7,
        (char*)xorstr_("subtract"), (char*)xorstr_("(II)I"), &Java_ru_kotopushka_Main_subtract8,
        (char*)xorstr_("divide"), (char*)xorstr_("(II)I"), &Java_ru_kotopushka_Main_divide9,
        (char*)xorstr_("add"), (char*)xorstr_("(FF)F"), &Java_ru_kotopushka_Main_add10,
        (char*)xorstr_("multiply"), (char*)xorstr_("(FF)F"), &Java_ru_kotopushka_Main_multiply11,
        (char*)xorstr_("subtract"), (char*)xorstr_("(FF)F"), &Java_ru_kotopushka_Main_subtract12,
        (char*)xorstr_("divide"), (char*)xorstr_("(FF)F"), &Java_ru_kotopushka_Main_divide13,
        (char*)xorstr_("add"), (char*)xorstr_("(DD)D"), &Java_ru_kotopushka_Main_add14,
        (char*)xorstr_("multiply"), (char*)xorstr_("(DD)D"), &Java_ru_kotopushka_Main_multiply15,
        (char*)xorstr_("subtract"), (char*)xorstr_("(DD)D"), &Java_ru_kotopushka_Main_subtract16,
        (char*)xorstr_("divide"), (char*)xorstr_("(DD)D"), &Java_ru_kotopushka_Main_divide17,
        (char*)xorstr_("reverseString"), (char*)xorstr_("(Ljava/lang/String;)Ljava/lang/String;"), &Java_ru_kotopushka_Main_reverseString18,
        (char*)xorstr_("sortArray"), (char*)xorstr_("([I)[I"), &Java_ru_kotopushka_Main_sortArray19,
    };
    env->RegisterNatives(env->FindClass(xorstr_("ru/kotopushka/Main")), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));
VMProtectEnd();

    /* CLASS REFERENCE TABLE */
    VMProtectBeginUltra("classes_-4804851434923530065");
    classes[0] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=412&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/System"))) ^ 1483839388960249092), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_5841603129441545325");
    classes[1] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=426&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/io/PrintStream"))) ^ 1407825126633040834), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_-1197919656735750424");
    classes[2] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=355&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("ru/kotopushka/Main"))) ^ 1102115598402490971), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_5215607289508586419");
    classes[3] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=29&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Object"))) ^ 1693704847673446569), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_-7363402688161862316");
    classes[4] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=384&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Integer"))) ^ 1477229451798545284), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_1437858931183058143");
    classes[5] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=43&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/AssertionError"))) ^ 1617690589641205608), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_8777870273781877862");
    classes[6] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=360&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/String"))) ^ 1093853173729135739), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_-8573368985910192179");
    classes[7] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=177&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/util/Arrays"))) ^ 1872173112384743726), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_5605196642513000973");
    classes[8] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=172&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Float"))) ^ 1827556048354403085), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_-5878030234940861581");
    classes[9] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=195&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Math"))) ^ 1789548917190798956), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_955757268345476655");
    classes[10] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=30&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Double"))) ^ 1692052364456762441), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("classes_-3850336773938903698");
    classes[11] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=222&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/StringBuilder"))) ^ 1797811337569186892), rtdsc)));
    VMProtectEnd();

    /* METHOD REFERENCE TABLE */
    VMProtectBeginUltra("method_7718858439293923469");
    methods[0] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=172&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("testMathOperations"), ("()V")) ^ 1827556048354403085), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_2846846657302494305");
    methods[1] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=430&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("testStringManipulation"), ("()V")) ^ 1401215193766304322), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-2563966566229557434");
    methods[2] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=221&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("testArraySorting"), ("()V")) ^ 1799463820785871020), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_8681780694596284234");
    methods[3] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=160&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("testFloatOperations"), ("()V")) ^ 1847385855544547213), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-1118302595995672109");
    methods[4] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=214&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("testDoubleOperations"), ("()V")) ^ 1811031207597627212), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-8497681402544942016");
    methods[5] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=271&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("add"), ("(II)I")) ^ 1240924244438532574), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-7182138909648287974");
    methods[6] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=221&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("multiply"), ("(II)I")) ^ 1799463820785871020), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_7937276903752826315");
    methods[7] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=93&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("subtract"), ("(II)I")) ^ 1587945874561022119), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_4027741541174609321");
    methods[8] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=321&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("divide"), ("(II)I")) ^ 1158300049244587804), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_4359144694250473751");
    methods[9] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=100&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[4]) ^ 3581756323439507830) ^ -2129607181471017521) ^ 1271893984134125950) ^ 202888041775193270) ^ -5561389586575503849) ^ -2509374247944999020 ^ rtdsc), ("valueOf"), ("(I)Ljava/lang/Integer;")) ^ 1523498999045570053), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-6786701857482760689");
    methods[12] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=160&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello0_proxy1"), ("(I)Ljava/lang/String;")) ^ 1847385855544547213), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-7382739998239648557");
    methods[16] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=355&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello1_proxy2"), ("(I)Ljava/lang/String;")) ^ 1102115598402490971), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-1986593709174972962");
    methods[20] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=311&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello2_proxy3"), ("(I)Ljava/lang/String;")) ^ 1227704374410092254), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_8626395356761389822");
    methods[24] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=246&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello3_proxy4"), ("(I)Ljava/lang/String;")) ^ 1758151723188898635), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-6929872102230280428");
    methods[25] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=307&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("reverseString"), ("(Ljava/lang/String;)Ljava/lang/String;")) ^ 1234314311571796062), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_3017118795491032632");
    methods[26] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=159&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello4_proxy5"), ("(Ljava/lang/String;)Ljava/lang/String;")) ^ 1901917823169959919), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-6367622672165136873");
    methods[27] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=170&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("sortArray"), ("([I)[I")) ^ 1830861014787771341), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_4489004577050898477");
    methods[28] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=135&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[7]) ^ -1972258257732135111) ^ -1482665240618784273) ^ -7588795930664841298) ^ 3471469483490007489) ^ 7992646183165588129) ^ 6324573918242664961 ^ rtdsc), ("toString"), ("([I)Ljava/lang/String;")) ^ 1888697953141519598), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_6608086166907704235");
    methods[30] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=137&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[7]) ^ -1972258257732135111) ^ -1482665240618784273) ^ -7588795930664841298) ^ 3471469483490007489) ^ 7992646183165588129) ^ 6324573918242664961 ^ rtdsc), ("equals"), ("([I[I)Z")) ^ 1885392982413184046), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-7617980712819786335");
    methods[31] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=224&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("add"), ("(FF)F")) ^ 1741626882432122763), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_387651698502635370");
    methods[32] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=67&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("multiply"), ("(FF)F")) ^ 1578030970965950055), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_7983927010382848716");
    methods[33] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=335&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("subtract"), ("(FF)F")) ^ 1135165275621075420), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_5048972253499979490");
    methods[34] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=264&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("divide"), ("(FF)F")) ^ 1252491635545256063), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_7092073659672966523");
    methods[35] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=187&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[8]) ^ 5370106294484648565) ^ 759740159447032055) ^ -7668359898795275752) ^ 6575451841470786499) ^ 981442019377987012) ^ -1465510096930316205 ^ rtdsc), ("valueOf"), ("(F)Ljava/lang/Float;")) ^ 1855648275922935149), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_3420584680871381491");
    methods[38] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=311&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), ("abs"), ("(F)F")) ^ 1227704374410092254), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_8727834167495558830");
    methods[39] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=138&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello5_proxy6"), ("(F)Ljava/lang/String;")) ^ 1883740503491467214), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-4389218853210816702");
    methods[44] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=278&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello6_proxy7"), ("(F)Ljava/lang/String;")) ^ 1282236346330472255), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_1385026256726907038");
    methods[49] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=209&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello7_proxy8"), ("(F)Ljava/lang/String;")) ^ 1819293627976015148), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-4541526557746854822");
    methods[50] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=165&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("add"), ("(DD)D")) ^ 1839123435166159277), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-9042686207462133025");
    methods[51] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=138&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("multiply"), ("(DD)D")) ^ 1883740503491467214), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-941382267910109131");
    methods[52] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=279&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("subtract"), ("(DD)D")) ^ 1280583863113788127), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_-9112919976302895073");
    methods[53] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=408&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("divide"), ("(DD)D")) ^ 1490449321826985604), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_5320890826831683929");
    methods[54] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=417&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[10]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), ("valueOf"), ("(D)Ljava/lang/Double;")) ^ 1422697479878165283), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_5034342924925240156");
    methods[57] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=313&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[9]) ^ 7525436674755709655) ^ 620299113252656278) ^ 1828841791111938200) ^ -3078244411855544664) ^ 6623551403928666147) ^ -3319018380042654890 ^ rtdsc), ("abs"), ("(D)D")) ^ 1224399407976723998), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_5838848398159328912");
    methods[58] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=394&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello8_proxy9"), ("(D)Ljava/lang/String;")) ^ 1460704611041769412), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_3453759416177344782");
    methods[63] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=53&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello9_proxy10"), ("(D)Ljava/lang/String;")) ^ 1654045233293158312), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_2884706288108617372");
    methods[68] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=120&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello10_proxy11"), ("(D)Ljava/lang/String;")) ^ 1543328810530681478), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_3698743506090669142");
    methods[73] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=207&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$hello11_proxy12"), ("(D)Ljava/lang/String;")) ^ 1769719110000654827), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("method_7545967739814937001");
    methods[74] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=125&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[7]) ^ -1972258257732135111) ^ -1482665240618784273) ^ -7588795930664841298) ^ 3471469483490007489) ^ 7992646183165588129) ^ 6324573918242664961 ^ rtdsc), ("sort"), ("([I)V")) ^ 1535066390152293541), rtdsc)));
    VMProtectEnd();

    /* FIELD REFERENCE TABLE */
    VMProtectBeginUltra("field_2226332056633417067");
    fields[0] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=4&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1682137460861690377), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_5803049694241794963");
    fields[1] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=3&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1683789948373341801), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-1242856009517888162");
    fields[2] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=289&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1211179537948283678), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_5230347050018536186");
    fields[3] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=207&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1769719110000654827), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-2625967320816873748");
    fields[4] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=150&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1916790180710051663), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7886642248246598323");
    fields[5] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=56&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1649087783643105928), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7768110161137224937");
    fields[6] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=304&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1239271761221848446), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-6922355318944882797");
    fields[7] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=341&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1178129856434731933), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_1916218845293604366");
    fields[8] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=264&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1252491635545256063), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-7835763018371215730");
    fields[9] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=272&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1292151249925544320), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_742682223736778376");
    fields[10] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=22&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1705272234485202762), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7627725208792909543");
    fields[11] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=308&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1232661828355111934), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-1162960564743409831");
    fields[12] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=332&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1140122725271127804), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-2204741741946729796");
    fields[13] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=268&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1245881698383552254), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-8347837083963315048");
    fields[14] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=420&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1417740030228112899), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_4252311323138359167");
    fields[15] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=322&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1156647566027903676), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_3068121211313120547");
    fields[16] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=434&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1447484741013329091), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-2902312464598656244");
    fields[17] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=266&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1249186669111887806), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-1355156282756103865");
    fields[18] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=367&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1082285791212346842), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_2475127542231253048");
    fields[19] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=251&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1749889302810510699), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_5491960010691563556");
    fields[20] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=13&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1667265107616565929), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-1107258837806738121");
    fields[21] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=130&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1896960373519907534), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-6912161080273893842");
    fields[22] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=142&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1877130566329763406), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_6827953580124836864");
    fields[23] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=288&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intB"), ("I")) ^ 1212832021164967806), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-1850596566394005108");
    fields[24] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=324&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("intA"), ("I")) ^ 1153342599594535420), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_6282879816745440893");
    fields[25] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=195&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1789548917190798956), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-7185582582426272817");
    fields[26] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=183&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1862258213084638958), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-4234950391534624734");
    fields[27] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=46&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1612733135696185927), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7955458733466119377");
    fields[28] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=361&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1092200690512451611), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-8020963583928734780");
    fields[29] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=227&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1736669428487103082), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_4392808250418365942");
    fields[30] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=275&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1287193800275491935), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7682110140302025416");
    fields[31] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=92&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1589598357777706247), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-7056673964151575844");
    fields[32] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=26&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1698662301618466250), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_5234917400873983905");
    fields[33] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=291&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1207874567219948125), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_3482518082886570927");
    fields[34] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=130&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1896960373519907534), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-2974867079949438941");
    fields[35] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=414&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1480534418231913540), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_2540668448627869229");
    fields[36] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=129&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatA"), ("F")) ^ 1898612861031558958), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_6848318181462222832");
    fields[37] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=427&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatB"), ("F")) ^ 1406172639121389410), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_3478079596362814999");
    fields[38] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=135&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatA"), ("F")) ^ 1888697953141519598), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-1812957312905853575");
    fields[39] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=124&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatB"), ("F")) ^ 1536718877663944966), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_3403118715567589426");
    fields[40] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=78&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatA"), ("F")) ^ 1559853646992490054), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-7265863107445119346");
    fields[41] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=332&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatB"), ("F")) ^ 1140122725271127804), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-5982623273445595814");
    fields[42] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=178&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatA"), ("F")) ^ 1870520629168059598), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_2674326912363965639");
    fields[43] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=378&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatB"), ("F")) ^ 1116987951647615419), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-5753690577616460624");
    fields[44] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=41&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1620995551779606568), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_1391475258407135543");
    fields[45] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=236&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatA"), ("F")) ^ 1721797075241978634), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-739941134226150683");
    fields[46] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=210&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatB"), ("F")) ^ 1817641144759331020), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-2183968224264380332");
    fields[47] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=410&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1487144359688584644), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_6956732759898034925");
    fields[48] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=160&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1847385855544547213), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_967174132387938217");
    fields[49] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=424&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatA"), ("F")) ^ 1411130093066409090), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_8349605142246290386");
    fields[50] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=244&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatB"), ("F")) ^ 1761456689622266891), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_3371956856908074729");
    fields[51] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=14&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1665612624399881801), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-3549757357615621015");
    fields[52] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=185&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1858953242356303405), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-5016452146424562102");
    fields[53] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=55&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatA"), ("F")) ^ 1650740266859790056), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-7998238370113895115");
    fields[54] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=208&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("floatB"), ("F")) ^ 1820946111192699277), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_830231718056748299");
    fields[55] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=134&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1890350436358203726), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7683461260282063051");
    fields[56] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=272&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1292151249925544320), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7415470971834544578");
    fields[57] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=202&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1777981530379042763), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-3795475071630363070");
    fields[58] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=172&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1827556048354403085), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-5162626866821004147");
    fields[59] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=379&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1115335468430931291), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-1358517356194372283");
    fields[60] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=111&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1505321675072110053), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-4868638020060655852");
    fields[61] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=276&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1285541317058807807), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_447503439408197716");
    fields[62] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=419&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1419392513444797027), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-4469397033076314058");
    fields[63] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=121&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1541676323019030054), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-7979129045074728069");
    fields[64] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=240&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1768066626783970699), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_1445431813318424606");
    fields[65] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=282&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1275626409168768447), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_7668306008608559778");
    fields[66] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=395&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1459052127825085284), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_9029984403165946113");
    fields[67] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=309&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1231009345138427806), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-3413974902097752323");
    fields[68] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=342&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1176477373218047805), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_3606940687472222550");
    fields[69] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=262&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1255796601978624319), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_2477899385506625455");
    fields[70] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=36&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1629257976452961800), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-3539342448042361940");
    fields[71] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=304&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1239271761221848446), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-3367041450044366794");
    fields[72] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=401&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1502016712933709093), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_4646057138041966698");
    fields[73] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=161&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1845733372327863085), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_4044423632312900151");
    fields[74] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=271&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1240924244438532574), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-3408369444323669998");
    fields[75] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=126&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1533413906935609413), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-7464945074106078651");
    fields[76] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=71&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1571421033804246246), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-4850310737081711513");
    fields[77] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=288&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1212832021164967806), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-4347775537801820023");
    fields[78] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=77&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1561506134504141478), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_8122940762036943023");
    fields[79] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=11&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleA"), ("D")) ^ 1670570069754966889), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_3975908826536787871");
    fields[80] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=438&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("doubleB"), ("D")) ^ 1440874803851625283), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-3987754412149106794");
    fields[81] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=53&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[2]) ^ -6425331615520984282) ^ 8524655634080561011) ^ 1969392742806804369) ^ -4445241120801586602) ^ 397771232168562501) ^ 6466404756279460668 ^ rtdsc), ("$assertionsDisabled"), ("Z")) ^ 1654045233293158312), rtdsc)));
    VMProtectEnd();
    VMProtectBeginUltra("field_-6849263320209882334");
    fields[82] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=425&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[0]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1409477609849724962), rtdsc)));
    VMProtectEnd();

L108:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 0

    cstack0.l = env->FindClass(("ru/kotopushka/Main"));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // INVOKEVIRTUAL; Stack pointer: 1

    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID(env->FindClass(("java/lang/Class")), ("desiredAssertionStatus"), ("()Z")));
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // IFNE; Stack pointer: 1

    if (cstack0.i != 0) goto L109;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_1; Stack pointer: 0

    cstack0.i = 1;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // GOTO; Stack pointer: 1

    goto L110;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L109:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_0; Stack pointer: 0

    cstack0.i = 0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L110:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // PUTSTATIC; Stack pointer: 1

    env->SetStaticBooleanField(env->FindClass(("ru/kotopushka/Main")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/Main")), ("$assertionsDisabled"), ("Z")),  cstack0.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L111:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // ICONST_5; Stack pointer: 0

    cstack0.i = 5;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // PUTSTATIC; Stack pointer: 1

    env->SetStaticIntField(env->FindClass(("ru/kotopushka/Main")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/Main")), ("intA"), ("I")), (jint) cstack0.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L112:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // BIPUSH; Stack pointer: 0

    cstack0.i = (jint) 10;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // PUTSTATIC; Stack pointer: 1

    env->SetStaticIntField(env->FindClass(("ru/kotopushka/Main")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/Main")), ("intB"), ("I")), (jint) cstack0.i);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L113:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 0

    cstack0.f = 5.5f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // PUTSTATIC; Stack pointer: 1

    env->SetStaticFloatField(env->FindClass(("ru/kotopushka/Main")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/Main")), ("floatA"), ("F")), cstack0.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L114:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 0

    cstack0.f = 2.5f;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // PUTSTATIC; Stack pointer: 1

    env->SetStaticFloatField(env->FindClass(("ru/kotopushka/Main")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/Main")), ("floatB"), ("F")), cstack0.f);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L115:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 0

    cstack0.d = 10.0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // PUTSTATIC; Stack pointer: 2

    env->SetStaticDoubleField(env->FindClass(("ru/kotopushka/Main")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/Main")), ("doubleA"), ("D")), cstack0.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L116:
    
    { 
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // LDC; Stack pointer: 0

    cstack0.d = 3.0;
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // PUTSTATIC; Stack pointer: 2

    env->SetStaticDoubleField(env->FindClass(("ru/kotopushka/Main")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/Main")), ("doubleB"), ("D")), cstack0.d);
    
    }

    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    
    { // RETURN; Stack pointer: 0

    return;
    
    }

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
        jclass clazz = env->FindClass(xorstr_("[Z"));
        boolean_array_class = (jclass)env->NewGlobalRef(clazz);


    }
#ifdef __cplusplus
}
#endif
#endif
