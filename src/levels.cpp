/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/ 


#include "Global.h"
#include "unixutils.h"

int i;



cScene::cScene()
{
	this->parts = NULL;

	Bonus_Kick_Mode=0;

	BGCOLOR = 0;

	RESTARTLevel = new char[100];

	SCREEN_RECT.w = WIDTH;
	SCREEN_RECT.h = HEIGHT;
	SCREEN_RECT.x = 0;
	SCREEN_RECT.y = 0;

	HTiles =  WIDTH/40 + 2;
	VTiles =  HEIGHT/40 +1;

	TEXTUREcount = 1;
	nowlevel = new char[2];
	
	COINcount = 10;
	for(i=0;i<MAX_X;i++)
		for(int n=0;n<MAX_Y;n++)
		{
			TILE[i][n] = 0;
			BONUS[i][n] = 0;
			COIN[i][n] = 0;
		}

	for(i=0;i<MAX_TEXTURES;i++)
		Textures[i] = 0;

	CASTLE.level = new char[2];

	Textures[MAX_TEXTURES-2] = LoadIMG(DATADIR "gfx/background/bush1.png");
	Textures[MAX_TEXTURES-3] = LoadIMG(DATADIR "gfx/background/bush2.png");
	Textures[MAX_TEXTURES-4] = LoadIMG(DATADIR "gfx/background/hedge_1b.png");
	Textures[MAX_TEXTURES-5] = LoadIMG(DATADIR "gfx/background/bush4.png");
	Textures[MAX_TEXTURES-6] = LoadIMG(DATADIR "gfx/background/bush5.png");
	Textures[MAX_TEXTURES-7] = LoadIMG(DATADIR "gfx/background/bush6.png");
	Textures[MAX_TEXTURES-8] = LoadIMG(DATADIR "gfx/background/cloud1.png");
	Textures[MAX_TEXTURES-9] = LoadIMG(DATADIR "gfx/background/cloud2.png");

	SAVE_SURFACE = LoadIMG(DATADIR "gfx/menu/saved.jpg");
	CASTLE.big_s = LoadIMG(DATADIR "gfx/tiles/bigcastle.png");
	CASTLE.small_s = LoadIMG(DATADIR "gfx/tiles/smallcastle.png");
	CASTLE.flag = LoadIMG(DATADIR "gfx/bonus/flag.png");
}

void cScene::loadLevel(char* scenename,int startpoint)
{
	initBackground();

	unloadLevel();

	PLAYER->x_speed = 0;
	PLAYER->y_speed = -2.33;

	cout<<"...creating level"<<endl;
	ifstream ifs(scenename, ios::in);

	char *contents = new char[670];

	for (; ifs.getline(contents, 670);)
		if ( contents[0] != '#' && strlen(contents) > 2)
		{		
			getParts(contents);

			interpreteSceneLine();
			
			delete[](parts);
		}
		
	ifs.close();
	delete[](contents);
	cout<<"...Level successful created"<<endl;

	PLAYER->x = STARTPOINTS[startpoint].x;
	PLAYER->y = STARTPOINTS[startpoint].y;
	
	if(pixelation && strcmp(nowlevel,scenename) && gameStartedVirtual)
	{
		changescreen();
	}

	if(STARTPOINTS[startpoint].type!=0)
	{		
		PLAYER->update();
		PLAYER->x_speed = 0;
		PLAYER->y_speed = 0;
		
			if(STARTPOINTS[startpoint].type==1)//DOWN
				PLAYER->y-=60;
			else if(STARTPOINTS[startpoint].type==2)//RIGHT
				PLAYER->x-=60;
			else if(STARTPOINTS[startpoint].type==3)//LEFT
				PLAYER->x+=60;
			else if(STARTPOINTS[startpoint].type==4)//UP
				PLAYER->y+=60;
		PLAYSOUND2(S_PIPE);
		for(int n=0;n<30;n++)
		{
			SDL_FillRect(screen,0,LEVEL->BGCOLOR);
			drawBackground();
			if(STARTPOINTS[startpoint].type==1)//DOWN
				PLAYER->y+=2;
			else if(STARTPOINTS[startpoint].type==2)//RIGHT
				PLAYER->x+=2;
			else if(STARTPOINTS[startpoint].type==3)//LEFT
				PLAYER->x-=2;
			else if(STARTPOINTS[startpoint].type==4)//UP
				PLAYER->y-=2;

				tmp_rect.x = PLAYER->x-cam_x;
				tmp_rect.y = PLAYER->y-cam_y;
				SDL_UpperBlit(PLAYER->Surface_R[PLAYER->stat][0],0,screen,&tmp_rect);
				drawLevel();
				drawGLscreen();
			SDL_Delay(10);
		}
	}
	delete[] nowlevel;
	nowlevel = new char[100];
	strcpy(nowlevel,scenename);
//	cout<<" NOWLEVEL : "<<nowlevel<<"                   "<<scenename<<endl;
}

