#include "fileLockIM.h"
#define DEBUG 1

struct flock g_flr = {F_RDLCK, SEEK_CUR, 0, 0, 0};
struct flock g_flw = {F_WRLCK, SEEK_CUR, 0, 0, 0};

bool writeToFile(int fd, const char* msg, size_t len)
{
    ssize_t len_writen = 0;
    g_flw.l_type = F_WRLCK;
    if (-1 == (fcntl(fd, F_SETLKW, &g_flw))) { perror("fcntl"); return -1; }
    #ifdef DEBUG
    printf("Acquired write lock succussfully\n");
    #endif

    len_writen = write(fd, msg, len);

    g_flw.l_type = F_UNLCK;
    if (-1 == (fcntl(fd, F_SETLK, &g_flw))) { perror("fcntl"); return -1; }
    #ifdef DEBUG
    printf("Released write lock succussfully\n");
    #endif

    if (len_writen == len) return true;
    return false;
}

int readFromFile(int fd, char* buf, size_t size)
{
    int ret;
    g_flr.l_type = F_RDLCK;
    int count;    

    while (true) {
        if (-1 == ioctl(fd, FIONREAD, &count)) { perror("ioctl"); return -1; }
        if (count > size) { 
            printf("size to be read:%d is bigger than buffer size:%zd, please enlarge buffer size\n", count, size);
            return -1;
        }
        else if (0 == count) {
            continue;
        }
        else {
            #ifdef DEBUG
            printf("size to be read is %d\n", count);
            #endif
            break;
        }
    }  
        
    if (-1 == (fcntl(fd, F_SETLKW, &g_flr))) { perror("fcntl"); return -1; }
    #ifdef DEBUG
    printf("Acquired read lock succssfully\n");
    #endif 
    ret = read(fd, buf, count);
    if (0 == ret && 0 != count) {
        printf("my own write\n");
    }
    else if (ret != count) { printf("read size:%d is not equal to \"size to be read:\"%d\n", ret, count); return -1;}
    
    
    g_flr.l_type = F_UNLCK;
    if (-1 == (fcntl(fd, F_SETLKW, &g_flr))) { perror("fcntl"); return -1; }
    #ifdef DEBUG
    printf("Released read lock succussfully\n");
    #endif
    
    return ret;
}
