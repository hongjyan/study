public class Dog {
    private String name;

    public Dog(String Name) {
	name = Name;
    }
 
    public String getName() {
	return name;
    }

    public void setName(String newname) {
	name = newname;
    }

    public static void main(String[] args) {
	Dog aDog = new Dog("Max");
	foo(aDog);

	//can not get conclusion according to jugements below. 
	//"JAVA object assignment by methond call" is the same as "pointer parameter in  C method call". the actual parameter 's address is passed as value
	//to formal parameter. You can change the content where pointer point to, since you can *formalParameter = xxx, but you can not where actualParamter 
	//points to since the value of pointer is passed as value during method call. See Dog.c
	if (aDog.getName().equals("Max")) {  
	    System.out.println("Java passes by value");
	} else if (aDog.getName().equals("Fifi")) {
	    System.out.println("Java passes by reference");
	}
    }

    public static void foo(Dog d) {
	System.out.println("d.getName().equals(\"Max\") is " + d.getName().equals("Max"));
	d.setName("Fifi");
	d = new Dog("dahuang"); //can not do this in C++ if d is reference type foo(Dog& d), since can not assign twice for reference.
        Sytem.out.println(d.getName());
	System.out.println("d.getName().equals(\"Fifi\") is " + d.getName().equals("Fifi"));
    }	
}
