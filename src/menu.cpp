/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/

#include "Global.h"
#include "unixutils.h"

bool hideMenu;


SDL_Surface *Menu_SURFACE[7],*ARROW_SURFACE,*LevelListBack;
SDL_Rect Menu_RECT[6];
int choice,frame_count,Levellistcount,levelchoice=0;
char** LevelList,**LevelNameList;
float arrow_speed;
bool levelMenu=0;

void initMenu()
{

	LevelListBack = LoadIMG(DATADIR "gfx/menu/levellistback.png");
	Menu_SURFACE[6] = LoadIMG(DATADIR "gfx/menu/mario.png");

	Menu_SURFACE[0] = LoadIMG(DATADIR "gfx/menu/levellist.png");
	Menu_SURFACE[1] = LoadIMG(DATADIR "gfx/menu/newgame.png");
	Menu_SURFACE[2] = LoadIMG(DATADIR "gfx/menu/load.png");
	if(!show_blood)
		Menu_SURFACE[3] = LoadIMG(DATADIR "gfx/menu/bloodn.png");
	else
		Menu_SURFACE[3] = LoadIMG(DATADIR "gfx/menu/bloody.png");	
	if(nosound==0)
		Menu_SURFACE[4] = LoadIMG(DATADIR "gfx/menu/soundn.png");
	else
		if(nosound==1)
			Menu_SURFACE[4] = LoadIMG(DATADIR "gfx/menu/soundy.png");	
		if(nosound==2)
			Menu_SURFACE[4] = LoadIMG(DATADIR "gfx/menu/allsoundy.png");	
	Menu_SURFACE[5] = LoadIMG(DATADIR "gfx/menu/exit.png");
	ARROW_SURFACE = LoadIMG(DATADIR "gfx/menu/arrow.png");
	
	PLAYER = new cPlayer(SDLK_a,SDLK_d,SDLK_w,SDLK_s,SDLK_q,SDLK_e,12);
	gameStarted = 0;
	choice = 100;
	arrow_speed = 0;	
	for(int i=0;i<7;i++)
	{
		Menu_RECT[i].x = 200;	
	}

	Menu_RECT[1].y = 80 ;
	Menu_RECT[0].y = 80 + 60 ;
	Menu_RECT[2].y = 80 + 60  + 60 ;
	Menu_RECT[3].y = 80 + 60  + 60 + 60;
	Menu_RECT[4].y = 80 + 60  + 60 + 60 + 60;
	Menu_RECT[5].y = 80 + 60  + 60 + 60 + 60 + 60;

	frame_count = -100;
	PLAYER->incincible = 1;


	/// Load Level List!
	loadlevellist();

}
void loadlevellist()
{
	ifstream ifs;
	
#ifdef __unix__
	char buf[MAX_PATH], *home_dir = get_homedir();
	snprintf(buf, MAX_PATH, "%s/.megamario/levellist.txt",
		home_dir? home_dir:".");
	ifs.open(buf, ios::in);
	if (!ifs.is_open())
		/* attention: fall through to after #endif */
#endif
	ifs.open(DATADIR "levels/levellist.txt", ios::in);

	char *contents = new char[150];
	Levellistcount=0;
	LevelList = new char*[80];
	LevelNameList = new char*[80];
	
	for (; ifs.getline(contents, 150);)
	{
		/* strip trailing CR as on Unix ifs.getline() doesn't do that. */
		if (contents[strlen(contents)-1] == '\r')
			contents[strlen(contents)-1] = 0;
		/* Level path should always start with "data/" which we
		   replace with DATADIR */
		if (strncmp(contents, "data/", 5))
		{
			ifs.getline(contents, 150);
			continue;
		}
		LevelList[Levellistcount] = new char[150];
		sprintf( LevelList[Levellistcount], "%s%s",	DATADIR, contents + 5);
			
		ifs.getline(contents, 150);
		/* strip trailing CR as on Unix ifs.getline() doesn't do that. */
		if (contents[strlen(contents)-1] == '\r')
			contents[strlen(contents)-1] = 0;
		LevelNameList[Levellistcount] = new char[150];
		strcpy(LevelNameList[Levellistcount],contents);			
		
		Levellistcount++;
		
	}
	ifs.close();
	delete[](contents);
}
void showMenu()
{	
	Gamepad->reset();
	arrow_speed = 8;	
	if(!gameStarted)
	{
		LEVEL->loadLevel(DATADIR "levels/menulevel",0);
	}
	hideMenu=0;
	choice = 0;
	while(!hideMenu) 
	{
		MenuEvent();
		framerate();
	}
	for(int i=0;i<400;i++)
		keys[i] = 0;


}

