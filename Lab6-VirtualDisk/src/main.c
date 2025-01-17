#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define SUPERBLOCK_SIZE 20
#define INODE_SIZE      80
#define BLOCK_SIZE      1024
#define DATA_SIZE       1020

/* <================================================> */
/*                      Objects                       */
/* <================================================> */

typedef struct {
    unsigned int disk_size;             /* 4 bytes */
    unsigned int inode_bitmap_offset;   /* 4 bytes */
    unsigned int block_bitmap_offset;   /* 4 bytes */
    unsigned int first_inode_offset;    /* 4 bytes */
    unsigned int first_block_offset;    /* 4 bytes */
} Superblock;                           /* Sum: 20 */

typedef struct {
    char file_name[64];                 /* 64 bytes */
    unsigned int file_name_offset;      /* 4 bytes */
    unsigned int hidden_flag;           /* 4 bytes */
    unsigned int file_size;             /* 4 bytes */
    unsigned int first_db_offset;       /* 4 bytes */
} Inode;                                /* Sum: 80 */

typedef struct {
    unsigned char data[DATA_SIZE];      /* X bytes */
    unsigned int next_offset;           /* 4 bytes */
} Block;                                /* Sum: ?  */

typedef struct {
    unsigned int block_nb;       
    Superblock superblock;
    Inode *inodes;
    Block *blocks;
    unsigned char *inode_bitmap;
    unsigned char *block_bitmap;
} Diskfile;

/* <===========================================================================> */
/*                              Constructors                                     */
/* <===========================================================================> */

Superblock init_superblock(unsigned int size, unsigned int block_nb ) {
    Superblock sb;
    sb.disk_size = size;
    sb.inode_bitmap_offset = 20;
    sb.block_bitmap_offset = sb.inode_bitmap_offset + block_nb;
    sb.first_inode_offset = sb.block_bitmap_offset + block_nb;
    sb.first_block_offset = sb.first_inode_offset + 80*block_nb;
    return sb;
}

unsigned char* init_bytemap(unsigned int length) {
    unsigned int i;
    unsigned char* bytemap = (unsigned char*)malloc(length * sizeof(unsigned char));
    for (i = 0; i < length; ++i) {
        bytemap[i] = 0;
    }
    return bytemap;
}

Inode* init_inodes(unsigned int inode_nb) {
    unsigned int i;
    Inode* inodes = (Inode*)malloc(inode_nb * sizeof(Inode));
    for (i = 0; i < inode_nb; ++i) {
        strcpy(inodes[i].file_name, "");
        inodes[i].file_name_offset = 0;
        inodes[i].hidden_flag = 0;
        inodes[i].file_size = 0;
        inodes[i].first_db_offset = 0;
    }
    return inodes;
}

Block* init_blocks(unsigned int block_nb) {
    unsigned int i;
    Block* blocks = (Block*)malloc(block_nb * sizeof(Block));
    for (i = 0; i < block_nb; ++i) {
        memset(blocks[i].data, 0, DATA_SIZE);
        blocks[i].next_offset = 0;
    }
    return blocks;
}

Diskfile* init_diskfile(unsigned int size) {
    unsigned int block_nb = (size - 20) / (86 + DATA_SIZE);

    Diskfile *diskfile = (Diskfile *)malloc(sizeof(Diskfile));
    diskfile->block_nb = block_nb;
    diskfile->superblock = init_superblock(size, block_nb);
    diskfile->inode_bitmap = init_bytemap(block_nb);
    diskfile->block_bitmap = init_bytemap(block_nb);
    diskfile->inodes = init_inodes(block_nb);
    diskfile->blocks = init_blocks(block_nb);

    return diskfile;
}

void cleanup(Diskfile* diskfile) {
    free(diskfile->inode_bitmap);
    free(diskfile->block_bitmap);
    free(diskfile->inodes);
    free(diskfile->blocks);
    free(diskfile);
}

/* <===========================================================================> */
/*                          Read&save operations                                 */
/* <===========================================================================> */

void save_to_file(Diskfile* diskfile, char* path) {
    FILE *file = fopen(path, "wb");
    fwrite(&diskfile->superblock, sizeof(Superblock), 1, file);
    fwrite(diskfile->inode_bitmap, sizeof(unsigned char), diskfile->block_nb, file);
    fwrite(diskfile->block_bitmap, sizeof(unsigned char), diskfile->block_nb, file);
    fwrite(diskfile->inodes, sizeof(Inode), diskfile->block_nb, file);
    fwrite(diskfile->blocks, sizeof(Block), diskfile->block_nb, file);
    fclose(file);
}

