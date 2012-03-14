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
  background_image(NULL),
  title_image(NULL),
  ratings_image(NULL),
  get_ready_image(NULL),
  temp_arrows_image_src(NULL), 
  home_arrows_image(NULL), 
  quarter_arrows_image(NULL), 
  eighth_arrows_image(NULL), 
  quarter_triplet_arrows_image(NULL), 
  sixteenth_arrows_image(NULL), 
  eighth_triplet_arrows_image(NULL), 
  thirtysecond_arrows_image(NULL), 
  sixteenth_triplet_arrows_image(NULL), 
  sixtyfourth_arrows_image(NULL), 
  sixtyfourth_triplet_arrows_image(NULL), 
  freeze_arrows_body_image(NULL),
  freeze_arrows_tail_image(NULL),
  freeze_arrows_head_image(NULL),
  marvellous_hit_image(NULL), 
  perfect_hit_image(NULL), 
  great_hit_image(NULL), 
  good_hit_image(NULL), 
  freeze_hit_image(NULL),
  arrowWidth(64),
  arrowHeight(64),
  pixelsPerMsAt1BPM(0.0)
{
}

bool Graphics::Init(string configFilePath)
{
  LOG(DEBUG_MINOR, "DanceClone::Graphics::Init(" << configFilePath << ")" << endl)

  screenWidth = sys.vid.ScreenWidth();
  screenHeight = sys.vid.ScreenHeight();
  
  goalOffset = screenHeight / 4;
  
  pixelsPerMsAt1BPM = screenHeight / 4000.0 / 120.0; // calculate once screen height is known
// based on the observed rate of 4000ms per screen height per quarter note
// at 120 BPM

  try
  {
    string backgroundImagePath = "Media/Game/background.png";
    string titleImagePath = "Media/Game/title.png";
    string getReadyImagePath = "Media/Game/getready.png";
  
    //TODO: fix hard-coded constants
    //background_image = sys.vid.LoadOptimize("Media/Game/background.png");
    background_image = sys.vid.LoadOptimizeAlpha(backgroundImagePath.c_str());
    if (!background_image) LOG(DEBUG_BASIC, "failed to load \"" << backgroundImagePath << "\"" << endl)

    //title_image = IMG_Load("Media/Game/title.png");
    title_image = sys.vid.LoadOptimizeAlpha(titleImagePath.c_str());
    if (!title_image) LOG(DEBUG_BASIC, "failed to load \"" << titleImagePath << "\"" << endl)
    
    //get_ready_image = IMG_Load("Media/Game/getready.png");
    get_ready_image = sys.vid.LoadOptimizeAlpha(getReadyImagePath.c_str());
    if (!get_ready_image) LOG(DEBUG_BASIC, "failed to load \"" << getReadyImagePath << "\"" << endl)
    
    // expand source PNGs creating other arrow directions
    //optimistically using SDL_HWSURFACE but not all these can fit into the hardware memory ;)
    //http://en.wikipedia.org/wiki/Wii states 3MB framebuffer of which 1.2 is used per screen + dbl buffering... not leaving much
    home_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/home_arrows.png", home_arrows_image, arrows_frames, 4);

    quarter_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/04thnote.png", quarter_arrows_image, arrows_frames, 4);

    eighth_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/08thnote.png", eighth_arrows_image, arrows_frames, 4);

    quarter_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/12thnote.png", quarter_triplet_arrows_image, arrows_frames, 4);

    sixteenth_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/16thnote.png", sixteenth_arrows_image, arrows_frames, 4);

    eighth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/24thnote.png", eighth_triplet_arrows_image, arrows_frames, 4);

    thirtysecond_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/32ndnote.png", thirtysecond_arrows_image, arrows_frames, 4);

    sixteenth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/48thnote.png", sixteenth_triplet_arrows_image, arrows_frames, 4);

    sixtyfourth_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/64thnote.png", sixtyfourth_arrows_image, arrows_frames, 4);

    //NOTE: same graphic used for 96th and 192nd notes
    sixtyfourth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/192ndnote.png", sixtyfourth_triplet_arrows_image, arrows_frames, 4);


    // hit animations, 1 row 8 col source to 8 row 8 col dest
    marvellous_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/marvelloushit.png", marvellous_hit_image, arrows_hit_frames, 8);

    perfect_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/perfecthit.png", perfect_hit_image, arrows_hit_frames, 8);

    great_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/greathit.png", great_hit_image, arrows_hit_frames, 8);

    good_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    ExpandArrows("Media/Game/goodhit.png", good_hit_image, arrows_hit_frames, 8);


    // freeze arrow graphics do not use the same source layout nor are they animated
    string freezeHitImagePath = "Media/Game/freezehit.png";
    string freezeHeadImagePath = "Media/Game/freezehead.png";
    string freezeBodyImagePath = "Media/Game/freezebody.png";
    string freezeTailImagePath = "Media/Game/freezetail.png";
 
    //freeze_hit_image = IMG_Load("Media/Game/freezehit.png");
    //freeze_arrows_head_image = IMG_Load("Media/Game/freezehead.png");
    freeze_hit_image = sys.vid.LoadOptimizeAlpha(freezeHitImagePath.c_str());
    if (!freeze_hit_image) LOG(DEBUG_BASIC, "failed to load \"" << freezeHitImagePath << "\"" << endl)
    freeze_arrows_head_image = sys.vid.LoadOptimizeAlpha(freezeHeadImagePath.c_str());
    if (!freeze_arrows_head_image) LOG(DEBUG_BASIC, "failed to load \"" << freezeHeadImagePath << "\"" << endl)

    for(int b = 0; b < 4; b++)
    {
      for(int a = 0; a < 4; a++)
      {
        freeze_head_frames[b*4 + a].x = a*64; freeze_head_frames[b*4 + a].y = b*64;
        freeze_head_frames[b*4 + a].w = 64;   freeze_head_frames[b*4 + a].h = 64;
      }
    }
    //freeze_arrows_body_image = IMG_Load("Media/Game/freezebody.png");
    //freeze_arrows_tail_image = IMG_Load("Media/Game/freezetail.png");
    freeze_arrows_body_image = sys.vid.LoadOptimizeAlpha(freezeBodyImagePath.c_str());
    if (!freeze_arrows_body_image) LOG(DEBUG_BASIC, "failed to load \"" << freezeBodyImagePath << "\"" << endl)
    freeze_arrows_tail_image = sys.vid.LoadOptimizeAlpha(freezeTailImagePath.c_str());
    if (!freeze_arrows_tail_image) LOG(DEBUG_BASIC, "failed to load \"" << freezeTailImagePath << "\"" << endl)
    
    for(int b = 0; b < 3; b++)
    {
      for(int a = 0; a < 4; a++)
      {
        freeze_body_frames[b*4 + a].x = a*64; freeze_body_frames[b*4 + a].y = b*128;
        freeze_body_frames[b*4 + a].w = 64;   freeze_body_frames[b*4 + a].h = 128;
      }
      for(int a = 0; a < 4; a++)
      {
        freeze_tail_frames[b*4 + a].x = a*64; freeze_tail_frames[b*4 + a].y = b*64;
        freeze_tail_frames[b*4 + a].w = 64;   freeze_tail_frames[b*4 + a].h = 64;
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
  
  return (background_image && title_image && get_ready_image && freeze_hit_image && freeze_arrows_head_image && freeze_arrows_body_image && freeze_arrows_tail_image);
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
  if (background_image)
  {
    SDL_FreeSurface(background_image);
    background_image = NULL;
  }
  if (title_image)
  {
    SDL_FreeSurface(title_image);
    title_image = NULL;
  }
  if (ratings_image)
  {
    SDL_FreeSurface(ratings_image);
    ratings_image = NULL;
  }
  if (get_ready_image)
  {
    SDL_FreeSurface(get_ready_image);
    get_ready_image = NULL;
  }
  if (temp_arrows_image_src)
  {
    SDL_FreeSurface(temp_arrows_image_src);
    temp_arrows_image_src = NULL;
  }
  if (home_arrows_image)
  {
    SDL_FreeSurface(home_arrows_image);
    home_arrows_image = NULL;
  }
  if (quarter_arrows_image)
  {
    SDL_FreeSurface(quarter_arrows_image);
    quarter_arrows_image = NULL;
  }
  if (eighth_arrows_image)
  {
    SDL_FreeSurface(eighth_arrows_image);
    eighth_arrows_image = NULL;
  }
  if (quarter_triplet_arrows_image)
  {
    SDL_FreeSurface(quarter_triplet_arrows_image);
    quarter_triplet_arrows_image = NULL;
  }
  if (sixteenth_arrows_image)
  {
    SDL_FreeSurface(sixteenth_arrows_image);
    sixteenth_arrows_image = NULL;
  }
  if (eighth_triplet_arrows_image)
  {
    SDL_FreeSurface(eighth_triplet_arrows_image);
    eighth_triplet_arrows_image = NULL;
  }
  if (thirtysecond_arrows_image)
  {
    SDL_FreeSurface(thirtysecond_arrows_image);
    thirtysecond_arrows_image = NULL;
  }
  if (sixteenth_triplet_arrows_image)
  {
    SDL_FreeSurface(sixteenth_triplet_arrows_image);
    sixteenth_triplet_arrows_image = NULL;
  }
  if (sixtyfourth_arrows_image)
  {
    SDL_FreeSurface(sixtyfourth_arrows_image);
    sixtyfourth_arrows_image = NULL;
  }
  if (sixtyfourth_triplet_arrows_image)
  {
    SDL_FreeSurface(sixtyfourth_triplet_arrows_image);
    sixtyfourth_triplet_arrows_image = NULL;
  }
  if (freeze_arrows_body_image)
  {
    SDL_FreeSurface(freeze_arrows_body_image);
    freeze_arrows_body_image = NULL;
  }
  if (freeze_arrows_tail_image)
  {
    SDL_FreeSurface(freeze_arrows_tail_image);
    freeze_arrows_tail_image = NULL;
  }
  if (freeze_arrows_head_image)
  {
    SDL_FreeSurface(freeze_arrows_head_image);
    freeze_arrows_head_image = NULL;
  }
  if (marvellous_hit_image)
  {
    SDL_FreeSurface(marvellous_hit_image);
    marvellous_hit_image = NULL;
  }
  if (perfect_hit_image)
  {
    SDL_FreeSurface(perfect_hit_image);
    perfect_hit_image = NULL;
  }
  if (great_hit_image)
  {
    SDL_FreeSurface(great_hit_image);
    great_hit_image = NULL;
  }
  if (good_hit_image)
  {
    SDL_FreeSurface(good_hit_image);
    good_hit_image = NULL;
  }
  if (freeze_hit_image)
  {
    SDL_FreeSurface(freeze_hit_image);
    freeze_hit_image = NULL;
  }
}

void Graphics::DrawBackground()
{
  sys.vid.ApplySurface(0, 0, background_image, sys.vid.screen, NULL);
}

}
