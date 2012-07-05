/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


#include "Global.h"

cBonus::cBonus()
{

	SURFACE[0]= LoadIMG(DATADIR "gfx/bonus/pilz.png");
	SURFACE[1]= LoadIMG(DATADIR "gfx/bonus/blume.png");
	SURFACE[2]= LoadIMG(DATADIR "gfx/bonus/stern.png");
	SURFACE[3]= LoadIMG(DATADIR "gfx/bonus/life.png");

	online = 0;
}
void cBonus::init(int _x,int _y,int _type)
{
	x = _x*40;
	y = _y*40-5;
	
	type = _type;

	x_speed = (rand()%2) ? -1.5f:1.5f;
	
	if(type==1)
		x_speed = 0;
	online =1;
	counter = 0;
	y_speed = 0;
	PLAYSOUND1(S_OUTPUT);
}

void cBonus::update()
{
	if(!online)
		return;
	
	rect.x = (int)x - cam_x;
	rect.y = (int)y - cam_y;
	
	if(counter<35)
	{
		counter++;
		y--;
	}
	else
	{
		down_touch = LEVEL->posValid(x+10,y+40) + LEVEL->posValid(x+30,y+40);
		left_touch = LEVEL->posValid(x,y+35);
		right_touch = LEVEL->posValid(x+40,y+35);
		up_touch = LEVEL->posValid(x+20,y-5);

		if((right_touch+left_touch))
			x_speed *= -1;

		if(y_speed>=0)
		{
			if(!down_touch)
			{
				y += y_speed;
				if(y_speed<2)
					y_speed+=0.05;
			}
			else{ y_speed = 0;}
		}else
			y_speed+=0.1;
		
		x += x_speed;
		if(y_speed<0&&up_touch);
		else y += y_speed;

		if(collision(&rect,&PLAYER->rect))
		{
			if(type==0)
				PLAYER->changeStat(1);
			else if(type==1)
			{
				if(PLAYER->stat==2)
				{
					SCORE->init(x,y,1000);
					PLAYSOUND1(S_KICK);
				}
				else
					PLAYER->changeStat(2);
			}
			else if(type==2)
			{
				PLAYER->STARPOWER += 500;
				if(nosound==2)
					playMusic("star.mid");
			}
			else if(type==3)
			{
				PLAYSOUND1(S_1UP);
				HUD.lifes++;
				SCORE->init(x,y,0);
			}			
			online = 0;
			return;
		}
	}

	SDL_UpperBlit(SURFACE[type],0,screen,&rect);
	
	
}



cCrack::cCrack()
{
	SURFACE[0] = LoadIMG(DATADIR "gfx/bonus/cracked.png");
	SURFACE[1] = LoadIMG(DATADIR "gfx/bonus/cracked2.png");
	CRACKcount = 0;
	for(int i=0;i<50;i++)
	{
		CRACK[i].x =0;
		CRACK[i].y =0;
		CRACK[i].x_speed =0;
		CRACK[i].y_speed =0;
	}
}
void cCrack::init(int _x,int _y,int grey)
{
	if(CRACKcount>42)
		CRACKcount = 0;
	HUD.score += 50;
	//CRACKcount++;
	int i;
	for(i=CRACKcount;i<CRACKcount+10;i++)
	{	
		CRACK[i].x = _x*40;
		CRACK[i].y = _y*40;
		CRACK[i].x_speed = (rand()%2) ? rand()%3+2 : -(rand()%3+2);
		CRACK[i].y_speed = -rand()%-5-4;
		CRACK[i].grey = grey;
	}
	CRACKcount = i;
	PLAYSOUND2(S_CRACK);
}
void cCrack::update()
{
	for(int i=0;i<50;i++)
	{
		if(CRACK[i].y>0&&CRACK[i].y<4000)
		{
			CRACK[i].x += CRACK[i].x_speed;
			CRACK[i].y += CRACK[i].y_speed;
			//CRACK[i].x_speed = (rand()%2) ? rand()%4+2 : rand()%-4+2;
			CRACK[i].y_speed += 0.2;
			rect.x = CRACK[i].x-cam_x;
			rect.y = CRACK[i].y-cam_y;
			SDL_UpperBlit(SURFACE[CRACK[i].grey],0,screen,&rect);
		}
	}
}


