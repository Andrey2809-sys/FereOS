#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include "../video/vid.h"
#include "../ports/port.h"

#define TIME_PORT 0x70


typedef struct {
    uint16_t year;
    uint8_t  month;
    uint8_t  day; 
} date_t;

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} time_t;

uint8_t read_month(void);
uint8_t read_day(void);
uint16_t read_year(void);
date_t  read_date(void);

uint8_t read_seconds(void);
uint8_t read_minutes(void);
uint8_t read_hours(void);
time_t  read_time();

void show_time(void);
void print_time(time_t time);
void print_date(date_t date);

uint16_t convert_bcd(uint16_t bcd);


#endif