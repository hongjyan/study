public class constructor {
    public  constructor(String aname, int aage) {
	name = aname;
	age = aage;
	id = nextid;
    }
    
    public constructor(int aage) {
	this("Employee: anonymous", aage);
    }

    public void output() {
	System.out.println("Name is " + name + ", id is " + id + ", age is " + age + ", salary is " + salary);
    } 


    private String name;
    private int id;
    private double age;
    private double salary = 10000;
    
    private static int nextid = 1;
    
    static {
	nextid++;
	System.out.println("Hello world!");
    }

    public static void main(String args[]) {
	constructor E1 = new constructor();
	constructor E2 = new constructor(30);
	constructor E3 = new constructor("Li Lei", 23);

	E1.output();
	E2.output();
	E3.output();
   }
}
