#include "SampleEditor.h"
#include "Tracker.h"

extern Tracker *tracker;

SampleEditor::SampleEditor() :
	loop_cbuf("00"),
	loop_title("Loop..."),
	loop_valtext(loop_cbuf),
	loop_incbtn("+", incloop, this, true),
	loop_decbtn("-", decloop, this, true),

  finetune_cbuf("+000"),
  finetune_title("F.tune"),
  finetune_valtext(finetune_cbuf),
  finetune_incbtn("+", incfinetune, this, true),
  finetune_decbtn("-", decfinetune, this, true)
{
}

void SampleEditor :: update_loop()
{
	sprintf(loop_cbuf, "$%04x",
	  ::tracker->samples[ ::tracker->main_window.samplepanel.currow ].rel_loop);
}

/* Same code as Instrument editor fine tune (detune). Maybe we can
 * abstract the function out to reduce code size?? TODO*/
void SampleEditor :: update_finetune()
{
  int8_t ft = ::tracker->samples[ ::tracker->main_window.samplepanel.currow ].finetune;
  char sign = '+';
  if (ft < 0)
    sprintf(finetune_cbuf, "%04d", ft);
  else
    sprintf(finetune_cbuf, "%c%03d", sign, ft);
}

void SampleEditor :: set_coords(int x, int y)
{
  /* (x,y)
   * Vol......7f +-
   * Pan......80 +-
   * F.Tune +000 +-*/

  int yy = y;

	loop_title.rect.x = x;
	loop_title.rect.y = y;
	loop_valtext.rect.x = x + ((sizeof("Loop...")-1) * CHAR_WIDTH);
	loop_valtext.rect.y = y;
	loop_incbtn.rect.x  = loop_valtext.rect.x + (sizeof("$0000") * CHAR_WIDTH);
	loop_incbtn.rect.y = y;
	loop_decbtn.rect.x = loop_incbtn.rect.x + CHAR_WIDTH + 5;
	loop_decbtn.rect.y = y;

  y += CHAR_HEIGHT + 5;

  finetune_title.rect.x = x;
  finetune_title.rect.y = y;
  finetune_valtext.rect.x = x + ((sizeof("F.Tune ")) * CHAR_WIDTH);
  finetune_valtext.rect.y = y;
  finetune_incbtn.rect.x  = finetune_valtext.rect.x + (sizeof("+000") * CHAR_WIDTH);
  finetune_incbtn.rect.y = y;
  finetune_decbtn.rect.x = finetune_incbtn.rect.x + CHAR_WIDTH + 5;
  finetune_decbtn.rect.y = y;
}

int SampleEditor::handle_event(const SDL_Event &ev)
{
	loop_incbtn.check_event(ev);
	loop_decbtn.check_event(ev);

  finetune_incbtn.check_event(ev);
  finetune_decbtn.check_event(ev);
}

void SampleEditor::draw(SDL_Surface *screen/*=::render->screen*/)
{
	update_loop();
  update_finetune();

	loop_title.draw(screen);
	loop_valtext.draw(screen);
	loop_incbtn.draw(screen);
	loop_decbtn.draw(screen);

  finetune_title.draw(screen);
  finetune_valtext.draw(screen);
  finetune_incbtn.draw(screen);
  finetune_decbtn.draw(screen);
}

int SampleEditor::incloop(void *i)
{
	SampleEditor *ie = (SampleEditor *)i;
	Sample *s = &::tracker->samples[::tracker->main_window.samplepanel.currow];
	s->inc_loop();

	ie->update_loop();
}

int SampleEditor::decloop(void *i)
{
	SampleEditor *ie = (SampleEditor *)i;
	Sample *s = &::tracker->samples[::tracker->main_window.samplepanel.currow];
	s->dec_loop();

	ie->update_loop();
}



int SampleEditor::incfinetune(void *i)
{
  SampleEditor *ie = (SampleEditor *)i;
  Sample *s = &::tracker->samples[::tracker->main_window.samplepanel.currow];
  s->inc_finetune();
  ie->update_finetune();
}

int SampleEditor::decfinetune(void *i)
{
  SampleEditor *ie = (SampleEditor *)i;
	Sample *s = &::tracker->samples[::tracker->main_window.samplepanel.currow];
	s->dec_finetune();
  ie->update_finetune();
}
