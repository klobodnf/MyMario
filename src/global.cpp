/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/



#include "Global.h"
#include "unixutils.h"



SDL_Surface *screen;
SDL_Surface *helpscreen;
SDL_Surface *cheatscreen;

SDL_Event event;

Uint8 *keys;

char* cheatcode;

bool Real_Fullscreen;

 int WIDTH = 640,GL_Width=1024;
 int HEIGHT = 480,GL_Height=768;

double frame_multi;

#ifndef __unix__
bool NoFrameCheck = 0;
#endif
bool gameStarted = 0,gameStartedVirtual = 0,pixelation=1,show_blood = 0,cheater=0,cheats_enabled=0;
int nosound = 1;

Uint32 magenta;
int cam_x=50,cam_y=3200,bonus_activ=0;

Mix_Chunk *SOUND[MAX_SOUNDS];
s_music_archive MusicArchive;

int musicvolume=MIX_MAX_VOLUME,soundvolume;

SDL_Color HUDColor;
SDL_Color HUDColorbg;
TTF_Font* font;

int GOOMBAcount;
cGoomba* GOOMBA[MAXENEMY];
int GOOMBABIGcount;
cGoombaBig *GOOMBABIG[5];
int TURTLEcount;
cTurtle *TURTLE[MAXENEMY];
int TURTLEFLYcount;
cTurtleFly *TURTLEFLY[MAXENEMY];
int SPINYcount;
cSpiny *SPINY[MAXENEMY];
int FIRECIRCLEcount;
cFireCircle *FIRECIRCLE[MAXENEMY];
int CANNONcount;
cCannon *CANNON[MAXENEMY];
int KILLERBLUMEcount;
cKillerBlume *KILLERBLUME[MAXENEMY];
int BRETTcount;
cBrett *BRETT[MAXENEMY];

cBowser* BOWSER = 0;

cScene *LEVEL;
cPlayer *PLAYER;
cBonus* BONUS_DYNAMIC[5];
cGlidder *GLIDDER;
cCrack *CRACK;
cFireball *FIREBALL[2];
cScore* SCORE;
cFire* FIRE;
cBlood *BLOOD_GENERATOR;

cGamepad *Gamepad;
HUD_STRUCT HUD;