int kick_count = 200;
int kick_realy,kick_realx;
int kick_x,kick_y;
int kick_texture;
void kickUp(int BOXx,int BOXy)
{
	if(kick_count<16)
		return;
	kick_count = 0;;
	kick_realy = 0;
	kick_realx = 0;
	kick_x = 0;
	kick_y = 0;
	kick_texture = 0;

	kick_x = BOXx;
	kick_y = BOXy;
	kick_texture = LEVEL->BONUS[BOXx][BOXy];
	LEVEL->BONUS[BOXx][BOXy] = 8;
	kick_realx = kick_x * 40;
	kick_realy = kick_y * 40;

	for(int i=0;i<5;i++)
		if(((BONUS_DYNAMIC[i]->rect.x+cam_x)/40==kick_x&&(BONUS_DYNAMIC[i]->rect.y+5+cam_y)/40==kick_y-1) || ((BONUS_DYNAMIC[i]->rect.x+40+cam_x)/40==kick_x&&(BONUS_DYNAMIC[i]->rect.y+cam_y)/40==kick_y-1))
		{
			BONUS_DYNAMIC[i]->y_speed = -3;
			BONUS_DYNAMIC[i]->x_speed *= -1;
		}
	for(int i=0;i<GOOMBAcount;i++)
	{
		if(((GOOMBA[i]->x)/40==kick_x&&(GOOMBA[i]->y+10)/40==kick_y-1) || ((GOOMBA[i]->x+40)/40==kick_x&&(GOOMBA[i]->y+10)/40==kick_y-1))
			GOOMBA[i]->die();
	}
	for(int i=0;i<TURTLEcount;i++)
	{
		if(((TURTLE[i]->x)/40==kick_x&&(TURTLE[i]->y)/40==kick_y-2) || ((TURTLE[i]->x+40)/40==kick_x&&(TURTLE[i]->y)/40==kick_y-2))
			TURTLE[i]->die();
	}
	for(int i=0;i<SPINYcount;i++)
	{
		if(((SPINY[i]->x)/40==kick_x&&(SPINY[i]->y+10)/40==kick_y-1) || ((SPINY[i]->x+40)/40==kick_x&&(SPINY[i]->y+10)/40==kick_y-1))
			SPINY[i]->die();
	}
	if(!kick_texture)
		kick_count = 500;
}

#define KICKSPEED 16
void updateKickUp()
{
	if(kick_count<KICKSPEED)
	{
		kick_count++;
		if(kick_count<KICKSPEED/2)
			kick_realy-=2;
		else
			kick_realy+=2;

		tmp_rect.x = kick_realx - cam_x;
		tmp_rect.y = kick_realy - cam_y;
		SDL_UpperBlit(LEVEL->Bonus_Textures[kick_texture],0,screen,&tmp_rect);
		if(kick_count == KICKSPEED)
			LEVEL->BONUS[kick_x][kick_y] = kick_texture;
	}
}



cGlidder::cGlidder()
{
	SURFACE[0] = LoadIMG(DATADIR "gfx/effects/glid_0.png");
	SURFACE[1] = LoadIMG(DATADIR "gfx/effects/glid_1.png");
	SURFACE[2] = LoadIMG(DATADIR "gfx/effects/glid_2.png");
	SURFACE[3] = LoadIMG(DATADIR "gfx/effects/glid_3.png");
	GLIDcount = 0;
	for(int i=0;i<50;i++)
	{
		GLID[i].x =0;
		GLID[i].y =0;
		GLID[i].x_speed =0;
		GLID[i].y_speed =0;
		GLID[i].count = 0;
	}
}
void cGlidder::init(int _x,int _y,int _speed_x,int _speed_y)
{
	if(GLIDcount>40)
		GLIDcount = 0;
	int n;
	for( n=GLIDcount;n<GLIDcount+10;n++)
	{	
		GLID[n].x = _x*40 + rand()%40;
		GLID[n].y = _y*40 + rand()%40;
		GLID[n].x_speed = _speed_x;
		GLID[n].y_speed = _speed_y;
		GLID[n].count = ((float)(rand()%25))/10.0f;
	}
	GLIDcount = n;
}
void cGlidder::update()
{
	for(int i=0;i<50;i++)
	{
		if(GLID[i].y>0&&GLID[i].count<6)
		{
			GLID[i].x += GLID[i].x_speed += (rand()%2) ? rand()%3-1 : 0;
			GLID[i].y += GLID[i].y_speed += (rand()%2) ? rand()%3-1 : 0;
			rect.x = GLID[i].x-cam_x;
			rect.y = GLID[i].y-cam_y;
			SDL_UpperBlit(SURFACE[(int)(GLID[i].count)%4],0,screen,&rect);
			GLID[i].count+=0.1;
		}
	}
}

cScore::cScore()
{
	for(int i=0;i<20;i++)
	{
		SCORE_S[i].text = new char[2];
		strcpy(SCORE_S[i].text," ");
	}
	SCOREcount = 0;
}

void cScore::init(int _x,int _y,int _score)
{
	HUD.score += _score;

	if(SCOREcount>=20)
		SCOREcount = 0;
	
	SCORE_S[SCOREcount].x = _x;
	SCORE_S[SCOREcount].y = _y+20;
	SCORE_S[SCOREcount].count = 0;

	delete[](SCORE_S[SCOREcount].text);
	SCORE_S[SCOREcount].text = new char[6];
	if(_score==0)
		sprintf(SCORE_S[SCOREcount].text,"1-UP");
	else
		sprintf(SCORE_S[SCOREcount].text,"%d",_score);
	SCOREcount++;
}
void cScore::update()
{
	for(int i=0;i<20;i++)
	{
		if(SCORE_S[i].x>0&&SCORE_S[i].count<50)
		{
			SCORE_S[i].y--;
//			StringColor(screen,SCORE_S[i].x-cam_x,SCORE_S[i].y-cam_y,SCORE_S[i].text,1440);
			StringColor(screen,SCORE_S[i].x-cam_x-1,SCORE_S[i].y-cam_y-1,SCORE_S[i].text,-10);
			SCORE_S[i].count++;
		}
	}
}