void cScene::interpreteSceneLine()
{
	if (!strcmp (parts[0], "TEXTURE"))
	{
		char buf[MAX_PATH];
		/* Texture path should always start with "data/" which we
		   replace with DATADIR */
		if (strncmp(parts[1], "data/", 5))
			return;

		sprintf(buf,  "%s%s", DATADIR, parts[1] + 5);
		Textures[TEXTUREcount] = LoadIMG(buf);
		TEXTUREcount++;	
	}
	else if (!strcmp (parts[0], "::TILES->"))
	{
		if (!strcmp (parts[1], "BEGIN"))
			tileread = 1;
		else
			tileread = 0;
	}
	else if (!strcmp (parts[0], "::BONUS->"))
	{
		if (!strcmp (parts[1], "BEGIN"))
			bonusread = 1;
		else
			bonusread = 0;
	}
	else if (!strcmp (parts[0], "::COIN->"))
	{
		if (!strcmp (parts[1], "BEGIN"))
			coinread = 1;
		else
			coinread = 0;
	}
	else if (tileread)
	{
		TILE[atoi(parts[0])][atoi(parts[1])] = atoi(parts[2]);
	}
	else if (bonusread)
	{
		BONUS[atoi(parts[0])][atoi(parts[1])] = atoi(parts[2]);
	}
	else if (coinread)
	{
		COIN[atoi(parts[0])][atoi(parts[1])] = 1;
	}
	///ENEMIES :
	else if (!strcmp (parts[0], "GOOMBA"))
	{
		GOOMBA[GOOMBAcount] = new cGoomba(atoi(parts[1])*40,atoi(parts[2])*40);
		GOOMBAcount++;
	}
	else if (!strcmp (parts[0], "TURTLE"))
	{
		TURTLE[TURTLEcount] = new cTurtle(atoi(parts[1])*40,atoi(parts[2])*40);
		TURTLEcount++;
	}
	else if (!strcmp (parts[0], "TURTLEFLY"))
	{
		TURTLEFLY[TURTLEFLYcount] = new cTurtleFly(atoi(parts[1])*40,atoi(parts[2])*40);
		TURTLEFLYcount++;
	}
	else if (!strcmp (parts[0], "SPINY"))
	{
		SPINY[SPINYcount] = new cSpiny(atoi(parts[1])*40,atoi(parts[2])*40);
		SPINYcount++;
	}
	else if (!strcmp (parts[0], "FIRECIRCLE"))
	{
		FIRECIRCLE[FIRECIRCLEcount] = new cFireCircle(atoi(parts[1])*40,atoi(parts[2])*40,atoi(parts[3]));
		FIRECIRCLEcount++;
	}
	else if (!strcmp (parts[0], "KILLERBLUME"))
	{
		KILLERBLUME[KILLERBLUMEcount] = new cKillerBlume((atoi(parts[1])*40) + 20,((atoi(parts[2])+1)*40 ));
		KILLERBLUMEcount++;
	}
	else if (!strcmp (parts[0], "CANNON"))
	{
		CANNON[CANNONcount] = new cCannon(atoi(parts[1])*40,atoi(parts[2])*40);
		LEVEL->BONUS[atoi(parts[1])][atoi(parts[2])] = 8;
		CANNONcount++;
	}
	else if (!strcmp (parts[0], "BRETT"))
	{
		BRETT[BRETTcount] = new cBrett(atoi(parts[1]),atoi(parts[2]));
		BRETTcount++;
	}
	else if (!strcmp (parts[0], "FIRE"))
	{
		FIRE->newFire(atoi(parts[1])*40,atoi(parts[2])*40);
	}
	else if (!strcmp (parts[0], "GOOMBABIG"))
	{
		GOOMBABIG[GOOMBABIGcount] = new cGoombaBig(atoi(parts[1])*40,atoi(parts[2])*40);
		GOOMBABIGcount++;
	}
	//AUXILLIARY :
	else if (!strcmp (parts[0], "BGCOLOR"))
		BGCOLOR = atoi(parts[1]);
	else if (!strcmp (parts[0], "START"))
	{
		STARTPOINTS[atoi(parts[1])].x = atoi(parts[2]);
		STARTPOINTS[atoi(parts[1])].y = atoi(parts[3]);
		STARTPOINTS[atoi(parts[1])].type = atoi(parts[4]);
	}
	else if (!strcmp (parts[0], "EXIT"))
	{
		EXITPOINTS[atoi(parts[1])].x = atoi(parts[2]);
		EXITPOINTS[atoi(parts[1])].y = atoi(parts[3]);
		EXITPOINTS[atoi(parts[1])].startpoint = atoi(parts[5]);
		EXITPOINTS[atoi(parts[1])].level = new char[100];
		EXITPOINTS[atoi(parts[1])].type = atoi(parts[6])+1;
		/* Level path should always start with "data/" which we
		   replace with DATADIR */
		if (strncmp(parts[4], "data/", 5))
			return;

		sprintf(EXITPOINTS[atoi(parts[1])].level, 	"%s%s",	DATADIR, parts[4] + 5);
	}
	else if (!strcmp (parts[0], "CASTLE"))
	{
		CASTLE.x = atoi(parts[1]);
		CASTLE.y = atoi(parts[2]);
		CASTLE.big = atoi(parts[3]);
		CASTLE.level = new char[100];
		/* Level path should always start with "data/" which we
		   replace with DATADIR */
		if (strncmp(parts[4], "data/", 5))
			return;

		sprintf(CASTLE.level,  "%s%s",			DATADIR, parts[4] + 5);
		CASTLE.startpoint = atoi(parts[5]);
		CASTLE.available = 1;
	}
	else if (!strcmp (parts[0], "MUSIC"))
		playMusic(parts[1]);
	else if (!strcmp (parts[0], "BOWSER"))
		BOWSER = new cBowser(atoi(parts[1]),atoi(parts[2])+1,atoi(parts[3]));
	else if (!strcmp (parts[0], "BACKGROUND"))
	{
		char buf[MAX_PATH];
		/* Texture path should always start with "data/" which we
		   replace with DATADIR */
		if (strncmp(parts[1], "data/", 5))
			return;

		if(Textures[MAX_TEXTURES-1])
			SDL_FreeSurface(Textures[MAX_TEXTURES-1]);
		sprintf(buf,  "%s%s", DATADIR, parts[1] + 5);
		Textures[MAX_TEXTURES-1] = LoadIMG(buf);
		back_type = 1;
		back_x[0] = 0;
		SDL_LockSurface(Textures[MAX_TEXTURES-1]); 
		BGCOLOR = getpixel(Textures[MAX_TEXTURES-1],300,479);
		SDL_UnlockSurface(Textures[MAX_TEXTURES-1]); 
	}
	else if (!strcmp (parts[0], "RESTART"))
	{
		/* Level path should always start with "data/" which we
		   replace with DATADIR */
		if (strncmp(parts[1], "data/", 5))
			return;

		sprintf(RESTARTLevel,  "%s%s",
			DATADIR, parts[1] + 5);
	}
	else if (!strcmp (parts[0], "TIP"))
	{
		showTip(parts[1],parts[2],parts[3],parts[4],parts[5],parts[6],parts[7]);
	}else if (!strcmp (parts[0], "THEGAMEENDSHERE"))
	{
		THEGAMEENDSHERE();
		return;
	}
}

