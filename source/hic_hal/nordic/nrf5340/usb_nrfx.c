#include <string.h>

#include "usb.h"
#include "rl_usb.h"

#include "nrf.h" /* Device header */
#include "nrf53_erratas.h"
#include "nrfx.h"
#include "nrfx_power.h"
#include "nrfx_usbd.h"

#define __NO_USB_LIB_C
#include "usb_config.c"

#define USBD_EP_NUM_MAX (8)
static uint32_t ep_buf[2][USBD_EP_NUM_MAX][64 / (sizeof(uint32_t))]; /*TODO*/

#ifdef __RTX
static OS_MUT usbd_hw_mutex;
#endif

static const nrfx_power_config_t power_config = {
    .dcdcen = 0, /* device gets 1.8V from external regulator */
};

#define USB_REQTYPE_GET_TYPE(bmRequestType) (((bmRequestType) >> 5) & 0x03U)

static bool usb_power_ldo_ready = false;
static bool usb_clear_setup_ready = false;
static bool usb_setup_ready = false;
static bool usb_setup_no_data_stage = false;

static void usbd_event_handler(nrfx_usbd_evt_t const *const p_event) {
  uint8_t ep_dir, ep_num;

  switch (p_event->type) {
  case NRFX_USBD_EVT_SUSPEND: /*(/)*/
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_SUSPEND, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Suspend_Event) {
      USBD_P_Suspend_Event();
    }
#endif
    break;
  case NRFX_USBD_EVT_RESET: /*(/)*/
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_RESET, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Reset_Event) {
      USBD_P_Reset_Event();
    }
#endif
    usbd_reset_core();
    usb_setup_ready = false;
    break;
  case NRFX_USBD_EVT_WUREQ: /*(/)*/
    break;
  case NRFX_USBD_EVT_RESUME: /*(/)*/
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_RESUME, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Resume_Event) {
      USBD_P_Resume_Event();
    }
#endif
    break;
  case NRFX_USBD_EVT_SOF: /*(/)*/
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_SOF, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_SOF_Event) {
      USBD_P_SOF_Event();
    }
#endif
    break;
  case NRFX_USBD_EVT_EPTRANSFER: /*(/)*/
    ep_dir = (p_event->data.eptransfer.ep & USB_ENDPOINT_DIRECTION_MASK) >> 7;
    ep_num = p_event->data.eptransfer.ep & ~USB_ENDPOINT_DIRECTION_MASK;
#ifdef __RTX
    if (USBD_RTX_EPTask[ep_num]) {
      if (ep_dir != 0U) {
        isr_evt_set(USBD_EVT_IN, USBD_RTX_EPTask[ep_num]);
      } else {
        isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[ep_num]);
      }
    }
#else
    if (USBD_P_EP[ep_num]) {
      if (ep_dir != 0U) {
        USBD_P_EP[ep_num](USBD_EVT_IN);
      } else {
        USBD_P_EP[ep_num](USBD_EVT_OUT);
      }
    }
#endif
    if (ep_num == 0 && usb_clear_setup_ready) {
      usb_clear_setup_ready = false;
      nrfx_usbd_setup_clear();
    }

    break;
  case NRFX_USBD_EVT_SETUP: /*(/)*/
    usb_setup_ready = true;
    usb_clear_setup_ready = false;
    if ((NRF_USBD->BREQUEST != USB_REQUEST_SET_ADDRESS) ||
        (USB_REQTYPE_GET_TYPE(NRF_USBD->BMREQUESTTYPE) != REQUEST_STANDARD)) {
#ifdef __RTX
      if (USBD_RTX_EPTask[0]) {
        isr_evt_set(USBD_EVT_SETUP, USBD_RTX_EPTask[0]);
      }
#else
      if (USBD_P_EP[0]) {
        USBD_P_EP[0](USBD_EVT_SETUP);
      }
#endif
    }
    break;
  }
  USBD_SignalHandler();
}

static void power_usb_event_handler(nrfx_power_usb_evt_t event) {
  switch (event) {
  case NRFX_POWER_USB_EVT_DETECTED:
    if (!nrfx_usbd_is_enabled()) {
      nrfx_usbd_enable();
    }
    break;
  case NRFX_POWER_USB_EVT_REMOVED:
    if (nrfx_usbd_is_started()) {
      nrfx_usbd_stop();
    }
    if (nrfx_usbd_is_enabled()) {
      nrfx_usbd_disable();
    }
    break;
  case NRFX_POWER_USB_EVT_READY:
    if (!nrfx_usbd_is_started()) {
      usb_power_ldo_ready = true;
      nrfx_usbd_start(true);
    }
    break;
  default:
    return;
  }
}

static const nrfx_power_usbevt_config_t usbevt_config = {
    .handler = power_usb_event_handler};

/* USB Hardware Functions */
void USBD_Init(void) {
#ifdef __RTX
  os_mut_init(&usbd_hw_mutex);
#endif
  nrfx_usbd_init(usbd_event_handler);
  nrfx_power_init(&power_config);
  nrfx_power_usbevt_init(&usbevt_config);
}
void USBD_Connect(BOOL con) {
  if (con) {
    while (!usb_power_ldo_ready) {
    }
    nrfx_usbd_start(true);
  } else {
    nrfx_usbd_stop();
  }
}
void USBD_Reset(void) {}
void USBD_Suspend(void) { nrfx_usbd_suspend(); }
void USBD_Resume(void) {}
void USBD_WakeUp(void) { nrfx_usbd_wakeup_req(); }
void USBD_WakeUpCfg(BOOL cfg) {}
void USBD_SetAddress(U32 adr, U32 setup) {}
void USBD_Configure(BOOL cfg) {}
void USBD_ConfigEP(USB_ENDPOINT_DESCRIPTOR *pEPD) {
  nrfx_usbd_ep_t ep = pEPD->bEndpointAddress;
  nrfx_usbd_ep_max_packet_size_set(ep, pEPD->wMaxPacketSize);
}

