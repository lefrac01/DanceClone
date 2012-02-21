void Game_menu_titlescreen(){

  apply_surface(rmode->viWidth/2-300/2, 30, titleimage, screen, NULL);
  
  if(WiiDash_button(rmode->viWidth/2,55+4*40,600,10,1,1,(char*)"Play")){gamestate=3;}
  if(WiiDash_button(rmode->viWidth/2,55+5*40,600,10,1,1,(char*)"Credits")){gamestate=1;}

}
