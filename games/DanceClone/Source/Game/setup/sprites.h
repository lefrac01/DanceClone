//TODO: load animated arrows
//TODO: marvellous rating
//
//TODO: support extra home arrows animation for flashing on the beat
//
#include <string>
using std::string;
#include <SDL/SDL_rotozoom.h>

SDL_Surface* backgroundimage = NULL;
SDL_Surface* titleimage = NULL;
SDL_Surface* ratingsimage = NULL; SDL_Rect ratingsframes[3];
SDL_Surface* getreadyimage = NULL;

// all non-hold arrows have same layout so use only one frames array
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

SDL_Surface* freeze_arrows_body_image = NULL;  SDL_Rect freeze_body_frames[12];
SDL_Surface* freeze_arrows_tail_image = NULL;  SDL_Rect freeze_tail_frames[12];
SDL_Surface* freeze_arrows_head_image = NULL;  SDL_Rect freeze_head_frames[16];

SDL_Rect arrows_hit_frames[32];
SDL_Surface* marvellous_hit_image = NULL; 
SDL_Surface* perfect_hit_image = NULL; 
SDL_Surface* great_hit_image = NULL; 
SDL_Surface* good_hit_image = NULL; 
SDL_Surface* freeze_hit_image = NULL; 

void set_arrow_frame(SDL_Rect* dest, int index, int x, int y, int w, int h)
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
void expand_arrows(string source_file, SDL_Surface* dest, SDL_Rect* dest_frames, int src_cols)
{
  int aw = 64;
  int ah = 64;
  
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
      set_arrow_frame(dest_frames, k*src_cols+col, col*aw, k*ah, aw, ah);
      
      // delete temp surface
      SDL_FreeSurface(temp_surface_rotated);
    }
  }
  
  SDL_FreeSurface(temp_surface);
  SDL_FreeSurface(temp_src);
}

//TODO: nice error log instead of hard crash when something is missing
void Game_setup_sprites()
{
  backgroundimage = IMG_LoadOptimize("Media/Game/background.png");
  titleimage = IMG_Load("Media/Game/title.png");
  getreadyimage = IMG_Load("Media/Game/getready.png");
  
  // expand source PNGs creating other arrow directions
  //optimistically using SDL_HWSURFACE but not all these can fit into the hardware memory ;)
  //http://en.wikipedia.org/wiki/Wii states 3MB framebuffer of which 1.2 is used per screen + dbl buffering... not leaving much
  home_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/home_arrows.png", home_arrows_image, arrows_frames, 4);

  quarter_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/04thnote.png", quarter_arrows_image, arrows_frames, 4);

  eighth_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/08thnote.png", eighth_arrows_image, arrows_frames, 4);

  quarter_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/12thnote.png", quarter_triplet_arrows_image, arrows_frames, 4);

  sixteenth_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/16thnote.png", sixteenth_arrows_image, arrows_frames, 4);

  eighth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/24thnote.png", eighth_triplet_arrows_image, arrows_frames, 4);

  thirtysecond_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/32ndnote.png", thirtysecond_arrows_image, arrows_frames, 4);

  sixteenth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/48thnote.png", sixteenth_triplet_arrows_image, arrows_frames, 4);

  sixtyfourth_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/64thnote.png", sixtyfourth_arrows_image, arrows_frames, 4);

  //NOTE: same graphic used for 96th and 192nd notes
  sixtyfourth_triplet_arrows_image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 256, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/192ndnote.png", sixtyfourth_triplet_arrows_image, arrows_frames, 4);


  // hit animations, 1 row 8 col source to 8 row 8 col dest
  marvellous_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/marvelloushit.png", marvellous_hit_image, arrows_hit_frames, 8);

  perfect_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/perfecthit.png", perfect_hit_image, arrows_hit_frames, 8);

  great_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/greathit.png", great_hit_image, arrows_hit_frames, 8);

  good_hit_image = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, 512, 256, 32, 0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
  expand_arrows("Media/Game/goodhit.png", good_hit_image, arrows_hit_frames, 8);


  // freeze arrow graphics do not use the same source layout nor are they animated
  freeze_hit_image = IMG_Load("Media/Game/freezehit.png");
  freeze_arrows_head_image = IMG_Load("Media/Game/freezehead.png");
  for(int b = 0; b < 4; b++)
  {
    for(int a = 0; a < 4; a++)
    {
      freeze_head_frames[b*4 + a].x = a*64; freeze_head_frames[b*4 + a].y = b*64;
      freeze_head_frames[b*4 + a].w = 64;   freeze_head_frames[b*4 + a].h = 64;
    }
  }
  freeze_arrows_body_image = IMG_Load("Media/Game/freezebody.png");
  freeze_arrows_tail_image = IMG_Load("Media/Game/freezetail.png");
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
