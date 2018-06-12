#include "RF.h"
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
#include "gpio.h"
}

static ETSTimer timer;
volatile rfparams_t rfparams;

static void reset() {
  rfparams.rcvstate = STATE_IDLE;
  rfparams.rawlen = 0;
}
static void ICACHE_FLASH_ATTR read_timeout(void *arg) {
  os_intr_lock();
  if (rfparams.rawlen) {
    rfparams.rcvstate = STATE_STOP;
  }
  os_intr_unlock();
}

static void ICACHE_FLASH_ATTR gpio_intr(void *arg) {
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

  if (rfparams.rcvstate == STATE_STOP) {
    return;
  }
  static uint32_t start = 0;
  uint32_t now = system_get_time();
  uint32_t shift = 0;
  uint32_t time_out = RF_TIMER_OFF;
  if (rfparams.rcvstate == STATE_IDLE) {
    rfparams.rcvstate = STATE_MARK;
    //rfparams.rawbuf[rfparams.rawlen++] = 20;
  } else if (rfparams.rawlen < RAWBUF) {
    //rfparams.rawbuf[rfparams.rawlen++] = (now - start) / USECPERTICK + 1;
    unsigned int pulse = now - start;
    if (pulse > MIN_PULSE_LENGTH) {
      rfparams.rawbuf[rfparams.rawlen++] = pulse;
    } else {
       rfparams.rcvstate = STATE_STOP;
       // Try to remove small pulse ...
       /**if (rfparams.rawlen > 1) {
         shift = rfparams.rawbuf[--rfparams.rawlen];
         time_out -= shift/1000;
       } else
         reset();**/
    } 
  } else
      rfparams.rcvstate = STATE_STOP;    
      
  start = now - shift;

  os_timer_disarm(&timer);
  os_timer_arm(&timer, time_out, 0);
}

RFrecv::RFrecv(int recvpin) {
  rfparams.recvpin = recvpin;
}

// initialization
void RFrecv::enableRX() {

  // initialize state machine variables
  rfparams.rcvstate = STATE_IDLE;
  rfparams.rawlen = 0;

  // set pin modes
  GPIO_DIS_OUTPUT(rfparams.recvpin);

  // Initialize timer
  os_timer_disarm(&timer);
  os_timer_setfn(&timer, (os_timer_func_t *)read_timeout, &timer);

  // ESP Attach Interrupt
  ETS_GPIO_INTR_DISABLE();
  ETS_GPIO_INTR_ATTACH(gpio_intr, NULL);
  gpio_pin_intr_state_set(GPIO_ID_PIN(rfparams.recvpin), GPIO_PIN_INTR_ANYEDGE);
  ETS_GPIO_INTR_ENABLE();

}

void RFrecv::disableRX() {
  //irReadTimer.stop();
  //os_timer_disarm(&irReadTimer);
  ETS_INTR_LOCK();
  ETS_GPIO_INTR_DISABLE();
}

void RFrecv::resume() {
  rfparams.rcvstate = STATE_IDLE;
  rfparams.rawlen = 0;
}

// Decodes the received IR message
// Returns 0 if no data ready, 1 if data ready.
// Results of decoding are stored in results
int RFrecv::decode(rfdecode_results *results) {
  if (rfparams.rcvstate != STATE_STOP) {
    return ERR;
  }
  results->rawbuf = rfparams.rawbuf;
  results->rawlen = rfparams.rawlen;
  // Try to decode ...
  // ...
  // Throw away and start over
  resume();
  return DECODED;
}
#endif
// ---------------------------------------------------------------
