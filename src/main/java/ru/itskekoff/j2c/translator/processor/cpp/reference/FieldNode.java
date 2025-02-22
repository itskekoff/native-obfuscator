package ru.itskekoff.j2c.translator.processor.cpp.reference;

import java.util.Random;

public class FieldNode {
    private final String className;
    private final String name;
    private final boolean isStatic;
    private final int id;
    private final String signature;
    private long seed;
    long clinit, kluch, kluch2, kluch3, kluch4, kluch5,kluch6;

    public FieldNode(String className,String name, String signature,  boolean isStatic, int id) {
        this.className = className;
        this.name = name;
        this.isStatic = isStatic;
        this.id = id;
        this.signature = signature;
        seed = new Random().nextInt(0x15 * 0x15 ^ 0x16 >> 2);

        long serverSeed = (seed ^ (0xDC9A ^ (new Random(583485834L).nextLong())));
        System.out.println(serverSeed);

        Random random = new Random(serverSeed);
        clinit = random.nextLong();
        kluch = random.nextLong();
        kluch2 = random.nextLong();
        kluch3 = random.nextLong();
        kluch4 = random.nextLong();
        kluch5 = random.nextLong();
        kluch6 = random.nextLong();
        long d = 16958223540479980461L;
    }

    //ХЪВАЪХАВХЪВАФЫХЪАВЫХЪФЫАВЪХ, Я ПОТОМ САМ ПЕРЕДЕЛАЮ, ТЫ ПОКА ЭТО НЕ ТРОГАЙ ЫВАХЪФВАХЪАВХЪВАХЪФЪАХФВАХЪЫ


    public long getClinit() {
        return clinit;
    }

    public long getKluch() {
        return kluch;
    }

    public long getKluch2() {
        return kluch2;
    }

    public long getKluch3() {
        return kluch3;
    }

    public long getKluch4() {
        return kluch4;
    }

    public long getKluch5() {
        return kluch5;
    }

    public long getKluch6() {
        return kluch6;
    }

    public long getSeed() {
        return seed;
    }

    public String getSignature() {
        return signature;
    }

    public int getId() {
        return id;
    }

    public String isStatic() {
        return isStatic ? "Static" : "";
    }

    public boolean isStaticVal() {
        return isStatic;
    }

    public String getName() {
        return name;
    }

    public String getClassName() {
        return className;
    }
}
