class showSubClassName_fromSuperClass {
    public void displayClass() {
	String name = getClass().getName();
	System.out.println(name);
    }

    public static void main(String[] args) {
	B b = new B();
	b.displayClass();
	C c = new C();
	c.displayClass();
    }
};

class B extends A {
};

class C extends A {
};

