#pragma once

#include "memory.h"

struct Mouse_Hexdump_Area : Memory
{
  enum submodes { HARD_EDIT=0, EASY_EDIT=1 };
  void update_editing_address();
  void draw_cursor(SDL_Surface *screen, Uint32 color);
  void inc_cursor_row();
  void dec_cursor_row();
  void inc_cursor_pos();
  void dec_cursor_pos();
  void add_addr(int i);

  int tmp_ram=0; char draw_tmp_ram=0;
  int submode=0;
  
  Uint8 highnibble=1;
  Uint8 horizontal=1;
  int old_addr=0;
  static  int MOUSE_HEXDUMP_START_Y;
}; 


