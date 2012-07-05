/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


#ifndef __LEVELS_H__
#define __LEVELS_H__


class cScene
{
   public:
	cScene();
	/*LOADINGS::*/
	void loadLevel(char* scenename,int startpoint);
	void getParts(char *Text);
	void interpreteSceneLine();

	
	/*EXCECUTATION::*/
	void drawLevel();
	
	void saveLevel();
	void loadGame();

	void initBackground();
	void drawBackground();
	bool back_type;
	#define MAX_BACK 40
	int back_x[MAX_BACK];
	int back_y[MAX_BACK];
	int back_T[MAX_BACK];

	void unloadLevel();

	int posValid(int x,int y);
	int touchBonus(int x,int y);
	void BonusCheck(int x1,int y1,int x2,int y2);
	void castlelation();
	bool castled;
		
	void updatePhysic();

	int Bonus_Kick_Mode;

	int partcount;
	char **parts;
	ifstream ifs;
	bool tileread;
	bool bonusread;
	bool coinread;
	float COINcount;

	int BGCOLOR;
	char* nowlevel,*RESTARTLevel;
	
	int TEXTUREcount;
	
	



	#define MAX_X 400
	#define MAX_Y 100
	int TILE[MAX_X][MAX_Y];

	bool COIN[MAX_X][MAX_Y];
	
	int BONUS[MAX_X][MAX_Y];

	#define MAX_TEXTURES 100
	#define MAX_BONUS_TEXTURES 21
	SDL_Surface* Textures[MAX_TEXTURES];
	SDL_Surface* Bonus_Textures[MAX_BONUS_TEXTURES];
	SDL_Surface* SAVE_SURFACE;

	int VTiles,HTiles;

	struct START_POINT
	{
		int x,y;
		int type;
	}STARTPOINTS[5];
	struct EXIT_POINT
	{
		int x,y;
		char* level;
		int type;
		int startpoint;
	}EXITPOINTS[5];
	struct CASTELSTR
	{
		bool available;
		int x,y;
		bool big;
		char* level;
		int startpoint;
		SDL_Surface* small_s;
		SDL_Surface* big_s,*flag;
	}CASTLE;

	SDL_Rect SCREEN_RECT;
};

#endif
