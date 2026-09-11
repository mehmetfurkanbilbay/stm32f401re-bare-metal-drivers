#ifndef RTC_H
#define RTC_H

#include <stdint.h>

/*
 * RTC Register Map
 *
 * RTC Base Address: 0x40002800
 *
 * +0x00 -> TR    (Time Register)
 * +0x04 -> DR    (Date Register)
 * +0x08 -> CR    (Control Register)
 * +0x0C -> ISR   (Initialization and Status Register)
 * +0x10 -> PRER  (Prescaler Register)
 * +0x24 -> WPR   (Write Protection Register)
 */
typedef struct {
    volatile uint32_t TR;
    volatile uint32_t DR;
    volatile uint32_t CR;
    volatile uint32_t ISR;
    volatile uint32_t PRER;
    uint32_t RESERVED0[3];  // WUTR, CALIBR, ALRMAR - not used here (0x14-0x1C)
    volatile uint32_t WPR;
} RTC_TypeDef;

#define RTC_BASE 0x40002800UL
#define RTC ((RTC_TypeDef *)RTC_BASE)

typedef struct {
    uint8_t hours;    // 0-23
    uint8_t minutes;  // 0-59
    uint8_t seconds;  // 0-59
} RTC_Time_t;

typedef struct {
    uint8_t day;    // 1-31
    uint8_t month;  // 1-12
    uint8_t year;   // 0-99 (offset from 2000)
} RTC_Date_t;

/* ==================== BASIC RTC FUNCTIONS ==================== */
void RTC_Init(void);
void RTC_SetTime(RTC_Time_t *time);
void RTC_GetTime(RTC_Time_t *time);
void RTC_SetDate(RTC_Date_t *date);
void RTC_GetDate(RTC_Date_t *date);

#endif
