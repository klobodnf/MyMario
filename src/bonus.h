/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/




class cBonus
{
public:
	cBonus();
	void update();
	void init(int _x,int _y,int _type);
	

	float y_speed,x_speed;
	float x,y;
	SDL_Rect rect;
	SDL_Surface *SURFACE[4];
	
	int type,counter;
	bool down_touch,left_touch,right_touch,up_touch;
	bool online;
};


class cCrack
{
public:
	cCrack();
	void update();
	void init(int _x,int _y,int grey);
		
	struct CRACK_STRUCT
	{
		float y_speed,x_speed;
		float x,y;
		int grey;
	}CRACK[50];
	int CRACKcount;
	SDL_Rect rect;
	SDL_Surface *SURFACE[2];
	
	int counter;
};

void kickUp(int BOXy,int BOXx);
void updateKickUp();


class cGlidder
{
public:
	cGlidder();
	void update();
	void init(int _x,int _y,int _speed_x,int _speed_y);
		
	struct GLID_STRUCT
	{
		float y_speed,x_speed;
		float x,y;
		float count;
	}GLID[50];
	int GLIDcount;
	SDL_Rect rect;
	SDL_Surface *SURFACE[4];

};



class cScore
{
public:
	cScore();
	void update();
	void init(int _x,int _y,int _score);
		
	struct SCORE_STRUCT
	{
		float x,y;
		char* text;
		int count;
	}SCORE_S[20];
	int SCOREcount;
};
