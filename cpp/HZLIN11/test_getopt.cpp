#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>

int main(int argc, char* argv[]) {
    int aflag = 0;
    int bflag = 0;
    char* cvalue = NULL;
    int index;
    int c;

    opterr = 1; //If an error was detected, and the first  character  of optstring is not a colon, 
                //and the external variable opterr is non-zero (which is the default), getopt() prints an error message. 
    
    printf("argc is %d\n", argc);

    while (-1 != (c = getopt(argc, argv, "abc:"))) { // : indicts that there is a argument following option:c
        switch (c) {
            case 'a':
                aflag = 1;
                break;
            case 'b':
                bflag = 1;
                break;
            case 'c':
                cvalue = optarg; //optarg is extern global variable define in getopt library.
                break;
            case '?':        //unknow option or option-without-argument
                if (optopt == 'c') fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt)) fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                else fprintf(stderr, "Unknown option character'\\x%x'.\n", optopt);
                return -1;
            default:
                exit(-1);
        }
    }

     printf("aflag is %d, bflag is %d, cvalue is %s\n", aflag, bflag, cvalue);

     //unknown argument will come here.
     for (index = optind; index < argc; index++) //optind is the index of the first argv-element that is not an option.
         printf("Non-option argument %s\n", argv[index]);
     
     return 0;
}
