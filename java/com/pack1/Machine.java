package com.pack1;

public class Machine {
    public String  name;
    protected String description; //package and subclass visibility
    private int id;
    String serialNumber;  //package visibility

    public Machine() {
        name = "machine";
        description = "r2";
        id = 1;
        serialNumber = "A1";
   }
}
   
