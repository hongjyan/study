public class sub extends sup {
    public sub() {
	super();
    }

    public void method_instance() {
        System.out.println("the instance method in sub");
    }   
    
	
    public static void main(String[] args) {
        sup base = new sup();
        sub derived = new sub();
        base.method_instance();
        derived.method_instance();
        base = derived;
        base.method_instance();

	sub derived2 = new sub();
    }

}   

