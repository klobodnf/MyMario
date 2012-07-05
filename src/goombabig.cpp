/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/	


#include "Global.h"

SDL_Surface *GoombaBig_SURFACE[3];
void initGOOMBAbig()
{
	GoombaBig_SURFACE[0] = LoadIMG(DATADIR "gfx/characters/goombabig1.png");
	GoombaBig_SURFACE[1] = LoadIMG(DATADIR "gfx/characters/goombabig2.png");
	GoombaBig_SURFACE[2] = LoadIMG(DATADIR "gfx/characters/goombabig3.png");
}
cGoombaBig::cGoombaBig(int x,int y) : cEnemy(x,y)
{
	W = GoombaBig_SURFACE[0]->w;
	H = GoombaBig_SURFACE[0]->h;
	scoring = 100;
}

void cGoombaBig::add_update()
{
	if(dying)
		return;
	if((playerTouchLeft()+playerTouchRight()))
		if(!PLAYER->stat)
			PLAYER->die();
		else
			PLAYER->changeStat(0);
	
}
void cGoombaBig::headbang()
{
	DEATHcount ++;
	PLAYER->y_speed=-10;
	PLAYSOUND1(S_STOMP); 
	SCORE->init(x,y,scoring);
	BLOOD_GENERATOR->newBlood(x,y);
	BLOOD_GENERATOR->newBlood(x,y);
	BLOOD_GENERATOR->newBlood(x,y);
	BLOOD_GENERATOR->newBlood(x,y);
}
void cGoombaBig::draw()
{
	if(DEATHcount)
		SDL_UpperBlit(GoombaBig_SURFACE[2],0,screen,&rect);
	else 
		SDL_UpperBlit(GoombaBig_SURFACE[walkcount],0,screen,&rect);
}
