#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>

int main() {
    const char *in = "game"; 
    int len = strlen(in);
    char *out = new char[len];
    short int *p_data = new short int [len];

    int i = 0;
    for (; i<len; i++) {
	p_data[i] = (short int) (in[i]);
    }
    
    do {
	out[len-i] = (char) (p_data[len-i]);
    }
    while(i--);

    printf("out is %s\n", out);  
    
    delete out;
    delete p_data;  

    std::string s;
    s.push_back('g');
    s.push_back('a');
    s.push_back('m');
    s.push_back('e');
    bool equal = strcmp(s.c_str(), "game");
    std::cout << "equal is " << equal << std::endl; 

    std::cout << s.compare("fuck") << std::endl;
}	
