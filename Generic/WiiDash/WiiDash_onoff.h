int WiiDash_onoffslide(int onpos,int offpos){
int currentpos=(int)((onpos)*(WDonoffpercent/100)+(offpos)*((100-WDonoffpercent)/100));
return currentpos;}

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
      for(int x=0;x<rmode->viWidth;x++)for(int y=0;y<rmode->viHeight;y++)
        SDL_GetRGB(getpixel(screen,x,y),
        screen->format,&WDrgb1[x][y][0],&WDrgb1[x][y][1],&WDrgb1[x][y][2]);
      #endif
      #ifdef WII
      Uint32 color;
      Uint16* pPosition=(Uint16*)screen->pixels;
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "extracting separate rgb pixels into stash 1" << endl;
        //#log.close();
      }

      /*
      for(int y=0;y<rmode->viHeight;y++)
      {
        for(int x=0;x<rmode->viWidth;x++)
        {
          color=*pPosition;
          
          
          pWDrgb1[x][y][0]=(((color&63488)>>11)<<3);
          pWDrgb1[x][y][1]=(((color&2016)>>5)<<2);
          pWDrgb1[x][y][2]=(((color&31)>>0)<<3);
          pPosition++;
        }
      }
      */
     
      for(int x=0;x<rmode->viWidth;x++)
      {
        int inner_offset_base = x*rmode->viHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + rmode->viHeight*3;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
        {
          color=*pPosition;
          pWDrgb1[inner_offset  ]=(((color&63488)>>11)<<3);
          pWDrgb1[inner_offset+1]=(((color&2016)>>5)<<2);
          pWDrgb1[inner_offset+2]=(((color&31)>>0)<<3);
          pPosition++;
        }      
      }
      #endif
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "copying stash 2 into stash 1" << endl;
        //#log.close();
      }
      memcpy(pWDrgb2,pWDrgb1,rmode->viWidth*rmode->viHeight*3*sizeof(Uint8));
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "initial blur on stash 2" << endl;
        //#log.close();
      }

      /*
      for(int a=0;a<1;a++){
        for(int x=3;x<rmode->viWidth-3;x++)for(int y=3;y<rmode->viHeight-3;y++){
          pWDrgb2[x][y][0]=(pWDrgb1[x][y-3][0]+pWDrgb1[x][y+3][0]+pWDrgb1[x-3][y][0]+pWDrgb1[x+3][y][0])/4;
          pWDrgb2[x][y][1]=(pWDrgb1[x][y-3][1]+pWDrgb1[x][y+3][1]+pWDrgb1[x-3][y][1]+pWDrgb1[x+3][y][1])/4;
          pWDrgb2[x][y][2]=(pWDrgb1[x][y-3][2]+pWDrgb1[x][y+3][2]+pWDrgb1[x-3][y][2]+pWDrgb1[x+3][y][2])/4;
        }for(int x=3;x<rmode->viWidth-3;x++)for(int y=3;y<rmode->viHeight-3;y++){
          pWDrgb1[x][y][0]=(pWDrgb2[x][y-3][0]+pWDrgb2[x][y+3][0]+pWDrgb2[x-3][y][0]+pWDrgb2[x+3][y][0]+1)/4;
          pWDrgb1[x][y][1]=(pWDrgb2[x][y-3][1]+pWDrgb2[x][y+3][1]+pWDrgb2[x-3][y][1]+pWDrgb2[x+3][y][1]+1)/4;
          pWDrgb1[x][y][2]=(pWDrgb2[x][y-3][2]+pWDrgb2[x][y+3][2]+pWDrgb2[x-3][y][2]+pWDrgb2[x+3][y][2]+1)/4;
        }
      }
      */
      for(int x=3;x<rmode->viWidth-3;x++)
      {
        int inner_base = x*rmode->viHeight*3;
        int inner_left_base = (x-3)*rmode->viHeight*3;
        int inner_right_base = (x+3)*rmode->viHeight*3;
        
        int lower_bound = inner_base + 3*3;
        int upper_bound = inner_base + (rmode->viHeight-3)*3;
        int inner_up = inner_base;
        int inner_down = inner_base + 6*3;
        int inner_left = inner_left_base + 3*3;
        int inner_right = inner_right_base + 3*3;
        for (int inner = lower_bound; inner < upper_bound; inner += 3, inner_left += 3, inner_right += 3)
        {
          pWDrgb2[inner  ] = (  pWDrgb1[inner_up  ] + pWDrgb1[inner_down  ] + pWDrgb1[inner_left  ] + pWDrgb1[inner_right  ]  )/4;
          pWDrgb2[inner+1] = (  pWDrgb1[inner_up+1] + pWDrgb1[inner_down+1] + pWDrgb1[inner_left+1] + pWDrgb1[inner_right+1]  )/4;
          pWDrgb2[inner+2] = (  pWDrgb1[inner_up+2] + pWDrgb1[inner_down+2] + pWDrgb1[inner_left+2] + pWDrgb1[inner_right+2]  )/4;
        }
      }
      for(int x=3;x<rmode->viWidth-3;x++)
      {
        int inner_base = x*rmode->viHeight*3;
        int inner_left_base = (x-3)*rmode->viHeight*3;
        int inner_right_base = (x+3)*rmode->viHeight*3;
        
        int lower_bound = inner_base + 3*3;
        int upper_bound = inner_base + (rmode->viHeight-3)*3;
        int inner_up = inner_base;
        int inner_down = inner_base + 6*3;
        int inner_left = inner_left_base + 3*3;
        int inner_right = inner_right_base + 3*3;
        for (int inner = lower_bound; inner < upper_bound; inner += 3, inner_left += 3, inner_right += 3, inner_up += 3, inner_down += 3)
        {
          pWDrgb1[inner  ] = (  pWDrgb2[inner_up  ] + pWDrgb2[inner_down  ] + pWDrgb2[inner_left  ] + pWDrgb2[inner_right  ] + 1 )/4;
          pWDrgb1[inner+1] = (  pWDrgb2[inner_up+1] + pWDrgb2[inner_down+1] + pWDrgb2[inner_left+1] + pWDrgb2[inner_right+1] + 1 )/4;
          pWDrgb1[inner+2] = (  pWDrgb2[inner_up+2] + pWDrgb2[inner_down+2] + pWDrgb2[inner_left+2] + pWDrgb2[inner_right+2] + 1 )/4;
        }
      }
      
      
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "second blur back to stash 1" << endl;
        //#log.close();
      }
      /*
      for(int a=0;a<2;a++){
        for(int x=1;x<rmode->viWidth-1;x++)for(int y=1;y<rmode->viHeight-1;y++){
          pWDrgb2[x][y][0]=(pWDrgb1[x][y-1][0]+pWDrgb1[x][y+1][0]+pWDrgb1[x-1][y][0]+pWDrgb1[x+1][y][0])/4;
          pWDrgb2[x][y][1]=(pWDrgb1[x][y-1][1]+pWDrgb1[x][y+1][1]+pWDrgb1[x-1][y][1]+pWDrgb1[x+1][y][1])/4;
          pWDrgb2[x][y][2]=(pWDrgb1[x][y-1][2]+pWDrgb1[x][y+1][2]+pWDrgb1[x-1][y][2]+pWDrgb1[x+1][y][2])/4;
        }for(int x=1;x<rmode->viWidth-1;x++)for(int y=1;y<rmode->viHeight-1;y++){
          pWDrgb1[x][y][0]=(pWDrgb2[x][y-1][0]+pWDrgb2[x][y+1][0]+pWDrgb2[x-1][y][0]+pWDrgb2[x+1][y][0])/4;
          pWDrgb1[x][y][1]=(pWDrgb2[x][y-1][1]+pWDrgb2[x][y+1][1]+pWDrgb2[x-1][y][1]+pWDrgb2[x+1][y][1])/4;
          pWDrgb1[x][y][2]=(pWDrgb2[x][y-1][2]+pWDrgb2[x][y+1][2]+pWDrgb2[x-1][y][2]+pWDrgb2[x+1][y][2])/4;
        }
      }
      */
      for(int a=0;a<2;a++){
        for(int x=1;x<rmode->viWidth-1;x++)
        {
          int inner_base = x*rmode->viHeight*3;
          int inner_left_base = (x-1)*rmode->viHeight*3;
          int inner_right_base = (x+1)*rmode->viHeight*3;
          
          int lower_bound = inner_base + 1*3;
          int upper_bound = inner_base + (rmode->viHeight-1)*3;
          int inner_up = inner_base;
          int inner_down = inner_base + 2*3;
          int inner_left = inner_left_base + 1*3;
          int inner_right = inner_right_base + 1*3;
          for (int inner = lower_bound; inner < upper_bound; inner ++, inner_left ++, inner_right ++, inner_up ++, inner_down ++)
          {
            pWDrgb2[inner  ] = (  pWDrgb1[inner_up  ] + pWDrgb1[inner_down  ] + pWDrgb1[inner_left  ] + pWDrgb1[inner_right  ]  )/4;
            pWDrgb2[inner+1] = (  pWDrgb1[inner_up+1] + pWDrgb1[inner_down+1] + pWDrgb1[inner_left+1] + pWDrgb1[inner_right+1]  )/4;
            pWDrgb2[inner+2] = (  pWDrgb1[inner_up+2] + pWDrgb1[inner_down+2] + pWDrgb1[inner_left+2] + pWDrgb1[inner_right+2]  )/4;
          }
        }
        for(int x=1;x<rmode->viWidth-1;x++)
        {
          int inner_base = x*rmode->viHeight*3;
          int inner_left_base = (x-1)*rmode->viHeight*3;
          int inner_right_base = (x+1)*rmode->viHeight*3;
          
          int lower_bound = inner_base + 1*3;
          int upper_bound = inner_base + (rmode->viHeight-1)*3;
          int inner_up = inner_base;
          int inner_down = inner_base + 2*3;
          int inner_left = inner_left_base + 1*3;
          int inner_right = inner_right_base + 1*3;
          for (int inner = lower_bound; inner < upper_bound; inner ++, inner_left ++, inner_right ++, inner_up ++, inner_down ++)
          {
            pWDrgb1[inner  ] = (  pWDrgb2[inner_up  ] + pWDrgb2[inner_down  ] + pWDrgb2[inner_left  ] + pWDrgb2[inner_right  ]  )/4;
            pWDrgb1[inner+1] = (  pWDrgb2[inner_up+1] + pWDrgb2[inner_down+1] + pWDrgb2[inner_left+1] + pWDrgb2[inner_right+1]  )/4;
            pWDrgb1[inner+2] = (  pWDrgb2[inner_up+2] + pWDrgb2[inner_down+2] + pWDrgb2[inner_left+2] + pWDrgb2[inner_right+2]  )/4;
          }
        }
      }
      
      
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "finalise blur effect on stash 1" << endl;
        //#log.close();
      }
      
      /*
      for(int y=0;y<rmode->viHeight;y++)for(int x=0;x<rmode->viWidth;x++){
        pWDrgb1[x][y][0]=in8bitrange(pWDrgb1[x][y][0]+50);
        pWDrgb1[x][y][1]=in8bitrange(pWDrgb1[x][y][1]+50);
        pWDrgb1[x][y][2]=in8bitrange(pWDrgb1[x][y][2]+50);
        pWDrgb1[x][y][0]=in8bitrange(pWDrgb1[x][y][0]-100);
        pWDrgb1[x][y][1]=in8bitrange(pWDrgb1[x][y][1]-100);
        pWDrgb1[x][y][2]=in8bitrange(pWDrgb1[x][y][2]-100);
        pWDrgb1[x][y][0]=in8bitrange(pWDrgb1[x][y][0]+100);
        pWDrgb1[x][y][1]=in8bitrange(pWDrgb1[x][y][1]+100);
        pWDrgb1[x][y][2]=in8bitrange(pWDrgb1[x][y][2]+100);
      }
      */
      
      for(int x = 0; x < rmode->viWidth; x++)
      {
        int inner_offset_base = x*rmode->viHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + rmode->viHeight*3;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
        {
          pWDrgb1[inner_offset  ] = in8bitrange( pWDrgb1[inner_offset  ] + 50);
          pWDrgb1[inner_offset+1] = in8bitrange( pWDrgb1[inner_offset+1] + 50);
          pWDrgb1[inner_offset+2] = in8bitrange( pWDrgb1[inner_offset+2] + 50);
          pWDrgb1[inner_offset  ] = in8bitrange( pWDrgb1[inner_offset  ] - 100);
          pWDrgb1[inner_offset+1] = in8bitrange( pWDrgb1[inner_offset+1] - 100);
          pWDrgb1[inner_offset+2] = in8bitrange( pWDrgb1[inner_offset+2] - 100);
          pWDrgb1[inner_offset  ] = in8bitrange( pWDrgb1[inner_offset  ] + 100);
          pWDrgb1[inner_offset+1] = in8bitrange( pWDrgb1[inner_offset+1] + 100);
          pWDrgb1[inner_offset+2] = in8bitrange( pWDrgb1[inner_offset+2] + 100);
        }
      }
      
      
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "recomposing separated RGB values for blur into screen WDbackgroundblurred" << endl;
        //#log.close();
      }
      #ifdef WIN
      for(int y=0;y<rmode->viHeight;y++)for(int x=0;x<rmode->viWidth;x++)
        putpixel(WDbackgroundblurred,x,y,
          SDL_MapRGB(WDbackgroundblurred->format,
          pWDrgb1[x][y][0],pWDrgb1[x][y][1],pWDrgb1[x][y][2]));
      #endif
      #ifdef WII
      /*
      pPosition=(Uint16*)WDbackgroundblurred->pixels;
      for(int y=0;y<rmode->viHeight;y++)for(int x=0;x<rmode->viWidth;x++){
        *pPosition=
          (pWDrgb1[x][y][0]>>3<<11)|
          (pWDrgb1[x][y][1]>>2<<5)|
          (pWDrgb1[x][y][2]>>3<<0);
        pPosition++;}
      */
      pPosition=(Uint16*)WDbackgroundblurred->pixels;
      for(int x=0;x<rmode->viWidth;x++)
      {
        int inner_offset_base = x*rmode->viHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + rmode->viHeight*3;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
        {
          *pPosition = 
            (pWDrgb1[inner_offset  ] >> 3 << 11) |
            (pWDrgb1[inner_offset+1] >> 2 << 5 ) |
            (pWDrgb1[inner_offset+2] >> 3 << 0 );
          pPosition++;
        }
      }
      #endif
      
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        //#log.open("debug", std::ios_base::app);
        log << "copy WDbackground pixels into stash 2" << endl;
        //#log.close();
      }
      /*
      for(int y=0;y<rmode->viHeight;y++)for(int x=0;x<rmode->viWidth;x++)
        SDL_GetRGB(getpixel(WDbackground,x,y),WDbackground->format,
          &pWDrgb2[x][y][0],&pWDrgb2[x][y][1],&pWDrgb2[x][y][2]);
      */
      for(int x=0;x<rmode->viWidth;x++)
      {
        int inner_offset_base = x*rmode->viHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + rmode->viHeight*3;
        int y = 0;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3, y++)
        {
          SDL_GetRGB(getpixel(WDbackground,x,y),WDbackground->format,
            &(pWDrgb2[inner_offset  ]), &(pWDrgb2[inner_offset+1]), &(pWDrgb2[inner_offset+2]) );
        }
      }
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
  for(int y=0;y<rmode->viHeight;y++)for(int x=0;x<rmode->viWidth;x++){
    r=in8bitrange((pWDrgb2[x][y][0]*part1+pWDrgb1[x][y][0]*part2)/100);
    g=in8bitrange((pWDrgb2[x][y][1]*part1+pWDrgb1[x][y][1]*part2)/100);
    b=in8bitrange((pWDrgb2[x][y][2]*part1+pWDrgb1[x][y][2]*part2)/100);
    color=
      (r>>fmt->Rloss<<fmt->Rshift)|
      (g>>fmt->Gloss<<fmt->Gshift)|
      (b>>fmt->Bloss<<fmt->Bshift);
    putpixel(screen,x,y,color);
  }
