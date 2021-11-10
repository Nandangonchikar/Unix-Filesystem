/* The team members and their contibutions are as follows
Chin-An Chang - initfs and print_block functions
Nandan Gonchikar - init_root and openfs functions
Sayantan Kundu - main and add_block_to_free_list functions
for further information please refer to Readme.txt*/

#include "mod-v6.h"

using namespace std;

int main () {
    cout << "Welcome to CS 5348 Project 2" << endl;

    while (1) {
        char user_input[MAX_INPUT_STRING_SIZE];
        char *command = NULL,*p1 = NULL, *p2 = NULL;

        cout << endl << "Please enter command:" << endl;
        cin.getline(user_input, MAX_INPUT_STRING_SIZE);
        command = strtok(user_input, " ");
        if (strcmp(command, "open") == 0){
            p1 = strtok(NULL, " ");
            if (!p1) {
                cout << "Please provide a file name parameter (ex. open file_name)" << endl;
            } else {
                openfs(p1);
            }
        } else if (strcmp(command, "initfs") == 0) {
            p1 = strtok(NULL, " ");
            p2 = strtok(NULL, " ");
            if (!p1 || !p2) {
                cout << "Please provide block number and inode number" << endl;
            } else {
                initfs(atoi(p1), atoi(p2));
            }
        } else if (strcmp(command, "q") == 0) {
            if (fd != -1) {
                int ret = q();
                if (ret != 0) {
                    cout << "Close file failed, error code = " << ret << endl;
                }
            }
            break;
        } else if (strcmp(command, "print") == 0) {
            p1 = strtok(NULL, " ");
            print_block(atoi(p1));
        } else {
            cout << "Unknown command" << endl;
        }
    }

    return 0;
}

int openfs(char* file_name) {
    fd = open(file_name, O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
    if (fd == -1) {
        cout << "Open file failed" << endl;
    } else {
        cout << "Done" << endl;
    }
    return 0;
}

int initfs(int block_num, int inode_num) {
    /* Start initializing super block */
    sb.isize = inode_num;
    sb.fsize = block_num;
    sb.nfree = 1; //Initialization starts from 1 because free[0] should be zero to indicate that there is no free blocks

    for (int i = 0; i < FREE_SIZE; i++) {
        sb.free[i] = 0;
    }

    init_root();

    if(fd == -1) {
        cout << "Please open a file first" << endl;
        return -1;
    }

    for (int i = 0; i < BLOCK_SIZE / sizeof(unsigned int); i++) {
        empty_space[i] = 0;
    }

    for (int i = 2 + sb.isize + 1; i < block_num; i++) {    //File system size - boot info - superblock - blocks for inodes - root directory block
        add_block_to_free_list(i);
    }

    lseek(fd, BLOCK_SIZE, SEEK_SET);
    write(fd, &sb, BLOCK_SIZE);

    return 0;
}

void init_root() {
    dir_type root_data;
    int first_data_block = 2 + sb.isize;    //Skip Boot info + Super block + i-lists

    /* Initialize root i-node */
    root_inode.flags = FLAG_INODE_ALLOCATED | FLAG_TYPE_DIRECTORY | FLAG_SIZE_SMALL | FLAG_DEFAULT_PERMIT; //Should use small file size?
    root_inode.nlinks = 0;
    root_inode.uid = 0;
    root_inode.gid = 0;
    root_inode.size0 = 2 * sizeof(dir_type);
    root_inode.size1 = 0;
    root_inode.addr[0] = 2 + sb.isize;
    for (int i = 1; i < INODE_ADDR_SIZE; i++) {
        root_inode.addr[i] = 0;
    }

    lseek(fd, 2 * BLOCK_SIZE, SEEK_SET);    //Root is at first i-node
    write(fd, &root_inode, INODE_SIZE);

    /* Initialize root data block */
    root_data.inode = 1;
    root_data.filename[0] = '.';
    root_data.filename[1] = '\0';

    lseek(fd, first_data_block * BLOCK_SIZE, SEEK_SET);
    write(fd, &root_data, sizeof(dir_type));

    root_data.filename[0] = '.';
    root_data.filename[1] = '.';
    root_data.filename[2] = '\0';

    write(fd, &root_data, sizeof(dir_type));
}

int add_block_to_free_list(int block_number) {
    if (sb.nfree == FREE_SIZE) {
        unsigned int free_block_list[BLOCK_SIZE / sizeof(unsigned int)];
        free_block_list[0] = FREE_SIZE;
        for (int i = 0;i < BLOCK_SIZE / sizeof(unsigned int); i++) {
            if (i < FREE_SIZE) {
                free_block_list[i + 1] = sb.free[i];
            } else {
                free_block_list[i] = 0;
            }
        }
        lseek(fd, block_number * BLOCK_SIZE, SEEK_SET);
        write(fd, &free_block_list, BLOCK_SIZE);
        sb.nfree = 0;
    } else {
        lseek(fd, block_number * BLOCK_SIZE, SEEK_SET);
        write(fd, empty_space, BLOCK_SIZE);
        sb.free[sb.nfree++] = block_number;
    }

    return 0;
}

int q() {
    return close(fd);
}

void print_block (int block_number) {
    unsigned char block_data[BLOCK_SIZE];
    lseek(fd, block_number * BLOCK_SIZE, SEEK_SET);
    read(fd, &block_data, BLOCK_SIZE);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (i % 8 == 0)
            cout << endl;
        printf("%02X ", block_data[i]);
    }
}

