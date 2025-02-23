package ru.itskekoff.j2c.translator.processor.cpp.reference;

import lombok.Getter;
import ru.itskekoff.j2c.translator.configuration.TranslatorConfiguration;

import java.util.Random;

@Getter
public class ReferenceNode {
    private final String className;
    private final String name;
    private final boolean isStatic;
    private final int id;
    private final String signature;
    private final long initialSeed;
    private final long clinitKey;

    private final @Getter long[] referenceKeys = new long[6];

    public ReferenceNode(String className, String name, String signature, boolean isStatic, int id) {
        this.className = className;
        this.name = name;
        this.isStatic = isStatic;
        this.id = id;
        this.signature = signature;
        initialSeed = new Random().nextInt(0x15 * 0x15 ^ 0x16 >> 2);

        CustomRandom randomSource = new CustomRandom(TranslatorConfiguration.IMP.REFERENCE.SEED_CONSTANT);
        long serverSeed = initialSeed ^ (TranslatorConfiguration.IMP.REFERENCE.XOR_CONSTANT ^ randomSource.nextLong());
        randomSource = new CustomRandom(serverSeed);

        clinitKey = randomSource.nextLong();

        for (int i = 0; i < referenceKeys.length; i++) {
            referenceKeys[i] = randomSource.nextLong();
        }
    }

    public String isStatic() {
        return isStatic ? "Static" : "";
    }

    public boolean isStaticVal() {
        return isStatic;
    }

    static class CustomRandom {
        private long seed;

        public CustomRandom(long seed) {
            this.seed = seed;
        }

        public long nextLong() {
            seed = (seed * TranslatorConfiguration.IMP.REFERENCE.RANDOM_FACTOR + 1);
            return seed;
        }
    }
}
