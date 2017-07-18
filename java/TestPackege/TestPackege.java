import java.util.ArrayList;

public class TestPackege {

    public static void main(String[] args) {
	String orig = "Game";
//	String copy = orig;
	orig.toLowerCase();
	System.out.println("orig is " + orig);
//	System.out.println("copy is " + copy);
//	System.out.println("lCopy is " + lCopy);

	protectedClass pc = new protectedClass();
	System.out.println("pc's protected member is " + pc.flag); // can access the protected member of other class if they are in the same package namespace.
//	System.out.println("pc's private member is " + pc.i);  //can't access private member still even in same package namespace.
	//can int compare Integer?
	int i = 1;
	Integer in = new Integer(1);
	System.out.println(i== (int)in);
	System.out.println((Integer)i== in);  //attention, return false
	
	String hello = "hello world!!";
//	System.out.println(hello.substring(0, -1)); //-1 is not allowed, will trigger exception.


//2015.10.12
	int ii = 10&0xff;
	System.out.println("Integer.toString(in) is " + Integer.toString(ii));

	//ArrayList<Integer> al = null;
	ArrayList<Integer> al = new ArrayList();
	al.add(1);
    }
}
