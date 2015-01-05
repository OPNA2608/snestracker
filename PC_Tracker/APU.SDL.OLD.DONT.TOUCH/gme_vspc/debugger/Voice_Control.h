#pragma once
#include "SDL.h"
#include "globals.h"
#include "Player_Context.h"
#include "Screen.h"
struct Voice_Control : public Player_Context
{
  Voice_Control();

  //Music_Player *player;
  Uint8 was_keyed_on=0;
  Uint8 muted_toggle_protect=0; // for toggle protection during MouseMoition event
  Uint8 muted=0;


  void checkmouse_mute(Uint16 &x,Uint16 &y);
  
  void checkmouse_solo(Uint16 &x,Uint16 &y);
  
  void checkmouse(Uint16 &x, Uint16 &y, Uint8 &b);

  Uint8 is_muted(int i);
  void toggle_mute(uint8_t m);  // channel num
  void toggle_mute_all();
  void mute_all();
  void unmute_all();
};