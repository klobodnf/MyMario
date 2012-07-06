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

// 蘑菇头被MARIO残忍的践踏了、囧
void cGoomba::headbang()
{
	DEATHcount ++;
	// 踩中蘑菇头后MARIO会向上弹起
	PLAYER->y_speed=-10;
	PLAYSOUND1(S_STOMP); 
	// 然后加分
	SCORE->init(x,y,scoring);
	// 然后蘑菇头会被踩出血来、囧
	BLOOD_GENERATOR->newBlood(x,y);
}
void cGoomba::draw()
{
	if(DEATHcount)
		// 如果蘑菇头被MARIO践踏后、就会画一幅被踩扁的图画
		SDL_UpperBlit(GOOMBA_SURFACE[2],0,screen,&rect);
	else 
		// 否则、播放正常走路的动作
		SDL_UpperBlit(GOOMBA_SURFACE[walkcount],0,screen,&rect);
}
