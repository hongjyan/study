public class sub extends sup {
    public sub(String name) {
	super(name);
    }

    public void method_instance() {
        System.out.println("the instance method in sub");
    }   

    public void printSupName(sup someone) {
        System.out.println("someone's name is " + someone.name);
    }
    
	
    public static void main(String[] args) {
        sup base = new sup("base");
        sub derived = new sub("derived");
        base.method_instance();
        derived.method_instance();
        base = derived;
        base.method_instance();
	sub derived2 = new sub("derived2");

        /* can sub access sup's protected field */
        base = new sup("base");
        base.printName();
        derived.printName();
        derived.printSupName(base);
    }


}   

