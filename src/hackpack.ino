#include "application.h"
#include "cellular_hal.h"
#include <string.h>
#include <stdint.h>
#include "neopixel.h"
#include "cmd.h"
#include "pixel_grid.h"
#include "states.h"
#include "sms.h"

/**
 * @inits
 */
PRODUCT_ID(3920);
PRODUCT_VERSION(8);
SYSTEM_MODE(SEMI_AUTOMATIC);
//SYSTEM_MODE(AUTOMATIC);

/* Set up Twilio APN - required with Twilio SIM */
STARTUP(cellular_credentials_set("wireless.twilio.com", "", "", NULL));

/* Receive SMS callback. Requires base firmware 0.5.1.rc2
https://github.com/spark/firmware/releases/tag/v0.5.1-rc.2 */
STARTUP(cellular_sms_received_handler_set(sms_received, NULL, NULL));

volatile int newSMS = 0;
String smsBody;

/**
 * @Setup
 */
void setup()
{

    init_system();

    /* Delete all potentially stale SMS from the modem */
    SmsDeleteFlag flag = DELETE_ALL;
    smsDeleteAll(flag);


    block_scale = 1;
    block_glow = 0;
    block_jitter = 0;
}

/**
 * SMS Stuff
 */
void setStateFromIncomingKeyword(String command) {
  command.toLowerCase();
  parse_command(command);
}

void sms_received(void* data, int index)
{
    newSMS++;
}

void smsCommandSetState(String body) {
  /* Strip CommandSid from beginning of SMS body */
   String keyword = body.substring(body.indexOf(" ") + 1);
   setStateFromIncomingKeyword(keyword);
}

/**
 * @loop
 */
void loop()
{
  if (newSMS > 0) {
    smsBody = checkUnreadSMS();
    Serial.print("SMS BODY:"); Serial.println(smsBody);
    if (smsBody != NULL) {
        smsCommandSetState(smsBody);
    }
    if (newSMS > 0) newSMS--;
  }

  update_display();
}
