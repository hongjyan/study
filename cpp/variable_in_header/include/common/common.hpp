#ifndef COMMON_H  //ifndef must be here, otherwise, static variable define here will be multidefined if header file is included multi times.
#define COMMON_H
/********************** define **********************/
    //internal linkage
        //const imply static. 
        //So every source file including this header will have one copy of c_foo.
        //But since c_foo is const, so every copy's value is the same. 
        int const c_foo = 6; 
    
        //every soure file has one copy. And these copies's value may diff.
        static int bar = 6;
        static int kar = 6;


    //external linkage. it is not allowed to define external variable in header.
    /*
     g++ -Wall -std=c++11 main.cpp plus.cpp square.cpp
     multiple definition of `c_war/war' 
     error: ld returned 1 exit status.
     since variable must be defined only once.if it is defined in header file, 
     it will be defined multitimes when it be included by multi files 
    */

         //extern const int c_war = 6; //explicit extern erase implicit static of const.
         //int war = 6; //external linkage since normal variable taken as non-static. 




/********************** declare **********************/
    //declare one variable using extern in header to make it global.
    //define it only one source file.
    //declare it using extern in other source files. 
    //external linkage.
    extern int tar;



#endif
