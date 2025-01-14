#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define MAX_DISK_NAME 20

typedef struct {
    unsigned int disk_size;             /* 4 bytes */
    unsigned int block_size;            /* 4 bytes */
    unsigned int inode_bitmap_offset;   /* 4 bytes */
    unsigned int first_inode_offset;    /* 4 bytes */
    unsigned int block_bitmap_offset;   /* 4 bytes */
    unsigned int first_block_offset;    /* 4 bytes */
} Superblock;                           /* Sum: 24 */

typedef struct {
    unsigned char *bitmap;
} Inode_bitmap;

typedef struct {
    char file_name[64];                 /* 64 bytes */
    unsigned int file_name_offset;      /* 4 bytes */
    unsigned int hidden_flag;           /* 4 bytes */
    unsigned int file_size;             /* 4 bytes */
    unsigned int first_db_offset;       /* 4 bytes */
} Inode;                                /* Sum: 80 */

typedef struct {
    unsigned char *bitmap;
} Block_bitmap;

typedef struct {
    unsigned int *data;
    unsigned int next_db_offset;
} Datablock;   

typedef struct {
    char *diskfile_name;
    unsigned int block_nb;       
    
    Superblock superblock;
    Inode_bitmap used_inodes;
    Inode *inodes;
    Block_bitmap used_blocks;
    Datablock *datablocks;
    
} Diskfile;

/*
void help() {};
void delete_diskfile(char *name) {}
*/

void create_diskfile(char *name, unsigned int size, unsigned int block_size) {
    Diskfile *diskfile = (Diskfile *)malloc(sizeof(Diskfile));

    strcpy(diskfile->diskfile_name, name);
    diskfile-> block_nb = (size - 28) / (84.25 + block_size);

    diskfile->superblock = create_superblock();





};



int main(int argc, char *argv[]) {
    // FILE *file = fopen("disk.bin", "wb+");
    // Superblock superblock = {1024, 64, 256, 512, 768, 1024};
    // write_superblock(file, &superblock);
    return 0;
}