package test;
import java.util.Arrays;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class test {
    public static void wrongWayChangeObject(String s) {
	s = "test";  //"test" is converted to a new String implicitly.
    }

    public static void rightWayChangeObject(String s) {
	s.toUpperCase();
    }
    
    public static void cannotChangePrimitive(int i) {
	i = 10;
    }

    public static void main(String[] args) {
	Double d = new Double(0x41);
        Integer i = new Integer(0x41);
        System.out.println("d.tostring() is " + d.toString());
        System.out.println("i.tostring() is " + i.toString());
        String s = "game" + i.intValue();
	System.out.println("s is " + s);
//2015/11/20
	String proName = "pro1";
        if ("pro1 " == proName) {
	    System.out.println("yes");
	}
//2015.12.17
        System.out.println(s.substring(0, 0));
        //System.out.println(s.substring(0,s.indexOf('.'))); //Exception will throw if take -1 as index
//2015.12.18
        String[] names = {"ming", "feng", "yue", "meng"};
        //System.out.println(names[1:3]);
        for (String name : Arrays.copyOfRange(names, 1, 3)) {
	    System.out.println(name);
        }

//2016.05.09
	String str = "game";
	wrongWayChangeObject(str);
	System.out.println(str);
	rightWayChangeObject(str);
	System.out.println(str);
	str.toUpperCase(); //seems no way to change String content.
	System.out.println(str);
	int j = 1;
	cannotChangePrimitive(j);
	System.out.println(j);
//2019.3.28 test if new file("existfilename") will empty file content
        File f_handler = new File("test/story.txt");
        try {
            FileInputStream fis = new FileInputStream(f_handler);
            int content = 0;
            while (-1 != (content=fis.read())) {
                 System.out.print((char) content);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
