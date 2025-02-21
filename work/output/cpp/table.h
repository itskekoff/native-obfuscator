#include "jni.h"
#include <iostream>
#include <cstdlib>

long lrand()
{
    if (sizeof(int) < sizeof(long))
        return (static_cast<long>(rand()) << (sizeof(int) * 8)) |
        rand();

    return rand();
}

#define TEMPLATE_1 _rotl(keys[0] ^ (keys[1] ^ (keys[2] | 0x15 ^ (keys[3] ^ (keys[3]) ^ (keys[4] ^ 0xDB9CADA) ^ _rotl(keys[8], 2) ^ 0xF9DBAA9B) ^ 0xDAAABBFF) ^ 0xBA9D7FC4D3) ^ 487374737448484L, 16) ^ (keys[16] ^ (keys[15] / (keys[14] * (keys[13]  % ~(keys[12] - (keys[11] + (keys[10]  ^ ((__int64)keys))))))))

class RBM {
private:
    int keys[24];
    int keyMain;
    long rshift;
    long lshift;
    int poses[16];
    __int64 _struct;
    void updateKeys() {
        for (int i = 0; i < 24; i++)
            keys[i] = rand();

        for (int i = 0; i < 16; i++)
            poses[i] = (rand() % 16);

        keyMain = rand();
        rshift = lrand();
        lshift = lrand();
    }

public:
    RBM() {}
    RBM(jobject address) {
        _struct = (__int64)address;
        updateKeys();
        applyEncryption();
    }

    void applyEncryption() {
        _struct = 0x15 ^ _struct;
    }

    jclass applyDecryption() {
        jclass value = (jclass)(_struct ^ 0x15);
        return value;
    }
};

RBM classes[61];
jmethodID methods[31];