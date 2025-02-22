
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

// net/minecraft/client/main/Main$1
jobject JNICALL  Java_net_minecraft_client_main_Main_000241_getPasswordAuthentication0(JNIEnv *env, jobject obj) {
    // stack count: 4, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack3 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = obj; 

L1:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[0]) ^ 1462029091406386664) ^ -4422275841591708416) ^ 3083288352467461573) ^ 7439238045646489145) ^ 6320909143762318174) ^ 4847116492084148928 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 3
    cstack2.l = env->GetObjectField(cstack2.l, (jfieldID)(((((((__int64)(fields[0]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 4
    cstack3.l = env->GetObjectField(cstack3.l, (jfieldID)(((((((__int64)(fields[1]) ^ 8547872324256554992) ^ -5498889650702349623) ^ -759615308319477592) ^ -2562482895169281656) ^ 217631773358662865) ^ -8994628070054659371 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 4
    cstack3.l = env->CallObjectMethod(cstack3.l, env->GetMethodID((jclass)(((((((__int64)(classes[2]) ^ -8071268989319398026) ^ -4093130502032984182) ^ 803798650454651412) ^ -2040445874672540438) ^ 8612339873230356020) ^ 8987726342042594107 ^ rtdsc), ("toCharArray"), ("()[C")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[0]) ^ 1462029091406386664) ^ -4422275841591708416) ^ 3083288352467461573) ^ 7439238045646489145) ^ 6320909143762318174) ^ 4847116492084148928 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[0]) ^ 1462029091406386664) ^ -4422275841591708416) ^ 3083288352467461573) ^ 7439238045646489145) ^ 6320909143762318174) ^ 4847116492084148928 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;[C)V")), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L2:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main_000241__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 0, locals count: 0, try-catches: 0
    jvalue cstack_exception = {};
    JNINativeMethod jniMethods[] = {
        (char*)xorstr_("getPasswordAuthentication"), (char*)xorstr_("()Ljava/net/PasswordAuthentication;"), &Java_net_minecraft_client_main_Main_000241_getPasswordAuthentication0,
    };
    env->RegisterNatives(env->FindClass(xorstr_("net/minecraft/client/main/Main$1")), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /* CLASS REFERENCE TABLE */
    classes[0] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=233&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/net/PasswordAuthentication"))) ^ 1726754529186998314), rtdsc)));
    classes[1] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=109&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/main/Main$1"))) ^ 1508626645800445605), rtdsc)));
    classes[2] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=87&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/String"))) ^ 1597860782451061479), rtdsc)));

    /* METHOD REFERENCE TABLE */

    /* FIELD REFERENCE TABLE */
    fields[0] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=243&rtdsc={}", ((__int64)env->GetFieldID((jclass)(((((((__int64)(classes[1]) ^ 6613460115114169325) ^ 391680293803681178) ^ 644670705603847912) ^ 4167518849878952174) ^ -5410068454344846214) ^ -6592441684008400929 ^ rtdsc), ("val$s1"), ("Ljava/lang/String;")) ^ 1763109172838951019), rtdsc)));
    fields[1] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=178&rtdsc={}", ((__int64)env->GetFieldID((jclass)(((((((__int64)(classes[1]) ^ 6613460115114169325) ^ 391680293803681178) ^ 644670705603847912) ^ 4167518849878952174) ^ -5410068454344846214) ^ -6592441684008400929 ^ rtdsc), ("val$s2"), ("Ljava/lang/String;")) ^ 1870520629168059598), rtdsc)));

    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
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
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[3]) ^ -361924032171661291) ^ -4879513484784625911) ^ 714946177156313700) ^ 3484020499700898500) ^ 700413496629877770) ^ -1699730115847343150 ^ rtdsc), (jmethodID)(((((((__int64)(methods[0]) ^ -1751340082308492391) ^ 4268463661987838046) ^ 3312691773165698157) ^ 547774970916942860) ^ 8007055340012758185) ^ 6873252012586650127 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L2:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[3]) ^ -361924032171661291) ^ -4879513484784625911) ^ 714946177156313700) ^ 3484020499700898500) ^ 700413496629877770) ^ -1699730115847343150 ^ rtdsc), ("getIntegratedServer"), ("()Lnet/minecraft/server/integrated/IntegratedServer;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal2.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L5:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal2.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal2.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 1
    cstack1.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[4]) ^ 4347670644983803227) ^ 6878928919107070660) ^ -5079902803658827256) ^ 6059690329079490787) ^ 7387361649947990294) ^ 4210099593081688277 ^ rtdsc), ("initiateShutdown"), ("(Z)V")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L3:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L7:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main_000242__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 0, locals count: 0, try-catches: 0
    jvalue cstack_exception = {};
    JNINativeMethod jniMethods[] = {
        (char*)xorstr_("run"), (char*)xorstr_("()V"), &Java_net_minecraft_client_main_Main_000242_run1,
    };
    env->RegisterNatives(env->FindClass(xorstr_("net/minecraft/client/main/Main$2")), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /* CLASS REFERENCE TABLE */
    classes[3] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=349&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/Minecraft"))) ^ 1164909986406291612), rtdsc)));
    classes[4] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=253&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/server/integrated/IntegratedServer"))) ^ 1746584336377142443), rtdsc)));

    /* METHOD REFERENCE TABLE */
    methods[0] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=387&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[3]) ^ -361924032171661291) ^ -4879513484784625911) ^ 714946177156313700) ^ 3484020499700898500) ^ 700413496629877770) ^ -1699730115847343150 ^ rtdsc), ("getInstance"), ("()Lnet/minecraft/client/Minecraft;")) ^ 1472271997853525604), rtdsc)));

    /* FIELD REFERENCE TABLE */

    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
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
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // ICONST_1; Stack pointer: 0
    cstack0.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[5]) ^ 2594923822176602416) ^ 1557471884758856299) ^ -3277796931903460299) ^ -2730219732956039618) ^ -77805910531583728) ^ 8669217890393880134 ^ rtdsc), (jmethodID)(((((((__int64)(methods[1]) ^ 2523717525700722568) ^ 6421691280209120460) ^ -7448244974675007834) ^ 2104472778838932846) ^ 1766866002815549890) ^ -2336747010554836505 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
L3:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 1
    cstack0.l = env->GetObjectField(cstack0.l, (jfieldID)(((((((__int64)(fields[2]) ^ -6902771106016275053) ^ -545492473407551547) ^ 5751309424324279115) ^ 3819494166684479831) ^ 1291288864410370957) ^ -133933880735384336 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[7]) ^ -4893502963842458895) ^ 8094983765822153052) ^ 7712543240930449572) ^ 7268363458101975727) ^ 7848529627211031874) ^ 1458608368918250941 ^ rtdsc), ("run"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L1:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[8]) ^ -4198794938774043345) ^ 3520995196730888369) ^ -2809701589634051169) ^ -486885816508305909) ^ 4195208703372108019) ^ -2827882699593712993 ^ rtdsc), (jfieldID)(((((((__int64)(fields[3]) ^ -2994693902938013152) ^ 4636523523336221627) ^ -5000338826938458210) ^ 2955707966803744481) ^ -4048178259973960205) ^ -6446560465454882173 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("Exception in client thread"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[9]) ^ -5623814141443781664) ^ -3345662036776034505) ^ 6926192013254400738) ^ -1005785087247291670) ^ -6022557563838545430) ^ 242116980513405612 ^ rtdsc), ("error"), ("(Ljava/lang/String;Ljava/lang/Throwable;)V")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L5:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L7:
