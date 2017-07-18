public class derived extends base {
    public int foo() {
	System.out.println("I am \"int foo()\" in derived class"); //can't hide(same name, same parameter, different prototype/return value) in java, but can in C++ if not virtual, then hide will apply.
        return 1;
    }
    public void bar() {
        System.out.println("I am \"void bar()\" in derived class"); //can override(same name, same prototype) | can in C++ if virtual 
    }
    public void bar(int i) {
        System.out.println("I am \"void bar(int i)\" in derived class"); //can overload(same name, different parameter), also can in C++ if not virtual, then hide apply
    }
    public static void main(String args[]) {
	derived obj = new derived();
	obj.foo();
        obj.bar();
        obj.bar(1);
    }
}
