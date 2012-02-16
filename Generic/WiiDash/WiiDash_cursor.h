bool WDusecursors=0;
bool WDusecursor[4]={0,0,0,0};

void WiiDash_cursor(){
	for(int a=0;a<4;a++)WDusecursor[a]=0;
	if(WDusecursors){
		#ifdef WII
		for(int a=0;a<4;a++){
			if(ir[a].valid){
				WDusecursor[a]=1;
			}else if(expans[a].type==WPAD_EXP_CLASSIC){
				WDusecursor[a]=1;
				double mag = expans[a].classic.rjs.mag;
				double ang = expans[a].classic.rjs.ang;
				if(mag>0){
					ang=(ang-90)*0.0174532925;
					cursorx[a]+=mag*cos(ang)*15;
					cursory[a]+=mag*sin(ang)*15;
				}
			}
		}
		if(wiimoteactive[0] && !wiimoteactive[1] && !wiimoteactive[2] && !wiimoteactive[3]){
			if(WDusecursor[0])apply_surface(cursorx[0]-48,cursory[0]-48,WDcursorimage,screen,&WDcursorframes[3]);
		}else{
			if(WDusecursor[0])apply_surface(cursorx[0]-48,cursory[0]-48,WDcursorimage,screen,&WDcursorframes[6]);
			if(WDusecursor[1])apply_surface(cursorx[1]-48,cursory[1]-48,WDcursorimage,screen,&WDcursorframes[9]);
			if(WDusecursor[2])apply_surface(cursorx[2]-48,cursory[2]-48,WDcursorimage,screen,&WDcursorframes[12]);
			if(WDusecursor[3])apply_surface(cursorx[3]-48,cursory[3]-48,WDcursorimage,screen,&WDcursorframes[15]);
		}
		#endif
		#ifdef WIN
		if(SDL_GetAppState() & SDL_APPMOUSEFOCUS)WDusecursor[0]=1;
		if(WDusecursor[0])apply_surface(cursorx[0]-48,cursory[0]-48,WDcursorimage,screen,&WDcursorframes[3]);
		#endif		
	}
	WDusecursors=1;	
}