void read_from_file(Diskfile* diskfile, char* path) {
    FILE* file = fopen(path, "rb");
    fread(&diskfile->superblock, sizeof(Superblock), 1, file);
    fread(diskfile->inode_bitmap, sizeof(unsigned char), diskfile->block_nb, file);
    fread(diskfile->block_bitmap, sizeof(unsigned char), diskfile->block_nb, file);
    fread(diskfile->inodes, sizeof(Inode), diskfile->block_nb, file);
    fread(diskfile->blocks, sizeof(Block), diskfile->block_nb, file);
    fclose(file);
}

unsigned int read_diskfile_size(char *path) {
    Superblock superblock;
    FILE *file = fopen(path, "rb");
    fread(&superblock, sizeof(Superblock), 1, file);
    fclose(file);
    return superblock.disk_size;
}

/* <===========================================================================> */
/*                            Other                                              */
/* <===========================================================================> */

unsigned int* free_indeces(Diskfile* diskfile, unsigned char* bitmap) {
    unsigned int i, j;
    unsigned int free_count = 0;
    unsigned int* free_indeces = (unsigned int*)malloc(diskfile->block_nb * sizeof(unsigned int));
    /* unsigned int* free_indeces = (unsigned int*)malloc(total_indeces * sizeof(unsigned int)); */

    if (free_indeces == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    for (i=0; i<diskfile->block_nb; i++) {
        if (bitmap[i] == 0) {
            free_indeces[free_count++] = i;
        }
    }

    return free_indeces;
}


unsigned int index_to_offset(Diskfile* diskfile, unsigned int index) {
    Superblock sb = diskfile->superblock;
    unsigned int offset = sb.first_block_offset + BLOCK_SIZE*index;
    return offset;
}

unsigned int offset_to_index(Diskfile* diskfile, unsigned int offset) {
    Superblock sb = diskfile->superblock;
    unsigned int index = (offset - sb.first_block_offset) / BLOCK_SIZE;
    return index;
}

/* <===========================================================================> */
/*                            Adding&removing files                              */
/* <===========================================================================> */

void add_file(Diskfile* diskfile, char *path) {
    int i=0;
    size_t bytes_to_read;

    unsigned int* free_inodes_index = free_indeces(diskfile, diskfile->inode_bitmap);
    unsigned int first_inode_index = free_inodes_index[0];
    free(free_inodes_index);
    int* free_blocks_index = (int*)free_indeces(diskfile, diskfile->block_bitmap);

    FILE *file;
    unsigned int file_offset;
    unsigned int file_in_bytes;
    unsigned int file_in_blocks;

    char *filename;
    unsigned int filename_offset;
    unsigned int filename_in_bytes;
    unsigned int filename_in_blocks;

    Inode *curr_inode;
    Block *curr_block;
    unsigned int curr_block_index;
    unsigned int next_block_index;

    size_t bytes_read;

    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    file_in_bytes = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);
    file_in_blocks = (file_in_bytes + DATA_SIZE - 1) / DATA_SIZE;

    /* Get filename size*/
    filename = path;
    filename_in_bytes = (unsigned int)strlen(filename);
    if (filename_in_bytes <= 64) {
        filename_in_blocks = 0;
    } else {
        filename_in_blocks = ((file_in_bytes - 64) + DATA_SIZE - 1) / DATA_SIZE;
    }

    /* Check if file fits */
    free_blocks_index[filename_in_blocks + file_in_blocks+1] = (unsigned int)-1;
    while(free_blocks_index[i] != (unsigned int)-1) {
        i++; 
    }
    if (i < file_in_blocks + file_in_blocks) {
        printf("Not enough space to store the file.\n");
        free(free_blocks_index);
        return;
    }

    /* <============== Map the bytemap ==============> */

    diskfile->inode_bitmap[first_inode_index] = 1;
    for (i=0; i<file_in_blocks+filename_in_blocks; i++) {
        diskfile->block_bitmap[free_blocks_index[i]] = 1;
    }

    /* <================================== Save the inode ==================================> */

    curr_inode = &diskfile->inodes[first_inode_index];
    strncpy((char *)curr_inode->file_name, filename, 64);
    curr_inode->file_name_offset = 0;
    curr_inode->hidden_flag = 0;
    curr_inode->file_size = file_in_bytes;
    curr_inode->first_db_offset = index_to_offset(diskfile, free_blocks_index[filename_in_blocks]);
    
    /* Set hidden flag if needed */
    if (filename[0] == '.') {
        curr_inode->hidden_flag = 1;
    }

    /* Save filename if over 64 bytes */
    curr_block_index = free_blocks_index[0];
    for (i=0; i<filename_in_blocks; i++) 
    {   

        if (filename_offset + DATA_SIZE > filename_in_bytes) {
            bytes_to_read = filename_in_bytes - filename_offset;
        } else {
            bytes_to_read = DATA_SIZE;
        }

        curr_block = &diskfile->blocks[curr_block_index];   /* Get current block*/
        if (filename_offset >= strlen(filename)) {
            printf("filename_offset out of bounds\n");
            return;
        }
        memcpy(curr_block->data, &filename[filename_offset], bytes_to_read);

        /* Set the proper pointers*/
        next_block_index = free_blocks_index[i+1];
        if (i == filename_in_blocks - 1) {
            curr_block->next_offset = 0;                                        /* NULL */
        } else {
            curr_block->next_offset = index_to_offset(diskfile, next_block_index); /* Next block */
        }
        /* Next block */
        curr_block_index = next_block_index;
    }
    
    /* <============================= Save actual data from file ============================> */

    file = fopen(path, "rb");
    curr_block_index = free_blocks_index[filename_in_blocks];

    for (i=filename_in_blocks; i<filename_in_blocks + file_in_blocks; i++) 
    {   
        if (file_offset + DATA_SIZE > file_in_bytes) {
            bytes_to_read = file_in_bytes - file_offset;    /* Last, not full block */
        } else {
            bytes_to_read = DATA_SIZE;                      /* Full block           */
        }

        curr_block = &diskfile->blocks[curr_block_index];                                           /* Get current block */
        bytes_read = fread(curr_block->data, sizeof(unsigned char), bytes_to_read, file);    /* Read&save data    */
        
        /* Set the proper pointers*/
        next_block_index = free_blocks_index[i+1];
        if (i == filename_in_blocks + file_in_blocks - 1) {
            curr_block->next_offset = 0;                                        /* NULL */
        } else {
            curr_block->next_offset = index_to_offset(diskfile, next_block_index); /* Link next */
        }
        curr_block_index = next_block_index;                                    /* Next block */
    }
    fclose(file);

    free(free_blocks_index);
    return;
}

