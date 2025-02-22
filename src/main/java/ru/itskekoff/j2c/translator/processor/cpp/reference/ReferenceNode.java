package ru.itskekoff.j2c.translator.processor.cpp.reference;

import lombok.Getter;

import java.util.Random;

public class ReferenceNode {
    @Getter
    private final String className;
    @Getter
    private final String name;
    private final boolean isStatic;
    @Getter
    private final int id;
    @Getter
    private final String signature;
    @Getter
    private long seed;
    @Getter
    long clinit, kluch, kluch2, kluch3, kluch4, kluch5,kluch6;

    public ReferenceNode(String className, String name, String signature, boolean isStatic, int id) {
        this.className = className;
        this.name = name;
        this.isStatic = isStatic;
        this.id = id;
        this.signature = signature;
        seed = new Random().nextInt(0x15 * 0x15 ^ 0x16 >> 2);

        long serverSeed = (seed ^ (0xDC9A ^ (new Random(583485834L).nextLong())));

        Random random = new Random(serverSeed);
        clinit = random.nextLong();
        kluch = random.nextLong();
        kluch2 = random.nextLong();
        kluch3 = random.nextLong();
        kluch4 = random.nextLong();
        kluch5 = random.nextLong();
        kluch6 = random.nextLong();
    }

    //ХЪВАЪХАВХЪВАФЫХЪАВЫХЪФЫАВЪХ, Я ПОТОМ САМ ПЕРЕДЕЛАЮ, ТЫ ПОКА ЭТО НЕ ТРОГАЙ ЫВАХЪФВАХЪАВХЪВАХЪФЪАХФВАХЪЫ


    public String isStatic() {
        return isStatic ? "Static" : "";
    }

    public boolean isStaticVal() {
        return isStatic;
    }

}
