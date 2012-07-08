/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


class cEnemy
{
public:
	cEnemy(int _x,int _y);
	
	void update();

	void MakeMyWalk();	
	void die();

	virtual void add_update();
	// headbang、也就是敌人被MARIO踩中了的意思、MARIO的特有动作、
	// 此处是虚拟函数、具体的实现由具体的敌人类型来决定
	virtual void headbang();
	virtual void draw();

	int playerTouchRight();
	int playerTouchLeft();
	
	int scoring;
	int H,W,dir,speed;

	bool down_touch,left_touch,right_touch,dead;
	bool dying;
	
	float y_speed;
	int x,y,DEATHcount;
	SDL_Rect rect;
		
	int walkcount,walkcount2,walkadd;

};

//LOADING IMAGE
void initGOOMBA();
void initSPINY();
void initTURTLE();
void initCANNON();
void initBRETT();
void initKILLERBLUUME();
void initGOOMBAbig();


class cGoomba : public cEnemy
{
public:
	cGoomba(int x,int y);

	virtual void add_update();
	virtual void headbang();
	virtual void draw();

};

class cGoombaBig : public cEnemy
{
public:
	cGoombaBig(int x,int y);

	virtual void add_update();
	virtual void headbang();
	virtual void draw();

};
class cSpiny : public cEnemy
{
public:
	cSpiny(int x,int y);

	virtual void add_update();
	virtual void headbang();
	virtual void draw();
};


class cTurtle : public cEnemy
{
public:
	cTurtle(int x,int y);

	virtual void add_update();
	virtual void headbang();
	virtual void draw();

	int down;
};

class cTurtleFly : public cEnemy
{
public:
	cTurtleFly(int x,int y);

	virtual void add_update();
	virtual void headbang();
	virtual void draw();
	int count;
};

class cFireCircle
{
public:
	cFireCircle(int _x,int _y,int _size);

	void update();
	int x,y,size;
	float dir_x,dir_y,angle;
	SDL_Rect rect;
	float counta2;
};

class cCannon
{
public:
	cCannon(int _x,int _y);

	void update();
	int x,y;
	int count;
	int bullet_x,bullet_y;
	int dir,die;
	bool updating;
};

class cKillerBlume
{
public:
	cKillerBlume(int _x,int _y);

	void update();
	int x,y;
	int count;
	bool dead;
	float timer;
	SDL_Rect rect;
};

class cBrett
{
public:
	cBrett(int _x,int _y);

	void update();
	int x,y;
	int count;
	int dir;
};


class cBowser
{
public:
	cBowser(int _x,int _y,int _hammers);

	void update();
	int x,y,W,H,fx,fy,static_x,static_y;
	int count,count2,getX;
	float timer;
	SDL_Rect rect;
	bool down_touch,arrived;
	bool godown;
	int health;
	SDL_Surface *BOWSER_SURFACE[4],*HAMMER_SURFACE[4];

	bool fire_flying;
	void hit();

	int hammers,HAMMERcount;
	struct hamma
	{
		float x,y,x_speed;
		float y_speed;
	}HAMMERS[30];
};
