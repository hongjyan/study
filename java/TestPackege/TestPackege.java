import java.util.ArrayList;

public class TestPackege {

    public static void main(String[] args) {
	String orig = "Game";
	String copy = orig;
	String lowerCopy = orig.toLowerCase();
	System.out.println("orig is " + orig);
	System.out.println("copy is " + copy);
        System.out.println("lowerCopy is " + lowerCopy);

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
	ArrayList<Integer> al = new ArrayList<Integer>();
	al.add(1);
//2017.8.30
        {
            //do not know why
            String s2 = new StringBuilder("ja").append("va").toString();
            System.out.println(s2 == s2.intern()); //false

            String s1 = new StringBuilder("go").append("od").toString();
            System.out.println(s1 == s1.intern()); //true
        }
//2017.8.31
        {
            long h = Math.round(11.5);  //11.5+0.5 = 12, round will make argument+0.5, then get integer in down direction.
            long j = Math.round(-11.5); //-11.5+0.5 = -11.
            long l = Math.round(11.4); //11.4+0.5 = 11.9, Integer is 11.
            System.out.println("h is " + h + ", j is " + j + ", l is " + l);
        }

        //switch can accpet "byte, char, short, int, String, enum". No long
        String b = "2";  
        switch(b) {
        case "2":
            System.out.println(b);
            break;
        default:
            System.out.println("default");
            break;
        }

        //String equality testing
        String s1 = "Programming";
        String s2 = new String("Programming");
        String s3 = "Program";
        String s4 = "ming";
        String s5 = "Program" + "ming";
        String s6 = s3 + s4;
        System.out.println(s1 == s2);  //false
        System.out.println(s1 == s5);  //true 
        System.out.println(s1 == s6);  //false
        System.out.println(s1 == s6.intern()); //true
        System.out.println(s2 == s2.intern()); //false
        
        //one file can include multiple class as long as there is only one public class. Not recommended.
        System.out.println(PClass.age);
        //Recommended for inner class
        //It is wired that private file of inner-class can be accessed by outer-class, which is unable in C++
        System.out.println(InnerClass.age);
        TestPackege tp = new TestPackege();
        InnerClass ic = tp.new InnerClass();  //must create outer-class in advance then new inner class except inner class is static. Not same as C++
        ic.printName(tp);
       
    }        

    private class InnerClass {
         private final static int age = 18;
         private void printName(TestPackege tp) { //not able to declare static method for inner class except inner class is static.
             System.out.println(tp.name); //ok to access private file of outer-class, same as C++
         }
    }
   
    private String name = "myName";
};


//not recommended.
class PClass {
    public final static int age = 18;
}; 

