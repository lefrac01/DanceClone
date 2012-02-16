//TODO: SDL "uint"

SDL_Surface *IMG_LoadOptimize( std::string filename ){
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL; 
	loadedImage = IMG_Load( filename.c_str() ); 
	if( loadedImage != NULL ){
		optimizedImage = SDL_DisplayFormat( loadedImage );
		Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );
		SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
		SDL_FreeSurface( loadedImage );
	} 
	return optimizedImage;
}

SDL_Surface *IMG_LoadOptimizeAlpha( std::string filename ){
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL; 
	loadedImage = IMG_Load( filename.c_str() ); 
	if( loadedImage != NULL ){
		optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface( loadedImage );
	} 
	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ){
	SDL_Rect offset;offset.x = x; offset.y = y;
	SDL_BlitSurface( source, clip, destination, &offset );
}
