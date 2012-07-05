/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/

class cPlayer
{
public:
	cPlayer(int LEFT,int RIGHT,int UP,int DOWN,int B,int A,int _speed_jump_height);
	
	void update();
	void changeStat(int staty);
	void draw();
	void die();
	void movecam();
	
	void MakeMyWalk();	
	
	int H,W,invincible;

	bool dir,top_touch,top_bonus_touch,down_touch,left_touch,right_touch,touch_now;
	bool Aed,Bed,KeyDown_down;
	float y_speed,x_speed,max_speed;
	bool dead,incincible,STARPOWER_OVER;
	
	float x,y;
	SDL_Rect rect;
	SDL_Surface *Surface_R[3][4],*Surface_L[3][4];
	
	int walkcount,walkcount2,walkadd,JUMPcount;
	int jumpheight,STARPOWER;
	int stat,speed_jump_height;

	int K_LEFT,K_RIGHT,K_UP,K_DOWN,K_B,K_A;
};




class cFireball
{
public:
	cFireball();
	SDL_Surface *FIREBALLS[4];

	bool init(int _x, int _y,int dir);
	void update();

	bool online;
	SDL_Rect rect;
	int x,y;
	float x_speed,y_speed,ani;
	bool down_touch,center_touch;
};
