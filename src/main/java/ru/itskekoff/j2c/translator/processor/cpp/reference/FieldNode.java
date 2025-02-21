package ru.itskekoff.j2c.translator.processor.cpp.reference;

public class FieldNode {
    private final String className;
    private final String name;
    private final boolean isStatic;
    private final int id;
    private final String signature;

    public FieldNode(String className,String name, String signature,  boolean isStatic, int id) {
        this.className = className;
        this.name = name;
        this.isStatic = isStatic;
        this.id = id;
        this.signature = signature;
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
