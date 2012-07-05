/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/

#include "Global.h"
#include <math.h>

cFireCircle::cFireCircle(int _x,int _y,int _size)
{
	size = _size;
	x=_x+10;
	y=_y+10;
	angle = rand()%360;
	counta2 = 1;
}
void cFireCircle::update()
{
	tmp_rect.x = cam_x-size*20;
	tmp_rect.y = cam_y-size*20;
	tmp_rect.h = cam_y+HEIGHT+size*40;
	tmp_rect.w = cam_x+WIDTH+size*40;
	
	if(!pointCollision(&tmp_rect,x,y))
		return;
	
	dir_x = (double)sin(angle *0.017453292519943295)*20.0f;
	dir_y = (double)cos(angle *0.017453292519943295)*20.0f;
	
	for(int i=0;i<size;i++)
	{
		rect.x = x-cam_x + (dir_x*i);
		rect.y = y-cam_y + (dir_y*i);
		SDL_UpperBlit(FIREBALL[0]->FIREBALLS[(int)counta2],0,screen,&rect);
		if(collision(&PLAYER->rect,&rect))
			if(!PLAYER->stat)
				PLAYER->die();
			else
				PLAYER->changeStat(0);
	}
	angle+=2;
	if(angle>=360)
		angle = 0;

	counta2+=0.1f;
	if(counta2 >= 3)
		counta2 = 0;

}

#define BULLETSPEED 3
SDL_Surface *CANNON_SURFACE[3];
void initCANNON()
{
	CANNON_SURFACE[0] = LoadIMG(DATADIR "gfx/characters/cannon.png");
	CANNON_SURFACE[1] = LoadIMG(DATADIR "gfx/characters/cannon_bulletr.png");
	CANNON_SURFACE[2] = LoadIMG(DATADIR "gfx/characters/cannon_bulletl.png");
}

cCannon::cCannon(int _x,int _y)
{
	x=_x;
	y=_y;
	count = 400;
	die = 0;
	
}
void cCannon::update()
{
	
	tmp_rect.x = x - cam_x;
	tmp_rect.y = y - cam_y;
	tmp_rect.w = 40;
	tmp_rect.h = 40;
	
	if(!collision(&tmp_rect,&LEVEL->SCREEN_RECT))
		updating=0;
	else
		updating =1;

	SDL_UpperBlit(CANNON_SURFACE[0],0,screen,&tmp_rect);
	
	
	count++;
	tmp_rect.x = bullet_x-cam_x;
	tmp_rect.y = bullet_y-cam_y;
	if(updating&&count >= 400&&!collision(&tmp_rect,&LEVEL->SCREEN_RECT))
	{
		bullet_x = x;
		bullet_y = y+9; 
		PLAYSOUND1(S_CANNON);
		dir = (PLAYER->x < x) ? -BULLETSPEED : BULLETSPEED;
		bullet_x += (dir>0)? 30 : -30;
		count = rand()%400;
	}
	bullet_x += dir;
	
	SDL_BlitSurface(CANNON_SURFACE[(dir<0)?2:1],0,screen,&tmp_rect);

	if(die)
	{
		die++;
		bullet_y+=4;
		if(bullet_y-cam_y>=HEIGHT)
		{
			die = 0;
			bullet_x+=WIDTH;
			count = 250;
		}
		
	}else 
	{
		if(PLAYER->x+40 >= bullet_x && PLAYER->x <= bullet_x+40)
			{
				if(PLAYER->y+PLAYER->H >= bullet_y && PLAYER->y+PLAYER->H <= bullet_y+25/2)
				{
					die = 1;
					dir/=3;
					PLAYER->y_speed=-10;
					PLAYSOUND1(S_STOMP); 
					SCORE->init(bullet_x,bullet_y,150);
					return;
				}
			}
		
		if(collision(&PLAYER->rect,&tmp_rect))
			if(!PLAYER->stat)
				PLAYER->die();
			else
				PLAYER->changeStat(0);
			
	}
	
	

}
