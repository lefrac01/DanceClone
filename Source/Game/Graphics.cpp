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
  backgroundImage(NULL),
  titleImage(NULL),
  ratingsImage(NULL),
  getReadyImage(NULL),
  tempArrowsImageSrc(NULL), 
  homeArrowsImage(NULL), 
  quarterArrowsImage(NULL), 
  eighthArrowsImage(NULL), 
  quarterTripletArrowsImage(NULL), 
  sixteenthArrowsImage(NULL), 
  eighthTripletArrowsImage(NULL), 
  thirtysecondArrowsImage(NULL), 
  sixteenthTripletArrowsImage(NULL), 
  sixtyfourthArrowsImage(NULL), 
  sixtyfourthTripletArrowsImage(NULL), 
  freezeArrowsBodyImage(NULL),
  freezeArrowsTailImage(NULL),
  freezeArrowsHeadImage(NULL),
  marvellousHitImage(NULL), 
  perfectHitImage(NULL), 
  greatHitImage(NULL), 
  goodHitImage(NULL), 
  freezeHitImage(NULL),
  arrowWidth(64),
  arrowHeight(64)
{
}

bool Graphics::Init(string configFilePath)
{
  LOG(DEBUG_MINOR, "DanceClone::Graphics::Init(" << configFilePath << ")" << endl)

  screenWidth = sys.vid.ScreenWidth();
  screenHeight = sys.vid.ScreenHeight();
  
  constants.goalOffset = screenHeight / 4;
  
  sdlWhite = SDL_MapRGB(sys.vid.screen->format, 255, 255, 255);
  sdlBlack = SDL_MapRGB(sys.vid.screen->format, 0, 0, 0);
  sdlGrey =  SDL_MapRGB(sys.vid.screen->format, 150, 150, 150);
  sdlTeal =  SDL_MapRGB(sys.vid.screen->format, 200, 255, 250);
  sdlRed =   SDL_MapRGB(sys.vid.screen->format, 255, 0, 0);
  sdlGreen = SDL_MapRGB(sys.vid.screen->format, 0, 255, 0);
  sdlBlue  = SDL_MapRGB(sys.vid.screen->format, 0, 0, 255);
  sdlYellow= SDL_MapRGB(sys.vid.screen->format, 255, 255, 0);
  sdlCyan  = SDL_MapRGB(sys.vid.screen->format, 0, 255, 255);  

  
  constants.pixelsPerMsAt1Bpm = screenHeight / 4000.0 / 120.0; // calculate once screen height is known
// based on the observed rate of 4000ms per screen height per quarter note
// at 120 BPM

  try
  {
    string backgroundImagePath = "Media/Game/background.png";
    string titleImagePath = "Media/Game/title.png";
    string getReadyImagePath = "Media/Game/getready.png";
    string difficultyCursorImagePath = "Media/Game/diff_cursors.png";
  
    //TODO: fix hard-coded constants
    backgroundImage = sys.vid.LoadOptimizeAlpha(backgroundImagePath.c_str());
    if (!backgroundImage) LOG(DEBUG_BASIC, "failed to load \"" << backgroundImagePath << "\"" << endl)

    titleImage = sys.vid.LoadOptimizeAlpha(titleImagePath.c_str());
    if (!titleImage) LOG(DEBUG_BASIC, "failed to load \"" << titleImagePath << "\"" << endl)
    
    getReadyImage = sys.vid.LoadOptimizeAlpha(getReadyImagePath.c_str());
    if (!getReadyImage) LOG(DEBUG_BASIC, "failed to load \"" << getReadyImagePath << "\"" << endl)
    
    difficultyCursorImage = sys.vid.LoadOptimizeAlpha(difficultyCursorImagePath.c_str());
    if (!getReadyImage) LOG(DEBUG_BASIC, "failed to load \"" << difficultyCursorImagePath << "\"" << endl)
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

    
    // expand source PNGs creating other arrow directions
    //optimistically using SDL_HWSURFACE but not all these can fit into the hardware memory ;)
    //http://en.wikipedia.org/wiki/Wii states 3MB framebuffer of which 1.2 is used per screen + dbl buffering... not leaving much
    homeArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/home_arrows.png", homeArrowsImage, arrowsFrames, 4);

    quarterArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/04thnote.png", quarterArrowsImage, arrowsFrames, 4);

    eighthArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/08thnote.png", eighthArrowsImage, arrowsFrames, 4);

    quarterTripletArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/12thnote.png", quarterTripletArrowsImage, arrowsFrames, 4);

    sixteenthArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/16thnote.png", sixteenthArrowsImage, arrowsFrames, 4);

    eighthTripletArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/24thnote.png", eighthTripletArrowsImage, arrowsFrames, 4);

    thirtysecondArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/32ndnote.png", thirtysecondArrowsImage, arrowsFrames, 4);

    sixteenthTripletArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/48thnote.png", sixteenthTripletArrowsImage, arrowsFrames, 4);

    sixtyfourthArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/64thnote.png", sixtyfourthArrowsImage, arrowsFrames, 4);

    //NOTE: same graphic used for 96th and 192nd notes
    sixtyfourthTripletArrowsImage = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/192ndnote.png", sixtyfourthTripletArrowsImage, arrowsFrames, 4);


    // hit animations, 1 row 8 col source to 8 row 8 col dest
    marvellousHitImage = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/marvelloushit.png", marvellousHitImage, arrowsHitFrames, 8);

    perfectHitImage = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/perfecthit.png", perfectHitImage, arrowsHitFrames, 8);

    greatHitImage = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/greathit.png", greatHitImage, arrowsHitFrames, 8);

    goodHitImage = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/goodhit.png", goodHitImage, arrowsHitFrames, 8);


    // freeze arrow graphics do not use the same source layout nor are they animated
    string freezeHitImagePath = "Media/Game/freezehit.png";
    string freezeHeadImagePath = "Media/Game/freezehead.png";
    string freezeBodyImagePath = "Media/Game/freezebody.png";
    string freezeTailImagePath = "Media/Game/freezetail.png";
 
    //freezeHitImage = IMG_Load("Media/Game/freezehit.png");
    //freezeArrowsHeadImage = IMG_Load("Media/Game/freezehead.png");
    freezeHitImage = sys.vid.LoadOptimizeAlpha(freezeHitImagePath.c_str());
    if (!freezeHitImage) LOG(DEBUG_BASIC, "failed to load \"" << freezeHitImagePath << "\"" << endl)
    freezeArrowsHeadImage = sys.vid.LoadOptimizeAlpha(freezeHeadImagePath.c_str());
    if (!freezeArrowsHeadImage) LOG(DEBUG_BASIC, "failed to load \"" << freezeHeadImagePath << "\"" << endl)

    for(int b = 0; b < 4; b++)
    {
      for(int a = 0; a < 4; a++)
      {
        freezeHeadFrames[b*4 + a].x = a*64; freezeHeadFrames[b*4 + a].y = b*64;
        freezeHeadFrames[b*4 + a].w = 64;   freezeHeadFrames[b*4 + a].h = 64;
      }
    }
    //freezeArrowsBodyImage = IMG_Load("Media/Game/freezebody.png");
    //freezeArrowsTailImage = IMG_Load("Media/Game/freezetail.png");
    freezeArrowsBodyImage = sys.vid.LoadOptimizeAlpha(freezeBodyImagePath.c_str());
    if (!freezeArrowsBodyImage) LOG(DEBUG_BASIC, "failed to load \"" << freezeBodyImagePath << "\"" << endl)
    freezeArrowsTailImage = sys.vid.LoadOptimizeAlpha(freezeTailImagePath.c_str());
    if (!freezeArrowsTailImage) LOG(DEBUG_BASIC, "failed to load \"" << freezeTailImagePath << "\"" << endl)
    
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
    /*
    ratingsimage = IMG_Load("Media/Game/ratings.png");
    for(int a = 0; a < 3; a++)
    {
      ratingsframes[a].x = 0;     ratingsframes[a].y = a*50;
      ratingsframes[a].w = 125;   ratingsframes[a].h = 50;
    }
    */
  }
  catch (exception& e)
  {
    throw e;
  }
  
  return (backgroundImage && titleImage && getReadyImage && freezeHitImage && freezeArrowsHeadImage && freezeArrowsBodyImage && freezeArrowsTailImage && difficultyCursorImage);
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
void Graphics::ExpandArrows(string source_file, SDL_Surface* dest, SDL_Rect* dest_frames, int src_cols)
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
    //error_log.open("errors", std::ios_base::app);
    //error_log << "failed to load graphics resource \"" << source_file.c_str() << "\"" << endl;
    //error_log.close();
    string exceptionDetail = "ExpandArrows() failed to load file \"";
    exceptionDetail += source_file;
    exceptionDetail += "\"";
    throw exceptionDetail.c_str();
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
  
  // for each of the arrows to rotate (columns 0 to 3)
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
}

