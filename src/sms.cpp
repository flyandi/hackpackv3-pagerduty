#include "sms.h"

int _cbCMGL(int type, const char* buf, int len, CMGLparam* param)
{
    if ((type == TYPE_PLUS) && param && param->num) {
        // +CMGL: <ix>,...
        int ix;
        if (sscanf(buf, "\r\n+CMGL: %d,", &ix) == 1)
        {
            *param->ix++ = ix;
            param->num--;
        }
    }
    return WAIT;
}

int smsList(const char* stat /*= "ALL"*/, int* ix /*=NULL*/, int num /*= 0*/) {
    int ret = -1;
    CMGLparam param;
    param.ix = ix;
    param.num = num;
    if (RESP_OK == Cellular.command(_cbCMGL, &param, "AT+CMGL=\"%s\"\r\n", stat))
        ret = num - param.num;
    return ret;
}

bool smsDelete(int ix)
{
    /* <index> = Storage position */
    bool ok = false;
    ok = (RESP_OK == Cellular.command("AT+CMGD=%d\r\n", ix));
    return ok;
}

bool smsDeleteAll(SmsDeleteFlag flag) {
  /*
  * <flag> = Deletion flag. If present, and different from 0, <index> is ignored:
  * • 1: delete all the read messages from the preferred message storage, leaving unread messages
  * and stored mobile originated messages (whether sent or not) untouched
  * • 2: delete all the read messages from the preferred message storage and sent mobile originated
  * messages, leaving unread messages and unsent mobile originated messages untouched
  * • 3: delete all the read messages from the preferred message storage, sent and unsent mobile
  * originated messages leaving unread messages untouched
  * • 4: delete all the messages from the preferred message storage including unread messages
  */
  bool ok = false;
  ok = (RESP_OK == Cellular.command("AT+CMGD=1,%d\r\n", flag));
  return ok;
}

int _cbCMGR(int type, const char* buf, int len, CMGRparam* param)
{
    static bool getsmsbody = false;

    if (param) {
        if (type == TYPE_PLUS) {
            if (sscanf(buf, "\r\n+CMGR: \"%*[^\"]\",\"%[^\"]", param->num) == 1) {
                getsmsbody = true;
            }
        } else if ((type == TYPE_UNKNOWN) && getsmsbody) {
                    getsmsbody = false;
                    memcpy(param->buf, buf, len);
                    param->buf[len] = '\0';
                }
    }
    return WAIT;
}

bool smsRead(int ix, char* num, char* buf, int len)
{
    bool ok = false;
    CMGRparam param;
    param.num = num;
    param.buf = buf;
    ok = (RESP_OK == Cellular.command(_cbCMGR, &param, "AT+CMGR=%d\r\n", ix));

    Serial.print("b="); Serial.println(param.buf);
    return ok;
}

String checkUnreadSMS() {
    String body;
    char buf[512] = "";
    int ix[8];
    int n = smsList("REC UNREAD", ix, 8);

    Serial.print("n="); Serial.println(n);

    if (n > 8) n = 8;

    while (n-- > 0)
    {
        char num[32];
        if (smsRead(ix[n], num, buf, sizeof(buf))) {

            body = String(buf);

            // All done with this message, let's delete it
            smsDelete(ix[n]);
            return body;
        }
    }
}
