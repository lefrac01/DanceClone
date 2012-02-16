//TODO: minimum use d pad to speed up arrow entry, 10s, 50s right and up, minus opposite dirs
//

//NOTE: these two depend on pre-existing BPM data.
//TODO: support real-time stepping that gathers data and saves to file
//
//TODO: support beat by beat modification


int songbpm=115;
int songdelay=3075;
int numarrowstocreate=216;

void Game_menu_stepcreate(){
  
  char temptext[100];
  
  WiiDash_spritetext(40,55+1*40,(char*)"How fast is the song?",1);
  
  if(WiiDash_button(640/2-250-42+75,55+2*40,60,10,1,1,(char*)"")){songbpm=songbpm-1;}
  apply_surface(640/2-250-42-15/2-1+75,55+2*40-16/2-1,WDarrowsimage,screen,&WDarrowsframes[1]);
  if(WiiDash_button(640/2+250+42-75,55+2*40,60,10,1,1,(char*)"")){songbpm=songbpm+1;}
  apply_surface(640/2+250+42-15/2-1-75,55+2*40-16/2-1,WDarrowsimage,screen,&WDarrowsframes[3]);
  sprintf(temptext,"%d%s",songbpm," BPM");
  if(WiiDash_button(640/2,55+2*40,300,10,1,0,temptext)){}
  
  WiiDash_spritetext(40,55+3*40,(char*)"When does the first beat occur?",1);
  
  if(WiiDash_button(640/2-250-42+75,55+4*40,60,10,1,1,(char*)"")){songdelay=songdelay-1;}
  apply_surface(640/2-250-42-15/2-1+75,55+4*40-16/2-1,WDarrowsimage,screen,&WDarrowsframes[1]);
  if(WiiDash_button(640/2+250+42-75,55+4*40,60,10,1,1,(char*)"")){songdelay=songdelay+1;}
  apply_surface(640/2+250+42-15/2-1-75,55+4*40-16/2-1,WDarrowsimage,screen,&WDarrowsframes[3]);
  sprintf(temptext,"%d%s",songdelay," millisecond delay");
  if(WiiDash_button(640/2,55+4*40,300,10,1,0,temptext)){}
  
  WiiDash_spritetext(40,55+5*40,(char*)"How many beats are there before the song ends?",1);
  
  if(WiiDash_button(640/2-250-42+75,55+6*40,60,10,1,1,(char*)"")){numarrowstocreate=numarrowstocreate-1;}
  apply_surface(640/2-250-42-15/2-1+75,55+6*40-16/2-1,WDarrowsimage,screen,&WDarrowsframes[1]);
  if(WiiDash_button(640/2+250+42-75,55+6*40,60,10,1,1,(char*)"")){numarrowstocreate=numarrowstocreate+1;}
  apply_surface(640/2+250+42-15/2-1-75,55+6*40-16/2-1,WDarrowsimage,screen,&WDarrowsframes[3]);
  sprintf(temptext,"%d%s",numarrowstocreate," arrows");
  if(WiiDash_button(640/2,55+6*40,300,10,1,0,temptext)){}
  
  if(WiiDash_button(320,55+8*40,600,10,1,1,(char*)"Accept")){
    sprintf(temptext,"%s%s%s","Music/",songfilename,".dc");
    ofstream outdata;outdata.open(temptext);
    outdata << "<startbeginner>" << endl;
    for(int a=0; a<numarrowstocreate; a++){
      if(rand()%(10)<6){
        if(rand()%(2)==0){outdata << 0 << endl;}else{outdata << 3 << endl;}
        outdata << (int)((double)a*1000*60/(double)songbpm+songdelay) << endl;
        outdata << 0 << endl;
        outdata << "----" << endl;
      }
    }outdata << "<stop>" << endl;
    outdata << "<starteasy>" << endl;
    for(int a=0; a<numarrowstocreate; a++){
      if(rand()%(10)<8){
        outdata << rand()%(4) << endl;
        outdata << (int)((double)a*1000*60/(double)songbpm+songdelay) << endl;
        outdata << 0 << endl;
        outdata << "----" << endl;
      }
    }outdata << "<stop>" << endl;
    outdata << "<startmedium>" << endl;
    for(int a=0; a<numarrowstocreate; a++){
      outdata << rand()%(4) << endl;
      outdata << (int)((double)a*1000*60/(double)songbpm+songdelay) << endl;
      outdata << 0 << endl;
      outdata << "----" << endl;
    }outdata << "<stop>" << endl;   
    outdata << "<starthard>" << endl;
    for(int a=0; a<numarrowstocreate; a++){
      outdata << rand()%(4) << endl;
      outdata << (int)((double)a*1000*60/(double)songbpm+songdelay) << endl;
      outdata << 0 << endl;
      outdata << "----" << endl;
    }outdata << "<stop>" << endl;
    outdata << "<startchallenge>" << endl;
    for(int a=0; a<numarrowstocreate*2; a++){
      outdata << rand()%(4) << endl;
      outdata << (int)((double)a/2*1000*60/(double)songbpm+songdelay) << endl;
      outdata << 0 << endl;
      outdata << "----" << endl;
    }outdata << "<stop>" << endl;
    outdata.close();
    gamestate=4;
  }
  
  if(WiiDash_button(640-100-40,480-10-40,100,10,0,1,(char*)"Back"))gamestate=4;
}
