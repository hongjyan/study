public class sup {
    protected String name;

    public sup(String name_) {
        name = name_;
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

    public void printName() {
        System.out.println("My Name is " + name);
    }
}


