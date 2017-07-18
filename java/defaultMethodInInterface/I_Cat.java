package com.journaldev.java8.defaultmethod;

public interface I_Cat {

    default void miao() {
	System.out.println("Miao");
    }


    void miao();
    void purr();

}
