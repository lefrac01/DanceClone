//TODO: load animated arrows
//TODO: marvellous rating
//
//TODO: support extra home arrows animation for flashing on the beat
//
#include <string>
#include <SDL/SDL_rotozoom.h>

SDL_Surface* backgroundimage = NULL;
SDL_Surface* titleimage = NULL;
//SDL_Surface* arrowsimage = NULL; SDL_Rect arrowsframes[16];
SDL_Surface* ratingsimage = NULL; SDL_Rect ratingsframes[3];
SDL_Surface* holdimage = NULL; SDL_Rect holdframes[3];

// all arrows have same layout so use only one frames array
SDL_Rect arrows_frames[16];
SDL_Surface* temp_arrows_image_src = NULL; 
SDL_Surface* home_arrows_image = NULL; 
SDL_Surface* quarter_arrows_image = NULL; 
SDL_Surface* eighth_arrows_image = NULL; 
SDL_Surface* quarter_triplet_arrows_image = NULL; 
SDL_Surface* sixteenth_arrows_image = NULL; 
SDL_Surface* eighth_triplet_arrows_image = NULL; 
SDL_Surface* thirtysecond_arrows_image = NULL; 
SDL_Surface* sixteenth_triplet_arrows_image = NULL; 
SDL_Surface* sixtyfourth_arrows_image = NULL; 
SDL_Surface* sixtyfourth_triplet_arrows_image = NULL; 

//TODO: some OO thinking would avoid the possibility of hard crashes :-(
void set_arrow_frame(SDL_Rect* dest, int index, int x, int y, int w, int h)
{
  char buf[100];
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    //#log.open("debug", std::ios_base::app);
    sprintf(buf, "%X", (unsigned int)dest);
    log << "set_arrow_frame(" << buf << ", " << index << ", " << x << ", " << y << ", " << w << ", " << h << ")..." << endl;
    //#log.close();   
  }
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
void expand_arrows(string source_file, SDL_Surface* dest, SDL_Rect* dest_frames)
{
  int ret;
  char buf[100];
  
  int aw = 64;
  int ah = 64;
  int num_cols = 4;
  
  // source file format is 32 bit RGBA and rotation function also expects 32 bit
  SDL_Surface* temp_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 64, 32,
                    0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  SDL_SetAlpha(temp_surface, 0, SDL_ALPHA_OPAQUE);  // force SDL_SRCALPHA to off, 
  // enabled because temp_surface is created with an alpha channel (necessary to 
  // retain the alpha info from source file)

  SDL_Surface* temp_surface_rotated = NULL;
  SDL_Surface* temp_src = IMG_Load(source_file.c_str());
  if (!temp_src)
  {
    error_log.open("errors", std::ios_base::app);
    error_log << "failed to load graphics resource \"" << source_file.c_str() << "\"" << endl;
    error_log.close();
    return;
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
  for (int col = 0; col < num_cols; col++)
  {
    // set up blit rectangles
    resource_src.x = col * aw;
    resource_src.y = 0;
    resource_src.w = aw;
    resource_src.h = ah;
    
    if (DEBUG_LEVEL >= DEBUG_GUTS)
    {
      //#log.open("debug", std::ios_base::app);
      log << "SDL_BlitSurface temp_src to temp_surface (" << (unsigned int)temp_src->format->BitsPerPixel << " bpp to  " << (unsigned int)temp_surface->format->BitsPerPixel << " bpp ...";
      log << "src rect(x y w h: " << resource_src.x << resource_src.y << resource_src.w << resource_src.h << endl; 
      log << "dst rect(x y w h: " << rotation_src.x << rotation_src.y << rotation_src.w << rotation_src.h << endl; 
      //#log.close();
    }
    
    // obtain a working copy of the arrow to rotate
    ret = SDL_BlitSurface(temp_src, &resource_src, temp_surface, &rotation_src);
    if (DEBUG_LEVEL >= DEBUG_GUTS)
    {
      //#log.open("debug", std::ios_base::app);
      log << ret << endl;
      log << "returned dst rect(x y w h: " << rotation_src.x << rotation_src.y << rotation_src.w << rotation_src.h << endl; 
      //#log.close();
    }
    
    rotation_dest.x = col * aw;
    rotation_dest.y = 0;
    rotation_dest.w = aw;
    rotation_dest.h = ah;

    for (int k=0; k<4; k++)
    {
      // rotate
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "temp_surface_rotated = rotateSurface90Degrees... ";
        //#log.close();
      }
      temp_surface_rotated = rotateSurface90Degrees(temp_surface, k);
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        sprintf(buf, "%X", (unsigned int)temp_surface_rotated);
        //#log.open("debug", std::ios_base::app);
        log << buf << " with " << (unsigned int)temp_surface_rotated->format->BitsPerPixel << " bpp" << endl;
        //#log.close();  
      }

      // blit this on.
      rotation_dest.y = k*ah;
    
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "SDL_BlitSurface from temp_surface_rotated to dest... ";
        log << "src rect(x y w h: " << rotation_src.x << rotation_src.y << rotation_src.w << rotation_src.h << endl; 
        log << "dst rect(x y w h: " << rotation_dest.x << rotation_dest.y << rotation_dest.w << rotation_dest.h << endl; 
        //#log.close();
      }
      
      // turn off source alpha of rotated bitmap for dumb copy to staging surface
      ret = SDL_SetAlpha(temp_surface_rotated, 0, SDL_ALPHA_OPAQUE);
      ret = SDL_BlitSurface(temp_surface_rotated, &rotation_src, dest, &rotation_dest);
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << ret << endl;
        log << "returned dst rect(x y w h: " << rotation_dest.x << rotation_dest.y << rotation_dest.w << rotation_dest.h << endl; 
        //#log.close();
      }
      // and record the arrow frame rect
      set_arrow_frame(dest_frames, k*num_cols+col, col*aw, k*ah, aw, ah);
      
      // delete temp surface
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "SDL_FreeSurface temp_surface_rotated... ";
        //#log.close();
      }
      SDL_FreeSurface(temp_surface_rotated);
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "done." << endl;
        //#log.close();
      }
    }
  }
  
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    //#log.open("debug", std::ios_base::app);
    log << "SDL_FreeSurface temp_surface and temp_src... ";
    //#log.close();
  }
  SDL_FreeSurface(temp_surface);
  SDL_FreeSurface(temp_src);
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    //#log.open("debug", std::ios_base::app);
    log << "done." << endl;
    //#log.close();
  }
}

