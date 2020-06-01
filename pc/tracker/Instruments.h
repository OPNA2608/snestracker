#pragma once
#include <stdint.h>
#include "gui/Text_Edit_Rect.h"
#include "gui/Text.h"
#include "gui/Button.h"
#include "shared/Render.h"
#include "shared/dsptypes.h"
#include "FileLoader.h"
/* This number is hardcoded for now until sucessful testing is
 * done. Later, it will be made so that the limit can be dynamically
 * increased */
#define NUM_INSTR 0x40
#define INSTR_NAME_MAXLEN 22
#define APU_MAXVOL 0x7f

// TODO : take out meta info from Instrument into a InstrumentMeta
// datatype that inherits from Instrument. that should allow simplifying
// render_to_apu into memcpy on Instrument type into APU RAM
struct Instrument
{
  Instrument();
  ~Instrument();
  char name[INSTR_NAME_MAXLEN]; // the name of the instrument
	uint32_t used;
  /* the app-level sample data. How that gets exported into the snes
   * driver data is a different story */
	uint8_t srcn; // like a DIR offset
	Adsr adsr; // The volume envelope that will be used for this instrument
	/* Aside from the ADSR hardware volume envelope, the voice stereo volume
	 * may be adjusted real-time for additional effects.*/
	/* Note on volume envelopes as I learned from Milky Tracker. The graph
	 * view X-axis is specified in ticks, up to 0x100 ticks. So each point
	 * of the envelope is also specified in ticks */
	//VolumeEnvelope ve;
	//PanEnvelope pe;
	//  bool phaseflip=false;
  uint8_t vol;
  /* 0x80 = dead center, 0xFF = hard right, 0x00 = hard left */
  uint8_t pan;
  /* signed offset in semitones to pitch the sample. This will be used
   * directly by the SNES driver. The tracker must impose the restraints
   * on the range allowed */
  int8_t semitone_offset;
  /* used to pitch the sample for range of +/- one semitone. Tracker
   * software must impose retraints on the allowed range of values. Will
   * be used by SNES Driver */
  int8_t finetune;

  bool operator==(const Instrument& rhs);
  inline bool operator!=(const Instrument& rhs) { return !(*this == rhs); }

	static void inc_srcn(Instrument *i);
	static void dec_srcn(Instrument *i);
  static void inc_vol(Instrument *i);
  static void dec_vol(Instrument *i);
  static void inc_pan(Instrument *i);
  static void dec_pan(Instrument *i);
  static void inc_finetune(Instrument *i);
  static void dec_finetune(Instrument *i);
};

class InstrumentFileLoader : public FileLoader
{
public:
  InstrumentFileLoader(struct Instrument *instruments);
  size_t load(SDL_RWops *file, size_t chunksize);
  size_t save(SDL_RWops *file);

  enum SubChunkID {
    coreinfo=0,
    name,
    NUM_SUBCHUNKIDS
  };
private:
  struct Instrument *instruments;
};


struct Sample_Panel;
/* That defined the Data model above. Now time to get that into a view */
struct Instrument_Panel
{
  /* Initialize the panel view from an X/Y coordinate. Additionally, we
   * need a reference to the instruments */
  Instrument_Panel(Instrument *instruments, Sample_Panel *sp);
  ~Instrument_Panel();

  int event_handler(const SDL_Event &ev);
  void one_time_draw(SDL_Surface *screen=::render->screen);
  void draw(SDL_Surface *screen=::render->screen);
  void set_coords(int x, int y);

	void set_currow(int c);
	void inc_currow();
	void dec_currow();

  // callback funcs for the buttons
  static int load(void *ipanel);
  static int save(void *ipanel);
  static int zap(void *ipanel);

  Text title;
  Button loadbtn, savebtn, zapbtn;
  Text instr_indices[NUM_INSTR];

  // the number of instrument rows in GUI
  static const int NUM_ROWS = 8;
  int rows_scrolled = 0;
  // the current selected row
  int currow = 0;

  // 4 is for eg. "01|\0"
  char instr_index_strings[NUM_ROWS][4];
  Text_Edit_Rect instr_names[NUM_ROWS];
  /* a direct handle on the data, rather than accessing through an API */
  Instrument *instruments;
	// handle on the sample panel to update its currow when selecting an
	// instr
	Sample_Panel *samplepanel;
  /* Todo, calculate the panel rect */
  SDL_Rect rect; // define the boundaries of the entire panel
  SDL_Rect highlight_r; // the highlight rect of current select instr
};
