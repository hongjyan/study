public class VInF {
    public static String[] F() {
        String[] str = new String[2];
        str[0] = "Hello";
        str[1] = "World";
        return str;
    }

    public Foo getFoo() {
        Foo foo = new Foo(1);
        return foo;
    }

    public static void main(String[] args) {
        String[] strout = F();
        System.out.println(strout[0] + " " + strout[1]);
        
        VInF vinf = new VInF();
        System.out.println(vinf.getFoo().getAttribute());
    }
}