void cScene::getParts(char *Text)
{
	char *str = Text;
	partcount = 0;
	int len = 0, i = 0;

	
	str += strspn(str," ");
	
	while (*str)
	{
		partcount++;
		str += strcspn(str," ");
		str += strspn(str," ");
	}
	
	str = Text;
	
	parts = new char*[partcount];

	str += strspn(str," ");
	
	while (*str)
	{
		int len = (int)(strcspn(str," "));
		parts[i] = new char[len + 2];
		memcpy(parts[i],str,len);
		parts[i][len] = 0;
		str += len + strspn(str+len," ");
		i++; 
	}
	/* strip trailing CR as on Unix ifs.getline() doesn't do that. */
	if (parts[i-1][strlen(parts[i-1])-1] == '\r')
		parts[i-1][strlen(parts[i-1])-1] = 0;
}


void cScene::initBackground()
{
	back_x[0] = 50;
	back_T[0] = MAX_TEXTURES - 2;
	back_y[0] = 3999 - Textures[back_T[0]]->h;

	for(i=1;i<MAX_BACK/2;i++)
	{
		back_x[i] = back_x[i-1]+130+rand()%550;
		back_T[i] = MAX_TEXTURES - 2 - rand()%6;
		back_y[i] = 3999 - Textures[back_T[i]]->h;
	}

	back_x[MAX_BACK/2] = 50;
	back_T[MAX_BACK/2] = (rand()%2) ? MAX_TEXTURES-8 : MAX_TEXTURES-9;
	back_y[MAX_BACK/2] = 3960 - (rand()%500+250);

	for(i=MAX_BACK/2+1;i<MAX_BACK;i++)
	{
		back_x[i] = back_x[i-1]+180+rand()%400;
		back_T[i] = (rand()%2) ? MAX_TEXTURES-8 : MAX_TEXTURES-9;
		back_y[i] = 3750 - (rand()%400);
	}

}
void cScene::drawBackground()
{
	if(!back_type)
	{
		for(i=MAX_BACK/2;i<MAX_BACK;i++)
		{
			tmp_rect.x = back_x[i] - (cam_x/3);
			tmp_rect.y = back_y[i] - cam_y ;
			tmp_rect.h = Textures[back_T[i]]->h;
			tmp_rect.w = Textures[back_T[i]]->w;
			if(collision(&SCREEN_RECT,&tmp_rect))
				SDL_UpperBlit(Textures[back_T[i]],0,screen,&tmp_rect);
		}
		for(i=0;i<MAX_BACK/2;i++)
		{
			tmp_rect.x = back_x[i] - (cam_x/2);
			tmp_rect.y = back_y[i] - cam_y;
			tmp_rect.h = Textures[back_T[i]]->h;
			tmp_rect.w = Textures[back_T[i]]->w;
			if(collision(&SCREEN_RECT,&tmp_rect))
				SDL_UpperBlit(Textures[back_T[i]],0,screen,&tmp_rect);
		}
	}
	else
	{
			tmp_rect.y = 0;
			tmp_rect.x = -(cam_x/2)%640;
			SDL_UpperBlit(Textures[MAX_TEXTURES-1],0,screen,&tmp_rect);
			tmp_rect.x = -(cam_x/2) %640 + 640;
			SDL_UpperBlit(Textures[MAX_TEXTURES-1],0,screen,&tmp_rect);
	}
}