CATCH_0:
    if (env->IsInstanceOf(cstack_exception.l, (jclass)(((((((__int64)(classes[10]) ^ -3322421523075475643) ^ 6181723718199962845) ^ 5512617507048073865) ^ -5315302824492800163) ^ -1295579553242880779) ^ -5057441848249808922 ^ rtdsc))) { env->ExceptionClear(); goto L1; } 
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main_000243__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 0, locals count: 0, try-catches: 0
    jvalue cstack_exception = {};
    JNINativeMethod jniMethods[] = {
        (char*)xorstr_("run"), (char*)xorstr_("()V"), &Java_net_minecraft_client_main_Main_000243_run2,
    };
    env->RegisterNatives(env->FindClass(xorstr_("net/minecraft/client/main/Main$3")), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /* CLASS REFERENCE TABLE */
    classes[5] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=81&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("com/mojang/blaze3d/systems/RenderSystem"))) ^ 1607775681751166247), rtdsc)));
    classes[6] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=40&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/main/Main$3"))) ^ 1622648039291257992), rtdsc)));
    classes[7] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=61&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/Minecraft"))) ^ 1640825363264717992), rtdsc)));
    classes[8] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=410&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/main/Main"))) ^ 1487144359688584644), rtdsc)));
    classes[9] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=64&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("org/apache/logging/log4j/Logger"))) ^ 1582988424910969735), rtdsc)));
    classes[10] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=112&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Throwable"))) ^ 1556548680559121798), rtdsc)));

    /* METHOD REFERENCE TABLE */
    methods[1] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=337&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[5]) ^ 2594923822176602416) ^ 1557471884758856299) ^ -3277796931903460299) ^ -2730219732956039618) ^ -77805910531583728) ^ 8669217890393880134 ^ rtdsc), ("initGameThread"), ("(Z)V")) ^ 1184739793596435741), rtdsc)));

    /* FIELD REFERENCE TABLE */
    fields[2] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=31&rtdsc={}", ((__int64)env->GetFieldID((jclass)(((((((__int64)(classes[6]) ^ 147468974300986055) ^ 810003415206939326) ^ 9046553779006341991) ^ -3764880520381288386) ^ -3889652547172233894) ^ -1031916821786482987 ^ rtdsc), ("val$minecraft"), ("Lnet/minecraft/client/Minecraft;")) ^ 1690399881240078313), rtdsc)));
    fields[3] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=194&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[8]) ^ -4198794938774043345) ^ 3520995196730888369) ^ -2809701589634051169) ^ -486885816508305909) ^ 4195208703372108019) ^ -2827882699593712993 ^ rtdsc), ("LOGGER"), ("Lorg/apache/logging/log4j/Logger;")) ^ 1791201400407483084), rtdsc)));

    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
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
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 2
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("<init>"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal1.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L8:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("allowsUnrecognizedOptions"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L9:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("demo"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L10:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("disableMultiplayer"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L11:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("disableChat"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L12:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("fullscreen"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L13:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("checkGlErrors"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L14:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("server"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal2.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L15:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("port"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // SIPUSH; Stack pointer: 1
    cstack1.i = (jint) 25565;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), (jmethodID)(((((((__int64)(methods[2]) ^ -2371192174345477450) ^ 4926754349695221106) ^ -1246287639449856758) ^ 35151208283402605) ^ -5856827270465750443) ^ -3292272365500978427 ^ rtdsc), cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal3.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L16:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("gameDir"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 1
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc))) { cstack1.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 2
    cstack2 = cstack1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.l = env->NewStringUTF(("."));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack2.l, (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;)V")), cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal4.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L17:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("assetsDir"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal5.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L18:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("resourcePackDir"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal6.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L19:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("dataPackDir"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal7.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L20:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("proxyHost"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal8.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L21:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("proxyPort"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("8080"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal9.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L22:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("proxyUser"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal10.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L23:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("proxyPass"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal11.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L24:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("username"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack1.j = env->CallStaticLongMethod((jclass)(((((((__int64)(classes[17]) ^ 2046278030910464502) ^ -2648456827278992098) ^ -3666328297452500383) ^ -8077536007384552336) ^ 2660383639352325642) ^ -8937085656159616101 ^ rtdsc), (jmethodID)(((((((__int64)(methods[3]) ^ 5954355233988726403) ^ 2533559169464781077) ^ 4028767524994313908) ^ -8941322207265287687) ^ -2679083489326972816) ^ 3197031832830437677 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.j = 1000LL;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LREM; Stack pointer: 5
    cstack1.j = (cstack1.j % cstack3.j);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack1.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[4]) ^ 1754153563305909231) ^ -3535366334435350257) ^ -291519970345035757) ^ -319148983016515244) ^ 4490646387262354612) ^ -2044984586332700882 ^ rtdsc), cstack1.j);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal12.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L25:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("uuid"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal13.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L26:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("accessToken"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("required"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal14.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L27:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("version"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("required"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal15.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L28:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("width"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // SIPUSH; Stack pointer: 1
    cstack1.i = (jint) 854;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), (jmethodID)(((((((__int64)(methods[2]) ^ -2371192174345477450) ^ 4926754349695221106) ^ -1246287639449856758) ^ 35151208283402605) ^ -5856827270465750443) ^ -3292272365500978427 ^ rtdsc), cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal16.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L29:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("height"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // SIPUSH; Stack pointer: 1
    cstack1.i = (jint) 480;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), (jmethodID)(((((((__int64)(methods[2]) ^ -2371192174345477450) ^ 4926754349695221106) ^ -1246287639449856758) ^ 35151208283402605) ^ -5856827270465750443) ^ -3292272365500978427 ^ rtdsc), cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal17.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L30:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("fullscreenWidth"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal18.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L31:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("fullscreenHeight"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("ofType"), ("(Ljava/lang/Class;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal19.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L32:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("userProperties"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("{}"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal20.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L33:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("profileProperties"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("{}"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal21.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L34:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("assetIndex"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal22.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L35:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("userType"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("legacy"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal23.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L36:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("versionType"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("accepts"), ("(Ljava/lang/String;)Ljoptsimple/OptionSpecBuilder;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[12]) ^ -1092235205478016765) ^ 2126584786326738148) ^ -71405046224767839) ^ -4790128045648368896) ^ 5276070379289852082) ^ -2916221504252188479 ^ rtdsc), ("withRequiredArg"), ("()Ljoptsimple/ArgumentAcceptingOptionSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("release"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 2
    cstack2.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ANEWARRAY; Stack pointer: 3
    if (cstack2.i < 0) throw_re(env, xorstr_("java/lang/NegativeArraySizeException"), xorstr_("ARRAYLENGTH negative"), 404); else { cstack2.l = env->NewObjectArray(cstack2.i, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc), nullptr); }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultsTo"), ("(Ljava/lang/Object;[Ljava/lang/Object;)Ljoptsimple/ArgumentAcceptingOptionSpec;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal24.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L37:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("nonOptions"), ("()Ljoptsimple/NonOptionArgumentSpec;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal25.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L38:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[11]) ^ 4240861200269983456) ^ -4271486057132117419) ^ -2112202835256340046) ^ -5135014974790118486) ^ -8292374551593377248) ^ 6147896308925713990 ^ rtdsc), ("parse"), ("([Ljava/lang/String;)Ljoptsimple/OptionSet;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal26.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L39:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal25.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("valuesOf"), ("(Ljoptsimple/OptionSpec;)Ljava/util/List;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal27.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L40:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal27.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[20]) ^ -4163191786241136125) ^ 1088885499005756289) ^ 8498894468606498407) ^ -2904232074553275789) ^ 3272872748846024858) ^ 2675099753028128974 ^ rtdsc), ("isEmpty"), ("()Z")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L41;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L42:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[21]) ^ -2994693902938013152) ^ 4636523523336221627) ^ -5000338826938458210) ^ 2955707966803744481) ^ -4048178259973960205) ^ -6446560465454882173 ^ rtdsc), (jfieldID)(((((((__int64)(fields[4]) ^ 6905584587013691892) ^ 1278589800960039337) ^ -2730137621503616715) ^ -3590868178784052215) ^ -7240331206549842480) ^ 4962201315579268173 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal27.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[7]) ^ -2043464558502982255) ^ 3381554159126447184) ^ 6687500095978195487) ^ 8306161995284979952) ^ -8609425977196829869) ^ -4681390987001018974 ^ rtdsc), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[22]) ^ -4857899815604518971) ^ 5662874072391988267) ^ 574395229756414827) ^ 4851017200057005847) ^ 6926193672684948713) ^ 6961590817245125613 ^ rtdsc), ("println"), ("(Ljava/lang/String;)V")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L41:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal8.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal28.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L43:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc), (jfieldID)(((((((__int64)(fields[5]) ^ -5769876375246059300) ^ 5434255252943013168) ^ -609775860046642757) ^ -6349963611918048931) ^ -5107426189883530945) ^ 3688167513279379573 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal29.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L44:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal28.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L45;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L46:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // GETSTATIC; Stack pointer: 2
    cstack2.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[24]) ^ 842176999369401605) ^ -3763985158179292653) ^ -1475691060148093342) ^ 6926614278717981594) ^ -7542973471011157750) ^ -5318407886003479626 ^ rtdsc), (jfieldID)(((((((__int64)(fields[6]) ^ -7633082283617597823) ^ 6460605797703812512) ^ 4964958196648230280) ^ -4454654378664787565) ^ 5866945742775377974) ^ -1350425269140229665 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // NEW; Stack pointer: 3
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[25]) ^ 222324911627383843) ^ -3105694466176942298) ^ -6034670472763648257) ^ 6413990520379408635) ^ -2960112007780114761) ^ 2962811809618443437 ^ rtdsc))) { cstack3.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // DUP; Stack pointer: 4
    cstack4 = cstack3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal28.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal9.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // INVOKESTATIC; Stack pointer: 8
    cstack6.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack6.l, cstack7.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // CHECKCAST; Stack pointer: 7
    if (cstack6.l != nullptr && !env->IsInstanceOf(cstack6.l, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // INVOKEVIRTUAL; Stack pointer: 7
    cstack6.i = env->CallIntMethod(cstack6.l, env->GetMethodID((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), ("intValue"), ("()I")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // INVOKESPECIAL; Stack pointer: 7
    env->CallNonvirtualVoidMethod(cstack4.l, (jclass)(((((((__int64)(classes[25]) ^ 222324911627383843) ^ -3105694466176942298) ^ -6034670472763648257) ^ 6413990520379408635) ^ -2960112007780114761) ^ 2962811809618443437 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[25]) ^ 222324911627383843) ^ -3105694466176942298) ^ -6034670472763648257) ^ 6413990520379408635) ^ -2960112007780114761) ^ 2962811809618443437 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;I)V")), cstack5.l, cstack6.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc), ("<init>"), ("(Ljava/net/Proxy$Type;Ljava/net/SocketAddress;)V")), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // ASTORE; Stack pointer: 1
    clocal29.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
