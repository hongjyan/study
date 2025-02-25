#include <iostream>  //cin, cout
#include <stdio.h> //printf, linux man getline
//#include <cstddef>  // size_t, ssize_t
#include <stdlib.h> // for free
#include <sstream>  //istringstream
#include <fstream>  //ifstream

int main() {
    std::string name;
    std::string movie;

    std::cout<<"enter your name"<<std::endl;
    std::getline(std::cin, name);

    std::cout<<"enter your favourite movie"<<std::endl;
    std::getline(std::cin, movie);

    std::cout<<name<<"'s favourite movie is "<<movie<<std::endl;

    std::cout << "enter something, man" << std::endl;
    char s[100];
    std::cin.getline(s, sizeof(s)); //cin.getline has different syntax with std::getline
    std::cout<<"s is "<<s<<std::endl;

    char *line = NULL;
    size_t n = 0;
    ssize_t read;
    printf("please enter one line\n");
    if ((read = getline(&line, &n, stdin)) != -1) {
        printf("line is %s\n", line);
    }
    if (line) free(line);

//2021.2.22
//ex17.37
    std::string f;
    std::cout << "input file_path/file_name" << std::endl;
    std::cin >> f;
    std::ifstream  is(f, std::ifstream::in);
    //is >> std::noskipws;
    char sink[30];
    //while (is.getline(sink, sizeof(sink))) {  //is.failbit will be set if line character number exceed 30.
    while (is.getline(sink, sizeof(sink)) || !is.eof()) {
        std::cout << sink << std::endl;
        is.clear();
    }      
    is.close();


//2017.1.5
    std::string output = "interfaces in default instance:\r\n"
"-------------------------------\r\n"
"\r\n"
"  internal (Ethernet interface)\r\n"
"    VRF name(ID)        : default (0)\r\n"
"    owner               : /MMN-0\r\n"
"    MAC                 : fa:16:3e:c8:f0:22\r\n"
"    MTU                 : 1500\r\n"
"    admin state         : up\r\n"
"    IPv4 forwarding     : no\r\n"
"    IPv6 forwarding     : no\r\n";

    std::istringstream iss(output);

    char aline[65535] = {0};
    while (iss.getline(aline, 65535)) {
	printf("%s\n", aline);
    }

    std::string temp;
    std::cout << iss.fail() << iss.bad() << iss.eof() << std::endl; //don't know why iss failbit was set.
    iss.seekg(0);
    std::cout << iss.fail() << iss.bad() << iss.eof() << std::endl;
    iss.clear();  //clear failbit
    iss.seekg(0); //? to be done.
    std::cout << iss.fail() << iss.bad() << iss.eof() << std::endl;
    while (iss >> temp) {
        printf("%s\n", temp.c_str());
    }
    
    return 0;

}
