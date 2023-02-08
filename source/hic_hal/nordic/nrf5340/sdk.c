#include "device.h"
#include "IO_Config.h"
#include "daplink.h"
#include "compiler.h"
#include "util.h"
#include "nrfx.h"
#include "nrfx_clock.h"


void sdk_init()
{

	/* Enable instruction trace */
	/*
	NRF_TAD_S->ENABLE = TAD_ENABLE_ENABLE_Msk;
	NRF_TAD_S->CLOCKSTART = TAD_CLOCKSTART_START_Msk;
	NRF_TAD_S->PSEL.TRACECLK   = TAD_PSEL_TRACECLK_PIN_Traceclk;
	NRF_TAD_S->PSEL.TRACEDATA0 = TAD_PSEL_TRACEDATA0_PIN_Tracedata0;
	NRF_TAD_S->PSEL.TRACEDATA1 = TAD_PSEL_TRACEDATA1_PIN_Tracedata1;
	NRF_TAD_S->PSEL.TRACEDATA2 = TAD_PSEL_TRACEDATA2_PIN_Tracedata2;
	NRF_TAD_S->PSEL.TRACEDATA3 = TAD_PSEL_TRACEDATA3_PIN_Tracedata3;
	NRF_P0_S->PIN_CNF[TAD_PSEL_TRACECLK_PIN_Traceclk]
			&= ~(GPIO_PIN_CNF_MCUSEL_Msk | GPIO_PIN_CNF_DRIVE_Msk);
	NRF_P0_S->PIN_CNF[TAD_PSEL_TRACECLK_PIN_Traceclk]
			|= (GPIO_PIN_CNF_MCUSEL_TND << GPIO_PIN_CNF_MCUSEL_Pos)
			| (GPIO_PIN_CNF_DRIVE_E0E1 << GPIO_PIN_CNF_DRIVE_Pos);
	NRF_TAD_S->TRACEPORTSPEED = TAD_TRACEPORTSPEED_TRACEPORTSPEED_32MHz;
	*/
	if (!nrfx_clock_hfclk_is_running()) {
		nrfx_clock_hfclk_start();
	}
}
