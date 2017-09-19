import java.lang.Integer;
public class compareBetweenIntegerAndint {
    public static void main(String args[]) {
        int i = 10;
        Integer I = new Integer(10);
        Integer K = new Integer(10);
        Integer J =  10;  //10 autoboxing to Integer type.
        System.out.println(i==I); //true. I will be unautoboxing to int, then compare
        System.out.println(I==K); //false. I and K refer to different object.
        System.out.println(I==J); //false
    }
}