void cScene::castlelation()
{
	castled = 1;
	tmp_rect.x = CASTLE.x - cam_x +25;
	tmp_rect.y = CASTLE.y - cam_y +40;
	SCORE->init(PLAYER->x,PLAYER->y,(380+(CASTLE.y-PLAYER->y))*10);
	PLAYSOUND2(S_STAGE_CLEAR);
	for(int n=CASTLE.y - cam_y +40;n < CASTLE.y - cam_y +280 ; n+=2)
	{
		PLAYER->movecam();
		SDL_FillRect(screen,0,LEVEL->BGCOLOR);
		drawBackground();
		drawLevel();		
		SCORE->update();
		 
		tmp_rect.x = CASTLE.x - cam_x +25;
		tmp_rect.y = n;
		if(PLAYER->y<CASTLE.y+260)
			PLAYER->y+=2;
		SDL_UpperBlit(CASTLE.flag,0,screen,&tmp_rect);
		tmp_rect.x-=40;
		tmp_rect.y = PLAYER->y-cam_y;
		SDL_UpperBlit(PLAYER->Surface_R[PLAYER->stat][2],0,screen,&tmp_rect);

		drawGLscreen();
	}
	PLAYER->x_speed = 0;
	PLAYER->y_speed = 0;
	keys[PLAYER->K_RIGHT] = 1;
	keys[PLAYER->K_B] = 0;
	keys[PLAYER->K_A] = 0;						
	keys[PLAYER->K_DOWN] = 0;						
	keys[PLAYER->K_LEFT] = 0;
	PLAYER->y_speed=-10;
	int n=0;
	for(n=0;n<95;n++)
	{
		SDL_FillRect(screen,0,LEVEL->BGCOLOR);
		drawBackground();
		drawLevel();
		
		PLAYER->update();
		PLAYER->draw();
		
		drawGLscreen();
		framerate();
	}
	PLAYER->JUMPcount=400;
	drawLevel();
	drawGLscreen();
	SDL_Delay(500);
	keys[PLAYER->K_RIGHT] = 0;
	LEVEL->saveLevel();
	LEVEL->loadLevel(CASTLE.level,CASTLE.startpoint);
			return;
}//////////////////////////////////////////////////////////////////////////

