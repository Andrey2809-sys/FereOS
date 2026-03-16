#include "./fs.h"

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
        .last_sector = first_sector,

        .creation_date = read_date(),
        .creation_time = read_time()
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

int write_file_int(const char *_Name, uint8_t _Data[], int n)
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
        file.file_size = n;
        root_dir[file_index] = file;
        if (!(write_sectors(ROOT_DIR_SECTOR, 1, (uint16_t*) root_dir) && write_sectors(file.first_sector, 1, (uint16_t*)_Data))) {
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

int read_file_int(const char *_Name, uint8_t _Buffer[], int *n)
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

    *n = file.file_size;

    if (file.file_size < 512) {
        uint8_t buf[512] = {0};
        if(!read_sectors(file.first_sector, 1, buf)) {
            return READ_SECTORS_ERROR;
        }
        for (int i = 0; i < file.file_size; i++) {
            _Buffer[i] = buf[i];
        }
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
        else if (root_dir[i].attribute == READ) {
            kprint("/readonly/", false);
        }
        else if (root_dir[i].attribute == EXECUTABLE) {
            kprint("/executable/", false);
        }
        else if (root_dir[i].attribute == SYSTEM_FILE) {
            kprint("/sys/", false);
        }
        else {
            kprint("/", false);
        }

        kprint(root_dir[i].name, false);
        kprint("\n", false);
    }
}


int file_exist(const char *_Name)
{
    uint16_t root_dir_buffer[256];
    
    if (!read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer)) {
        return READ_SECTORS_ERROR;
    }
    
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;
    struct nano_fnode file = {0};

    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(root_dir[i].name, _Name)) {
            return SUCCESS;
        }
    }

    
    return FILE_NOT_EXIST;
    
}

struct nano_fnode find_file(const char *_Name)
{
    uint16_t root_dir_buffer[256];
    struct nano_fnode file = {0};
    
    if (!read_sectors(ROOT_DIR_SECTOR, 1, root_dir_buffer)) {
        return file;
    }
    
    struct nano_fnode *root_dir = (struct nano_fnode*) root_dir_buffer;

    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(root_dir[i].name, _Name)) {
            file = root_dir[i];
        }
    }
    return file;
}