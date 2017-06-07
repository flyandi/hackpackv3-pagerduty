#include "application.h"
#include "states.h"
#include "cmd.h"
#include "pixel_grid.h"

uint8_t state_transition_flag = 1;
uint8_t mode_transition_flag = 1;

SYSTEM_STATE CURRENT_STATE = STARTUP;
SYSTEM_MODE ALERT_MODE = NORMAL;

/**
 * Init System
 */
void init_system(void)
{
  Serial.begin(115200);

  init_pixels();

  //Connect to the cloud
  Particle.connect();

  // Expose our functions to the particle server
  Particle.function("command", parse_command);
}

//*****************************************
//Update the animation on the Neopixel grid
void update_display(void)
{
  if(!alert_mode())
  {
    run_display_state();
  }
  return;
}

/**
 * Runs the current alarm sequence
 * @returns int
 */
uint8_t alert_mode(void)
{
  switch(ALERT_MODE) {

    case(ALERT1): alert1_sequence(); return(1); break;
    case(ALERT2): alert2_sequence(); return(1); break;
    case(DOCKED): docked_sequence(); return(1); break;
    case(OK): ok_sequence(); return(1); break;
  }

  return(0);
}

/**
 * Runs
 */
void run_display_state(void)
{
  switch(CURRENT_STATE)
  {
      case(STARTUP):
          startup_display();
          break;

      case(BLOCK):
          if(state_transition_flag)
          {
              set_all_pixels(grid_color, grid_brightness);
              state_transition_flag = 0;
          }
          break;

      case(CHASE):
          chase_pixels(grid_color);
          break;

      case(FADE):
          fade_pixels(grid_color);
          break;

      case(SQUARED):
          squared_pixels(grid_color);
          break;
  }
  return;
}

//********************************************************************
//Clear the display and set the flag if a state transition has occured
void state_transition(void)
{
  set_all_pixels(BLACK, 0);
  delay(10);                    //TODO see if we can reduce this delay
  state_transition_flag = 1;
}
