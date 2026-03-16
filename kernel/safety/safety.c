#include "./safety.h"

char *set_password()
{
    key_obj key;
    char *password = kmalloc(MAX_PASSWORD_SIZE);

    kprint("[setting a password] enter a password: ", true);
    
    for (int pos = 0; pos < MAX_PASSWORD_SIZE; pos++) {
        read_key(&key);  
        if (key.scancode == ENTER) {
            password[pos] = 0;
            break;
        }
        password[pos] = key.ascii;
    } 

    kprint("\nPassword \"", false);
    kprint(password, true);
    kprint("\" sucessfully installed", false);
    kprint("\n", false);

    return password;
}

bool check_password()
{
    key_obj key;
    uint16_t superblock_buffer[256];    
    char *_Str = kmalloc(MAX_PASSWORD_SIZE);

    kprint("Password: ", false);
    for (int pos = 0; pos < MAX_PASSWORD_SIZE; pos++) {
        read_key(&key);  
        if (key.scancode == ENTER) {
            _Str[pos] = 0;
            break;
        }
        _Str[pos] = key.ascii;
    }

    // read password from superblock 
    if (!read_sectors(SUPERBLOCK_SECTOR, 1, superblock_buffer)) {
        kprint("[ERROR] while reading sectors\n", true);
        return false;
    }
    struct superblock *sb = (struct superblock*) superblock_buffer;
    char *password = sb->password;

    kprint("\n", false);
    if (strcmp(password, _Str)) {
        return true;
    }
    else {
        return false;
    }
}
