#include <arpa/inet.h>
#include <stdio.h>
int main() {
  in_addr addr;
  const char* ip = "256.152.189.116";
  if (inet_pton(AF_INET, ip, &addr) != 1) {
      printf("inet_pton failed\n");
  }
  else {
      printf("success\n");
  }
}