//TODO: nice error log instead of hard crash when something is missing
void Game_setup_sprites()
{
  backgroundimage = IMG_LoadOptimize("Media/Game/background.png");
  titleimage = IMG_Load("Media/Game/title.png");
  /*
  arrowsimage = IMG_Load("Media/Game/arrows.png"); 
  for(int a = 0; a < 4; a++)
  {
    for(int b = 0; b < 4; b++)
    {
      arrowsframes[a + b*4].x = a*70; arrowsframes[a + b*4].y = b*70;
      arrowsframes[a + b*4].w = 70;   arrowsframes[a + b*4].h = 70;
    }
  }
  */

  // expand source PNGs creating other arrow directions
//  temp_arrows_image_src = IMG_Load(
//  home_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
//  expand_arrows(home_arrows_image_src, home_arrows_image, home_arrows_frames);
//  SDL_FreeSurface(temp_arrows_image_src);

  //optimistically using SDL_HWSURFACE but not all these can fit into the hardware memory ;)
  //http://en.wikipedia.org/wiki/Wii states 3MB framebuffer of which 1.2 is used per screen + dbl buffering... not leaving much
  home_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/home_arrows.png", home_arrows_image, arrows_frames);

  quarter_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/04thnote.png", quarter_arrows_image, arrows_frames);

  eighth_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/08thnote.png", eighth_arrows_image, arrows_frames);

  quarter_triplet_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/12thnote.png", quarter_triplet_arrows_image, arrows_frames);

  sixteenth_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/16thnote.png", sixteenth_arrows_image, arrows_frames);

  eighth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/24thnote.png", eighth_triplet_arrows_image, arrows_frames);

  thirtysecond_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/32ndnote.png", thirtysecond_arrows_image, arrows_frames);

  sixteenth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/48thnote.png", sixteenth_triplet_arrows_image, arrows_frames);

  sixtyfourth_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/64thnote.png", sixtyfourth_arrows_image, arrows_frames);

  //NOTE: same graphic used for 96th and 192nd notes
  sixtyfourth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/192ndnote.png", sixtyfourth_triplet_arrows_image, arrows_frames);


  ratingsimage = IMG_Load("Media/Game/ratings.png");
  for(int a = 0; a < 3; a++)
  {
    ratingsframes[a].x = 0;     ratingsframes[a].y = a*50;
    ratingsframes[a].w = 125;   ratingsframes[a].h = 50;
  }
  holdimage = IMG_Load("Media/Game/hold.png");
  for(int a = 0; a < 3; a++)
  {
    holdframes[a].x = a*50;     holdframes[a].y = 0;
    holdframes[a].w = 50;     holdframes[a].h = 35;
  }
}
