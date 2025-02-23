package ru.itskekoff.j2c.translator.processor.cpp.reference;

import lombok.Getter;

import java.util.Random;

@Getter
public class ReferenceNode {
    private final String className;
    private final String name;
    private final boolean isStatic;
    private final int id;
    private final String signature;
    private long Seed;
    long clinit, kluch, kluch2, kluch3, kluch4, kluch5,kluch6;

    public ReferenceNode(String className, String name, String signature, boolean isStatic, int id) {
        this.className = className;
        this.name = name;
        this.isStatic = isStatic;
        this.id = id;
        this.signature = signature;
        Seed = new Random().nextInt(0x15 * 0x15 ^ 0x16 >> 2);

        CustomRandom randomSource = new CustomRandom(583485834L);
        long serverSeed = Seed ^ (0xDC9A ^ randomSource.nextLong());
        randomSource = new CustomRandom(serverSeed);

        clinit = randomSource.nextLong();
        kluch = randomSource.nextLong();
        kluch2 = randomSource.nextLong();
        kluch3 = randomSource.nextLong();
        kluch4 = randomSource.nextLong();
        kluch5 = randomSource.nextLong();
        kluch6 = randomSource.nextLong();
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
            seed = (seed * 6364136222846791005L + 1);
            return seed;
        }
    }
}