void MenuEvent()
{

	frame_count++;	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			exitos();
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
				{
					if(levelMenu)
						levelMenu = 0;
					else if(gameStartedVirtual)
					{	
						hideMenu = 1;
					}					
					return;
				}
				break;

				case SDLK_UP :
					if(levelMenu)
						levelchoice--;
					if(levelchoice<0)
						levelchoice = Levellistcount-1;
				break;
				case SDLK_DOWN :
					if(levelMenu)
						levelchoice++;
					if(levelchoice>Levellistcount-1)
						levelchoice = 0;
				break;

				case SDLK_l :
					levelMenu = !levelMenu;
				break;

				
	
		case SDLK_SPACE:
		case SDLK_RETURN:
	
					if(levelMenu)
					{
						levelMenu = 0;
						ofstream of("log.txt");

						cheater = 0;
						LEVEL->loadLevel(LevelList[levelchoice],0);
						
						resetPlayer();
						hideMenu = 1;
						gameStarted = 1;
						gameStartedVirtual = 1;
						HUD.coins = 0;
						HUD.score = 0;
						HUD.lifes = 5;
						cheater = 0;
						of<<LevelList[levelchoice]<<endl;
						return;
					}
					else
					{
						for(int i=0;i<6;i++)
						{
							if(collision(&tmp_rect,&Menu_RECT[i]))
							{
								if(i==0)
								{///////LEVELLIST
									levelMenu = !levelMenu;
									return;
								}
								else if(i==1)
								{//////NEW
									resetPlayer();
									instruct(DATADIR "gfx/menu/insctructions.jpg");
									instruct(DATADIR "gfx/story/story.jpg");									
									cheater = 0;
									LEVEL->loadLevel(DATADIR "levels/1/main",0);				
									hideMenu = 1;
									gameStarted = 1;
									gameStartedVirtual = 1;
									HUD.coins = 0;
									HUD.score = 0;
									HUD.lifes = 5;
									cheater = 0;
									return;
								}
								else if(i==2)
								{///////LOAD
									cheater = 0;
									resetPlayer();
									LEVEL->loadGame();
									gameStarted = 1;
									gameStartedVirtual = 1;
									hideMenu = 1;
								}
								else if(i==3)
								{
									show_blood = !show_blood;
									SDL_FreeSurface(Menu_SURFACE[3]);
									if(!show_blood)
										Menu_SURFACE[3] = LoadIMG(DATADIR "gfx/menu/bloodn.png");
									else
										Menu_SURFACE[3] = LoadIMG(DATADIR "gfx/menu/bloody.png");	
								}
								else if(i==4)
								{///////SOUND
									nosound++;
									if(nosound==3)
										nosound = 0;
									SDL_FreeSurface(Menu_SURFACE[4]);
									if(nosound==0)
									{
										Menu_SURFACE[4] = LoadIMG(DATADIR "gfx/menu/soundn.png");
										Mix_FadeOutMusic(1);//Mix_PauseMusic();
									}
									else
										if(nosound==1)
										{
											Menu_SURFACE[4] = LoadIMG(DATADIR "gfx/menu/soundy.png");	
											Mix_FadeOutMusic(1);// PauseMusic();
										}
										if(nosound==2)
										{
											Menu_SURFACE[4] = LoadIMG(DATADIR "gfx/menu/allsoundy.png");
											playMusic("megagroove.mid");
										}
								}
								else if(i==5)///////EXIT
									exitos();
								PLAYSOUND1(S_FIREBALL);
								return;
							}
						}
						PLAYSOUND1(S_CRACK);
					}
				break;
				}
			}
		}
	

	keys = SDL_GetKeyState(NULL);


	if((keys[SDLK_UP])&&!levelMenu)
	{
		arrow_speed-=0.3;
		if(arrow_speed<-6)
			arrow_speed = -6;		
	}
	if((keys[SDLK_DOWN])&&!levelMenu)
	{
		arrow_speed+=0.3;
		if(arrow_speed>6)
			arrow_speed = 6;
	}

	if(choice >= 390)
	{
		arrow_speed *= -1;
		choice = 389;
	}
	else if(choice <= 70)
	{
		arrow_speed *= -1;
		choice = 71;			
	}

		
	if(arrow_speed>0.1)
		arrow_speed-=0.1;
	else if(arrow_speed<0.1)
		arrow_speed+=0.1;

	
	SDL_FillRect(screen,0,LEVEL->BGCOLOR);
	if(gameStarted==0)
	{
				if(frame_count == 1)
			keys[SDLK_a] = 1;
		else if(frame_count == 50)
			keys[SDLK_a] = 0;
		else if(frame_count == 90)
		{
			keys[SDLK_d] = 1;
			keys[SDLK_e] = 1;
		}else if(frame_count == 92)
			keys[SDLK_e] = 0;
		else if(frame_count == 150)
			keys[SDLK_d] = 0;
		else if(frame_count == 220)
			keys[SDLK_e] = 1;
		else if(frame_count == 350)
		{
			keys[SDLK_q] = 1;
			keys[SDLK_e] = 0;
			keys[SDLK_d] = 1;
		}else if(frame_count == 560)
		{
			keys[SDLK_q] = 0;
			keys[SDLK_d] = 0;
		}else if(frame_count == 600)
		{
			keys[SDLK_e] = 1;
			keys[SDLK_d] = 1;
		}else if(frame_count == 605)
		{
			keys[SDLK_e] = 0;
			keys[SDLK_d] = 0;
		}
		else if(frame_count == 750)
			keys[SDLK_d] = 1;
		else if(frame_count == 770)
			keys[SDLK_e] = 1;
		else if(frame_count == 800)
			keys[SDLK_e] = 0;
		else if(frame_count == 850)
			keys[SDLK_e] = 1;
		else if(frame_count == 865)
			keys[SDLK_e] = 0;	
		else if(frame_count == 980)
		{
 			keys[SDLK_d] = 0;
			keys[SDLK_a] = 1;
		}if(frame_count == 1050)
		{
			keys[SDLK_d] = 1;
			keys[SDLK_q] = 1;
			keys[SDLK_a] = 0;
			keys[SDLK_e] = 1;
		}else if(frame_count == 1080)
			keys[SDLK_e] = 0;
		else if(frame_count == 1100)
			keys[SDLK_e] = 1;
		else if(frame_count == 1160)
		{
			keys[SDLK_q] = 1;
			keys[SDLK_e] = 0;
		}


		PLAYER->update();
		if(PLAYER->x_speed == 0&&PLAYER->y_speed == -2.33)
		{
			for(int i=0;i<400;i++)
				keys[i] = 0;
			frame_count = 2000;
		}

			
	}
	LEVEL->drawBackground();
	if(gameStarted==0)
		for(int i=0;i<5;i++)
			BONUS_DYNAMIC[i]->update();
	LEVEL->drawLevel();		
	if(gameStarted==0)
	{
		for(int i=0;i<GOOMBAcount;i++)
			GOOMBA[i]->update();
		for(int i=0;i<TURTLEcount;i++)
			TURTLE[i]->update();
		for(int i=0;i<TURTLEFLYcount;i++)
			TURTLEFLY[i]->update();
		for(int i=0;i<SPINYcount;i++)
			SPINY[i]->update();
		for(int i=0;i<FIRECIRCLEcount;i++)
			FIRECIRCLE[i]->update();
		for(int i=0;i<CANNONcount;i++)
			CANNON[i]->update();
		
		CRACK->update();
		updateKickUp();
		PLAYER->draw();
		FIREBALL[0]->update();
		FIREBALL[1]->update();
		GLIDDER->update();
	}

	if(levelMenu)
	{
		tmp_rect.x=130;
		tmp_rect.y=40;
		SDL_UpperBlit(LevelListBack,0,screen,&tmp_rect);
	
		tmp_rect.x+=20;
		tmp_rect.w = 360;
		tmp_rect.h = 10;
		tmp_rect.y = 99+levelchoice*12;
		SDL_FillRect(screen,&tmp_rect,SDL_MapRGB(screen->format, 128, 128,0) );

		for(int i=0;i<Levellistcount;i++)
		{
			StringColor(screen,tmp_rect.x+20,100+i*12,LevelNameList[i],532432);//532532
		}
	}
	else
	{		
		tmp_rect.x = 200;
		tmp_rect.y = 15;
		SDL_BlitSurface(Menu_SURFACE[6],0,screen,&tmp_rect);		

		tmp_rect.x=160;
		choice += arrow_speed;
		tmp_rect.y = choice;
		SDL_BlitSurface(ARROW_SURFACE,0,screen,&tmp_rect);
		
		
		for(int i=0;i<6;i++)
		{
			SDL_BlitSurface(Menu_SURFACE[i],0,screen,&Menu_RECT[i]);
		}
				
	}

	drawGLscreen();
}







