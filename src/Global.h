/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/



#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//C++'s

#include <stdlib.h>
//#include <stdio.h>

#ifndef __unix__
#include <windows.h>
#endif

#include <iostream>
#include <fstream>


#ifndef __GNUC__
#pragma warning(disable:4244)
#pragma warning(disable:4800)
#pragma warning(disable:4154)
#pragma warning(disable:4305)
#pragma warning(disable:4018)
#pragma warning(disable:4018)
#pragma warning(disable:4996)
#endif

using std::string;

using std::flush;
using std::ifstream;
using std::fstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::iostream;
using std::stringstream;
using std::ios;

#if defined( _MSC_VER ) && _MSC_VER < 1400 // no Visual Studio 8 and higher
using std::find_if;
#endif

//SDL's
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>

#include "SDL_gfxPrimitives.h"
#include "SDL_gfxPrimitives_font.h"

//Jensen's
#include "main.h"
#include "levels.h"
#include "functions.h"
#include "player.h"
#include "bonus.h"
#include "menu.h"
#include "enemy.h"
#include "particle.h"
#include "gamepad.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define Use_OPENGL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define CAPTION "MEGA MARIO"

#define SCREEN_BPP 32



#define ER cout<<"Nix da !!!"<<flush<<endl;




void loadGlobalSprites();
void unloadGlobalSprites();
void THEGAMEENDSHERE();
void instruct(char* path);
void drawGLscreen();
void InitOpenGL( void );
extern SDL_Surface *screen;
extern SDL_Surface *helpscreen;
extern SDL_Surface *cheatscreen;

extern SDL_Event event;

extern TTF_Font* font;

extern Uint8 *keys;
extern double frame_multi;
extern bool Real_Fullscreen;
extern char* cheatcode;

struct HUD_STRUCT
{
	int score;
	int lifes;
	int coins;
};

#define MAXSONGS 50
struct s_music_archive
{
	Mix_Music *song[MAXSONGS];
	char* name[MAXSONGS];
	int allocatecount;
	char* currentsong;
};
extern s_music_archive MusicArchive;


extern HUD_STRUCT HUD;

extern int WIDTH,GL_Width;
extern int HEIGHT,GL_Height;

#ifndef __unix__
extern bool NoFrameCheck;
#endif
extern bool gameStarted,gameStartedVirtual,pixelation,show_blood,cheater,cheats_enabled;
extern int nosound;


extern SDL_Color HUDColor;
extern SDL_Color HUDColorbg;

extern SDL_Rect tmp_rect;
extern int  cam_x,cam_y,bonus_activ;
//SDL_MapRGB(screen->format,255,0,0)
////////////////////////////////

#define PLAYSOUND1(nr) if(nosound)Mix_PlayChannel(1,SOUND[nr],0)
#define PLAYSOUND2(nr) if(nosound)Mix_PlayChannel(2,SOUND[nr],0)
#define PLAYSOUND0(nr) if(nosound)Mix_PlayChannel(0,SOUND[nr],0)
#define MAX_SOUNDS 20
extern int musicvolume,soundvolume;
extern Mix_Chunk *SOUND[MAX_SOUNDS];

#define S_GROW 0
#define S_JUMP 1
#define S_CRACK 2
#define S_COIN 3
#define S_KICK 4
#define S_STOMP 5
#define S_FIREBALL 6
#define S_STAGE_CLEAR 7
#define S_1UP 8
#define S_DIE 9
#define S_GAMEOVER 10
#define S_PAUSE 11
#define S_PIPE 12
#define S_FLAG 13
#define S_BOWSERFALL 14
#define S_SHRINK 15
#define S_OUTPUT 16
#define S_TOCK 17
#define S_STOMP2 18
#define S_CANNON 19
/////////////////////////////////

extern Uint32 magenta;

#define ENEMYSPEED -2
#define MAXENEMY 200
extern int GOOMBAcount;
extern cGoomba *GOOMBA[MAXENEMY];
extern int GOOMBABIGcount;
extern cGoombaBig *GOOMBABIG[5];
extern int TURTLEcount;
extern cTurtle *TURTLE[MAXENEMY];
extern int TURTLEFLYcount;
extern cTurtleFly *TURTLEFLY[MAXENEMY];
extern int SPINYcount;
extern cSpiny *SPINY[MAXENEMY];
extern int FIRECIRCLEcount;
extern cFireCircle *FIRECIRCLE[MAXENEMY];
extern int CANNONcount;
extern cCannon *CANNON[MAXENEMY];
extern int KILLERBLUMEcount;
extern cKillerBlume *KILLERBLUME[MAXENEMY];
extern int BRETTcount;
extern cBrett *BRETT[MAXENEMY];

extern cScene *LEVEL;
extern cPlayer *PLAYER;
extern cBonus* BONUS_DYNAMIC[5];
extern cCrack *CRACK;
extern cGlidder *GLIDDER;
extern cScore* SCORE;
extern cFire* FIRE;
extern cBowser* BOWSER;
extern cFireball *FIREBALL[2];
extern cBlood *BLOOD_GENERATOR;
extern cGamepad *Gamepad;

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

#ifndef DATADIR
#define DATADIR "data/"
#endif

#endif


