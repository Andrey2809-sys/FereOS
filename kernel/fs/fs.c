#include "fs.h"

bool init_fs(struct superblock *_Sb)
{
    strcpy(_Sb->signature, "NanoFS");
    _Sb->version   = 0x01;
    _Sb->first_data_sector = FIRST_DATA_SECTOR;
    _Sb->max_files = MAX_FILES;

    return write_sectors(SUPERBLOCK_SECTOR, 1, (uint16_t*)_Sb);
}


bool read_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer)
{
    uint8_t error = 0;

    uint16_t segment, offset;
    
    ptr_to_segoff(_Buffer, &segment, &offset);

    asm volatile (
        "cli\n\t"
        "push %%es\n\t"
        "movw %[seg], %%es\n\t"
        "int $0x13\n\t"
        "pop %%es\n\t"
        "sti"
        : "=@ccc"(error)
        : "a" ((0x02 << 8) | _Total_sectors),
          "c" ((0 << 8) | _Start_sector),
          "d" ((0 << 8) | 0x80),
          "b" (offset),
          [seg] "r" (segment)
        : "cc", "memory"
    );
    
    return (error == 0 ? true : false);
}


bool write_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer)
{
    uint8_t error = 0;

    uint16_t segment, offset;
    
    ptr_to_segoff(_Buffer, &segment, &offset);

    asm volatile (
        "cli\n\t"
        "push %%es\n\t"
        "movw %[seg], %%es\n\t"
        "int $0x13\n\t"
        "pop %%es\n\t"
        "sti"
        : "=@ccc"(error) 
        : "a"((0x03 << 8) | _Total_sectors),  // AH=0x03, AL=total_sectors
          "c"((0 << 8) | _Start_sector),      // CH=0, CL=start_sector
          "d"((0 << 8) | 0x80),               // DH=0, DL=0x80
          "b"(offset),
          [seg] "r" (segment)
        : "cc", "memory"
    );

    return (error == 0 ? true : false);
}


bool check_fs(void)
{
    uint16_t sb_buffer[256];

    read_sectors(SUPERBLOCK_SECTOR, 1, sb_buffer);
    struct superblock *sb = (struct superblock*) sb_buffer;

    return strcmp(sb->signature, "NanoFS");
}

int create_file(const char *_Name, uint8_t _Attr)
{
    int file_count = 0;
    uint16_t first_sector;
    uint8_t bitmap[512];
    uint16_t root_dir_buffer[256];
    
    if (!(read_sectors(BITMAP_SECTOR, 1, bitmap) && read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer))) {
        return READ_SECTORS_ERROR;
    }
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;
    

    for (int pos = 0; pos < 512; pos++) {
        if (bitmap[pos] == 0) {
            first_sector = pos;
            bitmap[pos] = 1;
            break;
        }
    }
    
    for (int i = 0; i < MAX_FILES; i++) {
        if (root_dir[i].attribute != 0) {
            file_count++;
        }
        if (strcmp(root_dir[i].name, _Name)) {
            return FILE_ALREADY_EXIST;
        }
    }
    
    struct nano_fnode file = {
        .attribute = _Attr,
        .file_size = 0,
        .first_sector = first_sector,
        .last_sector = first_sector 
    };
    strcpy(&file.name, _Name);
    
    root_dir[file_count+1] = file;
    
    
    if (!(write_sectors(BITMAP_SECTOR, 1, bitmap) && write_sectors(ROOT_DIR_SECTOR, 1, (uint16_t*) root_dir))) {
        return WRITING_SECTORS_ERROR;
    }
    
    return SUCCESS;
}

int write_file(const char *_Name, char *_Data)
{
    uint16_t root_dir_buffer[256];
    int file_index = -1;
    
    if (!read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer)) {
        return READ_SECTORS_ERROR;
    }
    
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;
    struct nano_fnode file = {0};

    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(root_dir[i].name, _Name)) {
            file = root_dir[i];
            file_index = i;
        }
    }
    if (file.attribute == READ) {
        return READONLY_FILE;
    }

    if (file.attribute == 0) {
        return FILE_NOT_EXIST;
    }

    if (strlen(_Data) < 512) {
        char temp[512];
        strcpy(temp, _Data);
        file.file_size = strlen(_Data);
        root_dir[file_index] = file;
        if (!(write_sectors(ROOT_DIR_SECTOR, 1, (uint16_t*) root_dir) && write_sectors(file.first_sector, 1, (uint16_t*)temp))) {
            return WRITING_SECTORS_ERROR;
        }
    }
    else {

    }
    
    return SUCCESS;
}

int read_file(const char *_Name, char *_Buffer)
{
    uint16_t root_dir_buffer[256];
    
    if (!read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer)) {
        return READ_SECTORS_ERROR;
    }
    
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;
    struct nano_fnode file = {0};

    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(root_dir[i].name, _Name)) {
            file = root_dir[i];
        }
    }

    if (file.attribute == 0) {
        return FILE_NOT_EXIST;
    }

    if (file.file_size < 512) {
        uint16_t buf[256] = {0};
        if(!read_sectors(file.first_sector, 1, buf)) {
            return READ_SECTORS_ERROR;
        }
        strcpy(_Buffer, (char*)buf);
    }

    return SUCCESS;
}

int remove_file(const char *_Name)
{
    uint8_t file_pos = -1;
    uint16_t first_sector;
    uint8_t bitmap[512];
    uint16_t root_dir_buffer[256];
    
    if (!(read_sectors(BITMAP_SECTOR, 1, bitmap) && read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer))) {
        return READ_SECTORS_ERROR;
    }
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;
    struct nano_fnode file = {0};

    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(root_dir[i].name, _Name)) {
            file = root_dir[i];
            file_pos = i;
        }
    }
    
    if (file.attribute == 0) {
        return FILE_NOT_EXIST;
    }

    if (file.file_size < 512) {
        strcpy(file.name, 0);
        file.file_size = 0;
        file.attribute = 0x00;
        
        bitmap[file.first_sector] = 0;

        uint16_t buf[256] = {0};
        write_sectors(file.first_sector, 1, buf);

        file.first_sector = 0;
        file.last_sector = 0;
    }
    
    root_dir[file_pos] = file;
    
    
    if (!(write_sectors(BITMAP_SECTOR, 1, bitmap) && write_sectors(ROOT_DIR_SECTOR, 1, (uint16_t*) root_dir))) {
        return WRITING_SECTORS_ERROR;
    }
    
    return SUCCESS;
}



void show_files(void)
{
    uint16_t root_dir_buffer[256];
        
    read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer);
    
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;
        
    for (int i = 0; i < MAX_FILES; i++) {
        if (root_dir[i].attribute == 0) {
            continue;
        }
        kprint(root_dir[i].name, false);
        kprint(" ", false);
    }
}

void free_place(int *x)
{
    int free_place = 0;
    uint8_t bitmap[512];

    read_sectors(BITMAP_SECTOR, 1, bitmap);

    for (int i = 0; i < 512; i++) {
        if (bitmap[i] == 0) {
            free_place++;
        }
    }

    *x = free_place;
}

struct nano_fnode find_file(const char *_Name)
{
    uint16_t root_dir_buffer[256];
    
    if (!read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer)) {
        return;
    }
    
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;
    struct nano_fnode file = {0};

    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(root_dir[i].name, _Name)) {
            file = root_dir[i];
        }
    }
    return file;
}



void strcpy(char *_Dest, const char *_Src) {
    while ((*_Dest++ = *_Src++)) ;
}