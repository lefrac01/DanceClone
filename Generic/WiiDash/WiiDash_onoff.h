int WiiDash_onoffslide(int onpos,int offpos){
int currentpos=(int)((onpos)*(WDonoffpercent/100)+(offpos)*((100-WDonoffpercent)/100));
return currentpos;}

Uint8 WDrgb1[640][480][3];
Uint8 WDrgb2[640][480][3];

void WiiDash_onoff(){
#ifdef WIN
if(keystate[SDLK_ESCAPE]==2){
#endif
#ifdef WII
if((WiiButtonsDown[0] & WPAD_BUTTON_HOME) || (WiiButtonsDown[0] & WPAD_CLASSIC_BUTTON_HOME)
|| (WiiButtonsDown[1] & WPAD_BUTTON_HOME) || (WiiButtonsDown[1] & WPAD_CLASSIC_BUTTON_HOME)
|| (WiiButtonsDown[2] & WPAD_BUTTON_HOME) || (WiiButtonsDown[2] & WPAD_CLASSIC_BUTTON_HOME)
|| (WiiButtonsDown[3] & WPAD_BUTTON_HOME) || (WiiButtonsDown[3] & WPAD_CLASSIC_BUTTON_HOME)){
#endif
	if(WDonoff==0){
		WDonoff=1;
		if(WDonoffpercent==0){
			apply_surface(0,0,screen,WDbackground,NULL);
			#ifdef WIN
			for(int x=0;x<640;x++)for(int y=0;y<480;y++)
				SDL_GetRGB(getpixel(screen,x,y),
				screen->format,&WDrgb1[x][y][0],&WDrgb1[x][y][1],&WDrgb1[x][y][2]);
			#endif
			#ifdef WII
			Uint32 color;
			Uint16* pPosition=(Uint16*)screen->pixels;
			for(int y=0;y<480;y++)for(int x=0;x<640;x++){
				color=*pPosition;
				WDrgb1[x][y][0]=(((color&63488)>>11)<<3);
				WDrgb1[x][y][1]=(((color&2016)>>5)<<2);
				WDrgb1[x][y][2]=(((color&31)>>0)<<3);
				pPosition++;}
			#endif
			memcpy(WDrgb2,WDrgb1,640*480*3*sizeof(Uint8));
			for(int a=0;a<1;a++){
				for(int x=3;x<640-3;x++)for(int y=3;y<480-3;y++){
					WDrgb2[x][y][0]=(WDrgb1[x][y-3][0]+WDrgb1[x][y+3][0]+WDrgb1[x-3][y][0]+WDrgb1[x+3][y][0])/4;
					WDrgb2[x][y][1]=(WDrgb1[x][y-3][1]+WDrgb1[x][y+3][1]+WDrgb1[x-3][y][1]+WDrgb1[x+3][y][1])/4;
					WDrgb2[x][y][2]=(WDrgb1[x][y-3][2]+WDrgb1[x][y+3][2]+WDrgb1[x-3][y][2]+WDrgb1[x+3][y][2])/4;
				}for(int x=3;x<640-3;x++)for(int y=3;y<480-3;y++){
					WDrgb1[x][y][0]=(WDrgb2[x][y-3][0]+WDrgb2[x][y+3][0]+WDrgb2[x-3][y][0]+WDrgb2[x+3][y][0]+1)/4;
					WDrgb1[x][y][1]=(WDrgb2[x][y-3][1]+WDrgb2[x][y+3][1]+WDrgb2[x-3][y][1]+WDrgb2[x+3][y][1]+1)/4;
					WDrgb1[x][y][2]=(WDrgb2[x][y-3][2]+WDrgb2[x][y+3][2]+WDrgb2[x-3][y][2]+WDrgb2[x+3][y][2]+1)/4;
				}
			}
			for(int a=0;a<2;a++){
				for(int x=1;x<640-1;x++)for(int y=1;y<480-1;y++){
					WDrgb2[x][y][0]=(WDrgb1[x][y-1][0]+WDrgb1[x][y+1][0]+WDrgb1[x-1][y][0]+WDrgb1[x+1][y][0])/4;
					WDrgb2[x][y][1]=(WDrgb1[x][y-1][1]+WDrgb1[x][y+1][1]+WDrgb1[x-1][y][1]+WDrgb1[x+1][y][1])/4;
					WDrgb2[x][y][2]=(WDrgb1[x][y-1][2]+WDrgb1[x][y+1][2]+WDrgb1[x-1][y][2]+WDrgb1[x+1][y][2])/4;
				}for(int x=1;x<640-1;x++)for(int y=1;y<480-1;y++){
					WDrgb1[x][y][0]=(WDrgb2[x][y-1][0]+WDrgb2[x][y+1][0]+WDrgb2[x-1][y][0]+WDrgb2[x+1][y][0])/4;
					WDrgb1[x][y][1]=(WDrgb2[x][y-1][1]+WDrgb2[x][y+1][1]+WDrgb2[x-1][y][1]+WDrgb2[x+1][y][1])/4;
					WDrgb1[x][y][2]=(WDrgb2[x][y-1][2]+WDrgb2[x][y+1][2]+WDrgb2[x-1][y][2]+WDrgb2[x+1][y][2])/4;
				}
			}
			for(int y=0;y<480;y++)for(int x=0;x<640;x++){
				WDrgb1[x][y][0]=in8bitrange(WDrgb1[x][y][0]+50);
				WDrgb1[x][y][1]=in8bitrange(WDrgb1[x][y][1]+50);
				WDrgb1[x][y][2]=in8bitrange(WDrgb1[x][y][2]+50);
				WDrgb1[x][y][0]=in8bitrange(WDrgb1[x][y][0]-100);
				WDrgb1[x][y][1]=in8bitrange(WDrgb1[x][y][1]-100);
				WDrgb1[x][y][2]=in8bitrange(WDrgb1[x][y][2]-100);
				WDrgb1[x][y][0]=in8bitrange(WDrgb1[x][y][0]+100);
				WDrgb1[x][y][1]=in8bitrange(WDrgb1[x][y][1]+100);
				WDrgb1[x][y][2]=in8bitrange(WDrgb1[x][y][2]+100);
			}
			#ifdef WIN
			for(int y=0;y<480;y++)for(int x=0;x<640;x++)
				putpixel(WDbackgroundblurred,x,y,
					SDL_MapRGB(WDbackgroundblurred->format,
					WDrgb1[x][y][0],WDrgb1[x][y][1],WDrgb1[x][y][2]));
			#endif
			#ifdef WII
			pPosition=(Uint16*)WDbackgroundblurred->pixels;
			for(int y=0;y<480;y++)for(int x=0;x<640;x++){
				*pPosition=
					(WDrgb1[x][y][0]>>3<<11)|
					(WDrgb1[x][y][1]>>2<<5)|
					(WDrgb1[x][y][2]>>3<<0);
				pPosition++;}
			#endif
			for(int y=0;y<480;y++)for(int x=0;x<640;x++)
				SDL_GetRGB(getpixel(WDbackground,x,y),WDbackground->format,
					&WDrgb2[x][y][0],&WDrgb2[x][y][1],&WDrgb2[x][y][2]);
		}
	}else{WDonoff=0;}
}

if(WDonoffpercent==0){
}else if(WDonoffpercent==100){
	apply_surface(0,0,WDbackgroundblurred,screen,NULL);
}else{
#ifdef WIN
	Uint32 color;
	Uint8 r,g,b;
	SDL_PixelFormat *fmt = screen->format;
	int part1=100-(int)WDonoffpercent;
	int part2=(int)WDonoffpercent;
	for(int y=0;y<480;y++)for(int x=0;x<640;x++){
		r=in8bitrange((WDrgb2[x][y][0]*part1+WDrgb1[x][y][0]*part2)/100);
		g=in8bitrange((WDrgb2[x][y][1]*part1+WDrgb1[x][y][1]*part2)/100);
		b=in8bitrange((WDrgb2[x][y][2]*part1+WDrgb1[x][y][2]*part2)/100);
		color=
			(r>>fmt->Rloss<<fmt->Rshift)|
			(g>>fmt->Gloss<<fmt->Gshift)|
			(b>>fmt->Bloss<<fmt->Bshift);
		putpixel(screen,x,y,color);
	}
#endif
#ifdef WII
	Uint8 r,g,b;
	/*int part1=100-(int)WDonoffpercent;
	int part2=(int)WDonoffpercent;*/
	//int change=(int)(WDonoffpercent/100*255);
	Uint16* pPosition=(Uint16*)screen->pixels;
	for(int y=0;y<480;y++)for(int x=0;x<640;x++){
		/*r=in8bitrange((WDrgb2[x][y][0]*part1+WDrgb1[x][y][0]*part2)/100);
		g=in8bitrange((WDrgb2[x][y][1]*part1+WDrgb1[x][y][1]*part2)/100);
		b=in8bitrange((WDrgb2[x][y][2]*part1+WDrgb1[x][y][2]*part2)/100);*/
		/*r=in8bitrange(uint8percentage[(signed int)WDrgb2[x][y][0]][part1]);
		g=in8bitrange(uint8percentage[(signed int)WDrgb2[x][y][1]][part1]);
		b=in8bitrange(uint8percentage[(signed int)WDrgb2[x][y][2]][part1]);*/
		r=WDrgb1[x][y][0];
		g=WDrgb1[x][y][1];
		b=WDrgb1[x][y][2];
		/*if(WDrgb1[x][y][0]<WDrgb2[x][y][0]){
			r=highnumber(WDrgb1[x][y][0],WDrgb2[x][y][0]-change);}
		else{
			r=lownumber(WDrgb1[x][y][0],WDrgb2[x][y][0]+change);}
		if(WDrgb1[x][y][1]<WDrgb2[x][y][1]){
			g=highnumber(WDrgb1[x][y][1],WDrgb2[x][y][1]-change);}
		else{
			g=lownumber(WDrgb1[x][y][1],WDrgb2[x][y][1]+change);}
		if(WDrgb1[x][y][2]<WDrgb2[x][y][2]){
			b=highnumber(WDrgb1[x][y][2],WDrgb2[x][y][2]-change);}
		else{
			b=lownumber(WDrgb1[x][y][2],WDrgb2[x][y][2]+change);}*/
		*pPosition=(r>>3<<11)|(g>>2<<5)|(b>>3<<0);
		pPosition++;
	}
#endif
}

if(WDonoff==0){
	WDonoffpercent=(WDonoffpercent*14+0*1)/15-0.5;
	if(WDonoffpercent<0)WDonoffpercent=0;}
if(WDonoff==1){
	WDonoffpercent=(WDonoffpercent*14+100*1)/15+0.5;
	if(WDonoffpercent>100)WDonoffpercent=100;}

}