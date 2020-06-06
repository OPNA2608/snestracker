#pragma once
#include <stdint.h>
#include "SDL.h"

/* The FileLoader class works as follows
Each Module that can be loaded from File contains a class that inherits FileLoader.
This class identifies with a particular FileLoader::ChunkID.

???

When the GUI app goes to open up the file, a top-level load() is called. This
function checks for the proper header data, such as FILETYPE string.

Once the header is read, the rest of the file is divided into chunks. Every chunk has
its own header which is simply chunkID:chunksize.

The top-level loader will read the ID, and use it to call the FileLoader * associated
with that chunk id.
*/

class FileLoader
{
public:
  FileLoader(uint8_t chunkid);
  
  enum ChunkID {
    SongSettings=0,
    Sample,
    Instrument,
    Pattern,
    PatternSequencer,
    NUM_CHUNKIDS
  };
  static FileLoader * ChunkIdMap[NUM_CHUNKIDS];
  static size_t loadchunks(SDL_RWops *file);

  virtual size_t load(SDL_RWops *file, size_t chunksize);
  virtual size_t save(SDL_RWops *file)=0;
  uint8_t chunkid;

  static size_t read (struct SDL_RWops* context,
                  void*             ptr,
                  size_t            size,
                  size_t            maxnum,
                  size_t            *bytecount);

  static size_t write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint32_t *chunksize_counter);
  static size_t write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint16_t *chunksize_counter);

  static size_t read_str_from_file(SDL_RWops *file, char *str_ptr, int size);
  static size_t read_str_from_file2(SDL_RWops *file, char *str_ptr, int chunksize, int bufsize);

};
