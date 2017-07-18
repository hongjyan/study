#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <iostream>

int main(int argc, char* argv[]) {
    const char *optstring = "abc:d:";
    int optindex = 0;
    bool doneparsing = false;
    int ret = 0;

    for (int i=0; i<argc; i++) {
        std::cout << argv[i] << std::endl;
    }

    static const struct option options[] = {
       { "apple",   no_argument,        NULL,  'a' },
       { "boy",     no_argument,        NULL,  'b' },
       { "cat",     required_argument,  NULL,  'c' },
       { "dog",     required_argument,  NULL,  'd' },
       { 0,         0,                  0,      0  }
   };

    //getopt_long parse argument the same as main though it has parameter argc, argv. two lines below can not take effect
    argc -= 2; 
    argv += 2;

    while (!doneparsing) {
        ret = getopt_long(argc, argv, optstring, options, &optindex);
        switch (ret) {
            case ('a'):
                printf("option apple\n");
                break;
            case ('b'):
                printf("option boy\n");
                break;
            case ('c'):
                printf("option cat, optarg is %s\n", optarg);
                break;
            case ('d'):
                printf("option dog, optarg is %s\n", optarg);
                break;
            case (-1):
                printf("parameter parsing should be done\n");
                doneparsing = true;
                break;
            case (':'):
                printf("ret is ':'\n");
                break;
            case ('?'):
                printf("ret is '?'\n");
                break;
            default:
                printf("should not come here\n");
                break;
       }
    }
}
                    
