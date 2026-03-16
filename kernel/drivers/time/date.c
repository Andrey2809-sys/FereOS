#include "./time.h"

uint8_t read_month(void)
{
    outb(TIME_PORT, 0x08);
    return inb(0x71);
}

uint8_t read_day(void)
{
    outb(TIME_PORT, 0x07);
    return inb(0x71);
}

uint16_t read_year(void)
{
    outb(TIME_PORT, 0x32);
    uint8_t century = inb(0x71);
    outb(TIME_PORT, 0x09);
    uint8_t year = inb(0x71);
    return ((century * 100) + year);
}

date_t read_date(void)
{
    return (date_t) {
        .year = convert_bcd(read_year()),
        .month = convert_bcd(read_month()),
        .day = convert_bcd(read_day())
    };
}
