/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/



#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#define MAX_FIRE 800
class cFire
{
   public:
	cFire();
	void update();
	void newFire(int _x,int _y);
	void reset();


	SDL_Surface *FIRESURF;

   private:
	struct fireparts_STRUCT
	{
		int x,y;
		int color;
	}fireparts[MAX_FIRE];
	int super_y;
	int super_x;
	
	SDL_Surface *SURFACE;
	
	SDL_Rect RECT;
	
	int x[300],y[300],FIREcount;
};

#define BLOOD_LEN 10
#define MAX_BLOOD 500
class cBlood
{
   public:
	cBlood();
	void update();
	void newBlood(int _x,int _y);
	void oneNewBlood(int _x,int _y);
	void reset();

   private:
	struct bloodparts_STRUCT
	{
		float x[BLOOD_LEN],y[BLOOD_LEN];
		float dir_x,dir_y;
		int index;
		bool down;
	}bloodparts[MAX_FIRE];
	int BLOODcount;
	
};
#endif
