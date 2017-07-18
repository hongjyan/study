public class Derived extends Base {
    public void foo() {
        System.out.println("I am in Derived");
    }

    public static void main(String args[]) {
        Derived obj = new Derived();
        obj.foo();
    }
};
