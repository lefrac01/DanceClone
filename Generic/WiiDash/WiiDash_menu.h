void WiiDash_menu(){
/*#ifdef WII
  if(WiiDash_button(320,
    WiiDash_onoffslide(240-125,240-125+480),
    300,15,1,1,(char*)"Resume game")){WDonoff=0;}
  if(WiiDash_button(320,
    WiiDash_onoffslide(240-75,240-75+480),
    300,15,1,1,(char*)"Exit to loader")){done=true;}
  if(WiiDash_button(320,
    WiiDash_onoffslide(240-25,240-25+480),
    300,15,1,1,(char*)"Restart Wii")){SYS_ResetSystem(SYS_RESTART,0,0);}
  if(WiiDash_button(320,
    WiiDash_onoffslide(240+25,240+25+480),
    300,15,1,1,(char*)"Shutdown Wii")){SYS_ResetSystem(SYS_POWEROFF,0,0);}
  if(WiiDash_button(320,
    WiiDash_onoffslide(240+75,240+75+480),
    300,15,1,1,(char*)"Save BMP screenshot")){WiiDash_savebmpscreenshot();}
  if(WiiDash_button(320,
    WiiDash_onoffslide(240+125,240+125+480),
    300,15,1,1,(char*)"Save PNG screenshot")){WiiDash_savepngscreenshot();}
#endif
#ifdef WIN
  if(WiiDash_button(320,
    WiiDash_onoffslide(240-125,240-125+480),
    300,15,1,1,(char*)"Resume game")){WDonoff=0;}
  if(WiiDash_button(320,
    WiiDash_onoffslide(240-75,240-75+480),
    300,15,1,1,(char*)"Exit")){done=true;}
#endif*/

int y=WiiDash_onoffslide(rmode->viHeight/2-125,rmode->viHeight/2-125+rmode->viHeight);

if(WiiDash_button(rmode->viWidth/2,y,300,15,1,1,(char*)"Resume game"))WDonoff=0;
y+=50;

#ifdef WIN
if(WiiDash_button(rmode->viWidth/2,y,300,15,1,1,(char*)"Exit"))done=true;
y+=50;
#endif

#ifdef WII
if(WiiDash_button(rmode->viWidth/2,y,300,15,1,1,(char*)"Exit to loader"))done=true;
y+=50;

if(WiiDash_button(rmode->viWidth/2,y,300,15,1,1,(char*)"Restart Wii"))
{
  //game_cleanup(); //so sorry
  generic_cleanup();
  //does SDL want a cleanup call?
  SYS_ResetSystem(SYS_RESTART,0,0);
}
y+=50;

if(WiiDash_button(rmode->viWidth/2,y,300,15,1,1,(char*)"Shutdown Wii"))
{
  //game_cleanup(); //so sorry
  generic_cleanup();
  //does SDL want a cleanup call?
  SYS_ResetSystem(SYS_POWEROFF,0,0);
}
y+=50;
#endif

if(WiiDash_button(rmode->viWidth/2,y,300,15,1,1,(char*)"Save BMP screenshot"))WiiDash_savebmpscreenshot();
y+=50;

#ifdef WII
if(WiiDash_button(rmode->viWidth/2,y,300,15,1,1,(char*)"Save PNG screenshot"))WiiDash_savepngscreenshot();
y+=50;
#endif

}
