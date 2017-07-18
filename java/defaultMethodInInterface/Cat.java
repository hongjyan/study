package com.journaldev.java8.defaultmethod;

public class Cat implements I_Cat {
    public void purr() {
	System.out.println("Purr");
    }
   
    public void miao() {
	System.out.println("miao in Cat");
    }

    public static void main(String[] args) {
	Cat cat = new Cat();
	cat.purr();
    }
}
