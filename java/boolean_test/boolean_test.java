public class boolean_test {
    public boolean f() {
	return true;
    }

    public static void main(String args[]) {
	boolean_test B = new boolean_test();
	if (B.f()) {
	    System.out.println("true");
	}
	else {
	    System.out.println("false");
	}
    }
}
