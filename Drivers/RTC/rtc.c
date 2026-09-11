#include "rtc.h"

/* Small helpers: RTC stores each field as two BCD nibbles (e.g. 45 minutes -> 0x45) */
static uint8_t RTC_ToBCD(uint8_t value)
{
    return (uint8_t)(((value / 10) << 4) | (value % 10));
}

static uint8_t RTC_FromBCD(uint8_t bcd)
{
    return (uint8_t)(((bcd >> 4) * 10) + (bcd & 0x0F));
}

static void RTC_Unlock(void)
{
    /* Fixed key sequence defined by ST (RM0368) to disable write protection */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
}

static void RTC_Lock(void)
{
    RTC->WPR = 0xFF; // any value other than the unlock sequence re-locks it
}

static void RTC_EnterInitMode(void)
{
    RTC->ISR |= (1UL << 7); // INIT bit
    while (!(RTC->ISR & (1UL << 6))) {
        // wait for INITF flag: RTC confirms it's ready to accept new time/date
    }
}

static void RTC_ExitInitMode(void)
{
    RTC->ISR &= ~(1UL << 7); // clear INIT bit, RTC resumes normal counting
}

void RTC_Init(void)
{
    RTC_Unlock();
    RTC_EnterInitMode();

    /* Prescaler: RTCCLK (typically LSE = 32.768kHz) needs to be divided down to 1Hz.
     * PRER has two fields: PREDIV_A (bits 22:16, async) and PREDIV_S (bits 14:0, sync).
     * Standard split: async = 127, sync = 255 -> 32768 / (127+1) / (255+1) = 1 Hz */
    RTC->PRER = (127UL << 16) | (255UL);

    RTC_ExitInitMode();
    RTC_Lock();
}

void RTC_SetTime(RTC_Time_t *time)
{
    RTC_Unlock();
    RTC_EnterInitMode();

    uint32_t tr = 0;
    tr |= ((uint32_t)RTC_ToBCD(time->hours))   << 16;
    tr |= ((uint32_t)RTC_ToBCD(time->minutes)) << 8;
    tr |= ((uint32_t)RTC_ToBCD(time->seconds)) << 0;
    RTC->TR = tr;

    RTC_ExitInitMode();
    RTC_Lock();
}

void RTC_GetTime(RTC_Time_t *time)
{
    uint32_t tr = RTC->TR; // reading TR also latches DR (RM0368 requirement, see note below)

    time->hours   = RTC_FromBCD((uint8_t)((tr >> 16) & 0x3F));
    time->minutes = RTC_FromBCD((uint8_t)((tr >> 8)  & 0x7F));
    time->seconds = RTC_FromBCD((uint8_t)((tr >> 0)  & 0x7F));
}

void RTC_SetDate(RTC_Date_t *date)
{
    RTC_Unlock();
    RTC_EnterInitMode();

    uint32_t dr = 0;
    dr |= ((uint32_t)RTC_ToBCD(date->year))  << 16;
    dr |= ((uint32_t)RTC_ToBCD(date->month)) << 8;
    dr |= ((uint32_t)RTC_ToBCD(date->day))   << 0;
    RTC->DR = dr;

    RTC_ExitInitMode();
    RTC_Lock();
}

void RTC_GetDate(RTC_Date_t *date)
{
    uint32_t dr = RTC->DR;

    date->year  = RTC_FromBCD((uint8_t)((dr >> 16) & 0xFF));
    date->month = RTC_FromBCD((uint8_t)((dr >> 8)  & 0x1F));
    date->day   = RTC_FromBCD((uint8_t)((dr >> 0)  & 0x3F));
}
