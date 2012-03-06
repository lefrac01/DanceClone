// Game_menu_debug

void Game_menu_debug()
{
  string temp_text;
  int y=40;

  temp_text = "debug";
  WiiDash_spritetext(25,y,(char*)temp_text.c_str(),1);


  apply_surface(30, 70, freeze_hit_image, screen, NULL);

  apply_surface(80, 260, freeze_arrows_tail_image, screen, &freeze_tail_frames[3]); //12
  apply_surface(80, 132, freeze_arrows_body_image, screen, &freeze_body_frames[3]); //12
  apply_surface(80, 100, freeze_arrows_head_image, screen, &freeze_head_frames[3]); //16

  WiiDash_spritetext(180,420,(char*)temp_text.c_str(),1);

  
  if(WiiDash_button(rmode->viWidth-300-40,rmode->viHeight-10-40,100,10,0,1,(char*)"Back"))gamestate=0;
}