L47:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L45;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L1:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal30.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L45:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal10.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal30.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L48:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal11.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal31.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L49:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal29.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 1
    cstack1.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc), (jfieldID)(((((((__int64)(fields[7]) ^ 4094798966467705820) ^ 4508431223996995662) ^ 8798573360857200779) ^ 7967550574248675869) ^ -7377243177638362763) ^ -8852797227722896368 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc), ("equals"), ("(Ljava/lang/Object;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L50;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal30.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.i = (jint) env->CallStaticBooleanMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[12]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L50;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal31.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.i = (jint) env->CallStaticBooleanMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[12]) ^ 8950455877425447974) ^ 7486956346759579152) ^ -7907051816071481002) ^ -2559345141116558903) ^ -1605426393980297428) ^ -6389018060149773495 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L50;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L51:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[26]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal30.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal31.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[26]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[26]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;Ljava/lang/String;)V")), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[27]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), (jmethodID)(((((((__int64)(methods[14]) ^ 1023842385704586461) ^ 3470731936676013802) ^ -1077871193726117296) ^ -8593297524070815344) ^ 9066303269922328924) ^ -3261475974737546211 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L50:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal16.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = env->CallIntMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), ("intValue"), ("()I")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal32.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L52:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal17.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = env->CallIntMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), ("intValue"), ("()I")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal33.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L53:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal18.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[18]) ^ -5110771494120616378) ^ 3292376377281913269) ^ -3993872679437108753) ^ 6758877445226190929) ^ -7838411154901404343) ^ -6101305999264491737 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal34.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L54:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal19.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[18]) ^ -5110771494120616378) ^ 3292376377281913269) ^ -3993872679437108753) ^ 6758877445226190929) ^ -7838411154901404343) ^ -6101305999264491737 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal35.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L55:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("fullscreen"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljava/lang/String;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal36.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L56:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("demo"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljava/lang/String;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal37.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L57:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("disableMultiplayer"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljava/lang/String;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal38.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L58:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("disableChat"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljava/lang/String;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISTORE; Stack pointer: 1
    clocal39.i = cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L59:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal15.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal40.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L60:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[28]) ^ -3871067318636580853) ^ 1975795006162114447) ^ 5124086145794001076) ^ 7784124974788238735) ^ 1442609996641028592) ^ -4217001321093753541 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 2
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[28]) ^ -3871067318636580853) ^ 1975795006162114447) ^ 5124086145794001076) ^ 7784124974788238735) ^ 1442609996641028592) ^ -4217001321093753541 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[28]) ^ -3871067318636580853) ^ 1975795006162114447) ^ 5124086145794001076) ^ 7784124974788238735) ^ 1442609996641028592) ^ -4217001321093753541 ^ rtdsc), ("<init>"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = (jclass)(((((((__int64)(classes[29]) ^ 585655680002786258) ^ -7083004358765815596) ^ -5239030748509630756) ^ -6179089020078568217) ^ -6635046673332244645) ^ 7076675640740244857 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 2
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[30]) ^ -3578942842442090990) ^ 2862704513318472606) ^ 1749277818686536450) ^ 25737946125234346) ^ -387652755563967674) ^ 7337641678493915560 ^ rtdsc))) { cstack2.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack3.l, (jclass)(((((((__int64)(classes[30]) ^ -3578942842442090990) ^ 2862704513318472606) ^ 1749277818686536450) ^ 25737946125234346) ^ -387652755563967674) ^ 7337641678493915560 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[30]) ^ -3578942842442090990) ^ 2862704513318472606) ^ 1749277818686536450) ^ 25737946125234346) ^ -387652755563967674) ^ 7337641678493915560 ^ rtdsc), ("<init>"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 3
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[28]) ^ -3871067318636580853) ^ 1975795006162114447) ^ 5124086145794001076) ^ 7784124974788238735) ^ 1442609996641028592) ^ -4217001321093753541 ^ rtdsc), ("registerTypeAdapter"), ("(Ljava/lang/reflect/Type;Ljava/lang/Object;)Lcom/google/gson/GsonBuilder;")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[28]) ^ -3871067318636580853) ^ 1975795006162114447) ^ 5124086145794001076) ^ 7784124974788238735) ^ 1442609996641028592) ^ -4217001321093753541 ^ rtdsc), ("create"), ("()Lcom/google/gson/Gson;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal41.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L61:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal41.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal20.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack1.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 2
    if (cstack1.l != nullptr && !env->IsInstanceOf(cstack1.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = (jclass)(((((((__int64)(classes[29]) ^ 585655680002786258) ^ -7083004358765815596) ^ -5239030748509630756) ^ -6179089020078568217) ^ -6635046673332244645) ^ 7076675640740244857 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[31]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), (jmethodID)(((((((__int64)(methods[23]) ^ 3763421705479725389) ^ 5105109909089321638) ^ 1669713846261134700) ^ 3129720308400980652) ^ -7398856919351568791) ^ -452442336679065606 ^ rtdsc), cstack0.l, cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[29]) ^ 585655680002786258) ^ -7083004358765815596) ^ -5239030748509630756) ^ -6179089020078568217) ^ -6635046673332244645) ^ 7076675640740244857 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal42.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L62:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal41.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal21.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack1.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 2
    if (cstack1.l != nullptr && !env->IsInstanceOf(cstack1.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = (jclass)(((((((__int64)(classes[29]) ^ 585655680002786258) ^ -7083004358765815596) ^ -5239030748509630756) ^ -6179089020078568217) ^ -6635046673332244645) ^ 7076675640740244857 ^ rtdsc);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 3
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[31]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), (jmethodID)(((((((__int64)(methods[23]) ^ 3763421705479725389) ^ 5105109909089321638) ^ 1669713846261134700) ^ 3129720308400980652) ^ -7398856919351568791) ^ -452442336679065606 ^ rtdsc), cstack0.l, cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[29]) ^ 585655680002786258) ^ -7083004358765815596) ^ -5239030748509630756) ^ -6179089020078568217) ^ -6635046673332244645) ^ 7076675640740244857 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal43.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L63:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal24.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal44.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L64:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal4.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal45.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L65:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal5.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljoptsimple/OptionSpec;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L66;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal5.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L67;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L66:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal45.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.l = env->NewStringUTF(("assets/"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), ("<init>"), ("(Ljava/io/File;Ljava/lang/String;)V")), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L67:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal46.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L68:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal6.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljoptsimple/OptionSpec;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L69;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal6.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L70;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L69:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal45.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 3
    cstack3.l = env->NewStringUTF(("resourcepacks/"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[15]) ^ 514449383526906410) ^ -2218784963315551435) ^ 9037265278133406029) ^ -1344396512578563049) ^ -4790374759985111027) ^ -3929289260208471782 ^ rtdsc), ("<init>"), ("(Ljava/io/File;Ljava/lang/String;)V")), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L70:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal47.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L71:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal13.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljoptsimple/OptionSpec;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L72;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal13.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[32]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), ("value"), ("(Ljoptsimple/OptionSet;)Ljava/lang/Object;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L73;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L72:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal12.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[32]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), ("value"), ("(Ljoptsimple/OptionSet;)Ljava/lang/Object;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[33]) ^ -2809378880047277653) ^ -5626981950041575589) ^ -5407447185643435626) ^ 2449359712275649732) ^ -3111433140010772396) ^ 7045879245681845345 ^ rtdsc), (jmethodID)(((((((__int64)(methods[30]) ^ -654048499776216563) ^ -5766422987646016774) ^ 4089754499968811030) ^ -7204336549640616023) ^ 2530676248834874036) ^ 5192370962569506661 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[34]) ^ -6610646638411719782) ^ 341417033748806611) ^ 2376501101511781785) ^ -3938892861978524558) ^ -538973887794625309) ^ -7026034959152234147 ^ rtdsc), ("toString"), ("()Ljava/lang/String;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L73:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal48.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L74:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal22.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("has"), ("(Ljoptsimple/OptionSpec;)Z")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L75;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal22.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[32]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), ("value"), ("(Ljoptsimple/OptionSet;)Ljava/lang/Object;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L76;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L75:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 0
    cstack0.l = nullptr;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L76:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal49.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L77:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal2.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal50.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L78:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal3.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jmethodID)(((((((__int64)(methods[8]) ^ -8181728074883735737) ^ 2254677085665964114) ^ 4576426831099190196) ^ 8644773420616251332) ^ 8605135292659287344) ^ -509984741984174284 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal51.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L79:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[35]) ^ -3981526404200918565) ^ 8323602593861062744) ^ 8896714326438539861) ^ 22600196367478889) ^ 1435405416069959916) ^ 4732031668589029684 ^ rtdsc), (jmethodID)(((((((__int64)(methods[33]) ^ -1786943230546432315) ^ 6700573359712970127) ^ -7995904289369818714) ^ 2965121228961912740) ^ 8929391298833808642) ^ 1370269559964808160 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L80:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[36]) ^ 3909483943576970321) ^ -3674807376334758738) ^ 9205681719562178195) ^ 8473898828776770618) ^ -8313988293306583276) ^ -3898492869445039567 ^ rtdsc), (jmethodID)(((((((__int64)(methods[34]) ^ -5221230579684954090) ^ -8806560113023657346) ^ -221244494497602673) ^ -1002647333194568917) ^ -7845615735472473019) ^ 2847726986123324192 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L81:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[36]) ^ 3909483943576970321) ^ -3674807376334758738) ^ 9205681719562178195) ^ 8473898828776770618) ^ -8313988293306583276) ^ -3898492869445039567 ^ rtdsc), (jmethodID)(((((((__int64)(methods[35]) ^ -7597479135379657899) ^ 4028496099978680431) ^ -2173189814525804464) ^ -6872000636709757445) ^ 4944609783954327517) ^ 4152557179186645007 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L82:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[17]) ^ 2046278030910464502) ^ -2648456827278992098) ^ -3666328297452500383) ^ -8077536007384552336) ^ 2660383639352325642) ^ -8937085656159616101 ^ rtdsc), (jmethodID)(((((((__int64)(methods[36]) ^ -5841082671721939148) ^ -8148269425316274287) ^ -4780223907113157588) ^ -1515271095828109172) ^ -3262754276536397327) ^ -7317797387669337066 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L83:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[37]) ^ -7779144521714842754) ^ -3206220990581658728) ^ -2571009676652813215) ^ 8647911174668974085) ^ 6782077121025359754) ^ 2095625263625744297 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal12.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 4
    cstack2.l = env->CallObjectMethod(cstack2.l, env->GetMethodID((jclass)(((((((__int64)(classes[32]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), ("value"), ("(Ljoptsimple/OptionSet;)Ljava/lang/Object;")), cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 3
    if (cstack2.l != nullptr && !env->IsInstanceOf(cstack2.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal48.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 4
    cstack4.l = clocal14.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 6
    cstack4.l = env->CallObjectMethod(cstack4.l, env->GetMethodID((jclass)(((((((__int64)(classes[32]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), ("value"), ("(Ljoptsimple/OptionSet;)Ljava/lang/Object;")), cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 5
    if (cstack4.l != nullptr && !env->IsInstanceOf(cstack4.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal23.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal26.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 7
    cstack5.l = env->CallObjectMethod(cstack5.l, env->GetMethodID((jclass)(((((((__int64)(classes[32]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), ("value"), ("(Ljoptsimple/OptionSet;)Ljava/lang/Object;")), cstack6.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 6
    if (cstack5.l != nullptr && !env->IsInstanceOf(cstack5.l, (jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 6
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[37]) ^ -7779144521714842754) ^ -3206220990581658728) ^ -2571009676652813215) ^ 8647911174668974085) ^ 6782077121025359754) ^ 2095625263625744297 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[37]) ^ -7779144521714842754) ^ -3206220990581658728) ^ -2571009676652813215) ^ 8647911174668974085) ^ 6782077121025359754) ^ 2095625263625744297 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")), cstack2.l, cstack3.l, cstack4.l, cstack5.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal52.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L84:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[38]) ^ 1900215797108186866) ^ 6131460453850120982) ^ 7244447902956007738) ^ 5025029537359274722) ^ 3575515013307340127) ^ -5491035119098674844 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 2
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[39]) ^ -4818647026516061107) ^ 4179285884438271428) ^ -7368681002249606084) ^ -999509579141846164) ^ 8778070166603151007) ^ 5453336996028210069 ^ rtdsc))) { cstack2.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 3
    cstack3 = cstack2;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 4
    cstack4.l = clocal52.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 5
    cstack5.l = clocal42.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal43.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal29.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 8
    env->CallNonvirtualVoidMethod(cstack3.l, (jclass)(((((((__int64)(classes[39]) ^ -4818647026516061107) ^ 4179285884438271428) ^ -7368681002249606084) ^ -999509579141846164) ^ 8778070166603151007) ^ 5453336996028210069 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[39]) ^ -4818647026516061107) ^ 4179285884438271428) ^ -7368681002249606084) ^ -999509579141846164) ^ 8778070166603151007) ^ 5453336996028210069 ^ rtdsc), ("<init>"), ("(Lnet/minecraft/util/Session;Lcom/mojang/authlib/properties/PropertyMap;Lcom/mojang/authlib/properties/PropertyMap;Ljava/net/Proxy;)V")), cstack4.l, cstack5.l, cstack6.l, cstack7.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 3
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[40]) ^ -7230498730448704840) ^ 999707721456189670) ^ -2182478311103773130) ^ -4451516624612064812) ^ 4043887571141450384) ^ 1255184740764656211 ^ rtdsc))) { cstack3.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 4
    cstack4 = cstack3;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 5
    cstack5.i = clocal32.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 6
    cstack6.i = clocal33.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal34.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal35.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 9
    cstack9.i = clocal36.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 10
    env->CallNonvirtualVoidMethod(cstack4.l, (jclass)(((((((__int64)(classes[40]) ^ -7230498730448704840) ^ 999707721456189670) ^ -2182478311103773130) ^ -4451516624612064812) ^ 4043887571141450384) ^ 1255184740764656211 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[40]) ^ -7230498730448704840) ^ 999707721456189670) ^ -2182478311103773130) ^ -4451516624612064812) ^ 4043887571141450384) ^ 1255184740764656211 ^ rtdsc), ("<init>"), ("(IILjava/util/OptionalInt;Ljava/util/OptionalInt;Z)V")), cstack5.i, cstack6.i, cstack7.l, cstack8.l, cstack9.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 4
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[41]) ^ -3247565585749077855) ^ 2266025828226146629) ^ 8878137333282602529) ^ 4863568216267896859) ^ -366039013850761645) ^ -1062713216844882498 ^ rtdsc))) { cstack4.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 5
    cstack5 = cstack4;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 6
    cstack6.l = clocal45.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 7
    cstack7.l = clocal47.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal46.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 9
    cstack9.l = clocal49.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 10
    env->CallNonvirtualVoidMethod(cstack5.l, (jclass)(((((((__int64)(classes[41]) ^ -3247565585749077855) ^ 2266025828226146629) ^ 8878137333282602529) ^ 4863568216267896859) ^ -366039013850761645) ^ -1062713216844882498 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[41]) ^ -3247565585749077855) ^ 2266025828226146629) ^ 8878137333282602529) ^ 4863568216267896859) ^ -366039013850761645) ^ -1062713216844882498 ^ rtdsc), ("<init>"), ("(Ljava/io/File;Ljava/io/File;Ljava/io/File;Ljava/lang/String;)V")), cstack6.l, cstack7.l, cstack8.l, cstack9.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 5
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[42]) ^ -6681852934887599630) ^ 5205636433494038068) ^ -1793946945554733046) ^ 895799654111415202) ^ 1305698025552508309) ^ 414744213608600830 ^ rtdsc))) { cstack5.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 6
    cstack6 = cstack5;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 7
    cstack7.i = clocal37.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal40.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 9
    cstack9.l = clocal44.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 10
    cstack10.i = clocal38.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ILOAD; Stack pointer: 11
    cstack11.i = clocal39.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 12
    env->CallNonvirtualVoidMethod(cstack6.l, (jclass)(((((((__int64)(classes[42]) ^ -6681852934887599630) ^ 5205636433494038068) ^ -1793946945554733046) ^ 895799654111415202) ^ 1305698025552508309) ^ 414744213608600830 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[42]) ^ -6681852934887599630) ^ 5205636433494038068) ^ -1793946945554733046) ^ 895799654111415202) ^ 1305698025552508309) ^ 414744213608600830 ^ rtdsc), ("<init>"), ("(ZLjava/lang/String;Ljava/lang/String;ZZ)V")), cstack7.l, cstack8.l, cstack9.l, cstack10.l, cstack11.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 6
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[43]) ^ -6464584400314474850) ^ -8438500251675273765) ^ -8534275098896726337) ^ 1405285662692232704) ^ -1454105266044607089) ^ 7974658577496376212 ^ rtdsc))) { cstack6.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 7
    cstack7 = cstack6;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 8
    cstack8.l = clocal50.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 9
    cstack9.l = clocal51.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 10
    cstack9.i = env->CallIntMethod(cstack9.l, env->GetMethodID((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), ("intValue"), ("()I")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 10
    env->CallNonvirtualVoidMethod(cstack7.l, (jclass)(((((((__int64)(classes[43]) ^ -6464584400314474850) ^ -8438500251675273765) ^ -8534275098896726337) ^ 1405285662692232704) ^ -1454105266044607089) ^ 7974658577496376212 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[43]) ^ -6464584400314474850) ^ -8438500251675273765) ^ -8534275098896726337) ^ 1405285662692232704) ^ -1454105266044607089) ^ 7974658577496376212 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;I)V")), cstack8.l, cstack9.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 7
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[38]) ^ 1900215797108186866) ^ 6131460453850120982) ^ 7244447902956007738) ^ 5025029537359274722) ^ 3575515013307340127) ^ -5491035119098674844 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[38]) ^ 1900215797108186866) ^ 6131460453850120982) ^ 7244447902956007738) ^ 5025029537359274722) ^ 3575515013307340127) ^ -5491035119098674844 ^ rtdsc), ("<init>"), ("(Lnet/minecraft/client/GameConfiguration$UserInformation;Lnet/minecraft/client/renderer/ScreenSize;Lnet/minecraft/client/GameConfiguration$FolderInformation;Lnet/minecraft/client/GameConfiguration$GameInformation;Lnet/minecraft/client/GameConfiguration$ServerInformation;)V")), cstack2.l, cstack3.l, cstack4.l, cstack5.l, cstack6.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal53.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L85:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[44]) ^ 4458129730548140939) ^ 531121335703089659) ^ -8852530988598333337) ^ -4625528966209300984) ^ 7394566230519058970) ^ -4738933396601094948 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = env->NewStringUTF(("Client Shutdown Thread"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[44]) ^ 4458129730548140939) ^ 531121335703089659) ^ -8852530988598333337) ^ -4625528966209300984) ^ 7394566230519058970) ^ -4738933396601094948 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[44]) ^ 4458129730548140939) ^ 531121335703089659) ^ -8852530988598333337) ^ -4625528966209300984) ^ 7394566230519058970) ^ -4738933396601094948 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;)V")), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal54.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L86:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal54.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 1
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[45]) ^ 5701483555472628995) ^ 163061474354706078) ^ -539500384199209673) ^ -7033461962096102605) ^ 1003055761091193040) ^ 8580879085735404648 ^ rtdsc))) { cstack1.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 2
    cstack2 = cstack1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 3
    cstack3.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jfieldID)(((((((__int64)(fields[8]) ^ -5695020437919661512) ^ 1518557362969196952) ^ 2755743970482853203) ^ 3828907424547680794) ^ -4177885650491411812) ^ 7682896148979273293 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack2.l, (jclass)(((((((__int64)(classes[45]) ^ 5701483555472628995) ^ 163061474354706078) ^ -539500384199209673) ^ -7033461962096102605) ^ 1003055761091193040) ^ 8580879085735404648 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[45]) ^ 5701483555472628995) ^ 163061474354706078) ^ -539500384199209673) ^ -7033461962096102605) ^ 1003055761091193040) ^ 8580879085735404648 ^ rtdsc), ("<init>"), ("(Lorg/apache/logging/log4j/Logger;)V")), cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[46]) ^ -5150024283209074242) ^ 4775964565235630109) ^ 3949203552568912158) ^ -5837339849284508677) ^ 8756456420594977683) ^ -4593052182342543489 ^ rtdsc), ("setUncaughtExceptionHandler"), ("(Ljava/lang/Thread$UncaughtExceptionHandler;)V")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L87:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[47]) ^ 1280363705071201808) ^ 6789751137262536745) ^ 2685468494635420119) ^ 4512405779020701763) ^ 8158376476538383116) ^ 2790184576523248218 ^ rtdsc), (jmethodID)(((((((__int64)(methods[37]) ^ -5695020437919661512) ^ 1518557362969196952) ^ 2755743970482853203) ^ 3828907424547680794) ^ -4177885650491411812) ^ 7682896148979273293 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal54.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[47]) ^ 1280363705071201808) ^ 6789751137262536745) ^ 2685468494635420119) ^ 4512405779020701763) ^ 8158376476538383116) ^ 2790184576523248218 ^ rtdsc), ("addShutdownHook"), ("(Ljava/lang/Thread;)V")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L88:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[48]) ^ 6613460115114169325) ^ 391680293803681178) ^ 644670705603847912) ^ 4167518849878952174) ^ -5410068454344846214) ^ -6592441684008400929 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 2
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[48]) ^ 6613460115114169325) ^ 391680293803681178) ^ 644670705603847912) ^ 4167518849878952174) ^ -5410068454344846214) ^ -6592441684008400929 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[48]) ^ 6613460115114169325) ^ 391680293803681178) ^ 644670705603847912) ^ 4167518849878952174) ^ -5410068454344846214) ^ -6592441684008400929 ^ rtdsc), ("<init>"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L89:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_1; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[46]) ^ -5150024283209074242) ^ 4775964565235630109) ^ 3949203552568912158) ^ -5837339849284508677) ^ 8756456420594977683) ^ -4593052182342543489 ^ rtdsc), (jmethodID)(((((((__int64)(methods[38]) ^ -7850350818190722602) ^ 1657998404868605433) ^ -6741457723719328045) ^ -4964140387245605067) ^ 8626749034372493372) ^ -8910339637322972342 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_1; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("Render thread"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_1; }
    // INVOKEVIRTUAL; Stack pointer: 2
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[46]) ^ -5150024283209074242) ^ 4775964565235630109) ^ 3949203552568912158) ^ -5837339849284508677) ^ 8756456420594977683) ^ -4593052182342543489 ^ rtdsc), ("setName"), ("(Ljava/lang/String;)V")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_1; }
L90:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), (jmethodID)(((((((__int64)(methods[39]) ^ 696114765567123970) ^ 5015932127244787724) ^ -9011658933449136837) ^ 1582435758342191628) ^ -6627842097056143265) ^ -1872357348942538368 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L91:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), (jmethodID)(((((((__int64)(methods[40]) ^ 6613460115114169325) ^ 391680293803681178) ^ 644670705603847912) ^ 4167518849878952174) ^ -5410068454344846214) ^ -6592441684008400929 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L92:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal53.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 3
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("<init>"), ("(Lnet/minecraft/client/GameConfiguration;)V")), cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal55.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L93:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), (jmethodID)(((((((__int64)(methods[41]) ^ -3977876763350400625) ^ -9174619974372040927) ^ 8091786109901520991) ^ -3410580333376337834) ^ 4209617868809212667) ^ -2279204605249727827 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L94:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L95;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L2:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L96:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jfieldID)(((((((__int64)(fields[9]) ^ 8879249580949568127) ^ -6095568331499708303) ^ 6369244210571555783) ^ 2275347366383446265) ^ 239245519366836190) ^ 1051761108316094186 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("Failed to create window: "));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal56.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[51]) ^ -4526522554616538540) ^ 5066195387299662291) ^ 7703254744352480906) ^ -8757896607804850552) ^ 6947807414398154741) ^ -1438764078093672446 ^ rtdsc), ("warn"), ("(Ljava/lang/String;Ljava/lang/Throwable;)V")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L97:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L3:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L98:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("Initializing game"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[35]) ^ -3981526404200918565) ^ 8323602593861062744) ^ 8896714326438539861) ^ 22600196367478889) ^ 1435405416069959916) ^ 4732031668589029684 ^ rtdsc), (jmethodID)(((((((__int64)(methods[42]) ^ -1388009318228057272) ^ 291153769398964748) ^ 4108331497419715658) ^ 6401439504168517623) ^ 4332120674460628301) ^ -7459628230001100069 ^ rtdsc), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal57.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L99:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal57.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("Initialization"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[35]) ^ -3981526404200918565) ^ 8323602593861062744) ^ 8896714326438539861) ^ 22600196367478889) ^ 1435405416069959916) ^ 4732031668589029684 ^ rtdsc), ("makeCategory"), ("(Ljava/lang/String;)Lnet/minecraft/crash/CrashReportCategory;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L100:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 0
    cstack0.l = nullptr;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[52]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal53.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 2
    cstack1.l = env->GetObjectField(cstack1.l, (jfieldID)(((((((__int64)(fields[10]) ^ -8434599757694800441) ^ -115820609444110884) ^ 8158926200633911) ^ -7894110407924115202) ^ -6159469534927065712) ^ 4873862510920792687 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETFIELD; Stack pointer: 2
    cstack1.l = env->GetObjectField(cstack1.l, (jfieldID)(((((((__int64)(fields[11]) ^ 1355219642397599596) ^ 2874053255878655122) ^ 6050988320869948783) ^ -3755467258223120127) ^ 9087917015930502249) ^ 6784913207928174642 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 2
    cstack2.l = nullptr;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 3
    if (cstack2.l != nullptr && !env->IsInstanceOf(cstack2.l, (jclass)(((((((__int64)(classes[53]) ^ -7779144521714842754) ^ -3206220990581658728) ^ -2571009676652813215) ^ 8647911174668974085) ^ 6782077121025359754) ^ 2095625263625744297 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal57.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 4
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), (jmethodID)(((((((__int64)(methods[43]) ^ -3322421523075475643) ^ 6181723718199962845) ^ 5512617507048073865) ^ -5315302824492800163) ^ -1295579553242880779) ^ -5057441848249808922 ^ rtdsc), cstack0.l, cstack1.l, cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L101:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal57.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), (jmethodID)(((((((__int64)(methods[44]) ^ -8765977014387813576) ^ 480858075648215093) ^ -7120700592690399464) ^ 5714803399937741197) ^ -6181083276640271741) ^ -5172526671744928166 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L102:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L95:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("isRenderOnThread"), ("()Z")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L103;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L104:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // NEW; Stack pointer: 0
    if (jobject obj = env->AllocObject((jclass)(((((((__int64)(classes[54]) ^ 8658331409820892703) ^ 6600046839603220993) ^ -4532243493258983672) ^ 5199041883251478189) ^ 224836353929731542) ^ 503083013972109020 ^ rtdsc))) { cstack0.l = obj; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // DUP; Stack pointer: 1
    cstack1 = cstack0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 2
    cstack2.l = env->NewStringUTF(("Game thread"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 3
    cstack3.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESPECIAL; Stack pointer: 4
    env->CallNonvirtualVoidMethod(cstack1.l, (jclass)(((((((__int64)(classes[54]) ^ 8658331409820892703) ^ 6600046839603220993) ^ -4532243493258983672) ^ 5199041883251478189) ^ 224836353929731542) ^ 503083013972109020 ^ rtdsc), env->GetMethodID((jclass)(((((((__int64)(classes[54]) ^ 8658331409820892703) ^ 6600046839603220993) ^ -4532243493258983672) ^ 5199041883251478189) ^ 224836353929731542) ^ 503083013972109020 ^ rtdsc), ("<init>"), ("(Ljava/lang/String;Lnet/minecraft/client/Minecraft;)V")), cstack2.l, cstack3.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L105:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[46]) ^ -5150024283209074242) ^ 4775964565235630109) ^ 3949203552568912158) ^ -5837339849284508677) ^ 8756456420594977683) ^ -4593052182342543489 ^ rtdsc), ("start"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L106:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("isRunning"), ("()Z")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L107;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L106;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L103:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ACONST_NULL; Stack pointer: 0
    cstack0.l = nullptr;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal56.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L108:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_3; }
    // ICONST_0; Stack pointer: 0
    cstack0.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_3; }
    // INVOKESTATIC; Stack pointer: 1
    env->CallStaticVoidMethod((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), (jmethodID)(((((((__int64)(methods[45]) ^ -5841082671721939148) ^ -8148269425316274287) ^ -4780223907113157588) ^ -1515271095828109172) ^ -3262754276536397327) ^ -7317797387669337066 ^ rtdsc), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_3; }
L109:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("run"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L110:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L107;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L4:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal57.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L111:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jfieldID)(((((((__int64)(fields[12]) ^ -7230498730448704840) ^ 999707721456189670) ^ -2182478311103773130) ^ -4451516624612064812) ^ 4043887571141450384) ^ 1255184740764656211 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("Unhandled game exception"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal57.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[51]) ^ -4526522554616538540) ^ 5066195387299662291) ^ 7703254744352480906) ^ -8757896607804850552) ^ 6947807414398154741) ^ -1438764078093672446 ^ rtdsc), ("error"), ("(Ljava/lang/String;Ljava/lang/Throwable;)V")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L107:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_4; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_4; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_4; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("shutdown"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_4; }
L112:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L113;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L114:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal56.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[46]) ^ -5150024283209074242) ^ 4775964565235630109) ^ 3949203552568912158) ^ -5837339849284508677) ^ 8756456420594977683) ^ -4593052182342543489 ^ rtdsc), ("join"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L113:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("shutdownMinecraftApplet"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L115:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L116;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L5:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_7; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_7; }
    // ASTORE; Stack pointer: 1
    clocal57.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_7; }
