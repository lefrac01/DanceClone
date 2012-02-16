int timehaspast;

#include "play/controls.h"

void Game_play(){

	timehaspast=songtime;
	songtime=WDgametime-songstarttime;
	timehaspast=songtime-timehaspast;
	
	/*Mix_RewindMusic();
	Mix_SetMusicPosition(songtime/1000);*/

	Game_play_controls();

	for(int a=0;a<n_ratings;a++){
		apply_surface(ratings[a]->posx,ratings[a]->posy,ratingsimage,screen,&ratingsframes[ratings[a]->type]);
		ratings[a]->posy=ratings[a]->posy-(int)((double)timehaspast/10);
		if(ratings[a]->posy<0-50){
			delete ratings[a];
			for(int b=a; b<n_ratings; b++)ratings[b]=ratings[b+1];
			n_ratings=n_ratings-1;
			a=a-1;
		}
	}
	
	if(leftcontrol==0)
		apply_surface(185,goalline,arrowsimage,screen,&arrowsframes[4]);
	else if(leftcontrol==1)
		apply_surface(185,goalline,arrowsimage,screen,&arrowsframes[8]);
	else
		apply_surface(185,goalline,arrowsimage,screen,&arrowsframes[0]);
	if(downcontrol==0)
		apply_surface(250,goalline,arrowsimage,screen,&arrowsframes[5]);
	else if(downcontrol==1)
		apply_surface(250,goalline,arrowsimage,screen,&arrowsframes[9]);
	else
		apply_surface(250,goalline,arrowsimage,screen,&arrowsframes[1]);
	if(upcontrol==0)
		apply_surface(320,goalline,arrowsimage,screen,&arrowsframes[6]);
	else if(upcontrol==1)
		apply_surface(320,goalline,arrowsimage,screen,&arrowsframes[10]);
	else
		apply_surface(320,goalline,arrowsimage,screen,&arrowsframes[2]);
	if(rightcontrol==0)
		apply_surface(385,goalline,arrowsimage,screen,&arrowsframes[7]);
	else if(rightcontrol==1)
		apply_surface(385,goalline,arrowsimage,screen,&arrowsframes[11]);
	else
		apply_surface(385,goalline,arrowsimage,screen,&arrowsframes[3]);

	SDL_Rect temprect={50,0,50,35};
	for(int a=0;a<n_arrows;a++){
		int posy=(int)(goalline+((double)(arrows[a]->time-songtime))/5);
		if(posy>0-70 && posy<480){
			if(arrows[a]->length){
				if(arrows[a]->direction==0){
					apply_surface(185+10,posy,holdimage,screen,&holdframes[0]);
					for(int b=0;b<floor((double)arrows[a]->length/5/35);b++)
						apply_surface(185+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
					temprect.h = (Uint16)(arrows[a]->length/5-floor((double)arrows[a]->length/5/35)*35);
					apply_surface(185+10,posy+(int)floor((double)arrows[a]->length/5/35)*35+35,holdimage,screen,&temprect);
					apply_surface(185+10,posy+arrows[a]->length/5+35,holdimage,screen,&holdframes[2]);}
				if(arrows[a]->direction==1){
					apply_surface(250+10,posy,holdimage,screen,&holdframes[0]);
					for(int b=0;b<floor((double)arrows[a]->length/5/35);b++)
						apply_surface(250+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
					temprect.h = (Uint16)(arrows[a]->length/5-floor((double)arrows[a]->length/5/35)*35);
					apply_surface(250+10,posy+(int)floor((double)arrows[a]->length/5/35)*35+35,holdimage,screen,&temprect);
					apply_surface(250+10,posy+arrows[a]->length/5+35,holdimage,screen,&holdframes[2]);}
				if(arrows[a]->direction==2){
					apply_surface(320+10,posy,holdimage,screen,&holdframes[0]);
					for(int b=0;b<floor((double)arrows[a]->length/5/35);b++)
						apply_surface(320+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
					temprect.h = (Uint16)(arrows[a]->length/5-floor((double)arrows[a]->length/5/35)*35);
					apply_surface(320+10,posy+(int)floor((double)arrows[a]->length/5/35)*35+35,holdimage,screen,&temprect);
					apply_surface(320+10,posy+arrows[a]->length/5+35,holdimage,screen,&holdframes[2]);}
				if(arrows[a]->direction==3){
					apply_surface(385+10,posy,holdimage,screen,&holdframes[0]);
					for(int b=0;b<floor((double)arrows[a]->length/5/35);b++)
						apply_surface(385+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
					temprect.h = (Uint16)(arrows[a]->length/5-floor((double)arrows[a]->length/5/35)*35);
					apply_surface(385+10,posy+(int)floor((double)arrows[a]->length/5/35)*35+35,holdimage,screen,&temprect);
					apply_surface(385+10,posy+arrows[a]->length/5+35,holdimage,screen,&holdframes[2]);}
			}
			if(arrows[a]->direction==0)
				apply_surface(185,posy,arrowsimage,screen,&arrowsframes[12]);
			if(arrows[a]->direction==1)
				apply_surface(250,posy,arrowsimage,screen,&arrowsframes[13]);
			if(arrows[a]->direction==2)
				apply_surface(320,posy,arrowsimage,screen,&arrowsframes[14]);
			if(arrows[a]->direction==3)
				apply_surface(385,posy,arrowsimage,screen,&arrowsframes[15]);
		}
	}

	if(combo>longestcombo)longestcombo=combo;

	char temptext[100];
	sprintf(temptext,"%d%s",boo," BOO");
	WiiDash_spritetext(320,440-18*3,temptext,2);
	sprintf(temptext,"%d%s",good," GOOD");
	WiiDash_spritetext(320,440-18*2,temptext,2);
	sprintf(temptext,"%d%s",perfect," PERFECT");
	WiiDash_spritetext(320,440-18*1,temptext,2);
	if(combo>2){
		sprintf(temptext,"%d%s",combo," COMBO");
		WiiDash_spritetext(320,440-18*0,temptext,2);
	}
	
	#ifdef WII
	if(!MP3Player_IsPlaying()){
		MP3Player_Stop();
		gamestate=9;
	}
	#endif
	
	#ifdef WIN
	if(WiiDash_button(640-100-40,480-10-40,100,10,0,1,(char*)"Back")){
		Mix_FreeMusic(music);
		gamestate=4;
	}else if(!Mix_PlayingMusic()){
		Mix_FreeMusic(music);
		gamestate=9;
	}else if(keystate[SDLK_ESCAPE]==2){
		Mix_PauseMusic();
	}else if(Mix_PausedMusic()){Mix_ResumeMusic();}
	#endif

	#ifdef WII
	if(WiiDash_button(640-100-40,480-10-40,100,10,0,1,(char*)"Back")){
		MP3Player_Stop();
		gamestate=4;
	}/*else if(!MP3Player_IsPlaying()){
		MP3Player_Stop();
		gamestate==9;
	}*//*else if((WiiButtons1Down & WPAD_BUTTON_HOME)
	|| (WiiButtons2Down & WPAD_BUTTON_HOME)
	|| (WiiButtons3Down & WPAD_BUTTON_HOME)
	|| (WiiButtons4Down & WPAD_BUTTON_HOME)){
		MP3Player_Pause();
	}else if(MP3Player_Paused()){MP3Player_Resume();}*/
	#endif

}