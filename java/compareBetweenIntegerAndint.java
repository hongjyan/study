import java.lang.Integer;
public class compareBetweenIntegerAndint {
    public static void main(String args[]) {
        int i = 10;
        Integer I = new Integer(100);
        if (i== (int) I) {
            System.out.println("compare success, Equal");
        }
        else {
            System.out.println("wrong");
        }
    }
}
