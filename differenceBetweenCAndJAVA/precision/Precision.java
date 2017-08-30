public class Precision {
    public static void main()
    {
        {
            short s = 1;
            s = s + 1;
            System.out.println(s);
            float f = 3.14;
            System.out.println(f);
        }
    }
}
/*
Test.java:6: error: incompatible types: possible lossy conversion from int to short
            s = s + 1;
                  ^
Test.java:8: error: incompatible types: possible lossy conversion from double to float
            float f = 3.14;
*/