L117:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), (jfieldID)(((((((__int64)(fields[13]) ^ 877780147607341529) ^ -6196094855904424733) ^ -8613839071322128087) ^ 4509268024967979010) ^ -8465309429832208207) ^ 184574566618362342 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("Exception during client thread shutdown"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 2
    cstack2.l = clocal57.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 3
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[51]) ^ -4526522554616538540) ^ 5066195387299662291) ^ 7703254744352480906) ^ -8757896607804850552) ^ 6947807414398154741) ^ -1438764078093672446 ^ rtdsc), ("error"), ("(Ljava/lang/String;Ljava/lang/Throwable;)V")), cstack1.l, cstack2.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L118:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("shutdownMinecraftApplet"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L119:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 0
    goto L116;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_7; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_7; }
    // ASTORE; Stack pointer: 1
    clocal58.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_7; }
L120:
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal55.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    env->CallVoidMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("shutdownMinecraftApplet"), ("()V")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L121:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal58.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L116:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L122:
CATCH_3:
    if (env->IsInstanceOf(cstack_exception.l, (jclass)(((((((__int64)(classes[55]) ^ 3506900386113110042) ^ 1786090704207831399) ^ -2093625846395370010) ^ 8470761079019015161) ^ -6490930125967622982) ^ -6504102879349925443 ^ rtdsc))) { env->ExceptionClear(); goto L4; } 
