/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


#include "Global.h"


SDL_Surface *TURTLE_SURFACE[2][2];
SDL_Surface *TURTLEFLY_SURFACE[2][2];
SDL_Surface *TURTLE_SURFACE_SHELL[4];

void initTURTLE()
{
	TURTLEFLY_SURFACE[0][1] = LoadIMG(DATADIR "gfx/characters/turtle/flyl1.png");
	TURTLEFLY_SURFACE[0][0] = LoadIMG(DATADIR "gfx/characters/turtle/flyl2.png");
	TURTLEFLY_SURFACE[1][1] = LoadIMG(DATADIR "gfx/characters/turtle/flyr1.png");
	TURTLEFLY_SURFACE[1][0] = LoadIMG(DATADIR "gfx/characters/turtle/flyr2.png");

	TURTLE_SURFACE[0][0] = LoadIMG(DATADIR "gfx/characters/turtle/turtlel1.png");
	TURTLE_SURFACE[0][1] = LoadIMG(DATADIR "gfx/characters/turtle/turtlel2.png");
	TURTLE_SURFACE[1][0] = LoadIMG(DATADIR "gfx/characters/turtle/turtler1.png");
	TURTLE_SURFACE[1][1] = LoadIMG(DATADIR "gfx/characters/turtle/turtler2.png");

	TURTLE_SURFACE_SHELL[0] = LoadIMG(DATADIR "gfx/characters/turtle/shell1.png");
	TURTLE_SURFACE_SHELL[1] = LoadIMG(DATADIR "gfx/characters/turtle/shell2.png");
	TURTLE_SURFACE_SHELL[2] = LoadIMG(DATADIR "gfx/characters/turtle/shell3.png");
	TURTLE_SURFACE_SHELL[3] = LoadIMG(DATADIR "gfx/characters/turtle/shell4.png");
}

cTurtle::cTurtle(int x,int y) : cEnemy(x,y)
{

	W = TURTLE_SURFACE[0][0]->w-4;
	H = TURTLE_SURFACE[0][0]->h;
	down = 0;
	scoring = 200;
}

void cTurtle::add_update()
{
	if(dying)
		return;
	if(down)
	{
		if(!dir)
		{
			down--;
			if(down==1)		
			{
				H = TURTLE_SURFACE[0][0]->h;
				y -= TURTLE_SURFACE[0][0]->h - TURTLE_SURFACE_SHELL[0]->h;
				dir = ENEMYSPEED;
				down = 0;
			}
		}
		else
		{
			for(int i=0;i<GOOMBAcount;i++)
				if(!GOOMBA[i]->DEATHcount)
				{
					if(collision(&GOOMBA[i]->rect,&this->rect))
						GOOMBA[i]->die();
				}
			for(int i=0;i<TURTLEcount;i++)
				if(!TURTLE[i]->dying)
				{
					if(TURTLE[i]->rect.x != rect.x && collision(&TURTLE[i]->rect,&this->rect))
						TURTLE[i]->die();
				}
			for(int i=0;i<TURTLEFLYcount;i++)
				if(!TURTLEFLY[i]->dead&&TURTLEFLY[i]->rect.x != rect.x && collision(&TURTLEFLY[i]->rect,&this->rect))
					TURTLEFLY[i]->die();
				
			for(int i=0;i<SPINYcount;i++)
				if(!SPINY[i]->dying)
				{
					if(collision(&SPINY[i]->rect,&this->rect))
						SPINY[i]->die();
				}
			LEVEL->Bonus_Kick_Mode = 1;//Switch Bonus_Kick_Mode to TURLTE Mode
			if(right_touch||left_touch)
				LEVEL->BonusCheck(this->x+W+15,this->y+H/1.25,this->x-15,this->y+H/1.25);
			LEVEL->Bonus_Kick_Mode = 0;//Switch Bonus_Kick_Mode back to Mario Mode
		}
	}
	
	if((playerTouchLeft()))
	{
		if(down && !dir)
		{
			x-=25;
			y-=10;
			dir = -7;
			SCORE->init(x,y,400);
			PLAYSOUND1(S_KICK);
		}
		else
		{
			if(!PLAYER->stat)
				PLAYER->die();
			else
				PLAYER->changeStat(0);
		}
	}
	else if(playerTouchRight())
	{
		if(down&&!dir)
		{
			x+=25;
			y-=10;
			dir = 7;
			SCORE->init(x,y,400);
			PLAYSOUND1(S_KICK);
		}
		else
		{
			if(!PLAYER->stat)
				PLAYER->die();
			else
				PLAYER->changeStat(0);
		}
	}

}
void cTurtle::headbang()
{
	PLAYER->y_speed=-10;
	PLAYSOUND1(S_STOMP2);
	down = 500;
	SCORE->init(x,y,100);
	H = TURTLE_SURFACE_SHELL[0]->h-3;
	if(dir)
		dir = 0;
	else
		dir = 7;
}
void cTurtle::draw()
{
	int d = (dir<0) ? 0 : 1;
	if(down)
		SDL_UpperBlit(TURTLE_SURFACE_SHELL[walkcount],0,screen,&rect);
	else 
		SDL_UpperBlit(TURTLE_SURFACE[d][walkcount],0,screen,&rect);
}




cTurtleFly::cTurtleFly(int x,int y) : cEnemy(x,y)
{
	W = TURTLEFLY_SURFACE[0][0]->w-4;
	H = TURTLEFLY_SURFACE[0][0]->h;
	scoring = 200;
	count = 50;
}

void cTurtleFly::add_update()
{
	if(dying)
		return;

	if(LEVEL->posValid(x+3,y) + LEVEL->posValid(x+W-3,y))
	{
		count = 30;
		down_touch = 0;
	}
		
	if(count<15)
	{
		y -= 2;
		y_speed = -4;
		count++;
	}
	
	if(down_touch)
		count = 0;
		
	if((playerTouchLeft()+playerTouchRight()))
		if(!PLAYER->stat)
			PLAYER->die();
		else
			PLAYER->changeStat(0);
}
void cTurtleFly::headbang()
{
	PLAYER->y_speed=-10;
	PLAYSOUND1(S_STOMP2);
	SCORE->init(x,y,200);
	dead  = 1;
	TURTLE[TURTLEcount] = new cTurtle(x,y);
	TURTLE[TURTLEcount]->dir = dir;
	TURTLEcount++;
}
void cTurtleFly::draw()
{
	int d = (dir<0) ? 0 : 1;
	SDL_UpperBlit(TURTLEFLY_SURFACE[d][walkcount],0,screen,&rect);
}