void USBD_DirCtrlEP(U32 dir) {}
void USBD_EnableEP(U32 EPNum) {
  nrfx_usbd_ep_t ep = EPNum;
  nrfx_usbd_ep_enable(ep);
}
void USBD_DisableEP(U32 EPNum) {
  nrfx_usbd_ep_t ep = EPNum;
  nrfx_usbd_ep_disable(ep);
}
void USBD_ResetEP(U32 EPNum) {
  nrfx_usbd_ep_t ep = EPNum;
  nrfx_usbd_ep_dtoggle_clear(ep);
  nrfx_usbd_ep_stall_clear(ep);
  nrfx_usbd_transfer_out_drop(ep);
}
void USBD_SetStallEP(U32 EPNum) {
  nrfx_usbd_ep_t ep = EPNum;
  nrfx_usbd_ep_stall(ep);
}
void USBD_ClrStallEP(U32 EPNum) {
  nrfx_usbd_ep_t ep = EPNum;
  nrfx_usbd_ep_stall_clear(ep);
}
void USBD_ClearEPBuf(U32 EPNum) {}
U32 USBD_ReadEP(U32 EPNum, U8 *pData, U32 cnt) {
  uint8_t ep_dir, ep_num;
  uint16_t len;

  ep_dir = (EPNum & USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num = EPNum & ~USB_ENDPOINT_DIRECTION_MASK;

  if (ep_num >= USBD_EP_NUM_MAX) {      /* If ep_num is not available */
    return 0U;
  }
  if (ep_dir != 0U) {                   /* If not OUT Endpoint */
    return 0U;
  }

  if ((ep_num == 0U) && (!usb_setup_ready) &&
      (nrfx_usbd_last_setup_dir_get() == NRFX_USBD_EPIN0)) {
    /* Status stage of Device-to-Host handled in USBD_WriteEP */
    /* Note - ep0_dir is set only after the setup packet has been read */
    return 0;
  }

  if ((ep_num == 0U) && (usb_setup_ready)) {
    uint16_t wlength;

    usb_setup_ready = false;
    pData[0] = NRF_USBD->BMREQUESTTYPE;
    pData[1] = NRF_USBD->BREQUEST;
    pData[2] = NRF_USBD->WVALUEL;
    pData[3] = NRF_USBD->WVALUEH;
    pData[4] = NRF_USBD->WINDEXL;
    pData[5] = NRF_USBD->WINDEXH;
    pData[6] = NRF_USBD->WLENGTHL;
    pData[7] = NRF_USBD->WLENGTHH;
    len = 8U;

    wlength = (pData[7] << 8) | (pData[6] << 0);
    if (wlength == 0) {
      usb_setup_no_data_stage = true;
      nrfx_usbd_setup_clear();
    } else {
      usb_setup_no_data_stage = false;
    }
  } else if ((ep_num == 0U) && usb_setup_no_data_stage) {
    len = 0;
  } else {
    len = NRF_USBD->SIZE.EPOUT[ep_num];

    if (len > cnt) {
      len = cnt;
    }

    nrfx_usbd_transfer_t transfer = {
        .p_data.rx = ep_buf[0][ep_num],
        .size = len,
    };
    nrfx_usbd_ep_transfer(EPNum, &transfer);
    /* Wait for transfer to finish */
    while (NRF_USBD->EVENTS_ENDEPOUT[ep_num] == 0U);
    /* Copy data received to requested pData */
    memcpy((void *)pData, (const void *)&ep_buf[ep_dir][ep_num][0], len);
  }
  return len;
}
U32 USBD_WriteEP(U32 EPNum, U8 *pData, U32 cnt) {
  uint8_t ep_dir, ep_num;
  uint16_t len, ep_max_len;
  ep_dir = (EPNum & USB_ENDPOINT_DIRECTION_MASK) >> 7;
  ep_num = EPNum & ~USB_ENDPOINT_DIRECTION_MASK;
  len = cnt;
  ep_max_len =  nrfx_usbd_ep_max_packet_size_get(EPNum);

  if (ep_num >= 8) {      /* If ep_num is not available */
    return 0U;
  }
  if (ep_dir == 0U) {     /* If not IN Endpoint */
    return 0U;
  }
  if ((ep_num == 0U) && usb_setup_no_data_stage) {
  /* There isn't a data stage so don't try to write data */
    return 0U;
  }
  if ((ep_num == 0U) && (nrfx_usbd_last_setup_dir_get() == NRFX_USBD_EPOUT0)) {
    /* Status stage of Host-to-Device handled elsewhere */
    return 0U;
  }
  if (len > ep_max_len) {
    len = ep_max_len;
  }
  /* If this is a short packet on endpoint 0 then it is the last packet
   of the transfer. Start the status stage after it is sent (the next IN event)
   */
  if ((ep_num == 0U) && cnt < ep_max_len) {
    usb_clear_setup_ready = true;
  }

  memcpy((void *)&ep_buf[ep_dir][ep_num][0], (const void *)pData, len);

  nrfx_usbd_transfer_t transfer = {
        .p_data.rx = ep_buf[0][ep_num],
        .size = len,
    };
  nrfx_usbd_ep_transfer(EPNum, &transfer);

  return len;
}
U32 USBD_GetFrame(void) {}
U32 USBD_GetError(void) {}
void USBD_Handler(void) {}
