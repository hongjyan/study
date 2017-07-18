public class derivedClass extends abstractClass {
    protected void abstract_f() {
	System.out.println("I am abstract_f() in derivedClass");
    }

    public static void main(String[] args) {
	derivedClass D = new derivedClass();
	D.f();
    }
}
