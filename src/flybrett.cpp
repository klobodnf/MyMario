/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/

#include "Global.h"


SDL_Surface *BRETT_SURFACE;
void initBRETT()
{
	BRETT_SURFACE = LoadIMG(DATADIR "gfx/characters/brett.png");
}

#define BRETTSPEED 3
cBrett::cBrett(int _x,int _y)
{
	x = _x;
	y = _y;
	count = 0;
	dir = 1;
}
void cBrett::update()
{
	if(count >= 100&&dir)
		// dir为0就是向左走
		dir = 0;
	else if(count <= 0&&!dir)
		// dir为1就是向右走
		dir = 1;

	if(dir)
	{
		y+=BRETTSPEED;
		count++;
	}
	else
	{
 		y-=BRETTSPEED;
		count--;
	}
	
	
	
	tmp_rect.x = x - cam_x;
	tmp_rect.y = y - cam_y;
	SDL_UpperBlit(BRETT_SURFACE,0,screen,&tmp_rect);

	if(PLAYER->JUMPcount==400)
	if(pointCollision(&tmp_rect,PLAYER->x+20-cam_x,PLAYER->y+PLAYER->H-cam_y))
	{
 		PLAYER->y = y - PLAYER->H;
		PLAYER->touch_now = 1;
		PLAYER->y_speed = -1;
		if(dir)
			PLAYER->y+=10;
	}
	
}
