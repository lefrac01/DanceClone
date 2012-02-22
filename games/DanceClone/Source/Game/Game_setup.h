//TODO: add second player
//

int longestcombo;
int combo;
int boo;
int good;
int perfect;

int songtime = 0;
int songstarttime = 0;

//NOTE: the following scheme is used to display arrows.  all arrows are
// placed in y positions according to their position in the song so that
// they fill a virtual area as wide as the screen and high enough
// to contain all arrows.  as the song plays the screen is scrolled 
// down this area at a rate determined by the current BPM.  the 
// viewport_offset variable retains the current offset from the start of
// the area.  to determine each arrow's screen y position, subtract this
// offset from its virtual y position.  to do this we track the last
// and first arrows visible on screen, updating these each cycle as
// necessary.
long viewport_offset = 0;
int frame_offset = 0; // pixels that the viewport moved this frame

int last_visible_arrow = 0;
int first_visible_arrow = 0;
int current_ratable_arrow[4];
int next_ratable_arrow[4];
// rating distances are pixel distances based on current BPM and screen
// dimensions
int boo_rating_distance = 0;
int good_rating_distance = 0;
int great_rating_distance = 0;
int perfect_rating_distance = 0;
int marvellous_rating_distance = 0;

const int base_ms_per_pixel = 350; // fixed rate for conversion 
  // to screen coordinates.  350 ms per pixel @ 1 BPM
  
int song_ms_per_pixel = 0;  // changes depending on BPM



int upcontrol = 0;
int downcontrol = 0;
int leftcontrol = 0;
int rightcontrol = 0;


#ifdef WIN
Mix_Music* music = NULL;
#endif

//#include "setup/classes.h"
#include "setup/sprites.h"

void Game_init()
{

#ifdef WIN
  Mix_OpenAudio(0,0,0,0);
  //Mix_VolumeMusic(100);
  //Mix_PlayMusic(music, -1);
#endif

  Game_setup_sprites();
  
  goaloffset = rmode->viHeight / 4; // Immitate DDR2 on Wii
}