void remove_file(Diskfile* diskfile, unsigned char *filename) {
    unsigned int i = (unsigned int)-1;
    unsigned int inode_index;
    unsigned int block_index;
    unsigned int block_offset;
    Inode* curr_inode;
    Block* curr_block;

    /* Search for the file */
    for (i = 0; i < diskfile->block_nb; i++) {
        curr_inode = &diskfile->inodes[i];
        if (strncmp((char *)curr_inode->file_name, (char *)filename, 64) == 0) {
            inode_index = i;
            break;
        }
    }
    /* File not found*/
    if (inode_index == (unsigned int)-1) {
        printf("File with such name not found.\n");
        return;
    }

    /* Unmap inode on bitmap */
    diskfile->inode_bitmap[inode_index] = 0;

    /* Unmap blocks on bitmap */
    block_offset = diskfile->inodes[inode_index].first_db_offset;
    block_index = offset_to_index(diskfile, block_offset);
    while (1==1) {
        diskfile->block_bitmap[block_index] = 0;

        block_offset = diskfile->blocks[block_index].next_offset;
        if (block_offset == 0) {
            return;  /* Reached NULL - last block*/
        }
        block_index = offset_to_index(diskfile, block_offset);
    }
}

Inode* find_inode(Diskfile* diskfile, unsigned char* filename) {
    unsigned int i = (unsigned int)-1;
    unsigned int inode_index;
    Inode* curr_inode;

    /* Search for the file */
    for (i = 0; i < diskfile->block_nb; i++) {
        curr_inode = &diskfile->inodes[i];
        if (strncmp((char *)curr_inode->file_name, (char *)filename, 64) == 0) {
            inode_index = i;
            break;
        }
    }
    /* File not found*/
    if (inode_index == (unsigned int)-1) {
        printf("File with such name not found.\n");
        return NULL;
    }
    /* File found */
    return &diskfile->inodes[inode_index];
}

