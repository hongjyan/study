public class sup {
    public sup() {
	f();
    }

    public void method_instance() {
	System.out.println("the instance method in sup");
    }

    public void f() {
	f_toBeCall();
    }
    
    public void f_toBeCall() {
	System.out.println("I am f_toBeCall in base class");
    }
}


