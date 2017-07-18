public class VInF {
    public static String[] F() {
	String[] str = new String[2];
	str[0] = "Hello";
	str[1] = "World";
	return str;
    }

    public static void main(String[] args) {
	String[] strout = F();
	System.out.println(str[0] + " " + str[1]);
    }
}
