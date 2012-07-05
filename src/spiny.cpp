/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/

#include "Global.h"


SDL_Surface *SPINY_SURFACE[4];

void initSPINY()
{	
	SPINY_SURFACE[0] = LoadIMG(DATADIR "gfx/characters/spiny1.png");
	SPINY_SURFACE[1] = LoadIMG(DATADIR "gfx/characters/spiny2.png");
	SPINY_SURFACE[2] = LoadIMG(DATADIR "gfx/characters/spiny3.png");
	SPINY_SURFACE[3] = LoadIMG(DATADIR "gfx/characters/spiny4.png");
}
cSpiny::cSpiny(int x,int y) : cEnemy(x,y)
{	
	W = SPINY_SURFACE[0]->w;
	H = SPINY_SURFACE[0]->h;
	scoring = 250;
}

void cSpiny::add_update()
{
	if(dying)
		return;
	if((playerTouchLeft()+playerTouchRight()))
		if(!PLAYER->stat)
			PLAYER->die();
		else
			PLAYER->changeStat(0);
	
}
void cSpiny::headbang()
{
	if(!PLAYER->stat)
		PLAYER->die();
	else
		PLAYER->changeStat(0);		
}
void cSpiny::draw()
{ 
	if(dir>0)walkcount += 2;
	SDL_UpperBlit(SPINY_SURFACE[walkcount],0,screen,&rect);
	if(dir>0)walkcount -= 2;
}
