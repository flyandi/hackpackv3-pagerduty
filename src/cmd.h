#ifndef CMD_H_
#define CMD_H_

#include <string.h>

extern uint8_t block_scale;
extern uint8_t block_glow;
extern uint8_t block_jitter;

int parse_command(String command);
String search_string(String data, char delimiter, int index);

#endif
