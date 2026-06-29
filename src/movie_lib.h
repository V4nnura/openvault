#ifndef MOVIE_LIB_H
#define MOVIE_LIB_H

#include <SDL.h>

namespace fallout {

typedef void*(MveMallocFunc)(size_t size);
typedef void(MveFreeFunc)(void* ptr);
typedef bool(MveReadFunc)(void* handle, void* buffer, int count);
typedef void(MveShowFrameFunc)(SDL_Surface*, int, int, int, int, int, int, int, int);
typedef void(MveSetPaletteFunc)(unsigned char* palette, int start, int count);

void MveSetMemory(MveMallocFunc* malloc_func, MveFreeFunc* free_func);
void MveSetIO(MveReadFunc* read_func);
void MveSetVolume(int volume);
void movieLibSetPan(int pan);
void _MVE_sfSVGA(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
void MveSetShowFrame(MveShowFrameFunc* proc);
void MveSetPalette(MveSetPaletteFunc* set_palette_func);
void _MVE_rmFrameCounts(int* frame_count_ptr, int* frame_drop_count_ptr);
int MVE_rmPrepMovie(void* handle, int dx, int dy, unsigned char track);
int _MVE_rmStepMovie();
void _MVE_rmEndMovie();
void _MVE_ReleaseMem();

} // namespace fallout

#endif /* MOVIE_LIB_H */