SDL_Rect tmp_rect;
int posx,posy;
void cScene::drawLevel()
{
	posx = cam_x/40;
	posy = cam_y/40;
	
	COINcount+=0.13;
	if(COINcount>13)
		COINcount=10;


	for(i = posx;i<posx+HTiles;i++)
	{
		for(int n = posy;n<posy+VTiles;n++)
		{
			if(COIN[i][n])
			{
				tmp_rect.x = (i*40) - cam_x;
				tmp_rect.y = (n*40) - cam_y;
				SDL_UpperBlit(Bonus_Textures[(int)COINcount],0,screen,&tmp_rect);
				if(collision(&tmp_rect,&PLAYER->rect))
				{
					COIN[i][n] = 0;
					PLAYSOUND2(S_COIN);
					HUD.coins++;
					HUD.score += 200;
					GLIDDER->init(i,n,0,0);
					if(HUD.coins>=100)
					{
						HUD.coins = 0;
						HUD.lifes++;
						PLAYSOUND1(S_1UP);
					}
				}
			}
			if(TILE[i][n])
			{
				tmp_rect.x = (i*40) - cam_x;
				tmp_rect.y = (n*40) - cam_y;
				SDL_UpperBlit(Textures[TILE[i][n]],0,screen,&tmp_rect);
			}
			else if(BONUS[i][n])
			{
				tmp_rect.x = (i*40) - cam_x;
				tmp_rect.y = (n*40) - cam_y;
				SDL_UpperBlit(Bonus_Textures[BONUS[i][n]],0,screen,&tmp_rect);
			}	
		}
	}
	if(CASTLE.available)
	{
		tmp_rect.x = CASTLE.x - cam_x;
		tmp_rect.y = CASTLE.y - cam_y;
		tmp_rect.w = 600; 
		tmp_rect.h = 360; 
		if(collision(&tmp_rect,&SCREEN_RECT))
		{
			if(!castled&&collision(&PLAYER->rect,&tmp_rect))
			{
				if(PLAYER->rect.x-10<tmp_rect.x+60)
					if(PLAYER->rect.y+PLAYER->H < tmp_rect.y+tmp_rect.h-40)
					{
						PLAYSOUND2(S_FLAG);
						castlelation();
					}
						
			}
			if(CASTLE.big)
				SDL_UpperBlit(CASTLE.big_s,0,screen,&tmp_rect);
			else 
				SDL_UpperBlit(CASTLE.small_s,0,screen,&tmp_rect);
			tmp_rect.x = CASTLE.x - cam_x +25;
			tmp_rect.y = CASTLE.y - cam_y +40;
			if(!castled)
				SDL_UpperBlit(CASTLE.flag,0,screen,&tmp_rect);
		}
	}
}
int cScene::posValid(int x,int y)
{
	x/=40;
	y/=40;
	
	return TILE[x][y]+BONUS[x][y];
}

int cScene::touchBonus(int x,int y)
{
	x/=40;
	y/=40;
	
	return BONUS[x][y];
}
	#define CRACKB 1
	#define PILZ 2
	#define LIFE 3
	#define STERN 4
	#define BOXEMPTY 7
	#define INVIS 8
	
