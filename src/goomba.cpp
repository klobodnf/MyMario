/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


#include "Global.h"

SDL_Surface *GOOMBA_SURFACE[3];
void initGOOMBA()
{
	GOOMBA_SURFACE[0] = LoadIMG(DATADIR "gfx/characters/goomba1.png");
	GOOMBA_SURFACE[1] = LoadIMG(DATADIR "gfx/characters/goomba2.png");
	GOOMBA_SURFACE[2] = LoadIMG(DATADIR "gfx/characters/goomba3.png");
}
cGoomba::cGoomba(int x,int y) : cEnemy(x,y)
{
	W = GOOMBA_SURFACE[0]->w;
	H = GOOMBA_SURFACE[0]->h;
	scoring = 100;
}

void cGoomba::add_update()
{
	if(dying)
		return;
	if((playerTouchLeft()+playerTouchRight()))
		if(!PLAYER->stat)
			PLAYER->die();
		else
			PLAYER->changeStat(0);
	
}
void cGoomba::headbang()
{
	DEATHcount ++;
	PLAYER->y_speed=-10;
	PLAYSOUND1(S_STOMP); 
	SCORE->init(x,y,scoring);
	BLOOD_GENERATOR->newBlood(x,y);
}
void cGoomba::draw()
{
	if(DEATHcount)
		SDL_UpperBlit(GOOMBA_SURFACE[2],0,screen,&rect);
	else 
		SDL_UpperBlit(GOOMBA_SURFACE[walkcount],0,screen,&rect);
}
