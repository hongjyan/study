import java.io.PrintStream;


class printStream {
    public void f1() throws Exception {
	System.out.println("hi");
	throw new Exception("test");
    };

    public void f2() throws Exception {
	f1();
    };

    public void f3() throws Exception {
	f2();
    };


    public static void main(String[] args) throws Exception {
	PrintStream ps = new PrintStream("printstream.txt");
	try {
	    printStream PS = new printStream();
	    PS.f3();
	}
	catch (Exception e) {
	    e.printStackTrace();
	    e.printStackTrace(ps);
	}
    }
}
