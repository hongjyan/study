#include <string>
#include <iostream>
#include <thread>

typedef void (*callback) (std::string);
 
struct Lib { 
     int start(int, callback);
     
     void threadFunc(int, callback);
};
