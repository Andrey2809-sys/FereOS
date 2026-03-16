#include "./fs.h"

bool init_fs(struct superblock *_Sb)
{
    strcpy(_Sb->signature, "NanoFS");
    _Sb->version   = SB_VERSION;
    _Sb->first_data_sector = FIRST_DATA_SECTOR;
    _Sb->max_files = MAX_FILES;
    _Sb->creation_date = read_date();
    _Sb->creation_time = read_time();

    return write_sectors(SUPERBLOCK_SECTOR, 1, (uint16_t*)_Sb);
}

bool check_fs(void)
{
    uint16_t sb_buffer[256];

    read_sectors(SUPERBLOCK_SECTOR, 1, sb_buffer);
    struct superblock *sb = (struct superblock*) sb_buffer;

    return (strcmp(sb->signature, "NanoFS") && 
            (sb->version == SB_VERSION) &&
            (sb->first_data_sector == FIRST_DATA_SECTOR) &&
            (sb->max_files == MAX_FILES)
    );
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

void strcpy(char *_Dest, const char *_Src) {
    while ((*_Dest++ = *_Src++)) ;
}