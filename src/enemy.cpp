/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


#include "Global.h"


cEnemy::cEnemy(int _x,int _y)
{
	dir = 0;
	
	x= _x;
	y= _y;
	y_speed = 0;
		
	walkcount = 0;
	walkcount2 = 0;
	walkadd = 0;
	speed = ENEMYSPEED;
	dir = speed;
	dead = 0;
	DEATHcount = 0;
	dying = 0;
	
}


void cEnemy::update()
{
	if(dead)
		return;

	if(DEATHcount>0)
	{
		DEATHcount++;
		if(DEATHcount>=2000)
			dead=1;
	}
	else
	{
		down_touch  = (bool)LEVEL->posValid(x+3,y+3+H) + LEVEL->posValid(x+W-3,y+3+H);
		left_touch  = LEVEL->posValid(x-1,y+H/1.25) + LEVEL->posValid(x-1,y+H/4);
		right_touch = LEVEL->posValid(x+W+1,y+H/1.25) + LEVEL->posValid(x+W+1,y+H/4);

		if(dying)
		{
			down_touch = 0;
			left_touch = 0;
			right_touch = 0;
			if(rect.y>=HEIGHT)
				dead = 1;
		}
		else if(PLAYER->x+40 >= x && PLAYER->x <= x+W)
		{
			if(PLAYER->y+PLAYER->H >= y && PLAYER->y+PLAYER->H <= y+H/2)
				headbang();
		}
		if((left_touch+right_touch)) 
			dir*=-1;

		if(!down_touch)
		{
			y += y_speed;
			if(y_speed<8)
				y_speed+=0.4f;					
		}
		else
			y_speed = 0;
		
		x += dir;

		//DEFINED IN EVERY SINGLE ENEMY:
		add_update();
		for(int i=0;i<TURTLEcount;i++)
			if(!TURTLE[i]->dead)
			{
				if(TURTLE[i]->down && TURTLE[i]->rect.x != rect.x && collision(&TURTLE[i]->rect,&this->rect))
				{
					dir*=-1;
					x += dir + dir;
				}
			}			

		MakeMyWalk();
	}
		rect.x = (int)x - cam_x;
		rect.y = (int)y - cam_y;


	//DEFINED IN EVERY SINGLE ENEMY:
	draw();
}

void cEnemy::die()
{
	dying = 1;
	y -= 40;
	SCORE->init(x,y,scoring);
	PLAYSOUND2(S_KICK);
	BLOOD_GENERATOR->newBlood(x,y);
}

// NOT USED NOW ://///////////////////////////
 void cEnemy::add_update(){}
 void cEnemy::headbang(){}
 void cEnemy::draw(){}
//////////////////////////////////////////////

int cEnemy::playerTouchRight()
{
	if(PLAYER->x+38 >= x && PLAYER->x+38 <= x+(W/2))
	{
		if(PLAYER->y+PLAYER->H >= y+(H/2) && PLAYER->y<=y+H)
			return true;
	}
	return 0;
}
int cEnemy::playerTouchLeft()
{
	if(PLAYER->x >=x+W/2 && PLAYER->x <= x+(W))
	{
		if(PLAYER->y+PLAYER->H >= y+(H/2) && PLAYER->y<=y+H)
			return true;
	}
	return 0;
}

void cEnemy::MakeMyWalk()
{
		walkcount2++;
		if(walkcount2 >= 12)
		{
			walkcount = (walkcount) ? 0 : 1;
			walkcount2=0;
		}
}
