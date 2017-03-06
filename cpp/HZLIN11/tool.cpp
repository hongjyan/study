#include <stdio.h>
#include <iostream>
#include <string.h>


//$1$2.....$32
void print1() {
    char output[200];
    char *pout = output;
    int j = 0;
    for (int i=1; i<=32; i++) {
	j = snprintf(pout, 10, "%c", '$');
	pout += j;
	j = snprintf(pout, 10, "%d", i);
	pout += j;
    }
    printf("%s\n", output);
}  

//file to one line
std::string fileToLine(const char *filename) {
    FILE *fd = fopen(filename, "r");
    std::string lines = "";
    char line[1000]; //max 1000 char per line
    memset(line, 0, 1000);

    if (NULL == fd) {
	printf("open file:%s failed\n", filename);
	return "";
    }
    
    while (!feof(fd)) {
	fgets(line, 1000, fd);
	lines += line;
	lines = lines.substr(0, lines.length()-1);
    }
    return lines;
}    


int main(int argc, char* argv[]) {
    print1();
    std::cout << fileToLine("tool.cpp") << std::endl;
}
