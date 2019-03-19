package com.pack2;

import com.pack1.Machine;

public class Robot extends Machine {
    public Robot() {
        Machine mach1 = new Machine();
        System.out.println(mach1.name);
        //System.out.println(mach1.description); //protect field is visible among same package, but subclass can access itselfprotect field.
        //System.out.println(mach1.id); //private only visible in the same class
        //System.out.println(mach1.serialNumber); //non access lable visible among same package
    }
}
        
