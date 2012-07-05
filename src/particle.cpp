/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/




#include "Global.h"

cFire::cFire()
{
	super_x=0;
	super_y=0;

	SURFACE = SDL_CreateRGBSurface(SDL_HWSURFACE,MAX_FIRE/20,65, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);


	for(int i=0;i<MAX_FIRE;i++)
	{
		super_x++;
		if(super_x==MAX_FIRE/20)
		{
			super_x=0;
			super_y+=2;
		}		
		fireparts[i].y = super_y;
		fireparts[i].x = super_x;
		fireparts[i].color = SDL_MapRGB(SURFACE->format,255,rand()%255, 3);
	}	
	FIREcount = 0;
	
	
	
	
	SDL_SetColorKey(SURFACE, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
	RECT.w = 40;
	RECT.h = 50;


	if(!Real_Fullscreen)
		FIRESURF = LoadIMG(DATADIR "gfx/effects/fire.png");
	else
	{
		FIRESURF = 0;
		for(int w=0;w<200;w++)
			update();
	}
	
}
void cFire::reset()
{
	FIREcount = 0;
}
void cFire::newFire(int _x,int _y)
{	
	x[FIREcount] = _x;
	y[FIREcount] = _y+15;
	FIREcount++;
}
void cFire::update()
{

	if(FIREcount==0)
		return;

	int i=0;

	if(!Real_Fullscreen)
	{
		SURFACE = FIRESURF;
	}else 
	{
		
		SDL_LockSurface(SURFACE);
		for(int a=0;a<MAX_FIRE;a++)
		{
			if(rand()%3)
			{
				fireparts[a].y--;
				if(fireparts[a].y < 50 - rand()%100)
				{
					fireparts[a].y = 60 + rand()%5;
				}
				if(rand()%50 == 1)
					FastPixelColorNolock(SURFACE,rand()%(MAX_FIRE/20),rand()%35,0);
			}
			FastPixelColorNolock(SURFACE,fireparts[a].x,fireparts[a].y,fireparts[a].color);				
		}
		SDL_UnlockSurface(SURFACE);
	}

	for(i=0;i<FIREcount;i++)
	{
		RECT.x = x[i]-cam_x;
		RECT.y = y[i]-cam_y;
		RECT.w=70;
		RECT.h = 70;
		
		if(collision(&RECT,&LEVEL->SCREEN_RECT))
		{	
			
			SDL_UpperBlit(SURFACE,0,screen,&RECT);
			if(collision(&PLAYER->rect,&RECT))
				if(!PLAYER->stat)
					PLAYER->die();
				else
					PLAYER->changeStat(0);
		}
	}
	
}







cBlood::cBlood()
{
	reset();	
}
void cBlood::reset()
{
	for(int i=0;i<MAX_BLOOD;i++)
	{
		for(int n=0;n<BLOOD_LEN;n++)
		{
			bloodparts[i].y[n] = 0;
			bloodparts[i].x[n] = 0;
		}
		bloodparts[i].dir_y = 0;
		bloodparts[i].dir_x = 0;
		bloodparts[i].index = 1;
		bloodparts[i].down = 1;
	}	
	BLOODcount = 0;
	
}
void cBlood::newBlood(int _x,int _y)
{

	if(BLOODcount>MAX_BLOOD-30)
		BLOODcount = 0;
	int n;
	for( n=BLOODcount;n<BLOODcount+30;n++)
	{	
		bloodparts[n].x[0] = _x + rand()%40;
		bloodparts[n].y[0] = _y + rand()%40;
		bloodparts[n].dir_x = (rand()%4000-2000)/800.0f;
		bloodparts[n].dir_y = -((float)(rand()%2000))/200.0f;
		bloodparts[n].down = 0;
		bloodparts[n].index = 1;
	}
	BLOODcount = n;
}
void cBlood::oneNewBlood(int _x,int _y)
{

	if(BLOODcount>MAX_BLOOD)
		BLOODcount = 0;	

		bloodparts[BLOODcount].x[0] = _x + rand()%40;
		bloodparts[BLOODcount].y[0] = _y + rand()%40;
		bloodparts[BLOODcount].dir_x = (rand()%4000-2000)/800.0f;
		bloodparts[BLOODcount].dir_y = -((float)(rand()%2000))/200.0f;
		bloodparts[BLOODcount].down = 0;
		bloodparts[BLOODcount].index = 1;

	BLOODcount++;
}


void cBlood::update()
{
	if(!show_blood)
		return;
	SDL_LockSurface(screen);

	for(int i=0;i<MAX_BLOOD;i++)
	{
		if(!bloodparts[i].down)
		{
			bloodparts[i].x[0] += (float)bloodparts[i].dir_x;
			bloodparts[i].y[0] += (float)bloodparts[i].dir_y;
			bloodparts[i].dir_y += (float)0.1f;

			for(int n=0;n<BLOOD_LEN;n++)
			{
				FastPixelColorNolock(screen,(int)bloodparts[i].x[n]-cam_x,(int)bloodparts[i].y[n]-cam_y,8394262);	
				FastPixelColorNolock(screen,(int)bloodparts[i].x[n]-cam_x+1,(int)bloodparts[i].y[n]-cam_y,8394262);	
				FastPixelColorNolock(screen,(int)bloodparts[i].x[n]-cam_x,(int)bloodparts[i].y[n]-cam_y+1,8394262);	
			}
	
			bloodparts[i].x[bloodparts[i].index] = bloodparts[i].x[0];
			bloodparts[i].y[bloodparts[i].index] = bloodparts[i].y[0];
			bloodparts[i].index++;
			if(bloodparts[i].index == BLOOD_LEN)
				bloodparts[i].index = 1;
			if(LEVEL->posValid(bloodparts[i].x[0],bloodparts[i].y[0]))
			{
				for(int n=1;n<BLOOD_LEN;n++)
				{
					bloodparts[i].y[n]=0;
				}
				bloodparts[i].down = 1;				
			}
		}else
		{
			FastPixelColorNolock(screen,(int)bloodparts[i].x[0]-cam_x,(int)bloodparts[i].y[0]-cam_y,8394262);	
			FastPixelColorNolock(screen,(int)bloodparts[i].x[0]-cam_x+1,(int)bloodparts[i].y[0]-cam_y,8394262);	
			FastPixelColorNolock(screen,(int)bloodparts[i].x[0]-cam_x,(int)bloodparts[i].y[0]-cam_y+1,8394262);	
		}
	}
	
	SDL_UnlockSurface(screen);
}




