#include "iwdg.h"

/* IWDG key values — fixed magic numbers defined by ST (RM0368), not something you calculate */
#define IWDG_KEY_ENABLE_ACCESS 0x5555UL // unlocks PR/RLR for writing
#define IWDG_KEY_RELOAD        0xAAAAUL // reloads the counter (feeds the watchdog)
#define IWDG_KEY_START         0xCCCCUL // starts the watchdog

void IWDG_Init(IWDG_Prescaler_t prescaler, uint16_t reloadValue)
{
    IWDG->KR = IWDG_KEY_ENABLE_ACCESS; // unlock PR and RLR

    IWDG->PR = (uint32_t)prescaler;

    while (IWDG->SR & (1UL << 0)) {
        // PVU bit: wait until the prescaler value is actually updated in hardware
    }

    IWDG->RLR = reloadValue & 0xFFFUL; // RLR is 12-bit, max 0xFFF

    while (IWDG->SR & (1UL << 1)) {
        // RVU bit: wait until the reload value is actually updated in hardware
    }

    IWDG_Reload(); // load RLR into the counter immediately
}

void IWDG_Start(void)
{
    IWDG->KR = IWDG_KEY_START;
}

void IWDG_Reload(void)
{
    IWDG->KR = IWDG_KEY_RELOAD;
}