void Graphics::Cleanup()
{
  if (backgroundImage)
  {
    SDL_FreeSurface(backgroundImage);
    backgroundImage = NULL;
  }
  if (titleImage)
  {
    SDL_FreeSurface(titleImage);
    titleImage = NULL;
  }
  if (ratingsImage)
  {
    SDL_FreeSurface(ratingsImage);
    ratingsImage = NULL;
  }
  if (getReadyImage)
  {
    SDL_FreeSurface(getReadyImage);
    getReadyImage = NULL;
  }
  if (tempArrowsImageSrc)
  {
    SDL_FreeSurface(tempArrowsImageSrc);
    tempArrowsImageSrc = NULL;
  }
  if (homeArrowsImage)
  {
    SDL_FreeSurface(homeArrowsImage);
    homeArrowsImage = NULL;
  }
  if (quarterArrowsImage)
  {
    SDL_FreeSurface(quarterArrowsImage);
    quarterArrowsImage = NULL;
  }
  if (eighthArrowsImage)
  {
    SDL_FreeSurface(eighthArrowsImage);
    eighthArrowsImage = NULL;
  }
  if (quarterTripletArrowsImage)
  {
    SDL_FreeSurface(quarterTripletArrowsImage);
    quarterTripletArrowsImage = NULL;
  }
  if (sixteenthArrowsImage)
  {
    SDL_FreeSurface(sixteenthArrowsImage);
    sixteenthArrowsImage = NULL;
  }
  if (eighthTripletArrowsImage)
  {
    SDL_FreeSurface(eighthTripletArrowsImage);
    eighthTripletArrowsImage = NULL;
  }
  if (thirtysecondArrowsImage)
  {
    SDL_FreeSurface(thirtysecondArrowsImage);
    thirtysecondArrowsImage = NULL;
  }
  if (sixteenthTripletArrowsImage)
  {
    SDL_FreeSurface(sixteenthTripletArrowsImage);
    sixteenthTripletArrowsImage = NULL;
  }
  if (sixtyfourthArrowsImage)
  {
    SDL_FreeSurface(sixtyfourthArrowsImage);
    sixtyfourthArrowsImage = NULL;
  }
  if (sixtyfourthTripletArrowsImage)
  {
    SDL_FreeSurface(sixtyfourthTripletArrowsImage);
    sixtyfourthTripletArrowsImage = NULL;
  }
  if (freezeArrowsBodyImage)
  {
    SDL_FreeSurface(freezeArrowsBodyImage);
    freezeArrowsBodyImage = NULL;
  }
  if (freezeArrowsTailImage)
  {
    SDL_FreeSurface(freezeArrowsTailImage);
    freezeArrowsTailImage = NULL;
  }
  if (freezeArrowsHeadImage)
  {
    SDL_FreeSurface(freezeArrowsHeadImage);
    freezeArrowsHeadImage = NULL;
  }
  if (marvellousHitImage)
  {
    SDL_FreeSurface(marvellousHitImage);
    marvellousHitImage = NULL;
  }
  if (perfectHitImage)
  {
    SDL_FreeSurface(perfectHitImage);
    perfectHitImage = NULL;
  }
  if (greatHitImage)
  {
    SDL_FreeSurface(greatHitImage);
    greatHitImage = NULL;
  }
  if (goodHitImage)
  {
    SDL_FreeSurface(goodHitImage);
    goodHitImage = NULL;
  }
  if (freezeHitImage)
  {
    SDL_FreeSurface(freezeHitImage);
    freezeHitImage = NULL;
  }
}

void Graphics::DrawBackground()
{
  sys.vid.ApplySurface(0, 0, backgroundImage, sys.vid.screen, NULL);
}

}
