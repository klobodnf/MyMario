/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


#include "Global.h"


SDL_Surface *KBLUM_SURFACE[2];

void initKILLERBLUUME()
{	
	KBLUM_SURFACE[0] = LoadIMG(DATADIR "gfx/characters/pirnaha1.png");
	KBLUM_SURFACE[1] = LoadIMG(DATADIR "gfx/characters/pirnaha2.png");
}
cKillerBlume::cKillerBlume(int _x,int _y)
{
	x=_x;
	y=_y;
	count = 1;
	dead = 0;
	timer = 0;
}
void cKillerBlume::update()
{
	if(dead)return;

	rect.x = x - cam_x;
	rect.y = y - cam_y;
	rect.w = 40;
	rect.h = 40;
	if(!collision(&rect,&LEVEL->SCREEN_RECT))
		return;
	if(count>0)
		
	{
		timer+=0.05f;
		count++;
		y-= (count>=46) ? ((count>=200) ? -1 : 0) : 1;
		if(count >= 243)
			count = -150;
		SDL_UpperBlit(KBLUM_SURFACE[(int)timer%2],0,screen,&rect);
		if(collision(&rect,&PLAYER->rect))
			if(!PLAYER->stat)
				PLAYER->die();
			else
				PLAYER->changeStat(0);
	}
	else 
	{
		if(count<0)
			count++;
		else
		{
			rect.x -=40;
			rect.y -= 80;
			rect.w += 80;
			rect.h += 200;
			if(!collision(&rect,&PLAYER->rect))
			{
				count = 1;
				timer = 0;
			}
		}
		rect.x = 900;
	}
		
	

}
