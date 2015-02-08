#ifndef LIB_IO_H
#define LIB_IO_H

#define O_RDWR 0


static inline int open(const char* path, int opts){
	return -1;
}


static inline int ioctl(int fd, int base, int address){
	return -1;
}


#endif
