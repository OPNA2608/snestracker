#include "FileLoader.h"
#include "DEBUGLOG.h"
#include <assert.h>
#include "Organization.h" // for APP_VERSION

std::unordered_map<uint8_t, FileLoader *> FileLoader::ChunkIdMap;

FileLoader::FileLoader(uint8_t chunkid) : chunkid(chunkid)
{
  ChunkIdMap[chunkid] = this;
}

/* TODO from Tracker file loading routine: Handle chunkids that the app
 * doesn't recognize safely - skip them. */

int FileLoader::load(SDL_RWops *file)
{
  return 0;
}

size_t FileLoader::write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint32_t *chunksize_counter)
{
  size_t ret = SDL_RWwrite(context, ptr, size, num);
  if (ret != num)
  {
    DEBUGLOG("Couldn't fully write data; Wrote %d of %d bytes\n", ret * size, num * size);
  }
  *chunksize_counter += ret * size;
}
size_t FileLoader::write (struct SDL_RWops* context, const void* ptr, size_t size, size_t num,
                        uint16_t *chunksize_counter)
{
  size_t ret = SDL_RWwrite(context, ptr, size, num);
  if (ret != num)
  {
    DEBUGLOG("Couldn't fully write data; Wrote %d of %d bytes\n", ret * size, num * size);
  }
  *chunksize_counter += ret * size;
}

// Now with bounds checking :D
// Note: size includes NULL byte
size_t FileLoader::read_str_from_file(SDL_RWops *file, char *str_ptr, int size)
{
  size_t count = 0;
  assert(size > 1); // if it was one, that's only enough room for an empty string!
  do {
    SDL_RWread(file, str_ptr, 1, 1);
    count++;
  } while ((--size) && *(str_ptr++) != 0);

  *str_ptr = 0; // need to write null byte incase we stopped looping cause size barrier was hit
  return count;
}

size_t FileLoader::read_str_from_file2(SDL_RWops *file, char *str_ptr, int chunksize, int bufsize)
{
  size_t count = 0;
  assert(chunksize > 0 && bufsize > 1); // if it was one, that's only enough room for an empty string!
  do {
    SDL_RWread(file, str_ptr++, 1, 1);
    count++;
  } while ((--chunksize) && (--bufsize - 1));

  *str_ptr = 0; // need to write null byte incase we stopped looping cause size barrier was hit
  return count;
}

/* Here's a small program to test how this works:
 *
#include <stdio.h>

void read_str_from_file(char *src, char *dest, int len)
{
do {
 *dest = *(src++);
 } while ( (--len) && *(dest++) != 0);

 *dest = 0;
 }

 int main()
 {
 char src[] = "Test";
 char dest[5];

 read_str_from_file(src, dest, 4);

 printf("%s\n", dest);
 return 0;
}*
*/
