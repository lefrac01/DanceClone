#include "WiiDash_setup.h"
#include "WiiDash_spritetext.h"
#include "WiiDash_cursor.h"
#include "WiiDash_button.h"
#include "WiiDash_onoff.h"
#include "WiiDash_screenshot.h"
#include "WiiDash_topbottombars.h"
#include "WiiDash_menu.h"

void WiiDash_run(){

	int oldruntime=WDruntime;
	WDruntime=SDL_GetTicks();
	WDframetime=WDruntime-oldruntime;
	WDframerate=(int)(1000/(double)WDframetime);
	if(WDonoffpercent==0)WDgametime=WDgametime+WDframetime;
	if(WDonoffpercent==0)WDtimespeed=(double)framecap/(double)WDframerate;
	if(WDonoffpercent!=0)WDtimespeed=60/(double)WDframerate;

	WiiDash_onoff();
	if(WDonoffpercent!=0){
		time_t now;
		now = time(NULL);
		WDtime = localtime(&now);
		WiiDash_menu();	
		WiiDash_topbottombars();
	}
	
	WiiDash_cursor();
	
	/*#ifdef WIN
	apply_surface(0,0,WDborderimage,screen,NULL);	
	#endif*/
	
	/*SDL_Rect temprect = {22,30,241,17};
	SDL_FillRect(screen,&temprect,SDL_MapRGB(screen->format,150,150,150));
	char temptext[100];	
	sprintf(temptext,"%s%d","Frames Per Second = ",WDframerate);
	WiiDash_spritetext(20,30,(char*)temptext,1);*/

}