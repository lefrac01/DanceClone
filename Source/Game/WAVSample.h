//      WAVSample.h
//      
//      Copyright 2012 Carl Lefrançois <carl.lefrancois@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "../Platform/LOG.H"
#include "../Platform/OS.h"
#include "../Platform/Tools.h"

// ENTIRE FILE IS USED ONLY ON WII

#include "malloc.h"
#ifdef WII
#include <asndlib.h>
#else
// note WIN32 may need special definition here
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define VOICE_MONO_8BIT       0
#define VOICE_MONO_16BIT      1
#define VOICE_STEREO_8BIT     2
#define VOICE_STEREO_16BIT    3
#endif

namespace DanceClone
{

class WAVSample
{
public:

struct WAVHeader
{
  char tag[4];      // e.g. "RIFF"  - big-endian form
  u32 nextChunkSize;
  char type[4];       // Contains the letters "WAVE"  - big-endian form
};


struct WAVFormatHeader
{
  char tag[4];
  u32 fmtLength;
  u16 waveType;
  u16 channels;
  u32 sampleRate;
  u32 bytesPerSecond;
  u16 blockAlignment;
  u16 bitResolution;
};

struct WAVDataHeader
{
  char tag[4];    // "data"  -  big-endian form
  u32 length;
};

private:

  
public:
  WAVSample();
  ~WAVSample();

  u8* data;
  u32 length;
  u8  channelCount;
  u32 sampleRate;
  u32 bitsPerSample;
  bool initialised;
  
  bool Load(string path);
};

}