void extract_file(Diskfile* diskfile, unsigned char* filename) {
    Inode* file_inode = find_inode(diskfile, filename);
    unsigned char *file_data = (unsigned char*)malloc(file_inode->file_size);
    
    FILE* output_file;
    Block* curr_block;
    unsigned int block_offset;
    unsigned int block_index;

    unsigned int bytes_to_copy;
    unsigned int file_offset = 0;

    block_offset = file_inode->first_db_offset;
    if (block_offset != 0) {
        while (1==1) {
            block_index = offset_to_index(diskfile, block_offset);
            curr_block = &diskfile->blocks[block_index];
            bytes_to_copy = (file_inode->file_size - file_offset) < DATA_SIZE ? (file_inode->file_size - file_offset) : DATA_SIZE;
            
            memcpy(file_data + file_offset, curr_block->data, bytes_to_copy);
            file_offset += bytes_to_copy;

            if (file_offset >= file_inode->file_size) {
                break;
            }
            block_offset = curr_block->next_offset;
            if (block_offset == 0) {
                break;
            }
        }
    }
    output_file = fopen(filename, "wb");
    fwrite(file_data, 1, file_inode->file_size, output_file);
    fclose(output_file);
}


/* <===========================================================================> */
/*                            Visualisasions                                     */
/* <===========================================================================> */

void list_files(Diskfile* diskfile, unsigned int list_hidden) {
    unsigned int i;

    unsigned char* inode_bytemap = diskfile->inode_bitmap;
    
    Inode inode;
    unsigned int address;
    unsigned int file_size;
    char file_name[64];

    printf("Stored files:\n");
    for (i=0; i< diskfile->block_nb; i++) {
        if (inode_bytemap[i] == 1) {
            inode = diskfile->inodes[i];
            if (inode.hidden_flag == 0 || list_hidden == 1) {
                printf("%u. File\n", i);
                printf("   Name: %s\n",  inode.file_name);
                printf("   Size: %u bytes\n",  inode.file_size);
                printf("   Address: %u'th byte\n",  inode.first_db_offset);
            }
        }
    }
}

void show_inode_bitmap(Diskfile* diskfile) {
    unsigned int i;
    unsigned int j;
    unsigned int block_nb = diskfile->block_nb;
    unsigned int col_max = 10;
    unsigned int row_max = block_nb / col_max + (block_nb % col_max != 0);
    char char_to_print;

    /* Comment */
    printf("Inode bitmap:\n");
    /* Spacer */
    for (i = 0; i < col_max + 1; ++i) {
        printf("---+");
    }
    printf("\n");
    /* Top legend */
    printf("   |");
    for (i = 0; i < col_max; ++i) {
        printf(" %u |", i);
    }
    printf("\n");
     /* Spacer */
    for (i = 0; i < col_max + 1; ++i) {
        printf("---+");
    }
    printf("\n");

    for (i = 0; i < row_max; ++i) {
        /* Left legend */
        if (i==0) {
            printf(" 0 |");
        } else {
            printf("%u |", i * 10);
        }
        /* Block bits */
        for (j = 0; j < col_max && i * col_max + j < block_nb; ++j) {
            printf(" %u |", (unsigned int) diskfile->inode_bitmap[i * col_max + j]);
        }
        printf("\n");
        /* Spacer */
        for (j = 0; j < col_max + 1; ++j) {
            printf("---+");
        }
        printf("\n");
    }
}

void show_block_bitmap(Diskfile* diskfile) {
    unsigned int i;
    unsigned int j;
    unsigned int block_nb = diskfile->block_nb;
    unsigned int col_max = 10;
    unsigned int row_max = block_nb / col_max + (block_nb % col_max != 0);
    char char_to_print;

    /* Comment */
    printf("Block bitmap:\n");
    /* Spacer */
    for (i = 0; i < col_max + 1; ++i) {
        printf("---+");
    }
    printf("\n");
    /* Top legend */
    printf("   |");
    for (i = 0; i < col_max; ++i) {
        printf(" %u |", i);
    }
    printf("\n");
     /* Spacer */
    for (i = 0; i < col_max + 1; ++i) {
        printf("---+");
    }
    printf("\n");

    for (i = 0; i < row_max; ++i) {
        /* Left legend */
        if (i==0) {
            printf(" 0 |");
        } else {
            printf("%u |", i * 10);
        }
        /* Block bits */
        for (j = 0; j < col_max && i * col_max + j < block_nb; ++j) {
            printf(" %u |", (unsigned int) diskfile->block_bitmap[i * col_max + j]);
        }
        printf("\n");
        /* Spacer */
        for (j = 0; j < col_max + 1; ++j) {
            printf("---+");
        }
        printf("\n");
    }
}

