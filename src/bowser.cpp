/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/

#include "Global.h"


cBowser::cBowser(int _x,int _y,int _hammers)
{
	fire_flying = 0;
	health = 10;
	static_x = _x;
	static_y = _y;
	int i=0;
	for(i=static_x;i<static_x+15;i++)
	{
		LEVEL->TILE[i][static_y+3] = 0;
		LEVEL->BONUS[i][static_y+3] = 1;
		LEVEL->BONUS[i][static_y-2] = 0;
		LEVEL->TILE[i][static_y-2] = 0;
	}
	 i--;
	LEVEL->BONUS[i][_y+1] = 17;
	LEVEL->BONUS[i][_y+2] = 8;

	LEVEL->BONUS[i+1][_y+3] = 1;
	LEVEL->BONUS[i+1][_y+2] = 20;
	LEVEL->BONUS[i+1][_y+1] = 20;
	LEVEL->BONUS[i+1][_y] = 20;
	LEVEL->BONUS[i+1][_y-1] = 20;

	BOWSER_SURFACE[0] = LoadIMG(DATADIR "gfx/characters/bowser1.png");
	BOWSER_SURFACE[1] = LoadIMG(DATADIR "gfx/characters/bowser2.png");
	BOWSER_SURFACE[2] = LoadIMG(DATADIR "gfx/bonus/fire1.png");
	BOWSER_SURFACE[3] = LoadIMG(DATADIR "gfx/bonus/fire2.png");

	HAMMER_SURFACE[0] = LoadIMG(DATADIR "gfx/characters/hammer1.png");
	HAMMER_SURFACE[1] = LoadIMG(DATADIR "gfx/characters/hammer2.png");
	HAMMER_SURFACE[2] = LoadIMG(DATADIR "gfx/characters/hammer3.png");
	HAMMER_SURFACE[3] = LoadIMG(DATADIR "gfx/characters/hammer4.png");

	W = BOWSER_SURFACE[0]->w;
	H = BOWSER_SURFACE[0]->h;
	x = (_x+5)*40;
	y = _y*40;
	hammers = _hammers;
	godown = 0;
	timer = 0;
	getX = x;
	count = 0;count2 = 0;
	arrived = 0;
	HAMMERcount = 0;

	rect.x=0;
	rect.w=0;
	rect.h=0;
	rect.y=0;

	for(i=0;i<hammers;i++)
	{		
		HAMMERS[i].y = 1200000;		
	}
}

void cBowser::update()
{
	if(fire_flying)
	{
		timer+=0.1f;
		fx-=2;
		rect.x = fx - cam_x;
		rect.y = fy - cam_y;
		SDL_UpperBlit(BOWSER_SURFACE[(int)timer%2+2],0,screen,&rect);
		if(collision(&PLAYER->rect,&rect))
			if(!PLAYER->stat)
				PLAYER->die();
			else
				PLAYER->changeStat(0);
	}
	if(PLAYER->x > x - 3000)
	{
		fire_flying = 1;

		if(	PLAYER->x < x - 700 && !collision(&LEVEL->SCREEN_RECT,&rect))
		{
			fx = cam_x+639;
			fy = PLAYER->y + (rand()%100)-40;
			if(!arrived)
			{
				arrived = 1;
				switch(rand()%3)
				{
					case 1:
						playMusic("bowser1.mid");
					break;
					case 2:
						playMusic("bowser2.mid");
					break;
					case 0:
						playMusic("bowser3.mid");
					break;
				} 
			}
		}
	}else return;
	
	down_touch  = LEVEL->posValid(x+3,y+3+H) + LEVEL->posValid(x+W-3,y+3+H);
	
	if(!down_touch)
		y+=2;

	if(getX==x)
		getX = static_x*40 + rand()%500;

	if(getX<x)
		x--;
	else
		x++;
	
	if(hammers)
	{
		if(HAMMERS[HAMMERcount].y-cam_y>WIDTH)
		{
			HAMMERS[HAMMERcount].x=x;
			HAMMERS[HAMMERcount].y=y;
			
			HAMMERS[HAMMERcount].x_speed=-1.5;
			HAMMERS[HAMMERcount].y_speed=-5;
		}
		
		HAMMERcount+=(rand()%(40-hammers)==0)?1:0;
		if(HAMMERcount==hammers)
			HAMMERcount = 0;
	int i=0;
		for(i=0;i<hammers;i++)
		{
			if(HAMMERS[i].y_speed<4)
				HAMMERS[i].y_speed+=0.1f;
			
			HAMMERS[i].x += (float)HAMMERS[i].x_speed;
			HAMMERS[i].y += (float)HAMMERS[i].y_speed;
			tmp_rect.x = HAMMERS[i].x -cam_x;
			tmp_rect.y = HAMMERS[i].y -cam_y;
			SDL_UpperBlit(HAMMER_SURFACE[(int)timer%4],0,screen,&tmp_rect);
			if(collision(&tmp_rect,&PLAYER->rect))
			{
				if(!PLAYER->stat)
					PLAYER->die();
				else
					PLAYER->changeStat(0);
			}
		}
	}

	count2++;
	if(!count)
		if(count2>150)
		{
			y-=5;
			if(count2==180)
				count2 = 0 - rand()%50;
		}

	rect.x = (int)x - cam_x;
	rect.y = (int)y - cam_y;
	SDL_UpperBlit(BOWSER_SURFACE[(int)timer%2],0,screen,&rect);
	
	if(collision(&PLAYER->rect,&rect))
		if(!PLAYER->stat)
			PLAYER->die();
		else
			PLAYER->changeStat(0);
	/// HIT THE AXE  JACK :_
		if(	PLAYER->x/40+2 >= static_x+15 &&   PLAYER->y/40 + 1 >= static_y &&  PLAYER->y/40 + 1 <= static_y+1)
		{
			if(count==0)
			{
				LEVEL->BONUS[static_x+14][static_y+1] = 0;
				LEVEL->BONUS[static_x+14][static_y+2] = 1;
				PLAYSOUND0(S_BOWSERFALL);
				playMusic("reggae.mid");
				godown = 1;
			}
		}
		if(godown)
		{
			count++;
			for(int i=static_x;i<static_x+15;i++)
				{
					LEVEL->TILE[i][static_y+3] = rand()%(LEVEL->TEXTUREcount-1);
					LEVEL->BONUS[i][static_y+3] = 0;					
				}
			if(count==160)
			{
				for(int i=static_x;i<static_x+15;i++)
				{
					LEVEL->TILE[i][static_y+3] = 0;
					LEVEL->BONUS[i][static_y+3] = 0;
					if(i%2)
						CRACK->init(i,static_y+3,0);
				}
				LEVEL->BONUS[static_x+15][static_y+2] = 0;
				LEVEL->BONUS[static_x+15][static_y+1] = 0;
				LEVEL->BONUS[static_x+15][static_y] = 0;
				LEVEL->BONUS[static_x+15][static_y-1] = 0;
				godown = 0;
			}
		}

}

void cBowser::hit()
{
	health--;
	BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
	BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
	BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
	BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
	if(health<=0)
	{
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		BLOOD_GENERATOR->newBlood(x+rect.w/2,y+rect.h/2);
		x=0;
		y=0;
	}
}