CATCH_2:
    if (env->IsInstanceOf(cstack_exception.l, (jclass)(((((((__int64)(classes[55]) ^ 3506900386113110042) ^ 1786090704207831399) ^ -2093625846395370010) ^ 8470761079019015161) ^ -6490930125967622982) ^ -6504102879349925443 ^ rtdsc))) { env->ExceptionClear(); goto L3; } 
CATCH_1:
    if (env->IsInstanceOf(cstack_exception.l, (jclass)(((((((__int64)(classes[56]) ^ -9164910935296123211) ^ 6890277661667253175) ^ -778192301475414923) ^ 2278485124731136314) ^ -1583812656562058696) ^ 3657371118220980062 ^ rtdsc))) { env->ExceptionClear(); goto L2; } 
CATCH_0:
    if (env->IsInstanceOf(cstack_exception.l, (jclass)(((((((__int64)(classes[57]) ^ 5555421317375384064) ^ 8942978755483819159) ^ -8075468257500253168) ^ 6069103586942691750) ^ 1918187139341174821) ^ -6419814450913205710 ^ rtdsc))) { env->ExceptionClear(); goto L1; } 
CATCH_7:
    goto L6;
CATCH_6:
    goto L6;
CATCH_5:
    goto L6;
CATCH_4:
    if (env->IsInstanceOf(cstack_exception.l, (jclass)(((((((__int64)(classes[58]) ^ -9164910935296123211) ^ 6890277661667253175) ^ -778192301475414923) ^ 2278485124731136314) ^ -1583812656562058696) ^ 3657371118220980062 ^ rtdsc))) { env->ExceptionClear(); goto L5; } 
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
jobject JNICALL  Java_net_minecraft_client_main_Main_toOptionalInt4(JNIEnv *env, jclass clazz, jobject arg0) {
    // stack count: 1, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = arg0; 

L123:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L124;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = env->CallIntMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), ("intValue"), ("()I")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[59]) ^ -874966675199859282) ^ 6929192183456912523) ^ -6811733199566761129) ^ -4280642037067551394) ^ 2516267083397769388) ^ 4643692863930554199 ^ rtdsc), (jmethodID)(((((((__int64)(methods[46]) ^ -8619914780585535940) ^ -8299059205480897988) ^ 415267284905611327) ^ -7387762153396020453) ^ -7096214650595286225) ^ -8618577208805869423 ^ rtdsc), cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L125;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L124:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod((jclass)(((((((__int64)(classes[59]) ^ -874966675199859282) ^ 6929192183456912523) ^ -6811733199566761129) ^ -4280642037067551394) ^ 2516267083397769388) ^ 4643692863930554199 ^ rtdsc), (jmethodID)(((((((__int64)(methods[47]) ^ -5987144909819184080) ^ 631647855812838793) ^ 6130552293295350533) ^ -6859449620498866433) ^ -2347622898286415546) ^ -3871746854903363104 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L125:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
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
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // ALOAD; Stack pointer: 1
    cstack1.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[19]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("valueOf"), ("(Ljoptsimple/OptionSpec;)Ljava/lang/Object;")), cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto CATCH_0; }
