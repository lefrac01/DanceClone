//TODO: load animated arrows
//TODO: marvellous rating

SDL_Surface* backgroundimage = NULL;
SDL_Surface* titleimage = NULL;
SDL_Surface* arrowsimage = NULL; SDL_Rect arrowsframes[16];
SDL_Surface* ratingsimage = NULL; SDL_Rect ratingsframes[3];
SDL_Surface* holdimage = NULL; SDL_Rect holdframes[3];

void Game_setup_sprites()
{
	backgroundimage = IMG_LoadOptimize("Media/Game/background.png");
	titleimage = IMG_Load("Media/Game/title.png");
	arrowsimage = IMG_Load("Media/Game/arrows.png"); 
	for(int a = 0; a < 4; a++)
  {
    for(int b = 0; b < 4; b++)
    {
      arrowsframes[a + b*4].x = a*70;	arrowsframes[a + b*4].y = b*70;
      arrowsframes[a + b*4].w = 70;		arrowsframes[a + b*4].h = 70;
    }
  }
	ratingsimage = IMG_Load("Media/Game/ratings.png");
	for(int a = 0; a < 3; a++)
  {
		ratingsframes[a].x = 0;			ratingsframes[a].y = a*50;
		ratingsframes[a].w = 125;		ratingsframes[a].h = 50;
  }
	holdimage = IMG_Load("Media/Game/hold.png");
	for(int a = 0; a < 3; a++)
  {
		holdframes[a].x = a*50;			holdframes[a].y = 0;
		holdframes[a].w = 50;			holdframes[a].h = 35;
  }
}
