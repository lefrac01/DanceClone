void WiiDash_setspritetextcolored(Uint32 color){
	Uint8 r1,g1,b1;
	SDL_GetRGB(color,WDfontimage->format,&r1,&g1,&b1);
	Uint8 r2,g2,b2,a2;
	for(int x=0;x<WDfontimage->w;x++)for(int y=0;y<WDfontimage->h;y++){
		color=getpixel(WDfontimage,x,y);
		SDL_GetRGBA(color,WDfontimage->format,&r2,&g2,&b2,&a2);
		color=SDL_MapRGBA(WDfontimagecolored->format,r1,g1,b1,a2);
		putpixel(WDfontimagecolored,x,y,color);
	}
}
void WiiDash_spritetextcolored(int posx,int posy,char* texttosprite,int leftmiddleright){
	int textlength=strlen(texttosprite);
	int textnumber;
	int letterprintx=0;
	int letterprinty=posy;
	if(leftmiddleright==1)letterprintx=posx;
	if(leftmiddleright==2)letterprintx=posx-textlength*11/2;
	if(leftmiddleright==3)letterprintx=posx-textlength*11;
	for(int b=0;b<textlength;b++){
		textnumber=getnumberfromchar(texttosprite[b]);
		apply_surface(letterprintx,letterprinty,WDfontimagecolored,screen,&WDfontframes[textnumber]);
		letterprintx+=11;
	}
}
void WiiDash_spritetext(int posx,int posy,char* texttosprite,int leftmiddleright){
	int textlength=strlen(texttosprite);
	int textnumber;
	int letterprintx=0;
	int letterprinty=posy;
	if(leftmiddleright==1)letterprintx=posx;
	if(leftmiddleright==2)letterprintx=posx-textlength*11/2;
	if(leftmiddleright==3)letterprintx=posx-textlength*11;
	for(int b=0;b<textlength;b++){
		textnumber=getnumberfromchar(texttosprite[b]);
		apply_surface(letterprintx,letterprinty,WDfontimage,screen,&WDfontframes[textnumber]);
		letterprintx+=11;
	}
}

int WiiDash_spritetextwrapped(int posx,int posy,char* texttosprite,int length){
	int textlength=strlen(texttosprite);
	int textnumber;
	int letterprintx=posx;
	int letterprinty=posy-20;
	int linecount=0;
	int lineend=0;
	for(int b=0;b<textlength;b++){
		if(b==lineend){
			lineend=b+length;
			if(b+length<textlength)
			for(int a=b;a<b+length;a++)if(texttosprite[a]==' ')lineend=a+1;
			linecount++;
			letterprintx=posx;
			letterprinty+=20;
		}
		textnumber=getnumberfromchar(texttosprite[b]);
		apply_surface(letterprintx,letterprinty,WDfontimage,screen,&WDfontframes[textnumber]);
		letterprintx+=11;
	}
	return linecount;
}