void loadGlobalSprites()
{
	SOUND[S_GROW] = Mix_LoadWAV (DATADIR "sfx/grow.wav");
	SOUND[S_JUMP] = Mix_LoadWAV (DATADIR "sfx/jump.wav");
	SOUND[S_CRACK] = Mix_LoadWAV (DATADIR "sfx/crack.wav");
	SOUND[S_COIN] = Mix_LoadWAV (DATADIR "sfx/coin.wav");
	SOUND[S_KICK] = Mix_LoadWAV (DATADIR "sfx/kick.wav");
	SOUND[S_STOMP] = Mix_LoadWAV (DATADIR "sfx/stomp.wav");
	SOUND[S_FIREBALL] = Mix_LoadWAV (DATADIR "sfx/fireball.wav");
	SOUND[S_STAGE_CLEAR] = Mix_LoadWAV (DATADIR "sfx/stage_clear.wav");
	SOUND[S_1UP] = Mix_LoadWAV (DATADIR "sfx/1-up.wav");
	SOUND[S_DIE] = Mix_LoadWAV (DATADIR "sfx/die.wav");
	SOUND[S_GAMEOVER] = Mix_LoadWAV (DATADIR "sfx/gameover.wav");
	SOUND[S_PAUSE] = Mix_LoadWAV (DATADIR "sfx/pause.wav");
	SOUND[S_PIPE] = Mix_LoadWAV (DATADIR "sfx/pipe.wav");
	SOUND[S_FLAG] = Mix_LoadWAV (DATADIR "sfx/flag.wav");
	SOUND[S_BOWSERFALL] = Mix_LoadWAV (DATADIR "sfx/bowserfall.wav");
	SOUND[S_SHRINK] = Mix_LoadWAV (DATADIR "sfx/shrink.wav");
	SOUND[S_OUTPUT] = Mix_LoadWAV (DATADIR "sfx/output.wav");
	SOUND[S_TOCK] = Mix_LoadWAV (DATADIR "sfx/tock.wav");
	SOUND[S_STOMP2] = Mix_LoadWAV (DATADIR "sfx/stomp_1.wav");
	SOUND[S_CANNON] = Mix_LoadWAV (DATADIR "sfx/cannon.wav");


	LEVEL->Bonus_Textures[1] = LoadIMG(DATADIR "gfx/bonus/crackstone.png");
	LEVEL->Bonus_Textures[2] = LoadIMG(DATADIR "gfx/bonus/box.png");
	LEVEL->Bonus_Textures[3] = LoadIMG(DATADIR "gfx/bonus/box.png");
	LEVEL->Bonus_Textures[4] = LoadIMG(DATADIR "gfx/bonus/box.png");
	LEVEL->Bonus_Textures[5] = LoadIMG(DATADIR "gfx/bonus/stern.png");
	LEVEL->Bonus_Textures[6] = LoadIMG(DATADIR "gfx/bonus/pilz.png");
	LEVEL->Bonus_Textures[7] = LoadIMG(DATADIR "gfx/bonus/boxempty.png");
	LEVEL->Bonus_Textures[8] = LoadIMG(DATADIR "gfx/bonus/invis.png");
	LEVEL->Bonus_Textures[9] = LoadIMG(DATADIR "gfx/bonus/life.png");
	LEVEL->Bonus_Textures[10] = LoadIMG(DATADIR "gfx/bonus/coin1.png");
	LEVEL->Bonus_Textures[11] = LoadIMG(DATADIR "gfx/bonus/coin2.png");
	LEVEL->Bonus_Textures[12] = LoadIMG(DATADIR "gfx/bonus/coin3.png");
	LEVEL->Bonus_Textures[13] = LoadIMG(DATADIR "gfx/bonus/coin4.png");
	LEVEL->Bonus_Textures[14] = LoadIMG(DATADIR "gfx/bonus/crackstone2.png");
	LEVEL->Bonus_Textures[15] = LoadIMG(DATADIR "gfx/bonus/invis.png");
	LEVEL->Bonus_Textures[16] = LoadIMG(DATADIR "gfx/bonus/box.png");
	LEVEL->Bonus_Textures[17] = LoadIMG(DATADIR "gfx/bonus/axe.png");
	LEVEL->Bonus_Textures[18] = LoadIMG(DATADIR "gfx/bonus/fire1.png");
	LEVEL->Bonus_Textures[19] = LoadIMG(DATADIR "gfx/bonus/fire2.png");
	LEVEL->Bonus_Textures[20] = LoadIMG(DATADIR "gfx/bonus/crackedlong.png");

	helpscreen = LoadIMG(DATADIR "gfx/story/help.png");
	cheatscreen = LoadIMG(DATADIR "gfx/bonus/cheater.png");

	initGOOMBA();
	initSPINY();
	initTURTLE();
	initCANNON();
	initKILLERBLUUME();
	initBRETT();
	initGOOMBAbig();


	HUDColor.r = 50;
	HUDColor.g = 190;
	HUDColor.b = 150;
	HUDColorbg.r = 30;
	HUDColorbg.g = 31;
	HUDColorbg.b = 31;
}
void unloadGlobalSprites()
{
	for(int i=1;i<MAX_BONUS_TEXTURES;i++)
		SDL_FreeSurface(LEVEL->Bonus_Textures[i]);
	
	SDL_FreeSurface(screen);
}