L129:
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L127:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal2.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L130:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INSTANCEOF; Stack pointer: 1
    cstack0.i = cstack0.l == nullptr ? false : env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFEQ; Stack pointer: 1
    if (cstack0.i == 0) goto L131;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L132:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal1.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // CHECKCAST; Stack pointer: 1
    if (cstack0.l != nullptr && !env->IsInstanceOf(cstack0.l, (jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc))) {}
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal3.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L133:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal3.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[14]) ^ -3614545994974998210) ^ 5294814206748637390) ^ 8887425834155538491) ^ 2443084204170204226) ^ 534683203257082783) ^ 1834659230167040889 ^ rtdsc), ("defaultValues"), ("()Ljava/util/List;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ASTORE; Stack pointer: 1
    clocal4.l = cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L134:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal4.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[20]) ^ -4163191786241136125) ^ 1088885499005756289) ^ 8498894468606498407) ^ -2904232074553275789) ^ 3272872748846024858) ^ 2675099753028128974 ^ rtdsc), ("isEmpty"), ("()Z")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L131;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L135:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal4.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 1
    cstack1.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEINTERFACE; Stack pointer: 2
    cstack0.l = env->CallObjectMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[20]) ^ -4163191786241136125) ^ 1088885499005756289) ^ 8498894468606498407) ^ -2904232074553275789) ^ 3272872748846024858) ^ 2675099753028128974 ^ rtdsc), ("get"), ("(I)Ljava/lang/Object;")), cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L131:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal2.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ATHROW; Stack pointer: 1
    if (cstack0.l == nullptr) env->ThrowNew((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ""); else env->Throw((jthrowable) cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L136:
CATCH_0:
    if (env->IsInstanceOf(cstack_exception.l, (jclass)(((((((__int64)(classes[55]) ^ 3506900386113110042) ^ 1786090704207831399) ^ -2093625846395370010) ^ 8470761079019015161) ^ -6490930125967622982) ^ -6504102879349925443 ^ rtdsc))) { env->ExceptionClear(); goto L127; } 
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
jboolean JNICALL  Java_net_minecraft_client_main_Main_isNotEmpty6(JNIEnv *env, jclass clazz, jobject arg0) {
    // stack count: 1, locals count: 1, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    clocal0.l = arg0; 

L137:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNULL; Stack pointer: 1
    if (env->IsSameObject(cstack0.l, nullptr)) goto L138;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ALOAD; Stack pointer: 0
    cstack0.l = clocal0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 1
    cstack0.i = (jint) env->CallBooleanMethod(cstack0.l, env->GetMethodID((jclass)(((((((__int64)(classes[16]) ^ -4490919406378598616) ^ 2634085689574530210) ^ 565106733178446161) ^ 7271501207859731184) ^ 6025471459872071580) ^ 4064218374528169521 ^ rtdsc), ("isEmpty"), ("()Z")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IFNE; Stack pointer: 1
    if (cstack0.i != 0) goto L138;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 0
    cstack0.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GOTO; Stack pointer: 1
    goto L139;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L138:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_0; Stack pointer: 0
    cstack0.i = 0;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L139:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IRETURN; Stack pointer: 1
    return (jboolean) cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L140:
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jboolean) 0; }

    return (jboolean) 0;
}
extern "C" JNIEXPORT void JNICALL  Java_net_minecraft_client_main_Main__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 3, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack1 = {}, cstack2 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    JNINativeMethod jniMethods[] = {
        (char*)xorstr_("main"), (char*)xorstr_("([Ljava/lang/String;)V"), &Java_net_minecraft_client_main_Main_main3,
        (char*)xorstr_("toOptionalInt"), (char*)xorstr_("(Ljava/lang/Integer;)Ljava/util/OptionalInt;"), &Java_net_minecraft_client_main_Main_toOptionalInt4,
        (char*)xorstr_("getValue"), (char*)xorstr_("(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;"), &Java_net_minecraft_client_main_Main_getValue5,
        (char*)xorstr_("isNotEmpty"), (char*)xorstr_("(Ljava/lang/String;)Z"), &Java_net_minecraft_client_main_Main_isNotEmpty6,
    };
    env->RegisterNatives(env->FindClass(xorstr_("net/minecraft/client/main/Main")), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /* CLASS REFERENCE TABLE */
    classes[11] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=381&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("joptsimple/OptionParser"))) ^ 1112030497702595739), rtdsc)));
    classes[12] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=352&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("joptsimple/OptionSpecBuilder"))) ^ 1107073048052543355), rtdsc)));
    classes[13] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=376&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Integer"))) ^ 1120292922375950971), rtdsc)));
    classes[14] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=126&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("joptsimple/ArgumentAcceptingOptionSpec"))) ^ 1533413906935609413), rtdsc)));
    classes[15] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=307&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/io/File"))) ^ 1234314311571796062), rtdsc)));
    classes[16] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=392&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/String"))) ^ 1464009581770104964), rtdsc)));
    classes[17] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=205&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/util/Util"))) ^ 1773024076434023083), rtdsc)));
    classes[18] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=120&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/main/Main"))) ^ 1543328810530681478), rtdsc)));
    classes[19] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=173&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("joptsimple/OptionSet"))) ^ 1825903565137718957), rtdsc)));
    classes[20] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=26&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/util/List"))) ^ 1698662301618466250), rtdsc)));
    classes[21] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=194&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/System"))) ^ 1791201400407483084), rtdsc)));
    classes[22] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=189&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/io/PrintStream"))) ^ 1852343305194599597), rtdsc)));
    classes[23] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=290&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/net/Proxy"))) ^ 1209527050436632253), rtdsc)));
    classes[24] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=389&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/net/Proxy$Type"))) ^ 1468967035715124644), rtdsc)));
    classes[25] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=289&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/net/InetSocketAddress"))) ^ 1211179537948283678), rtdsc)));
    classes[26] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=243&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/main/Main$1"))) ^ 1763109172838951019), rtdsc)));
    classes[27] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=412&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/net/Authenticator"))) ^ 1483839388960249092), rtdsc)));
    classes[28] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=236&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("com/google/gson/GsonBuilder"))) ^ 1721797075241978634), rtdsc)));
    classes[29] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=51&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("com/mojang/authlib/properties/PropertyMap"))) ^ 1657350199726526568), rtdsc)));
    classes[30] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=254&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("com/mojang/authlib/properties/PropertyMap$Serializer"))) ^ 1744931848865491019), rtdsc)));
    classes[31] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=173&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/util/JSONUtils"))) ^ 1825903565137718957), rtdsc)));
    classes[32] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=243&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("joptsimple/OptionSpec"))) ^ 1763109172838951019), rtdsc)));
    classes[33] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=340&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/entity/player/PlayerEntity"))) ^ 1179782339651416061), rtdsc)));
    classes[34] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=17&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/util/UUID"))) ^ 1713534654863590698), rtdsc)));
    classes[35] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=147&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/crash/CrashReport"))) ^ 1921747630360104047), rtdsc)));
    classes[36] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=18&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/util/registry/Bootstrap"))) ^ 1711882171646906570), rtdsc)));
    classes[37] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=41&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/util/Session"))) ^ 1620995551779606568), rtdsc)));
    classes[38] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=244&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/GameConfiguration"))) ^ 1761456689622266891), rtdsc)));
    classes[39] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=310&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/GameConfiguration$UserInformation"))) ^ 1229356861921743678), rtdsc)));
    classes[40] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=397&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/renderer/ScreenSize"))) ^ 1455747157096749732), rtdsc)));
    classes[41] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=329&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/GameConfiguration$FolderInformation"))) ^ 1145080179216147484), rtdsc)));
    classes[42] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=273&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/GameConfiguration$GameInformation"))) ^ 1290498766708860191), rtdsc)));
    classes[43] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=234&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/GameConfiguration$ServerInformation"))) ^ 1725102041675346890), rtdsc)));
    classes[44] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=118&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/main/Main$2"))) ^ 1546633776964049734), rtdsc)));
    classes[45] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=311&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/util/DefaultUncaughtExceptionHandler"))) ^ 1227704374410092254), rtdsc)));
    classes[46] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=171&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Thread"))) ^ 1829208531571087213), rtdsc)));
    classes[47] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=400&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Runtime"))) ^ 1503669196150393221), rtdsc)));
    classes[48] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=109&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("com/mojang/blaze3d/Empty3i"))) ^ 1508626645800445605), rtdsc)));
    classes[49] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=154&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("com/mojang/blaze3d/systems/RenderSystem"))) ^ 1910180243548347855), rtdsc)));
    classes[50] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=217&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/Minecraft"))) ^ 1806073753652607532), rtdsc)));
    classes[51] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=264&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("org/apache/logging/log4j/Logger"))) ^ 1252491635545256063), rtdsc)));
    classes[52] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=290&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/resources/LanguageManager"))) ^ 1209527050436632253), rtdsc)));
    classes[53] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=41&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/GameSettings"))) ^ 1620995551779606568), rtdsc)));
    classes[54] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=11&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/main/Main$3"))) ^ 1670570069754966889), rtdsc)));
    classes[55] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=135&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Throwable"))) ^ 1888697953141519598), rtdsc)));
    classes[56] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=360&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("net/minecraft/client/util/UndeclaredException"))) ^ 1093853173729135739), rtdsc)));
    classes[57] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=318&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/Exception"))) ^ 1216136987598336062), rtdsc)));
    classes[58] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=360&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/lang/InterruptedException"))) ^ 1093853173729135739), rtdsc)));
    classes[59] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=121&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("java/util/OptionalInt"))) ^ 1541676323019030054), rtdsc)));

    /* METHOD REFERENCE TABLE */
    methods[2] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=319&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[13]) ^ 6980440524340089680) ^ -2637108084718809583) ^ 635382204730911950) ^ 6588002857681677511) ^ -6310790667157723347) ^ 8956929946984194596 ^ rtdsc), ("valueOf"), ("(I)Ljava/lang/Integer;")) ^ 1214484504381651934), rtdsc)));
    methods[3] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=176&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[17]) ^ 2046278030910464502) ^ -2648456827278992098) ^ -3666328297452500383) ^ -8077536007384552336) ^ 2660383639352325642) ^ -8937085656159616101 ^ rtdsc), ("milliTime"), ("()J")) ^ 1873825595601427854), rtdsc)));
    methods[4] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=251&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("$hello0_proxy1"), ("(J)Ljava/lang/String;")) ^ 1749889302810510699), rtdsc)));
    methods[7] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=433&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("$hello1_proxy2"), ("(Ljava/util/List;)Ljava/lang/String;")) ^ 1449137224230013219), rtdsc)));
    methods[8] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=222&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("getValue"), ("(Ljoptsimple/OptionSet;Ljoptsimple/OptionSpec;)Ljava/lang/Object;")) ^ 1797811337569186892), rtdsc)));
    methods[12] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=29&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("isNotEmpty"), ("(Ljava/lang/String;)Z")) ^ 1693704847673446569), rtdsc)));
    methods[14] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=30&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[27]) ^ -1605277848506214756) ^ -4511453619141275034) ^ -7598084422947842668) ^ 5891953491292732825) ^ 7091923966057743700) ^ 3427201479820676166 ^ rtdsc), ("setDefault"), ("(Ljava/net/Authenticator;)V")) ^ 1692052364456762441), rtdsc)));
    methods[18] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=292&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("toOptionalInt"), ("(Ljava/lang/Integer;)Ljava/util/OptionalInt;")) ^ 1206222084003263997), rtdsc)));
    methods[23] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=25&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[31]) ^ -2556507201531180245) ^ -3256484254931500590) ^ -839179280744879342) ^ 541499467106464650) ^ -6793572390428938252) ^ 1662031992776878375 ^ rtdsc), ("fromJson"), ("(Lcom/google/gson/Gson;Ljava/lang/String;Ljava/lang/Class;)Ljava/lang/Object;")) ^ 1700314784835150378), rtdsc)));
    methods[30] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=331&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[33]) ^ -2809378880047277653) ^ -5626981950041575589) ^ -5407447185643435626) ^ 2449359712275649732) ^ -3111433140010772396) ^ 7045879245681845345 ^ rtdsc), ("getOfflineUUID"), ("(Ljava/lang/String;)Ljava/util/UUID;")) ^ 1141775208487811932), rtdsc)));
    methods[33] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=259&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[35]) ^ -3981526404200918565) ^ 8323602593861062744) ^ 8896714326438539861) ^ 22600196367478889) ^ 1435405416069959916) ^ 4732031668589029684 ^ rtdsc), ("crash"), ("()V")) ^ 1260754055923643999), rtdsc)));
    methods[34] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=427&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[36]) ^ 3909483943576970321) ^ -3674807376334758738) ^ 9205681719562178195) ^ 8473898828776770618) ^ -8313988293306583276) ^ -3898492869445039567 ^ rtdsc), ("register"), ("()V")) ^ 1406172639121389410), rtdsc)));
    methods[35] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=162&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[36]) ^ 3909483943576970321) ^ -3674807376334758738) ^ 9205681719562178195) ^ 8473898828776770618) ^ -8313988293306583276) ^ -3898492869445039567 ^ rtdsc), ("checkTranslations"), ("()V")) ^ 1844080889111178957), rtdsc)));
    methods[36] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=327&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[17]) ^ 2046278030910464502) ^ -2648456827278992098) ^ -3666328297452500383) ^ -8077536007384552336) ^ 2660383639352325642) ^ -8937085656159616101 ^ rtdsc), ("func_240994_l_"), ("()V")) ^ 1148385145649515740), rtdsc)));
    methods[37] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=320&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[47]) ^ 1280363705071201808) ^ 6789751137262536745) ^ 2685468494635420119) ^ 4512405779020701763) ^ 8158376476538383116) ^ 2790184576523248218 ^ rtdsc), ("getRuntime"), ("()Ljava/lang/Runtime;")) ^ 1159952532461271932), rtdsc)));
    methods[38] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=297&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[46]) ^ -5150024283209074242) ^ 4775964565235630109) ^ 3949203552568912158) ^ -5837339849284508677) ^ 8756456420594977683) ^ -4593052182342543489 ^ rtdsc), ("currentThread"), ("()Ljava/lang/Thread;")) ^ 1197959663624876061), rtdsc)));
    methods[39] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=396&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), ("initRenderThread"), ("()V")) ^ 1457399644608401156), rtdsc)));
    methods[40] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=109&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), ("beginInitialization"), ("()V")) ^ 1508626645800445605), rtdsc)));
    methods[41] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=364&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), ("finishInitialization"), ("()V")) ^ 1087243240862399226), rtdsc)));
    methods[42] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=149&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[35]) ^ -3981526404200918565) ^ 8323602593861062744) ^ 8896714326438539861) ^ 22600196367478889) ^ 1435405416069959916) ^ 4732031668589029684 ^ rtdsc), ("makeCrashReport"), ("(Ljava/lang/Throwable;Ljava/lang/String;)Lnet/minecraft/crash/CrashReport;")) ^ 1918442663926735791), rtdsc)));
    methods[43] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=112&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("fillCrashReport"), ("(Lnet/minecraft/client/resources/LanguageManager;Ljava/lang/String;Lnet/minecraft/client/GameSettings;Lnet/minecraft/crash/CrashReport;)V")) ^ 1556548680559121798), rtdsc)));
    methods[44] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=250&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[50]) ^ -5442148755108596809) ^ 3889055062374239246) ^ 7324011875381409488) ^ 1921047179378495712) ^ -7860024896614610371) ^ 2299048891779339026 ^ rtdsc), ("displayCrashReport"), ("(Lnet/minecraft/crash/CrashReport;)V")) ^ 1751541790322162123), rtdsc)));
    methods[45] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=327&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[49]) ^ -4127588638003196201) ^ -1343224198719375792) ^ 1360746453137496366) ^ -5321578332598245669) ^ 2350536790024974401) ^ 8178082205649970942 ^ rtdsc), ("initGameThread"), ("(Z)V")) ^ 1148385145649515740), rtdsc)));
    methods[46] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=243&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[59]) ^ -874966675199859282) ^ 6929192183456912523) ^ -6811733199566761129) ^ -4280642037067551394) ^ 2516267083397769388) ^ 4643692863930554199 ^ rtdsc), ("of"), ("(I)Ljava/util/OptionalInt;")) ^ 1763109172838951019), rtdsc)));
    methods[47] = (jmethodID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=334&rtdsc={}", ((__int64)env->GetStaticMethodID((jclass)(((((((__int64)(classes[59]) ^ -874966675199859282) ^ 6929192183456912523) ^ -6811733199566761129) ^ -4280642037067551394) ^ 2516267083397769388) ^ 4643692863930554199 ^ rtdsc), ("empty"), ("()Ljava/util/OptionalInt;")) ^ 1136817758837759548), rtdsc)));

    /* FIELD REFERENCE TABLE */
    fields[4] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=127&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[21]) ^ -2994693902938013152) ^ 4636523523336221627) ^ -5000338826938458210) ^ 2955707966803744481) ^ -4048178259973960205) ^ -6446560465454882173 ^ rtdsc), ("out"), ("Ljava/io/PrintStream;")) ^ 1531761423718925285), rtdsc)));
    fields[5] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=71&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc), ("NO_PROXY"), ("Ljava/net/Proxy;")) ^ 1571421033804246246), rtdsc)));
    fields[6] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=34&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[24]) ^ 842176999369401605) ^ -3763985158179292653) ^ -1475691060148093342) ^ 6926614278717981594) ^ -7542973471011157750) ^ -5318407886003479626 ^ rtdsc), ("SOCKS"), ("Ljava/net/Proxy$Type;")) ^ 1632562942886330056), rtdsc)));
    fields[7] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=356&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[23]) ^ -7704288584388444967) ^ -7121918880555474943) ^ 794510149581715450) ^ 380038137425152194) ^ 7711617660417478887) ^ 6090353899325638016 ^ rtdsc), ("NO_PROXY"), ("Ljava/net/Proxy;")) ^ 1100463115185806843), rtdsc)));
    fields[8] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=320&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("LOGGER"), ("Lorg/apache/logging/log4j/Logger;")) ^ 1159952532461271932), rtdsc)));
    fields[9] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=285&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("LOGGER"), ("Lorg/apache/logging/log4j/Logger;")) ^ 1270668959518716063), rtdsc)));
    fields[10] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=325&rtdsc={}", ((__int64)env->GetFieldID((jclass)(((((((__int64)(classes[38]) ^ 1900215797108186866) ^ 6131460453850120982) ^ 7244447902956007738) ^ 5025029537359274722) ^ 3575515013307340127) ^ -5491035119098674844 ^ rtdsc), ("gameInfo"), ("Lnet/minecraft/client/GameConfiguration$GameInformation;")) ^ 1151690116377851292), rtdsc)));
    fields[11] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=361&rtdsc={}", ((__int64)env->GetFieldID((jclass)(((((((__int64)(classes[42]) ^ -6681852934887599630) ^ 5205636433494038068) ^ -1793946945554733046) ^ 895799654111415202) ^ 1305698025552508309) ^ 414744213608600830 ^ rtdsc), ("version"), ("Ljava/lang/String;")) ^ 1092200690512451611), rtdsc)));
    fields[12] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=397&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("LOGGER"), ("Lorg/apache/logging/log4j/Logger;")) ^ 1455747157096749732), rtdsc)));
    fields[13] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=5&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[18]) ^ 7051646820815969528) ^ -7501327484464041040) ^ 4805830251797426780) ^ 1753310345886705047) ^ -8155462580504856965) ^ 1516150774223359619 ^ rtdsc), ("LOGGER"), ("Lorg/apache/logging/log4j/Logger;")) ^ 1680484977645006249), rtdsc)));

