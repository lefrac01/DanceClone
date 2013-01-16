//      Graphics.cpp
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

#include "Graphics.h"

namespace DanceClone
{

Graphics::Graphics(OS& os) :
  sys(os),
  images(NUM_IMAGES),
  arrowWidth(64),
  arrowHeight(64)
{
  std::fill(images.begin(), images.end(), (SDL_Surface*)NULL);
}

bool Graphics::Init(string configFilePath)
{
  LOG(DEBUG_MINOR, "DanceClone::Graphics::Init(" << configFilePath << ")" << endl)

  screenWidth = sys.vid.ScreenWidth();
  screenHeight = sys.vid.ScreenHeight();
  
  constants.goalOffset = screenHeight / 4;
  
  sdlWhite = SDL_MapRGBA(sys.vid.screen->format, 255, 255, 255, 255);
  sdlBlack = SDL_MapRGBA(sys.vid.screen->format, 0, 0, 0, 255);
  sdlDarkGrey =  SDL_MapRGBA(sys.vid.screen->format, 64, 64, 64, 255);
  sdlGrey =  SDL_MapRGBA(sys.vid.screen->format, 150, 150, 150, 255);
  sdlLightGrey =  SDL_MapRGBA(sys.vid.screen->format, 200, 200, 200, 255);
  sdlTeal =  SDL_MapRGBA(sys.vid.screen->format, 200, 255, 250, 255);
  sdlRed =   SDL_MapRGBA(sys.vid.screen->format, 255, 0, 0, 255);
  sdlDarkRed =   SDL_MapRGBA(sys.vid.screen->format, 128, 0, 0, 255);
  sdlGreen = SDL_MapRGBA(sys.vid.screen->format, 0, 255, 0, 255);
  sdlMediumGreen = SDL_MapRGBA(sys.vid.screen->format, 0, 192, 0, 255);
  sdlDarkGreen = SDL_MapRGBA(sys.vid.screen->format, 0, 128, 0, 255);
  sdlLightGreen = SDL_MapRGBA(sys.vid.screen->format, 155, 255, 145, 255);
  sdlBlue  = SDL_MapRGBA(sys.vid.screen->format, 0, 0, 255, 255);
  sdlYellow= SDL_MapRGBA(sys.vid.screen->format, 255, 255, 0, 255);
  sdlCyan  = SDL_MapRGBA(sys.vid.screen->format, 0, 255, 255, 255);  
  sdlLightBlue  = SDL_MapRGBA(sys.vid.screen->format, 160, 240, 255, 255);
  sdlBlueWhite  = SDL_MapRGBA(sys.vid.screen->format, 210, 240, 255, 255);

  
  constants.pixelsPerMsAt1Bpm = screenHeight / 4000.0 / 120.0; // calculate once screen height is known
// based on the observed rate of 4000ms per screen height per quarter note
// at 120 BPM

  string backgroundImagePath = "Media/Game/background.png";
  string songSelectBackgroundImagePath = "Media/Game/song_select_bg.png";
  string scoreBackgroundImagePath = "Media/Game/scorebg.png";
  string titleImagePath = "Media/Game/title.png";
  string comboLabelImagePath = "Media/Game/combolabel.png";
  string comboNumbersImagePath = "Media/Game/combonumbers.png";
  string okNgImagePath = "Media/Game/okng.png";
  string RatingsImagePath = "Media/Game/ratings.png";
  string getReadyImagePath = "Media/Game/getready.png";
  string defaultBannerImagePath = "Media/Game/mp3_no_banner_blue.png";
  string difficultyCursorImagePath = "Media/Game/diff_cursors.png";
  

  //TODO: fix hard-coded constants
  images[DefaultBg] = sys.vid.LoadOptimize(backgroundImagePath.c_str());
  if (!images[DefaultBg]) LOG(DEBUG_BASIC, "failed to load \"" << backgroundImagePath << "\"" << endl)
if(images[DefaultBg] && (images[DefaultBg]->flags & SDL_HWSURFACE) == SDL_HWSURFACE)
{
  LOG(DEBUG_BASIC, "created images[DefaultBg] from " << backgroundImagePath << ".  it is a HW surface" << endl)
}
else
{
  LOG(DEBUG_BASIC, "created images[DefaultBg] from " << backgroundImagePath << ".  it is a sw surface" << endl)
}

  images[SongSelectBg] = sys.vid.LoadOptimize(songSelectBackgroundImagePath.c_str());
  if (!images[SongSelectBg]) LOG(DEBUG_BASIC, "failed to load \"" << songSelectBackgroundImagePath << "\"" << endl)

  images[ScoreBg] = sys.vid.LoadOptimizeAlpha(scoreBackgroundImagePath.c_str());
  if (!images[ScoreBg]) LOG(DEBUG_BASIC, "failed to load \"" << scoreBackgroundImagePath << "\"" << endl)

  images[Title] = sys.vid.LoadOptimizeAlpha(titleImagePath.c_str());
  if (!images[Title]) LOG(DEBUG_BASIC, "failed to load \"" << titleImagePath << "\"" << endl)

  images[ComboLabel] = sys.vid.LoadOptimizeAlpha(comboLabelImagePath.c_str());
  if (!images[ComboLabel]) LOG(DEBUG_BASIC, "failed to load \"" << comboLabelImagePath << "\"" << endl)

  images[ComboNumbers] = sys.vid.LoadOptimizeAlpha(comboNumbersImagePath.c_str());
  if (!images[ComboLabel]) LOG(DEBUG_BASIC, "failed to load \"" << comboNumbersImagePath << "\"" << endl)

  images[OkNg] = sys.vid.LoadOptimizeAlpha(okNgImagePath.c_str());
  if (!images[OkNg]) LOG(DEBUG_BASIC, "failed to load \"" << okNgImagePath << "\"" << endl)

  images[Ratings] = sys.vid.LoadOptimizeAlpha(RatingsImagePath.c_str());
  if (!images[Ratings]) LOG(DEBUG_BASIC, "failed to load \"" << RatingsImagePath << "\"" << endl)

  for(int a = 0; a < 10; a++)
  {
    comboNumbersFrames[a].x = a*22;   comboNumbersFrames[a].y = 0;
    comboNumbersFrames[a].w = 22;     comboNumbersFrames[a].h = 33;
  }

  for(int a = 0; a < 2; a++)
  {
    okNgFrames[a].x = 0;        okNgFrames[a].y = a*16;
    okNgFrames[a].w = 36;       okNgFrames[a].h = 16;
  }

  for(int a = 0; a < 7; a++)
  {
    ratingsFrames[a].x = 0;     ratingsFrames[a].y = a*32;
    ratingsFrames[a].w = 92;    ratingsFrames[a].h = 32;
  }
  

  
  images[GetReady] = sys.vid.LoadOptimizeAlpha(getReadyImagePath.c_str());
  if (!images[GetReady]) LOG(DEBUG_BASIC, "failed to load \"" << getReadyImagePath << "\"" << endl)
  
  images[DefaultBanner] = sys.vid.LoadOptimize(defaultBannerImagePath.c_str());
  if (!images[DefaultBanner])
  {
    LOG(DEBUG_BASIC, "failed to load \"" << defaultBannerImagePath << "\"" << endl)
  }
  else
  {
    images[DefaultBannerMini] = zoomSurface(images[DefaultBanner], constants.bannerMiniZoomFactor, constants.bannerMiniZoomFactor, 0);
  }
  
  images[DifficultyCursor] = sys.vid.LoadOptimizeAlpha(difficultyCursorImagePath.c_str());
  if (!images[DifficultyCursor]) LOG(DEBUG_BASIC, "failed to load \"" << difficultyCursorImagePath << "\"" << endl)
  for(int r = 0; r < 2; r++)
  {
    for (int c = 0; c < 2; c++)
    {
      difficultyCursorFrames[r*2+c].x = c*32;
      difficultyCursorFrames[r*2+c].y = r*32;
      difficultyCursorFrames[r*2+c].w = 32;
      difficultyCursorFrames[r*2+c].h = 32;
    }
  }

  try
  {
    // expand source PNGs creating other arrow directions
    //optimistically using SDL_HWSURFACE but not all these can fit into the hardware memory ;)
    //http://en.wikipedia.org/wiki/Wii states 3MB framebuffer of which 1.2 is used per screen + dbl buffering... not leaving much
    images[HomeArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 384, 384, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/home_arrows.png", images[HomeArrows], homeArrowsFrames, 6, false);


    images[QuarterArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/04thnote.png", images[QuarterArrows], arrowsFrames, 4, true);

//    images[QuarterArrows_0_5] = SetupAntialiasedArrows(images[QuarterArrows]);

    images[EighthArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/08thnote.png", images[EighthArrows], arrowsFrames, 4, true);

    images[QuarterTripletArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/12thnote.png", images[QuarterTripletArrows], arrowsFrames, 4, true);

    images[SixteenthArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/16thnote.png", images[SixteenthArrows], arrowsFrames, 4, true);

    images[EighthTripletArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/24thnote.png", images[EighthTripletArrows], arrowsFrames, 4, true);

    images[ThirtysecondArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/32ndnote.png", images[ThirtysecondArrows], arrowsFrames, 4, true);

    images[SixteenthTripletArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/48thnote.png", images[SixteenthTripletArrows], arrowsFrames, 4, true);

    images[SixtyfourthArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/64thnote.png", images[SixtyfourthArrows], arrowsFrames, 4, true);

    //NOTE: same graphic used for 96th and 192nd notes
    images[SixtyfourthTripletArrows] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/192ndnote.png", images[SixtyfourthTripletArrows], arrowsFrames, 4, true);


    // hit animations, 1 row 8 col source to 8 row 8 col dest
    images[MarvellousHit] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/marvelloushit.png", images[MarvellousHit], arrowsHitFrames, 8, false);

    images[PerfectHit] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/perfecthit.png", images[PerfectHit], arrowsHitFrames, 8, false);

    images[GreatHit] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/greathit.png", images[GreatHit], arrowsHitFrames, 8, false);

    images[GoodHit] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/goodhit.png", images[GoodHit], arrowsHitFrames, 8, false);
  }
  catch(string e)
  {
    LOG(DEBUG_BASIC, e << endl)
  }

  // freeze arrow graphics do not use the same source layout nor are they animated
  string comboHitImagePath = "Media/Game/freezehit.png";
  string freezeHeadImagePath = "Media/Game/freezehead.png";
  string freezeBodyImagePath = "Media/Game/freezebody.png";
  string freezeTailImagePath = "Media/Game/freezetail.png";

  images[ComboHit] = sys.vid.LoadOptimizeAlpha(comboHitImagePath.c_str());
  if (!images[ComboHit]) LOG(DEBUG_BASIC, "failed to load \"" << comboHitImagePath << "\"" << endl)
  images[FreezeArrowsHead] = sys.vid.LoadOptimizeAlpha(freezeHeadImagePath.c_str());
  if (!images[FreezeArrowsHead]) LOG(DEBUG_BASIC, "failed to load \"" << freezeHeadImagePath << "\"" << endl)

  for(int b = 0; b < 4; b++)
  {
    for(int a = 0; a < 4; a++)
    {
      freezeHeadFrames[b*4 + a].x = a*64; freezeHeadFrames[b*4 + a].y = b*64;
      freezeHeadFrames[b*4 + a].w = 64;   freezeHeadFrames[b*4 + a].h = 64;
    }
  }
  images[FreezeArrowsBody] = sys.vid.LoadOptimizeAlpha(freezeBodyImagePath.c_str());
  if (!images[FreezeArrowsBody]) LOG(DEBUG_BASIC, "failed to load \"" << freezeBodyImagePath << "\"" << endl)
  images[FreezeArrowsTail] = sys.vid.LoadOptimizeAlpha(freezeTailImagePath.c_str());
  if (!images[FreezeArrowsTail]) LOG(DEBUG_BASIC, "failed to load \"" << freezeTailImagePath << "\"" << endl)
  
  for(int b = 0; b < 3; b++)
  {
    for(int a = 0; a < 4; a++)
    {
      freezeBodyFrames[b*4 + a].x = a*64; freezeBodyFrames[b*4 + a].y = b*128;
      freezeBodyFrames[b*4 + a].w = 64;   freezeBodyFrames[b*4 + a].h = 128;
    }
    for(int a = 0; a < 4; a++)
    {
      freezeTailFrames[b*4 + a].x = a*64; freezeTailFrames[b*4 + a].y = b*64;
      freezeTailFrames[b*4 + a].w = 64;   freezeTailFrames[b*4 + a].h = 64;
    }
  }
  
  bool imageNull = false;
  for (int i = 0; i < NUM_IMAGES; i++)
  {
    if (!images[i]) 
    {
      imageNull = true;
      LOG(DEBUG_BASIC, "Graphics::Init() returning null due to false image at index " << i << endl)
      break;
    }
  }
  
  return !imageNull;
}

void Graphics::SetArrowFrame(SDL_Rect* dest, int index, int x, int y, int w, int h)
{
  dest[index].x = x;
  dest[index].y = y;
  dest[index].w = w;
  dest[index].h = h;
}


// create other directions from the source single-direction arrows
// the expected source format is:
// 4 arrows pointing down,  64x64x32bit png each,
// arranged horizontally in 256w x 64h rectangle, with each column
// being an animation frame for one tick.  tick x = column x
void Graphics::ExpandArrows(string source_file, SDL_Surface* dest, SDL_Rect* dest_frames, int src_cols, bool colourkeyAlpha)
{
  int aw = arrowWidth;
  int ah = arrowHeight;
  
  // source file format is 32 bit RGBA and rotation function also expects 32 bit
  SDL_Surface* temp_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, aw, ah, 32,
                    0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);

  SDL_SetAlpha(temp_surface, 0, SDL_ALPHA_OPAQUE);  // force SDL_SRCALPHA to off, 
  // enabled because temp_surface is created with an alpha channel (necessary to 
  // retain the alpha info from source file)
  SDL_Surface* temp_surface_rotated = NULL;

  SDL_Surface* temp_src = IMG_Load(source_file.c_str());
  if (!temp_src)
  {
    string exceptionDetail = "ExpandArrows() failed to load file \"";
    exceptionDetail += source_file;
    exceptionDetail += "\"";
    throw exceptionDetail;
  }
  // disable use of alpha channel in source surface since an ordinary opaque blit is wanted
  SDL_SetAlpha(temp_src, 0, SDL_ALPHA_OPAQUE);

  SDL_Rect resource_src;
  SDL_Rect rotation_src;
  SDL_Rect rotation_dest;

  rotation_src.x = 0;
  rotation_src.y = 0;
  rotation_src.w = aw; 
  rotation_src.h = ah;  
  
  // for each of the arrows to rotate
  for (int col = 0; col < src_cols; col++)
  {
    // set up blit rectangles
    resource_src.x = col * aw;
    resource_src.y = 0;
    resource_src.w = aw;
    resource_src.h = ah;
    
    // obtain a working copy of the arrow to rotate
    SDL_BlitSurface(temp_src, &resource_src, temp_surface, &rotation_src);
    
    rotation_dest.x = col * aw;
    rotation_dest.y = 0;
    rotation_dest.w = aw;
    rotation_dest.h = ah;

    for (int k=0; k<4; k++)
    {
      // rotate
      temp_surface_rotated = rotateSurface90Degrees(temp_surface, k);

      // blit this on.
      rotation_dest.y = k*ah;
    
      // turn off source alpha of rotated bitmap for dumb copy to staging surface
      SDL_SetAlpha(temp_surface_rotated, 0, SDL_ALPHA_OPAQUE);
      SDL_BlitSurface(temp_surface_rotated, &rotation_src, dest, &rotation_dest);

      // and record the arrow frame rect
      SetArrowFrame(dest_frames, k*src_cols+col, col*aw, k*ah, aw, ah);
      
      // delete temp surface
      SDL_FreeSurface(temp_surface_rotated);
    }
  }
  
  SDL_FreeSurface(temp_surface);
  SDL_FreeSurface(temp_src);

  //#SDL_Surface* temp = dest;
  //#dest = SDL_DisplayFormat(dest);
  //#SDL_FreeSurface(temp);
  if (colourkeyAlpha)
  {
    Uint32 colourkey = SDL_MapRGB(dest->format, 0xFF, 0x00, 0xFF);
    SDL_SetColorKey(dest, SDL_SRCCOLORKEY, colourkey);
    SDL_SetAlpha(dest, SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
  }
}

/*
functions correctly but does not create a noticeable difference!

SDL_Surface* Graphics::SetupAntialiasedArrows(SDL_Surface* src)
{
  // create a new surface whose contents are a copy of the 
  // source surface shifted down a half-pixel.
  
  // 1)
  // create a copy of the source surface using the display's format.
  
  // 2)
  // iterate over each row of the copy starting at the bottom row.  for each
  // pixel, replace it with a blend of the current value and the value in 
  // the row above.  at each step, consider three cases of blending.  if no pixel has
  // the colourkey colour, blend both pixels, if both have colourkey colour,
  // write colourkey colour, if one has colourkey colour, blend the other
  // with grey.
  
  // 3)
  // fill top line of pixels in dest with colorkey colour.
  
  // 1)
  SDL_Surface* temp = SDL_DisplayFormat(src);
  
  // 2)
  Uint16* pBase = (Uint16*)temp->pixels;
  Uint16* pCurr;
  Uint16* pAbove;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 rAbove;
  Uint8 gAbove;
  Uint8 bAbove;
  Uint32 colourkey = SDL_MapRGB(temp->format, 255, 0, 255);
  bool isColourkey;
  bool aboveIsColourkey;
  
  int h = temp->h;
  int w = temp->w;
  for (int y = h - 1; y > 0; --y)
  {
    pCurr = pBase + y*w;
    pAbove = pBase + (y-1)*w;
    for (int x = 0; x < w; ++x)
    {
      Uint32 color = *pCurr;
      isColourkey = color == colourkey;
      SDL_GetRGB(color, temp->format, &r, &g, &b);

      Uint32 colorAbove = *pAbove;
      aboveIsColourkey = colorAbove == colourkey;
      SDL_GetRGB(colorAbove, temp->format, &rAbove, &gAbove, &bAbove);
      
      
      if (isColourkey && aboveIsColourkey)
      ;  // since it is already colourkey, leave it
      else if (isColourkey && !aboveIsColourkey)
      {
        // result = above + grey
        *pCurr = SDL_MapRGB(temp->format, (rAbove+128)/2, 
                                          (gAbove+128)/2, 
                                          (bAbove+128)/2);
      }
      else if (!isColourkey && aboveIsColourkey)
      {
        // result = current + grey
        *pCurr = SDL_MapRGB(temp->format, (r+128)/2, 
                                          (g+128)/2, 
                                          (b+128)/2);
      }
      else
      {
        // result = current + above
        *pCurr = SDL_MapRGB(temp->format, (rAbove+r)/2, 
                                          (gAbove+g)/2, 
                                          (bAbove+b)/2);
      }
      
      // advance to next pixel
      ++pCurr;
      ++pAbove;
    }
  }
 
  // 3)
  // do top row
  pCurr = pBase;
  for (int x = 0; x < w; ++x)
  {
    *pCurr =  (255 >> 3 << 11) |
              (0   >> 2 << 5 ) |
              (255 >> 3 << 0 );
    ++pCurr;
  }

  return temp;
}
*/

void Graphics::Cleanup()
{
  for (int i = 0; i < NUM_IMAGES; i++)
  {
    if (images[i]) 
    {
      SDL_FreeSurface(images[i]);
      images[i] = NULL;
    }
  }
}

}
