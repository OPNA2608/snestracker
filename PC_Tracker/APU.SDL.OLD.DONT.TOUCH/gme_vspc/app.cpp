#include "app.h"

App::App(int &argc, char **argv, int samplerate/*=44100*/) : 
debugger(argc,argv)
{
  Player_Context::player = &player;
  handle_error(player.init(samplerate) );
}

void App::run()
{
  // For now..
  debugger.run();
}

App::~App()
{
  if (screen)
    SDL_FreeSurface(screen);
  if (sdlTexture)
    SDL_DestroyTexture(sdlTexture);
  if (sdlRenderer)
    SDL_DestroyRenderer(sdlRenderer);
  if(sdlWindow)
    SDL_DestroyWindow(sdlWindow);
}