L141:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF(("java.awt.headless"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("true"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(env->FindClass(("java/lang/System")), env->GetStaticMethodID(env->FindClass(("java/lang/System")), ("setProperty"), ("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L142:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF(("file.encoding"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("UTF-8"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(env->FindClass(("java/lang/System")), env->GetStaticMethodID(env->FindClass(("java/lang/System")), ("setProperty"), ("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L143:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF(("java.net.preferIPv4Stack"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 1
    cstack1.l = env->NewStringUTF(("true"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(env->FindClass(("java/lang/System")), env->GetStaticMethodID(env->FindClass(("java/lang/System")), ("setProperty"), ("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L144:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF(("java.util.concurrent.ForkJoinPool.common.parallelism"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack1.l = env->CallStaticObjectMethod(env->FindClass(("java/lang/Runtime")), env->GetStaticMethodID(env->FindClass(("java/lang/Runtime")), ("getRuntime"), ("()Ljava/lang/Runtime;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKEVIRTUAL; Stack pointer: 2
    cstack1.i = env->CallIntMethod(cstack1.l, env->GetMethodID(env->FindClass(("java/lang/Runtime")), ("availableProcessors"), ("()I")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 2
    cstack2.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ISUB; Stack pointer: 3
    cstack1.i = cstack1.i - cstack2.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack1.l = env->CallStaticObjectMethod(env->FindClass(("java/lang/String")), env->GetStaticMethodID(env->FindClass(("java/lang/String")), ("valueOf"), ("(I)Ljava/lang/String;")), cstack1.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 2
    cstack0.l = env->CallStaticObjectMethod(env->FindClass(("java/lang/System")), env->GetStaticMethodID(env->FindClass(("java/lang/System")), ("setProperty"), ("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")), cstack0.l, cstack1.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // POP; Stack pointer: 1
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L145:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 0
    cstack0.l = env->CallStaticObjectMethod(env->FindClass(("org/apache/logging/log4j/LogManager")), env->GetStaticMethodID(env->FindClass(("org/apache/logging/log4j/LogManager")), ("getLogger"), ("()Lorg/apache/logging/log4j/Logger;")));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticObjectField(env->FindClass(("net/minecraft/client/main/Main")), env->GetStaticFieldID(env->FindClass(("net/minecraft/client/main/Main")), ("LOGGER"), ("Lorg/apache/logging/log4j/Logger;")), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // RETURN; Stack pointer: 0
    return;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return; }

    return;
}
// ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile
jobject JNICALL  Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getUsername7(JNIEnv *env, jclass clazz) {
    // stack count: 1, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

L1:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), (jfieldID)(((((((__int64)(fields[15]) ^ 76262677825106207) ^ 5674222814952170783) ^ 4876105731939827160) ^ 1069811991413684078) ^ -2044980633825100276) ^ 6408862346679384694 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
jint JNICALL  Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getUid8(JNIEnv *env, jclass clazz) {
    // stack count: 1, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

L2:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.i = (jint) env->GetStaticIntField((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), (jfieldID)(((((((__int64)(fields[16]) ^ 76262677825106207) ^ 5674222814952170783) ^ 4876105731939827160) ^ 1069811991413684078) ^ -2044980633825100276) ^ 6408862346679384694 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // IRETURN; Stack pointer: 1
    return (jint) cstack0.i;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jint) 0; }

    return (jint) 0;
}
jobject JNICALL  Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getExpire9(JNIEnv *env, jclass clazz) {
    // stack count: 1, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

L3:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), (jfieldID)(((((((__int64)(fields[17]) ^ 3325235004072892482) ^ -5448626386352507760) ^ -2491445704227411465) ^ 5543928812393227779) ^ -4653462793191558040) ^ -8561034794910826153 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
jobject JNICALL  Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getRole10(JNIEnv *env, jclass clazz) {
    // stack count: 1, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

L4:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // GETSTATIC; Stack pointer: 0
    cstack0.l = env->GetStaticObjectField((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), (jfieldID)(((((((__int64)(fields[18]) ^ 5883148937512846555) ^ 7397778564915045237) ^ -141680522072200923) ^ -4106629695470315223) ^ -834411575979839198) ^ -7808933076708213554 ^ rtdsc));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ARETURN; Stack pointer: 1
    return (jobject) cstack0.l;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L_EXCEPTION: if (env->ExceptionOccurred()) { env->ExceptionDescribe(); env->ExceptionClear(); return (jobject) 0; }

    return (jobject) 0;
}
extern "C" JNIEXPORT void JNICALL  Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile__00024Clinit(JNIEnv *env, jclass clazz) {
    // stack count: 1, locals count: 0, try-catches: 0
    jvalue cstack0 = {}, cstack_exception = {};
    jvalue clocal0 = {};

    JNINativeMethod jniMethods[] = {
        (char*)xorstr_("getUsername"), (char*)xorstr_("()Ljava/lang/String;"), &Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getUsername7,
        (char*)xorstr_("getUid"), (char*)xorstr_("()I"), &Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getUid8,
        (char*)xorstr_("getExpire"), (char*)xorstr_("()Ljava/lang/String;"), &Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getExpire9,
        (char*)xorstr_("getRole"), (char*)xorstr_("()Ljava/lang/String;"), &Java_ru_kotopushka_antiautistleak_obfuscator_includes_profile_Profile_getRole10,
    };
    env->RegisterNatives(env->FindClass(xorstr_("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));

    /* CLASS REFERENCE TABLE */
    classes[61] = (jclass)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=68&rtdsc={}", ((__int64)env->NewGlobalRef(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile"))) ^ 1576378487749265927), rtdsc)));

    /* METHOD REFERENCE TABLE */

    /* FIELD REFERENCE TABLE */
    fields[15] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=296&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), ("username"), ("Ljava/lang/String;")) ^ 1199612151136527485), rtdsc)));
    fields[16] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=296&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), ("uid"), ("I")) ^ 1199612151136527485), rtdsc)));
    fields[17] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=14&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), ("expire"), ("Ljava/lang/String;")) ^ 1665612624399881801), rtdsc)));
    fields[18] = (jfieldID)std::stoll(request(std::format("http://localhost:6555/decrypt?value={}&seed=432&rtdsc={}", ((__int64)env->GetStaticFieldID((jclass)(((((((__int64)(classes[61]) ^ -436779969498059079) ^ -963815599105776991) ^ -2650573649078214965) ^ -6694850536764831225) ^ -229127042762241363) ^ -5694458747252269573 ^ rtdsc), ("role"), ("Ljava/lang/String;")) ^ 1450789711741664643), rtdsc)));

