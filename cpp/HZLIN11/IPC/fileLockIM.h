#ifndef _FILELOCKIM_H
#define _FILELOCKIM_H
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <sys/ioctl.h>
#define FILERD "QQ"
#define FILEWR "QQ"

//should not define global variable in header file, otherwise, error below will report.
// /tmp/ccJFeZ37.o:(.data+0x0): multiple definition of `gg_flw'
//struct flock g_flw = { F_WRLCK, SEEK_CUR, 0, 0, 0 };
//struct flock g_flr = { F_RDLCK, SEEK_CUR, 0, 0, 0 };

extern struct flock g_flw;
extern struct flock g_flr;

bool writeToFile(int fd, const char* msg, size_t size);

int readFromFile(int fd, char* buf, size_t bufsize);
#endif