char *HUDTEXT=0;
SDL_Surface *hudSurf=0,*hudoverlay = 0;
HUD_STRUCT oldHUD;
void drawInGameStats()
{
	if(oldHUD.coins != HUD.coins||oldHUD.score != HUD.score||oldHUD.lifes != HUD.lifes)
	{
		if(hudSurf)
			SDL_FreeSurface(hudSurf);
		if(HUDTEXT)
			delete[] (HUDTEXT);
		HUDTEXT = new char[70];
		sprintf(HUDTEXT,"Source: %d    Coins : %d    Lift : %d Cheat_Enable: %d",HUD.score,HUD.coins,HUD.lifes,cheats_enabled);
		hudSurf = TTF_RenderText_Blended(font, HUDTEXT,HUDColorbg);	
		//hudSurf = TTF_RenderUTF8_Blended(font, "df",HUDColorbg);	
		tmp_rect.x=1;
		tmp_rect.y=1;
	    hudoverlay = TTF_RenderText_Blended(font,HUDTEXT,HUDColor);
		SDL_BlitSurface(hudoverlay,0,hudSurf,&tmp_rect);
		SDL_FreeSurface(hudoverlay);
	
	}

	tmp_rect.x=50;
	tmp_rect.y=25;
	if(hudSurf)		
		SDL_BlitSurface(hudSurf,0,screen,&tmp_rect);
	
	// 作弊模式一打开、就在左下角显示作弊图案
	if(cheater)
	{
		tmp_rect.x =0;
		tmp_rect.y = HEIGHT-cheatscreen->h;
		SDL_BlitSurface(cheatscreen,0,screen,&tmp_rect);
	}
	

	oldHUD.coins = HUD.coins;
	oldHUD.score = HUD.score;
	oldHUD.lifes = HUD.lifes;

}