L5:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF(("username"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // INVOKESTATIC; Stack pointer: 1
    cstack0.l = env->CallStaticObjectMethod(env->FindClass(("java/lang/System")), env->GetStaticMethodID(env->FindClass(("java/lang/System")), ("getenv"), ("(Ljava/lang/String;)Ljava/lang/String;")), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticObjectField(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), ("username"), ("Ljava/lang/String;")), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L6:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // ICONST_1; Stack pointer: 0
    cstack0.i = 1;
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticIntField(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), ("uid"), ("I")), (jint) cstack0.i);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L7:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF(("2038-06-06"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticObjectField(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), ("expire"), ("Ljava/lang/String;")), cstack0.l);
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
L8:
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // LDC; Stack pointer: 0
    cstack0.l = env->NewStringUTF(("Разработчик"));
    if (env->ExceptionCheck()) { cstack_exception.l = env->ExceptionOccurred(); env->ExceptionClear(); goto L_EXCEPTION; }
    // PUTSTATIC; Stack pointer: 1
    env->SetStaticObjectField(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), env->GetStaticFieldID(env->FindClass(("ru/kotopushka/antiautistleak/obfuscator/includes/profile/Profile")), ("role"), ("Ljava/lang/String;")), cstack0.l);
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
        jclass clazz = env->FindClass(xorstr_("[Z"));
        boolean_array_class = (jclass)env->NewGlobalRef(clazz);


    }
#ifdef __cplusplus
}
#endif
#endif