void scriptBonus(int x,int y)
{
	switch(LEVEL->BONUS[x][y])
	{
		case 1://CRACKBOX
			if(!LEVEL->Bonus_Kick_Mode&&!PLAYER->stat)
			{
				kickUp(x,y);
			}
			else
			{
				LEVEL->BONUS[x][y] = 0;	
				CRACK->init(x,y,0);
			}
		break;
		case 14://CRACKBOX 2
			if(!LEVEL->Bonus_Kick_Mode&&!PLAYER->stat)
			{
				kickUp(x,y);
			}
			else
			{
				LEVEL->BONUS[x][y] = 0;	
				CRACK->init(x,y,1);
			}
		break;
	
		case 2://PILZ
			bonus_activ = 20;
			LEVEL->BONUS[x][y] = BOXEMPTY;
			kickUp(x,y);
			for(i=0;i<5;i++)
				if(!BONUS_DYNAMIC[i]->online)
				{
					if(PLAYER->stat == 0)
						BONUS_DYNAMIC[i]->init(x,y,0);
					else 
						BONUS_DYNAMIC[i]->init(x,y,1);
					break;
				}
			PLAYSOUND2(S_TOCK);
		break;
		case 3:
			bonus_activ = 20;
			LEVEL->BONUS[x][y] = BOXEMPTY;	
			for(i=0;i<5;i++)
				if(!BONUS_DYNAMIC[i]->online)
				{
					BONUS_DYNAMIC[i]->init(x,y,2);
					break;
				}
			kickUp(x,y);
			PLAYSOUND2(S_TOCK);
		break;
		case 4:
			bonus_activ = 20;
			LEVEL->BONUS[x][y] = BOXEMPTY;	
			for(i=0;i<5;i++)
				if(!BONUS_DYNAMIC[i]->online)
				{
					BONUS_DYNAMIC[i]->init(x,y,3);
					break;
				}
			kickUp(x,y);
			PLAYSOUND2(S_TOCK);
		break;
		case 15:
			bonus_activ = 20;
			LEVEL->BONUS[x][y] = BOXEMPTY;	
			for(i=0;i<5;i++)
				if(!BONUS_DYNAMIC[i]->online)
				{
					BONUS_DYNAMIC[i]->init(x,y,3);
					break;
				}
			kickUp(x,y);
			PLAYSOUND2(S_TOCK);
		break;
		case 16:
			LEVEL->BONUS[x][y] = BOXEMPTY;	
			PLAYSOUND2(S_COIN);
			HUD.coins++;                /////COIN BOX
			kickUp(x,y);
			GLIDDER->init(x,y-1,0,-1);
		break;
		case BOXEMPTY:
			bonus_activ = 20;
			PLAYSOUND1(S_TOCK);
		break;
	}
}


void cScene::BonusCheck(int x1,int y1,int x2,int y2)
{
	x1/=40;
	y1/=40;
	x2/=40;
	y2/=40;
	if(! (BONUS[x1][y1]+BONUS[x2][y2]))
		return;
	

	scriptBonus(x1,y1);
	scriptBonus(x2,y2);
	
	if(Bonus_Kick_Mode==0)
		PLAYER->JUMPcount = 400;	
}



