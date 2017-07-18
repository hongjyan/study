public class staticMethod_object {
    public staticMethod_object() {
	System.out.println("I am creating object");
    }

    public static void foo() {}

    public static void main(String[] args ) {
	staticMethod_object.foo();
    }
}
