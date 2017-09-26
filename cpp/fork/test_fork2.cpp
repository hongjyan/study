#include <unistd.h>
#include <stdio.h>

int main() {
    int i = 0;
    pid_t pid = fork();
    if (!pid) {
	i++;
	printf("i is %d, the address of i is %p\n", i, &i);
	return 0;
    }
    usleep(100000);
    printf("i is %d, the address of i is %p\n", i, &i);
    return 0;
}