void THEGAMEENDSHERE()
{
#ifdef __unix__
char buf[MAX_PATH], *home_dir = get_homedir();
snprintf(buf, MAX_PATH, "%s/.megamario/levellist.txt",
	home_dir? home_dir:".");
ofstream filelistout(buf);
#else
ofstream filelistout(DATADIR "levels/levellist.txt");
#endif

filelistout<<"data/levels/old/lab/main\n!- The extreme MarioTestLab - Massacre -!\ndata/levels/menulevel2\n!- The funky death Jump -!\ndata/levels/old/2/main\n!- The first level of the original SMB -!\ndata/levels/old/1/main\n#- Nonsense -#\ndata/levels/1/main\nLev. 1 - The Escape from the castle\ndata/levels/2/main\nLev. 2 - Getaway\ndata/levels/3/main\nLev. 3 - The story starts\ndata/levels/4/main\nLev. 4 - Gettin' higher\ndata/levels/5/main\nLev. 5 - First castle\ndata/levels/6/main\n";
filelistout<<"Lev. 6 - Crossed pipes\ndata/levels/7/main\nLev. 7 - What about stars ?\ndata/levels/8/main\nLev. 8 - Bullet Bill strikes back\ndata/levels/9/main\nLev. 9 - Castle 2 - Deep down below\ndata/levels/10/main\nLev. 10 - The Egypt hole 1\ndata/levels/11/main\nLev. 11 - The Hole 2\ndata/levels/12/main\nLev. 12 - Nice weather\ndata/levels/13/main\nLev. 13 - Castle 3 - Trapped\ndata/levels/14/main\nLev. 14 - A lovely canyon\ndata/levels/15/main\nLev. 15 - Fire in the Hole\ndata/levels/16/main\nLev. 16 - Maze\ndata/levels/17/main\n";
filelistout<<"Lev. 17 - Canyon Bowser Battle\ndata/levels/18/main\nLev. 18 - ?????????????????\ndata/levels/19/main\n";
filelistout<<"Lev. 19 - Castle 4 - Climb The Evil\ndata/levels/20/main\nLev. 20 - Elevator Attator\ndata/levels/21/main\nLev. 21 - Big World\ndata/levels/22/main\nLev. 22 - Bullet Bill\ndata/levels/23/main\nLev. 23 - Way to hell\ndata/levels/24/main\nLev. 24 - Final Castle - Power Battle";
	
filelistout.close();
loadlevellist();

#ifdef __unix__
snprintf(buf, MAX_PATH, "%s/.megamario/cheats.txt",
	home_dir? home_dir:".");
ofstream cheatout(buf);
#else
ofstream cheatout("cheats.txt");
#endif
cheatout<<"Ohoh, seems like you completed the game.\n\nCheatmode is enabled.\n\n"<<endl;
cheatout<<"Cheats (Enter in Game):\n\n big = mushroom\n fire = firemario\n small = smallmario\n die = die\n\n\nPress 1 and 5 to fly!!! \nHave a Lot Of Fun!";
cheatout.close();

cheats_enabled = 1;	

	playMusic("ending.mid");
	SDL_Surface * surfs;	

	surfs = LoadIMG(DATADIR "gfx/story/outro1");


	bool ACDC=1;
	while(ACDC)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exitos();
					break;
				case SDL_KEYDOWN:
					ACDC = 0;
				break;
			}
		}
		SDL_UpperBlit(surfs,0,screen,0);
		
		drawGLscreen();
	}
	SDL_FillRect(screen,0,0);
	drawGLscreen();
	SDL_FreeSurface(surfs);
	surfs = LoadIMG(DATADIR "gfx/story/outro2");
	SDL_Delay(500);
ACDC=1;
	while(ACDC)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exitos();
					break;
				case SDL_KEYDOWN:
					ACDC = 0;
				break;
			}
		}
		SDL_UpperBlit(surfs,0,screen,0);
		drawGLscreen();
	}SDL_FillRect(screen,0,0);
	drawGLscreen();
	SDL_FreeSurface(surfs);
	surfs = LoadIMG(DATADIR "gfx/story/outro3");
	SDL_Delay(500);
	ACDC=1;
	while(ACDC)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exitos();
					break;
				case SDL_KEYDOWN:
					ACDC = 0;
				break;
			}
		}
		SDL_UpperBlit(surfs,0,screen,0);
		drawGLscreen();
	}SDL_FillRect(screen,0,0);
	drawGLscreen();
	SDL_FreeSurface(surfs);
	surfs = LoadIMG(DATADIR "gfx/story/outro4");
	SDL_Delay(500);
	ACDC=1;
	while(ACDC)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exitos();
					break;
				case SDL_KEYDOWN:
					ACDC = 0;
				break;
			}
		}
		SDL_UpperBlit(surfs,0,screen,0);
		drawGLscreen();
	}SDL_FillRect(screen,0,0);
	drawGLscreen();
	SDL_FreeSurface(surfs);
	surfs = LoadIMG(DATADIR "gfx/story/outro5");
	SDL_Delay(500);
	ACDC=1;
	while(ACDC)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exitos();
					break;
				case SDL_KEYDOWN:
					ACDC = 0;
				break;
			}
		}
		SDL_UpperBlit(surfs,0,screen,0);
		drawGLscreen();
	}SDL_FillRect(screen,0,0);
	drawGLscreen();
	SDL_FreeSurface(surfs);
	surfs = LoadIMG(DATADIR "gfx/story/outro6");
	SDL_Delay(500);
ACDC=1;
	while(ACDC)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exitos();
					break;
				case SDL_KEYDOWN:
					ACDC = 0;
				break;
			}
		}
		SDL_UpperBlit(surfs,0,screen,0);
		drawGLscreen();
	}
	SDL_FreeSurface(surfs);
	
	
	loadlevellist();
	showMenu();
}




void instruct(char* path)
{
	bool READY=0;

	SDL_Surface* sch = LoadIMG(path);
	
	SDL_UpperBlit(sch,0,screen,0);
	drawGLscreen();
	SDL_BlitSurface(sch,0,screen,0);
	drawGLscreen();
	
	SDL_Delay(2000);
	SDL_Event MegaEvent;
	while(!READY)
	{
		while(SDL_PollEvent(&MegaEvent))
		{
			switch(MegaEvent.type)
			{
					case SDL_KEYDOWN:
					READY=1;
				break;
								
			}			
		}
	}
	SDL_FreeSurface(sch);
}