#endif
#ifdef WII

  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    //#log.open("debug", std::ios_base::app);
    log << "perform wiidash open/close animation" << endl;
    //#log.close();
  }

  /*
  Uint8 r,g,b;
  int part1=100-(int)WDonoffpercent;   int part2=(int)WDonoffpercent;
  //int change=(int)(WDonoffpercent/100*255);
 // Uint16* pPosition=(Uint16*)screen->pixels;
  for(int y=0;y<rmode->viHeight;y++)for(int x=0;x<rmode->viWidth;x++){
    r=in8bitrange((WDrgb2[x][y][0]*part1+WDrgb1[x][y][0]*part2)/100);
    g=in8bitrange((WDrgb2[x][y][1]*part1+WDrgb1[x][y][1]*part2)/100);
    b=in8bitrange((WDrgb2[x][y][2]*part1+WDrgb1[x][y][2]*part2)/100);
    r=in8bitrange(uint8percentage[(signed int)WDrgb2[x][y][0]][part1]);
    g=in8bitrange(uint8percentage[(signed int)WDrgb2[x][y][1]][part1]);
    b=in8bitrange(uint8percentage[(signed int)WDrgb2[x][y][2]][part1]);
    r=pWDrgb1[x][y][0];
    g=pWDrgb1[x][y][1];
    b=pWDrgb1[x][y][2];
    if(WDrgb1[x][y][0]<WDrgb2[x][y][0]){
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
      b=lownumber(WDrgb1[x][y][2],WDrgb2[x][y][2]+change);}
      
      
    *pPosition=(r>>3<<11)|(g>>2<<5)|(b>>3<<0);
    pPosition++;
  }
  */




  Uint8 r,g,b;
  // int part1=100-(int)WDonoffpercent;   int part2=(int)WDonoffpercent;
  // int change=(int)(WDonoffpercent/100*255);
  Uint16* pPosition=(Uint16*)screen->pixels;
  
  for(int x=0;x<rmode->viWidth;x++)
  {
    int inner_offset_base = x*rmode->viHeight*3;
    int lower_bound = inner_offset_base;
    int upper_bound = inner_offset_base + rmode->viHeight*3;
    for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
    {
      // r=in8bitrange((   pWDrgb2[inner_offset  ] * part1 + pWDrgb1[inner_offset  ] * part2)/100);
      // g=in8bitrange((   pWDrgb2[inner_offset+1] * part1 + pWDrgb1[inner_offset+1] * part2)/100);
      // b=in8bitrange((   pWDrgb2[inner_offset+2] * part1 + pWDrgb1[inner_offset+2] * part2)/100);
      
      // r=in8bitrange(uint8percentage[(signed int)  pWDrgb2[inner_offset  ] ][part1]);
      // g=in8bitrange(uint8percentage[(signed int)  pWDrgb2[inner_offset+1] ][part1]);
      // b=in8bitrange(uint8percentage[(signed int)  pWDrgb2[inner_offset+2] ][part1]);

      r=pWDrgb1[inner_offset  ];
      g=pWDrgb1[inner_offset+1];
      b=pWDrgb1[inner_offset+2];      
      
      //  if(WDrgb1[x][y][0]<WDrgb2[x][y][0]){
      //  r=highnumber(WDrgb1[x][y][0],WDrgb2[x][y][0]-change);}
      //else{
      //  r=lownumber(WDrgb1[x][y][0],WDrgb2[x][y][0]+change);}
      //if(WDrgb1[x][y][1]<WDrgb2[x][y][1]){
      //  g=highnumber(WDrgb1[x][y][1],WDrgb2[x][y][1]-change);}
      //else{
      //  g=lownumber(WDrgb1[x][y][1],WDrgb2[x][y][1]+change);}
      //if(WDrgb1[x][y][2]<WDrgb2[x][y][2]){
      //  b=highnumber(WDrgb1[x][y][2],WDrgb2[x][y][2]-change);}
      //else{
      //  b=lownumber(WDrgb1[x][y][2],WDrgb2[x][y][2]+change);}
      
      *pPosition=(r>>3<<11)|(g>>2<<5)|(b>>3<<0);
      pPosition++;
    }
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