void cScene::saveLevel()
{
	SDL_UpperBlit(SAVE_SURFACE,0,screen,0);
	drawGLscreen();

#ifdef __unix__
	char buf[MAX_PATH], *home_dir = get_homedir();
	snprintf(buf, MAX_PATH, "%s/.megamario/save.sav",
		home_dir? home_dir:".");
	ofstream save(buf);
#else	
	ofstream save("data/save.sav");
#endif
	if(cheater)
		save<<"#This is a savegame for Mega Mario Bros.\n# ´This Savegame was modified. Fuckin' Cheata!!!\n LoL ;)"<<endl<<endl;	
	else
		save<<"#This is a savegame for Mega Mario Bros.\n#Just try to modify any value!"<<endl<<endl;
	if(cheater)
		save<<"LEVEL "<<"Yeah,that's_right.Looks_easy. "<<CASTLE.level<<" "<<CASTLE.startpoint<<" Go Home, CHEATER."<<endl;
	else
		save<<"LEVEL "<<"Yeah,that's_right.Looks_easy. "<<CASTLE.level<<" "<<CASTLE.startpoint<<" Rockin' Home."<<endl;
	save<<"SCORE "<<HUD.score<<" "<<HUD.score*3<<endl;
	save<<"COINS "<<HUD.coins<<" "<<HUD.coins*33<<endl;
	if(cheater)
		save<<"LIFE "<<HUD.lifes<<" "<< HUD.lifes*5.213<<endl;
	else
		save<<"LIFE "<<HUD.lifes<<" "<< HUD.lifes*333<<endl;
	save<<"STAT "<<PLAYER->stat<<" "<< PLAYER->stat+123456789<<endl;


	SDL_Delay(2000);
}
void cScene::loadGame()
{
#ifdef __unix__
	char buf[MAX_PATH], *home_dir = get_homedir();
	snprintf(buf, MAX_PATH, "%s/.megamario/save.sav",
		home_dir? home_dir:".");
	ifstream ifs(buf, ios::in);
#else	
	ifstream ifs("data/save.sav", ios::in);
#endif

	char *contents = new char[300];

	for (; ifs.getline(contents, 300);)
		if ( contents[0] != '#' && strlen(contents) > 2)
		{		
			getParts(contents);
			if (!strcmp (parts[0], "LEVEL"))
			{				
				loadLevel(parts[2],atoi(parts[3]));
			}
			else if (!strcmp (parts[0], "SCORE"))
			{
				HUD.score = atoi(parts[1]);
				if(HUD.score != ((atoi(parts[2])/3)))
					cheater = 1;
			}
			else if (!strcmp (parts[0], "COINS"))
			{
				HUD.coins = atoi(parts[1]);
				if(HUD.coins != ((atoi(parts[2])/33)))
					cheater = 1;
			}
			else if (!strcmp (parts[0], "LIFE"))
			{
				HUD.lifes = atoi(parts[1]);
				if(HUD.lifes != ((atoi(parts[2])/333)))
					cheater = 1;
			}
			else if (!strcmp (parts[0], "STAT"))
			{
				PLAYER->changeStat(atoi(parts[1]));
				if(PLAYER->stat != ((atoi(parts[2])-123456789)))
					cheater = 1;
			}
	
				
		//	delete[](parts);
		}
		
	ifs.close();
	delete[](contents);

}


void cScene::unloadLevel()
{
	int i=0;
	cout<<"...destroying level"<<endl;
	PLAYER->x = 320;
	PLAYER->y = 240;
	for(i=0;i<5;i++)
		BONUS_DYNAMIC[i]->online=0;

	for(i=0;i<TEXTUREcount;i++)
		SDL_FreeSurface(Textures[i]);
	
	TEXTUREcount = 1;
	
	for(i=0;i<MAX_X;i++)
		for(int n=0;n<MAX_Y;n++)
		{
			TILE[i][n] = 0;
			BONUS[i][n] = 0;
			COIN[i][n] = 0;
		}
	
	for(i=0;i<GOOMBAcount;i++)
		delete(GOOMBA[i]);
	for(i=0;i<TURTLEcount;i++)
		delete(TURTLE[i]);
	for(i=0;i<TURTLEFLYcount;i++)
		delete(TURTLEFLY[i]);
	for(i=0;i<SPINYcount;i++)
		delete(SPINY[i]);
	for(i=0;i<FIRECIRCLEcount;i++)
		delete(FIRECIRCLE[i]);
	for(i=0;i<CANNONcount;i++)
		delete(CANNON[i]);
	for(i=0;i<KILLERBLUMEcount;i++)
		delete(KILLERBLUME[i]);
	for(i=0;i<BRETTcount;i++)
		delete(BRETT[i]);
	for(i=0;i<GOOMBABIGcount;i++)
		delete(GOOMBABIG[i]);

	if(BOWSER)
		delete BOWSER;
	BOWSER = 0;

	tileread = 0;
	bonusread = 0;
	TEXTUREcount = 1;
	back_type = 0;
	
	FIRE->reset();
	BLOOD_GENERATOR->reset();

	COINcount = 10;

	GOOMBAcount = 0;
	TURTLEcount = 0;
	SPINYcount = 0;
	KILLERBLUMEcount = 0;
	CANNONcount = 0;
	TURTLEFLYcount = 0;
	FIRECIRCLEcount = 0;
	BRETTcount = 0;
	GOOMBABIGcount = 0;

	BGCOLOR = 2000003;
	tileread = 0;
	bonusread = 0;
	coinread = 0;	
	
	castled=0;

	CASTLE.available = 0;
	CASTLE.x = 0;
	CASTLE.y = 0;
	CASTLE.big = 0;
	CASTLE.startpoint = 0;

	cout<<"...Level destroyed"<<endl;
}
