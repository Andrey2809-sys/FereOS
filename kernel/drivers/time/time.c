#include "./time.h"
#include "time.h"

uint8_t read_seconds(void)
{
    outb(TIME_PORT, 0x00);
    return inb(0x71);
}

uint8_t read_minutes(void)
{
    outb(TIME_PORT, 0x02);
    return inb(0x71);
}

uint8_t read_hours(void)
{
    outb(TIME_PORT, 0x04);
    return inb(0x71);
}

time_t read_time()
{
    return (time_t) {
        .hours = convert_bcd(read_hours()),
        .minutes = convert_bcd(read_minutes()),
        .seconds = convert_bcd(read_seconds())
    };
}


void show_time(void)
{
    const uint8_t y_pos = 24;
    const uint8_t x_pos = 63;

    uint8_t y_before = *((CURSOR_Y));
    uint8_t x_before = *((CURSOR_X));

    //*((CURSOR_Y))
    setvga(x_pos, y_pos);

    date_t date = read_date();
    print_date(date);
    
    kprint("  ", false);

    time_t time = read_time();
    print_time(time);

    setvga(x_before, y_before);
}

void print_time(time_t time)
{
    if (time.hours < 10) {
        print_int(0);    
    }
    print_int(time.hours);
    
    kprint(":", false);
    
    if (time.minutes < 10) {
        print_int(0);
    }
    print_int(time.minutes);
}

void print_date(date_t date)
{
    print_int(date.year);
    kprint(".", false);
    if (date.month < 10) {
        print_int(0);
    };
    print_int(date.month);
    
    kprint(".", false);
    if (date.day < 10) {
        print_int(0);
    }
    print_int(date.day);
}


uint16_t convert_bcd(uint16_t bcd)
{
    return (((bcd >> 4) * 10) + (bcd & 0xF));
}
