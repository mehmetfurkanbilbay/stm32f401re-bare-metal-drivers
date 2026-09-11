#include "pwr.h"

void PWR_EnableBackupAccess(void)
{
    /* DBP bit (bit 8) in PWR_CR: without this, RTC/BDCR registers stay
     * write-protected even after RTC_Unlock() — this is a separate, outer
     * layer of protection covering the whole "backup domain" (RTC + backup
     * registers), not just the RTC peripheral itself. */
    PWR->CR |= (1UL << 8);
}