void printHelp() {
    printf("Available Commands:\n");
    printf("    -c   <diskname> <disksize>  - Create new disk.\n");
    printf("    -a   <diskname> <filename>  - Add file\n");
    printf("    -r   <diskname> <filename>  - Remove file\n");
    printf("    -e   <diskname> <filename>  - Extract file\n");
    printf("    -d   <diskname>             - Delete a disk\n");
    printf("    -inm <diskname>             - Show bitmap for inodes\n");
    printf("    -dbm <diskname>             - Show bitmap for blocks\n");
    printf("    -ls  <diskname>             - List normal files\n");
    printf("    -lsh <diskname>             - List normal + hidden files\n");
    printf("    -h                          - Print help message\n");
}
/* <================================================> */
/*                   Main program                     */
/* <================================================> */

int main(int argc, char *argv[]) {
    Diskfile* diskfile = NULL;
    char *diskname, *filename;
    unsigned int disksize;
    /*
    diskname = "diskfile.bin";
    filename = "normal_file.txt";

    diskfile = init_diskfile(11100);
    show_inode_bitmap(diskfile);
    save_to_file(diskfile, diskname);

    disksize = read_diskfile_size(diskname);
    diskfile = init_diskfile(disksize);
    read_from_file(diskfile, diskname);
    show_inode_bitmap(diskfile);
    add_file(diskfile, filename);
    list_files(diskfile, 0);
    show_inode_bitmap(diskfile);
    save_to_file(diskfile, diskname);
    return 0;
    */

    if (argc<2) {
        printf("Error: Invalid command.\n");

    } else if (strcmp(argv[1], "-c") == 0) {
        diskname = argv[2];
        disksize = atoi(argv[3]);

        diskfile = init_diskfile(disksize);
        save_to_file(diskfile, diskname);

        printf("Created '%s' disk with %u bytes storage.\n", diskname, disksize);

    } else if (strcmp(argv[1], "-a") == 0) {
        diskname = argv[2];
        filename = argv[3];

        disksize = read_diskfile_size(diskname);
        diskfile = init_diskfile(disksize);
        read_from_file(diskfile, diskname);
        add_file(diskfile, filename);
        save_to_file(diskfile, diskname);

        printf("Added file '%s' to disk '%s'.\n", filename, diskname);
    
    } else if (strcmp(argv[1], "-r") == 0) {
        diskname = argv[2];
        filename = argv[3];

        disksize = read_diskfile_size(diskname);
        diskfile = init_diskfile(disksize);
        read_from_file(diskfile, diskname);
        remove_file(diskfile, filename);
        save_to_file(diskfile, diskname);

        printf("Removed file '%s' from disk '%s'.\n", filename, diskname);

    } else if (strcmp(argv[1], "-e") == 0) {
        diskname = argv[2];
        filename = argv[3];

        disksize = read_diskfile_size(diskname);
        diskfile = init_diskfile(disksize);
        read_from_file(diskfile, diskname);
        extract_file(diskfile, filename);

        printf("Extracted file '%s' from disk '%s'.\n", filename, diskname);

    } else if (strcmp(argv[1], "-d") == 0) {
        diskname = argv[2];
        remove(diskname);

        printf("Removed disk '%s'.\n", diskname);

    } else if (strcmp(argv[1], "-inm") == 0) {
        diskname = argv[2];

        disksize = read_diskfile_size(diskname);
        diskfile = init_diskfile(disksize);
        read_from_file(diskfile, diskname);
        show_inode_bitmap(diskfile);

    } else if (strcmp(argv[1], "-dbm") == 0) {
        diskname = argv[2];

        disksize = read_diskfile_size(diskname);
        diskfile = init_diskfile(disksize);
        read_from_file(diskfile, diskname);
        show_block_bitmap(diskfile);

    } else if (strcmp(argv[1], "-ls") == 0) {
        diskname = argv[2];

        disksize = read_diskfile_size(diskname);
        diskfile = init_diskfile(disksize);
        read_from_file(diskfile, diskname);
        list_files(diskfile, 0);

    } else if (strcmp(argv[1], "-lsh") == 0) {
        diskname = argv[2];

        disksize = read_diskfile_size(diskname);
        diskfile = init_diskfile(disksize);
        read_from_file(diskfile, diskname);
        list_files(diskfile, 1);
    } else {
        printf("Error: Unknown command\n");
        printHelp();
    }
    return 0;
}