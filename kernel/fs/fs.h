#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>
#include "../drivers/video/vid.h"
#include "../shell/sh.h"
#include "../memory/mem.h"

#define FS_INFO "NanoFS v0.95"


#define KERNEL_END_SECTOR 35
#define MAX_FILES 16
#define SUPERBLOCK_SECTOR (KERNEL_END_SECTOR+1)
#define BITMAP_SECTOR     (KERNEL_END_SECTOR+2)
#define ROOT_DIR_SECTOR   (KERNEL_END_SECTOR+3)
#define FIRST_DATA_SECTOR (KERNEL_END_SECTOR+4)

#define MINIMAL_FILE_SIZE 1

// attributes
#define READ        0x01
#define WRITE       0x02
#define EXECUTABLE  0x10
#define SYSTEM_FILE 0xAA
#define DELETE      0x80

// file errors
#define SUCCESS 1
#define FILE_ALREADY_EXIST -1
#define FILE_NOT_EXIST -2
#define READONLY_FILE -3
#define PERMISSION_ERROR -4
#define WRITING_SECTORS_ERROR -10
#define READ_SECTORS_ERROR -20

struct superblock
{
    char      signature[7];         // NanoFS
    uint8_t   version;              
    uint16_t  total_data_sectors;
    uint16_t  first_data_sector;
    uint16_t  max_files;
} __attribute__((packed));


struct nano_fnode
{
    char      name[13];      // FILENAME.EXT 
    uint8_t   attribute;
    uint16_t  file_size;
    uint16_t  first_sector;
    uint16_t  last_sector;
} __attribute__((packed));

bool              init_fs(struct superblock *_Sb);
bool              read_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer);
bool              write_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer);

void              test_fs(void);
bool              check_fs(void);

// work with files
int               create_file(const char* _Name, uint8_t _Attr);
int               write_file(const char *_Name, char *_Data);
int               read_file(const char *_Name, char *_Buffer);
int               remove_file(const char *_Name);
void              show_files(void);
void              free_place(int *x);
struct nano_fnode find_file(const char *_Name);



void strcpy(char *_Dest, const char *_Src);

#endif