#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_INPUT_STRING_SIZE 200
#define INODE_SIZE 64
#define BLOCK_SIZE 1024
#define FREE_SIZE 251
#define INODE_ADDR_SIZE 9

/* i-node flags*/
#define FLAG_INODE_ALLOCATED 0x8000
#define FLAG_TYPE_BLOCK_SPECIAL_FILE 0x6000
#define FLAG_TYPE_DIRECTORY 0x4000
#define FLAG_TYPE_CHAR_SPECIAL_FILE 0x2000
#define FLAG_TYPE_PLAIN_FILE 0x0
#define FLAG_SIZE_SMALL 0x0
#define FLAG_SIZE_MEDIUM 0x800
#define FLAG_SIZE_LONG 0x1000
#define FLAG_SIZE_SUPER_LONG 0x1800
#define FLAG_DEFAULT_PERMIT 0x777 //rwx for world

typedef struct {
    int isize;
    int fsize;
    int nfree;
    unsigned int free[FREE_SIZE];
    int flock;
    int ilock;
    unsigned int fmod;
    unsigned int time;
} superblock_type; // Block size is 1024 Bytes; only 1023 Bytes are used

typedef struct {
    unsigned short flags;
    unsigned short nlinks;
    unsigned int uid;
    unsigned int gid;
    unsigned int size0;
    unsigned int size1;
    unsigned int addr[INODE_ADDR_SIZE];
    unsigned int actime;
    unsigned int modtime;
} inode_type; //64 Bytes in size

typedef struct {
    unsigned int inode;
    unsigned char filename[28];
} dir_type;//32 Bytes long

int fd = -1;
superblock_type sb;
inode_type root_inode;
unsigned int empty_space[BLOCK_SIZE / sizeof(unsigned int)];

int add_block_to_free_list(int);
int openfs(char*);
int initfs(int, int);
int q(void);
void init_root(void);
void print_block(int);
