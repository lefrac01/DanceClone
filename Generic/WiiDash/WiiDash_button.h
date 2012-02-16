void WiiDash_drawbutton(int x, int y,int w,int h,bool glow){
	//corners
	apply_surface(x-21,y-21,WDbuttonimage,screen,&WDbuttonframes[0+glow*9]);
	apply_surface(x-21,y+h,WDbuttonimage,screen,&WDbuttonframes[1+glow*9]);
	apply_surface(x+w,y-21,WDbuttonimage,screen,&WDbuttonframes[2+glow*9]);
	apply_surface(x+w,y+h,WDbuttonimage,screen,&WDbuttonframes[3+glow*9]);
	//edges
	for(int a=0; a<w/21; a++)
		apply_surface(x+a*21,y+h,WDbuttonimage,screen,&WDbuttonframes[4+glow*9]);
	SDL_Rect temprect1={4*21,glow*21,w-w/21*21,21};
	apply_surface(x+w/21*21,y+h,WDbuttonimage,screen,&temprect1);
	for(int a=0; a<h/21; a++)
		apply_surface(x+w,y+a*21,WDbuttonimage,screen,&WDbuttonframes[5+glow*9]);
	SDL_Rect temprect2={5*21,glow*21,21,h-h/21*21};
	apply_surface(x+w,y+h/21*21,WDbuttonimage,screen,&temprect2);
	for(int a=0; a<w/21; a++)
		apply_surface(x+a*21,y-21,WDbuttonimage,screen,&WDbuttonframes[6+glow*9]);
	SDL_Rect temprect3={6*21,glow*21,w-w/21*21,21};
	apply_surface(x+w/21*21,y-21,WDbuttonimage,screen,&temprect3);
	for(int a=0; a<h/21; a++)
		apply_surface(x-21,y+a*21,WDbuttonimage,screen,&WDbuttonframes[7+glow*9]);
	SDL_Rect temprect4={7*21,glow*21,21,h-h/21*21};
	apply_surface(x-21,y+h/21*21,WDbuttonimage,screen,&temprect4);
	//insides
	for(int a=0; a<h/21; a++)for(int b=0; b<w/21; b++)
		apply_surface(x+b*21,y+a*21,WDbuttonimage,screen,&WDbuttonframes[8+glow*9]);
	SDL_Rect temprect5={8*21,glow*21,w-w/21*21,21};
	for(int a=0; a<h/21; a++)
		apply_surface(x+w/21*21,y+a*21,WDbuttonimage,screen,&temprect5);
	SDL_Rect temprect6={8*21,glow*21,21,h-h/21*21};
	for(int a=0; a<w/21; a++)
		apply_surface(x+a*21,y+h/21*21,WDbuttonimage,screen,&temprect6);
	SDL_Rect temprect7={8*21,glow*21,w-w/21*21,h-h/21*21};
	apply_surface(x+w/21*21,y+h/21*21,WDbuttonimage,screen,&temprect7);
}

bool WiiDash_button(int x, int y,int w,int h,bool center,bool clickable,char* text){
	bool clicked=0;
	bool glow=0;
	if(center){x=x-w/2;y=y-h/2;}

	#ifdef WIN
	if(clickable && cursorx[0]>x-14 && cursory[0]>y-14 && cursorx[0]<x+w+13 && cursory[0]<y+h+13){
		if(mousestate[SDL_BUTTON_LEFT]==2)clicked=1;
		glow=1;}
	#endif

	#ifdef WII
	if(clickable)for(int a=0;a<4;a++){
		if(WDusecursor[a] && cursorx[a]>x-14 && cursory[a]>y-14 && cursorx[a]<x+w+13 && cursory[a]<y+h+13){
			if(WiiButtonsDown[a] & WPAD_BUTTON_A)clicked=1;
			if(WiiButtonsDown[a] & WPAD_CLASSIC_BUTTON_A)clicked=1;
			glow=1;
		}
	}
	#endif

	WiiDash_drawbutton(x,y,w,h,glow);

	x=x+w/2;y=y+h/2;
	WiiDash_spritetext(x,y-21/2,text,2);
	return clicked;
}