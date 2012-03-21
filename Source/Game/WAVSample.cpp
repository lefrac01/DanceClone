//      WAVSample.cpp
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

#include "WAVSample.h"

// ENTIRE FILE IS USED ONLY ON WII

namespace DanceClone
{

WAVSample::WAVSample() :
data(NULL),
initialised(false)
{
}

WAVSample::~WAVSample()
{
  if (data)
  {
    free(data);
  }
}

bool WAVSample::Load(string path)
{
  #ifdef LINUX
  bool doSwap = false;
  #endif
  #ifdef WII
  bool doSwap = true;
  #endif
  FILE* f;
  f = fopen(path.c_str(), "rb");
  if (!f)
  {
    return false;
  }
  
  WAVHeader header;
  size_t sizeRead = 0;
  sizeRead = fread(&header, sizeof(WAVHeader), 1, f);
  
  if (sizeRead == sizeof(WAVHeader) && strncmp(header.tag, "RIFF", 4) != 0)
  {
    LOG(DEBUG_BASIC, "did not find RIFF header when reading file " << path << endl)
    return false;
  }
  if (strncmp(header.type, "WAVE", 4) != 0)
  {
    LOG(DEBUG_BASIC, "did not find WAVE type reading file " << path << endl)
    return false;
  }
  
  WAVFormatHeader format;
  sizeRead = fread(&format, sizeof(WAVFormatHeader), 1, f);
  if (sizeRead == sizeof(WAVFormatHeader) && strncmp(format.tag, "fmt ", 4) != 0)
  {
    LOG(DEBUG_BASIC, "unrecognized WAV format header while reading file " << path << endl)
    return false;
  }

  if (doSwap)   // ⁾  !??
  {
    SwapByteOrder(format.bitResolution);
    SwapByteOrder(format.channels);
    SwapByteOrder(format.sampleRate);
  }
  
  sampleRate = format.sampleRate;
  bitsPerSample = format.bitResolution;

  
  WAVDataHeader dataHeader;
  sizeRead = fread(&dataHeader, sizeof(WAVDataHeader), 1, f);
  if (sizeRead == sizeof(WAVDataHeader) && strncmp(dataHeader.tag, "data", 4) != 0)
  {
    LOG(DEBUG_BASIC, "unrecognized WAV data header while reading file " << path << endl)
    return false;
  }

  if (doSwap)   // ⁾  !??
  {
    SwapByteOrder(dataHeader.length);
  }
  length = dataHeader.length;

  LOG(DEBUG_MINOR, "WAV: " << path << " sample rate: " << sampleRate << " bps: " << bitsPerSample << " length: " << length << endl)

  
  
  if (data)
  {
    free(data);
    data = NULL;
  }
  
  data = (u8*)memalign(32, length);
  sizeRead = fread(data, 1, length, f);
  if (sizeRead != length)
  {
    LOG(DEBUG_BASIC, "file shorter than indicated (wanted " << length << " got " << sizeRead << " while reading file " << path << endl)
    free(data);
    data = NULL;
    return false;
  }

  if (bitsPerSample == 16 )
  {
    channelCount = format.channels == 1 ? VOICE_MONO_16BIT : VOICE_STEREO_16BIT;
    if (doSwap)
    {
      // swap hiss for music ;)
      u16* swapper = (u16*)data;
      for (unsigned long i = 0; i < length/2; i++)
      {
        SwapByteOrder(*(swapper+i));
      }
    }
  }
  else
  {
    channelCount = format.channels == 1 ? VOICE_MONO_8BIT : VOICE_STEREO_8BIT;
  }

  initialised = true;
  
  return true;
}

}
