#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>
#include "../drivers/video/vid.h"
#include "../shell/sh.h"
#include "../memory/mem.h"
#include "../drivers/time/time.h"

#define FS_INFO "NanoFS v1.0"

#define SB_VERSION 0x01

#define KERNEL_END_SECTOR 51
#define MAX_FILES 16
#define SUPERBLOCK_SECTOR (KERNEL_END_SECTOR+1)
#define BITMAP_SECTOR     (KERNEL_END_SECTOR+3)
#define ROOT_DIR_SECTOR   (KERNEL_END_SECTOR+4)
#define FIRST_DATA_SECTOR (KERNEL_END_SECTOR+5)

#define MINIMAL_FILE_SIZE 1
#define MAX_FILE_SIZE     4096

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
    char      password[80];
    uint8_t   version;              
    uint16_t  total_data_sectors;
    uint16_t  first_data_sector;
    uint16_t  max_files;
    
    date_t    creation_date;
    time_t    creation_time;
} __attribute__((packed));


struct nano_fnode
{
    char      name[11];      
    uint8_t   attribute;
    uint16_t  file_size;
    uint16_t  first_sector;
    uint16_t  last_sector;
    
    date_t    creation_date;
    time_t    creation_time;
} __attribute__((packed));

bool              init_fs(struct superblock *_Sb);
bool              read_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer);
bool              write_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer);

void              test_fs(void);
bool              check_fs(void);

// work with files
int               create_file(const char* _Name, uint8_t _Attr);
int               write_file(const char *_Name, char *_Data);
int               write_file_int(const char *_Name, uint8_t _Data[], int n);
int               read_file(const char *_Name, char *_Buffer);
int               read_file_int(const char *_Name, uint8_t _Buffer[], int *n);
int               remove_file(const char *_Name);
void              show_files(void);
void              free_place(int *x);
int               file_exist(const char *_Name);
struct nano_fnode find_file(const char *_Name);



void strcpy(char *_Dest, const char *_Src);

#endif