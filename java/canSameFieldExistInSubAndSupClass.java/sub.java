class sub extends sup {
    public int j = 10;
    public int i = 11;
    public static void main(String args[]) {
        sub mySub = new sub();
        System.out.println("mySub.i is " + mySub.i + ",mySub.j is " + mySub.j); //java won't override field of super class, so can not use "mySub.super.j".
                                                                                //java override method of super class, so can use mySub.super.methodx()".
    }
}
