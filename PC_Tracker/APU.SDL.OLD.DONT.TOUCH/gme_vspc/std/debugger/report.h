#ifndef _report_h__
#define _report_h__

#include "SDL.h"
#include "Screen.h"
#include "gme/Spc_Dsp_Register_Map_Interface.h"

/* 
I cannot encapsulate this because the DSP needs to be able to access it
*/

struct Mem_Surface
{
public:
  static SDL_Rect memrect;

  Mem_Surface();
  ~Mem_Surface();
  void draw(SDL_Surface *screen);
  void init();
  void clear();
  void init_video();
  void fade_arrays();
  
  unsigned char *data, *cdata;
  SDL_Surface *sdl_surface, *sdl_csurface;
};


//extern int last_pc;





#define report_off(addr) do { int idx = (((addr)&0xff00)<<4); idx+= ((addr)%256)<<3; for (int i=0; i < 3; i++) { report::memsurface.data[idx+i]=0x00; report::memsurface.data[idx+2048+i]=0x00; report::memsurface.data[idx+4+i]=0x0; report::memsurface.data[idx+4+2048+i]=0x0; } } while(0)


#define report_memread(addr) do { int idx = (((addr)&0xff00)<<4)+2; idx+= ((addr)%256)<<3; report::memsurface.data[idx]=0xff; report::memsurface.data[idx+2048]=0xff; report::memsurface.data[idx+4]=0xff; report::memsurface.data[idx+4+2048]=0xff; report::used2[((addr)&0xff00)>>8]=1; report::used[(addr)&0xffff]=1; } while(0)
#define report_echomem(addr) do\
{\
int idx = (((addr)&0xff00)<<4);\
idx+= ((addr)%256)<<3;\
for (int i=0; i < 2; i++)\
{\
  report::memsurface.data[idx+i]=0xff;\
  report::memsurface.data[idx+2048+i]=0xff;\
  report::memsurface.data[idx+4+i]=0xff;\
  report::memsurface.data[idx+4+2048+i]=0xff;\
}\
} while(0)



#define report_memread2(addr, opcode) int i, idx;\
  if ((addr)!=report::last_pc)\
  {\
    for (i=0; i<5; i++)\
    {\
      idx = (((addr)+i)&0xff00)<<4; idx+= (((addr)+i)%256)<<3;\
      report::memsurface.data[idx]=0xff;\
      report::memsurface.data[idx+2048]=0xff;\
      report::memsurface.data[idx+4]=0xff;\
      report::memsurface.data[idx+4+2048]=0xff;\
      report::used2[((addr)&0xff00)>>8]=1;\
    }\
    report::used[(addr)]=1;\
    report::used[(addr)+1]=1;\
    report::used[(addr)+2]=1;\
    report::used[(addr)+3]=1;\
    report::used[(addr)+4]=1;\
    report::used[(addr)+5]=1;\
  }

#define report_memwrite(addr) do { int idx = (((addr)&0xff00)<<4)+1; idx += ((addr) % 256)<<3; report::memsurface.data[idx]=0xff; report::memsurface.data[idx+4] = 0xff; report::memsurface.data[idx+2048]=0xff; report::memsurface.data[idx+2048+4] = 0xff; } while(0)


/*#define report_memread(addr) 
#define report_echomem(addr) 
#define report_memread2(addr, opcode) 
#define report_memwrite(addr)*/

void report_cursor(int addr);


// Memsurface and the other entities are coupled together OK!! 
// ie. memsurface.clear() will also clear BRR_Headers SRCN LOOP_used arrays..
// it doesn't make sense to call one but not the other!! 
namespace report
{
  static const int BRR_HEADER_MAX=100, SRCN_MAX=0x200;
  extern int last_pc;
  extern int bcolor; // backup color
  extern Mem_Surface memsurface;
  extern unsigned char used2[0x101];
  extern unsigned char used[0x10006];

  struct Src
  {
    uint16_t dir_addr; //unused for now
    uint16_t brr_start=0xffff;
    uint16_t brr_end=0xffff;
    uint16_t brr_loop_start=0xffff;
    uint16_t brr_loop_end=0xffff;
  };
  extern Src src[MAX_SRCN_ENTRIES];

  int backup_color(int addr);

  void restore_color(int addr);
}
#endif



