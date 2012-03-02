// Game_menu_debug

void Game_menu_debug()
{
  string temp_text;
  int y=40;

  temp_text = "debug";
  WiiDash_spritetext(25,y,(char*)temp_text.c_str(),1);


  apply_surface(40, 80, quarter_arrows_image, screen, NULL);

  WiiDash_spritetext(180,420,(char*)temp_text.c_str(),1);

  
  if(WiiDash_button(rmode->viWidth-300-40,rmode->viHeight-10-40,100,10,0,1,(char*)"Back"))gamestate=0;
}
