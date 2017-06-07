/**
 * Command Interface
 */

#include "states.h"
#include "pixel_grid.h"
#include "cmd.h"

uint8_t block_scale = 1;
uint8_t block_glow = 0;
uint8_t block_jitter = 0;

/**
 * Handles the inbound commands
 * <string> command     The command string
 * @returns int
 */
int parse_command(String command)
{
    String commandKeyword = search_string(command, ',', 0);

    Serial.print("Command received:");
    Serial.println(commandKeyword);

    if(commandKeyword == "")
    {
      return(1);
    }

    // pager duty
    if(commandKeyword == "incident.trigger") {

        ALERT_MODE = ALERT1; CURRENT_STATE = SQUARED; grid_color = grid.Color(255, 0, 0); state_transition();

    } else if (commandKeyword == "incident.acknowledge") {

        CURRENT_STATE = SQUARED; grid_color = grid.Color(255, 255, 0); state_transition();

    } else if (commandKeyword == "incident.assign") {

          CURRENT_STATE = SQUARED; grid_color = grid.Color(0, 255, 0); state_transition();

    } else if (commandKeyword == "incident.resolve") {

          ALERT_MODE = OK; CURRENT_STATE = CLEAR; state_transition();

    } else if (commandKeyword == "incident.escalate") {

          CURRENT_STATE = SQUARED; grid_color = grid.Color(0, 0, 255); state_transition();
    }
    // stand by commands
    else if(commandKeyword == "clear") {

        CURRENT_STATE = CLEAR;  state_transition();

    } else if (commandKeyword == "block") {

        CURRENT_STATE = BLOCK; state_transition();

    } else if (commandKeyword == "fade") {

        CURRENT_STATE = FADE; state_transition();

    } else if (commandKeyword == "chase") {

        CURRENT_STATE = CHASE; state_transition();

    } else if (commandKeyword == "squared") {

        CURRENT_STATE = SQUARED; state_transition();

    } else if (commandKeyword == "ok") {

        ALERT_MODE = OK; state_transition();

    } else if (commandKeyword == "alert1") {

        ALERT_MODE = ALERT1; state_transition();

    } else if (commandKeyword == "alert2") {

        ALERT_MODE = ALERT2; state_transition();

    } else if (commandKeyword == "docked") {

        ALERT_MODE = DOCKED; state_transition();

    } else if (commandKeyword == "brightness") {

        String bright_str = search_string(command, ',', 1);

        grid_brightness = bright_str != "" ? bright_str.toInt() : 0;

        state_transition();

    } else if (commandKeyword == "color") {

        String red_str = search_string(command, ',', 1);
        String green_str = search_string(command, ',', 2);
        String blue_str = search_string(command, ',', 3);
        String bright_str = search_string(command, ',', 4);

        if(red_str == "" || green_str == "" || blue_str == "") {
          return(1);
        }

        grid_color = grid.Color(red_str.toInt(), green_str.toInt(), blue_str.toInt());

        if(CURRENT_STATE == STARTUP) {
          CURRENT_STATE = BLOCK;
        }

        state_transition();

    } else {

        return(1);
    }

    return 0;
}

/**
 * Search for a string in a string
 * @returns String
 */
String search_string(String data, char delimiter, int index)
{
    uint8_t i;
    uint8_t found = 0;
    int8_t strIndex[] = {0, -1};
    uint8_t maxIndex = data.length()-1;

    //***************************************
    //Search for delimiters within the string
    for(i=0; i<=maxIndex && found<=index; i++)
    {
      if(data.charAt(i)==delimiter || i==maxIndex)
      {
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }

    //***********************
    //If we found a delimiter
    if(found>index)
    {
      return(data.substring(strIndex[0], strIndex[1]));
    }
    else
    {
      return("");
    }
}
