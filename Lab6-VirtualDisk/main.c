#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

struct Superblock {
    unsigned int disk_size;             /* 4 bytes */
    unsigned int block_size;            /* 4 bytes */
    unsigned int block_nb;              /* 4 bytes */
    unsigned int inode_bitmap_offset;   /* 4 bytes */
    unsigned int first_inode_offset;    /* 4 bytes */
    unsigned int block_bitmap_offset;   /* 4 bytes */
    unsigned int first_block_offset;    /* 4 bytes */
};

struct Inode_bitmap {
    unsigned char *bitmap;      /* x bits at least*/
};

struct Inode {
    char file_name[64];         /* 64 bytes */
    unsigned int file_name_ptr; /* 4 bytes */
    unsigned int hidden_flag;   /* 4 bytes */
    unsigned int file_size;     /* 4 bytes */
    unsigned int first_block;   /* 4 bytes */
};

struct Block_bitmap {
    unsigned char *bitmap;      /* x bits at least*/
};

struct Data_block {
    unsigned int data_ptr;      /* x bytes */
    unsigned int next_block;    /* 4 bytes */
};


void write_superblock(FILE *file, struct Superblock *superblock) {
    fseek(file, 0, SEEK_SET);
    fwrite(superblock, sizeof(struct Superblock), 1, file);
}

int main(int argc, char *argv[]) {
    FILE *file = fopen("disk.bin", "wb+");

    struct Superblock superblock = {1024, 64, 256, 512, 768, 1024};
    write_superblock(file, &superblock);

    return 0;
}