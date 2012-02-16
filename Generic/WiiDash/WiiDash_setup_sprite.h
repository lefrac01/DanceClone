SDL_Surface *WDbackground = NULL;
SDL_Surface *WDbackgroundblurred = NULL;
SDL_Surface *WDborderimage = NULL;
SDL_Surface *WDtopbottomborderimage = NULL;
SDL_Surface *WDcursorimage = NULL;		SDL_Rect WDcursorframes[16];
SDL_Surface *WDfontimage = NULL;		SDL_Rect WDfontframes[95];
SDL_Surface *WDbuttonimage = NULL;		SDL_Rect WDbuttonframes[18];
SDL_Surface *WDarrowsimage = NULL;		SDL_Rect WDarrowsframes[4];
SDL_Surface *WDfontimagecolored = NULL;

void WiiDash_setup_sprite(){
WDbackground 			= SDL_DisplayFormat(screen);
WDbackgroundblurred 	= SDL_DisplayFormat(screen);
WDfontimage 			= IMG_LoadOptimizeAlpha("Media/WiiDash/Font.png");
#ifdef WII
	while(!WDfontimage){
	WDfontimage 		= IMG_LoadOptimizeAlpha("Media/WiiDash/Font.png");
	fatInitDefault();}
#endif
WDbuttonimage 			= IMG_LoadOptimizeAlpha("Media/WiiDash/Button.png");
WDarrowsimage 			= IMG_LoadOptimize("Media/WiiDash/Arrows.png");
WDborderimage 			= IMG_LoadOptimize("Media/WiiDash/Border.png");
WDtopbottomborderimage 	= IMG_LoadOptimize("Media/WiiDash/TopBottomBorder.png");
WDcursorimage 			= IMG_LoadOptimizeAlpha("Media/WiiDash/Cursor.png");
WDfontimagecolored	 	= IMG_LoadOptimizeAlpha("Media/WiiDash/Font.png");

for(int a=0; a<95; a++){
WDfontframes[a].x=a*15;
WDfontframes[a].y=0;
WDfontframes[a].w=15;
WDfontframes[a].h=21;}
for(int b=0; b<2; b++)for(int a=0; a<9; a++){
WDbuttonframes[a+b*9].x=a*21;
WDbuttonframes[a+b*9].y=b*21;
WDbuttonframes[a+b*9].w=21;
WDbuttonframes[a+b*9].h=21;}
for(int a=0; a<4; a++){
WDarrowsframes[a].x=a*16;
WDarrowsframes[a].y=0;
WDarrowsframes[a].w=16;
WDarrowsframes[a].h=16;}
for(int a=0; a<16; a++){
WDcursorframes[a].x=a*96;
WDcursorframes[a].y=0;
WDcursorframes[a].w=96;
WDcursorframes[a].h=96;}

}