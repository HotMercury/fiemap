/*
Use fiemap ioctl to get the file extent map
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <linux/fiemap.h>


int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0){
        perror("open");
        exit(1);
    }
    printf("\033[0;32;34m" "File: %s\n" "\033[m", argv[1]);

    char buf[4096];
    struct fiemap* fm = (struct fiemap*)buf;

    fm->fm_start = 0;
    fm->fm_length = FIEMAP_MAX_OFFSET;
    fm->fm_flags = 0;

    if(ioctl(fd, FS_IOC_FIEMAP, fm) < 0){
        perror("ioctl");
        close(fd);
        exit(1);
    }
    for (int i = 0; i < fm->fm_mapped_extents; i++){
        printf("Extent %d: logical %llu, physical %llu, length %llu\n", i, (unsigned long long)fm->fm_extents[i].fe_logical, (unsigned long long)fm->fm_extents[i].fe_physical, (unsigned long long)fm->fm_extents[i].fe_length);
    }

    close(fd);
    return 0;
}