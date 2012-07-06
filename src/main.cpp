/********************************************************************
	created:	2005/09/18
	created:	18:9:2005   10:13
	filename: 	main.cpp
	author:		Jens Wellmann (c)
*********************************************************************/


#include "Global.h"
#include "iniparser.h"
#include "unixutils.h"
#include <time.h>


bool running;

/* All the variables below are for the FPS counter, which uses the pentium
   performance counter on windows, this is rather non portable, we could
   create equivalent functionality in a cpu portable way under unix using
   gettimeofday, but currently thats rather low priority */
// 这里判断的是如果非UNIX系系统、则执行
#ifndef __unix__   
// QueryPerformanceFrequency()的频率
// 每秒获取多少CPU Performance Tick
// 必须要查询系统以得到QueryPerformanceCounter()返回的嘀哒声的频率才能计算出最小单位为多少每秒
LARGE_INTEGER tickspersecond;
// 过一段时间后再次调用QueryPerformanceCounter()结束的时间、获取系统的最小单位时间
LARGE_INTEGER currentticks;
// 第一次调用QueryPerformanceCounter()开始的时间、获取系统的最小单位时间
LARGE_INTEGER framedelay;

int fps,framecount = 5;
// char* FRAMECHAR = new char[1];
char* FRAMECHAR = NULL;
#endif
int targetfps=60;

	static char szIniFile [MAX_PATH];
	static char szAppName [MAX_PATH];	
	

#ifndef __unix__

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	// 第一次取出开始时间的值
	QueryPerformanceCounter(&framedelay);
	// 获取CPU的频率值
	QueryPerformanceFrequency(&tickspersecond); 
		// OPTIONS LOAD :
	GetModuleFileName (NULL, szIniFile, sizeof (szIniFile)) ;
    strcpy (strrchr (szIniFile, '.') + 1, "ini") ;
	// 从指定ini文件中读取字段所对应的整数值、并返回相应的整数值、如果未发现、
	// 则返回函数中的指定值

	// 函数原型GetPrivateProfileInt(字段名, 属性名, 指定返回属性值, 文件名)
	// 在这里字段名为mario、属性名为showFPS, 指定返回值为0, 文件名为mario.ini
	// 这里如果mario.ini文件中没有写上showFPS的话、这里就直接返回0
	if(GetPrivateProfileInt(szAppName, "showFPS",0, szIniFile))
		NoFrameCheck = 1;
		
	// 这里可能是调试用、其实是多余的、否则前面的判断语句就没用了
	// NoFrameCheck = 1;

#else  /* defined __unix__ */

int main(int argc, char *argv[])
{
	char *home_dir = get_homedir();
	snprintf(szIniFile, MAX_PATH, "%s/.megamario",
		home_dir? home_dir:".");
	check_and_create_dir(szIniFile);
	snprintf(szIniFile, MAX_PATH, "%s/.megamario/mario.ini",
		home_dir? home_dir:".");

#endif /* defined __unix__ */

	dictionary * d = iniparser_new(szIniFile);
	pixelation = iniparser_getboolean (d, "Mario:Pixelation", 1);
	show_blood = iniparser_getboolean (d, "Mario:Blood", 0);
	nosound    = iniparser_getint (d, "Mario:Sound", 2);

	GL_Width = iniparser_getint (d, "Mario:SCREEN_W", 640);
	GL_Height = iniparser_getint (d, "Mario:SCREEN_H", 480);

	WIDTH = GL_Width;
	HEIGHT = GL_Height;

	Real_Fullscreen = iniparser_getboolean (d, "Mario:FULLSCREEN", 1);
	//GAME SPEED READ IN :
	targetfps = iniparser_getint (d, "Mario:Gamespeed", targetfps);
	if(targetfps<10)	
		targetfps = 10;
	if(targetfps>150)	
		targetfps = 150;

	//CHEATS ENABLED ??
	cheats_enabled  = iniparser_getboolean (d, "Mario:CHEATS_ENABLED", 0);
	iniparser_free (d);

	
	srand(time(0)); 

    initGame(); //SCREEN INIT;
	
	
	initMenu();

	
	setGame();

	
	running = 1;
	while(running) 
	{
		gameEvent();
		framerate();
	}	

exitos();

return(0);
}


void initGame()
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) 
	{
        fprintf(stderr, "SDL konnte nicht initialisiert werden: %s\n",SDL_GetError());
	     running=0;
    }


	if (Mix_OpenAudio (44800, AUDIO_S16, 2, 4096) == -1)
	{
		printf ("Can't open audio subsystem...");
		nosound = true;
	}	

	int numtimesopened, frequency, channels;
	Uint16 format;
	numtimesopened=Mix_QuerySpec(&frequency, &format, &channels);


	if (TTF_Init()==-1)
	{
		printf ("Can't open TTF Subsystem...");
	}

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);



    atexit(SDL_Quit);

#ifdef Use_OPENGL


    SDL_SetVideoMode(GL_Width, GL_Height, SCREEN_BPP ,SDL_OPENGL|SDL_GL_DOUBLEBUFFER);
	
	InitOpenGL();

	Uint32 rmask, gmask, bmask, amask;

	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;

	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
	//SDL_Surface *temp;

	screen = SDL_CreateRGBSurface( SDL_HWSURFACE, WIDTH, HEIGHT, SCREEN_BPP , rmask, gmask, bmask, amask );

//	screen = SDL_DisplayFormat( temp );
//	SDL_FreeSurface( temp );
#else
	int flags = (SDL_DOUBLEBUF|SDL_RLEACCEL|SDL_HWSURFACE|SDL_SRCALPHA|SDL_SRCCOLORKEY);
	if(Real_Fullscreen)
		flags|=SDL_FULLSCREEN;
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, SCREEN_BPP ,flags);

#endif

	Gamepad = new cGamepad;
	Gamepad->init();
	SDL_ShowCursor(SDL_DISABLE);

	SDL_WM_SetCaption(CAPTION, NULL);

	magenta = SDL_MapRGB(screen->format, 255, 0, 255);

}

void setGame()
{
	LEVEL = new cScene();
	for(int i=0;i<5;i++)
		BONUS_DYNAMIC[i] = new cBonus();
	CRACK = new cCrack();
	FIREBALL[0] = new cFireball();
	FIREBALL[1] = new cFireball();
	GLIDDER = new cGlidder();
	SCORE = new cScore();
	FIRE = new cFire();
	
	BLOOD_GENERATOR = new cBlood();

	cheatcode = new char[10];
	
	font = TTF_OpenFont (DATADIR "style.TTF",26);
	TTF_SetFontStyle (font, 0) ;
	
	initMusic();

	loadGlobalSprites();
	
	showMenu();
	

}

void gameEvent()
{

	// 这里会不断从事件列表中获取事件信息、直到没有事情处理了、就返回0退出
	// 此次的事件循环、然后执行一些其它的事情、再次调用此函数继续检查事件信息
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
			exitos();
			break;
			
			case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{	showMenu();}
			if(event.key.keysym.sym == SDLK_RETURN)
				BLOOD_GENERATOR->newBlood(PLAYER->x,PLAYER->y);

			if(event.key.keysym.sym == SDLK_F12)
			{
				char gammel[100];
				sprintf(gammel,"screenshots/%d.bmp",rand()%10000);
				SDL_SaveBMP(screen,gammel);				
				delete[] gammel;
			}
		
			break;	
		}	
		Gamepad->Handle_Event();
	}	



	keyInput();

#ifdef Use_OPENGL

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
#endif
	// 因为第二个参数为0、所以此处意思是背景表面使用BGCOLOR的色彩来填充
	SDL_FillRect(screen,0,LEVEL->BGCOLOR);

	PLAYER->update();

	LEVEL->drawBackground();

	int i;
	for(i=0;i<KILLERBLUMEcount;i++)
		KILLERBLUME[i]->update();

	for(i=0;i<5;i++)
		BONUS_DYNAMIC[i]->update();

	i=0;
	LEVEL->drawLevel();	

	FIRE->update();

	for(i=0;i<GOOMBAcount;i++)
		GOOMBA[i]->update();

	for(i=0;i<TURTLEcount;i++)
		TURTLE[i]->update();

	for(i=0;i<TURTLEFLYcount;i++)
		TURTLEFLY[i]->update();

	for(i=0;i<SPINYcount;i++)
		SPINY[i]->update();

	for(i=0;i<FIRECIRCLEcount;i++)
		FIRECIRCLE[i]->update();

	for(i=0;i<CANNONcount;i++)
		CANNON[i]->update();

	for(i=0;i<BRETTcount;i++)
		BRETT[i]->update();

	for(i=0;i<GOOMBABIGcount;i++)
		GOOMBABIG[i]->update();


	if(show_blood&&!PLAYER->STARPOWER)
		BLOOD_GENERATOR->update();

	if(BOWSER)
		BOWSER->update();

	CRACK->update();

	updateKickUp();

	PLAYER->draw();

	FIREBALL[0]->update();
	FIREBALL[1]->update();

	GLIDDER->update();

	SCORE->update();

	drawInGameStats();

#ifndef __unix__
	if(NoFrameCheck)
		StringColor(screen,0,0,FRAMECHAR,14605935);
#endif

	if(keys[SDLK_h]||keys[SDLK_F1])
	{
		tmp_rect.x=WIDTH/2-125;
		tmp_rect.y=HEIGHT/2-22;
		SDL_UpperBlit(helpscreen,0,screen,&tmp_rect);
	}

	//Gamepad->drawStats();
	

	drawGLscreen();
	//SDL_Flip(screen);

	if(PLAYER->dead)
		died();
	

}



void died()
{
	if(HUD.lifes<=1)
	{
		PLAYSOUND1(S_GAMEOVER);
		gameStartedVirtual = 0;
		showMenu();		
	}
	else
	{
		SDL_FillRect(screen,0,0);
		StringColor(screen,300,230,"Reviving...",-6536532);
		drawGLscreen();

		SDL_Delay(100);

	//	cout<<" ACHTUNG ::::::::::: "<<LEVEL->nowlevel<<endl<<endl<<endl;

		LEVEL->loadLevel(LEVEL->RESTARTLevel,0);
		
		HUD.lifes--;
		PLAYER->invincible = 0;
	}
	PLAYER->dead = 0;
	PLAYER->x_speed = 0;
	PLAYER->y_speed = 0;
	PLAYER->changeStat(0);
}
void framerate()
{
#ifndef __unix__
	    // 检测是否要显示帧率
   		if(NoFrameCheck)
		{		
			framecount++;
			if(framecount>10)
			{
				// 这个函数返回当前高精确度性能计数器的时间值
				QueryPerformanceCounter(&currentticks);
				// 使用公式
				// 所耗时间 = (开始时间点 - 结束时间点) / CPU时钟频率
				// 这时的frame_multi其实就是每个循环所费的时间、由于下面已经设置了帧率限制
				// 所以
  				frame_multi = (double)(currentticks.QuadPart-framedelay.QuadPart)/((double)(tickspersecond.QuadPart*10)/65);
				framecount = 0;
				delete[] FRAMECHAR;
				FRAMECHAR = new char[10];
				// 所谓的帧率就是指这个程序一秒钟刷新了多少次画面的次数、由于事件处理和
				// 这个帧率计算是同时被调用的、所以只需要统计一下这个程序本身调用了几次便
				// 可以得出帧率了
				fps = (int)(65/frame_multi);
				sprintf(FRAMECHAR,"%d FPS",fps);			
				if (frame_multi <= 0)
					frame_multi = 1;
				// 把当前时间赋值给第一次获取的时间、作为下次计算时间的上次时间点
				framedelay = currentticks;
			}
		}
#endif
	// 由于要一直调用stime、所以把stime设置为静态变量
	// stime为开始时间、每次执行完毕后、便把当前时间存为开始时间
	// 以供下一次循环时把现在所存的时间与将来的时间相减
  	static Uint32 stime = 0;	
	// SDL_GetTicks()返回的是当前时间到初始化SDL所经过的时间数、
	// 此处用于计算游戏的帧率
	// 此处为设置游戏的帧率、因为SDL_GetTicks返回的是毫秒数、所以要
	// 使游戏每秒刷新targetfps帧、就必须等待1000/targetfps秒、然后才执行
	// 其它操作、这里假设游戏所执行的所有操作是瞬间进行的、因为真的狠快、
	while(SDL_GetTicks() - stime < 1000/targetfps);
	stime = SDL_GetTicks();
	
}




void exitos(void)
{
	// Write the Keys :

	char szBuffer [MAX_PATH];
	dictionary * d = iniparser_new("megamario.ini");
	/* Create the megamario section (in case of an empty ini file) */
	iniparser_setstr (d, "Mario", NULL);
	sprintf(szBuffer, "%i", pixelation);
	iniparser_setstr (d, "Mario:Pixelation", szBuffer);
	sprintf(szBuffer, "%i", show_blood);
	iniparser_setstr (d, "Mario:Blood", szBuffer);
	sprintf(szBuffer, "%i", nosound);
	iniparser_setstr (d, "Mario:Sound", szBuffer);
	sprintf(szBuffer, "%i", WIDTH);
	iniparser_setstr (d, "Mario:SCREEN_W", szBuffer);
	sprintf(szBuffer, "%i", HEIGHT);
	iniparser_setstr (d, "Mario:SCREEN_H", szBuffer);
	sprintf(szBuffer, "%i", Real_Fullscreen);
	iniparser_setstr (d, "Mario:FULLSCREEN", szBuffer);
	sprintf(szBuffer, "%i", targetfps);
	iniparser_setstr (d, "Mario:Gamespeed", szBuffer);
	if(cheats_enabled)
	{
		sprintf(szBuffer, "%i", cheats_enabled);
		iniparser_setstr (d, "Mario:CHEATS_ENABLED", szBuffer);
	}

	FILE *f = fopen(szIniFile, "w");
	iniparser_dump_ini (d, f);
	fclose(f);
	iniparser_free (d);

	running = 0;
	printf("\n\nbitmaps unloaded.\n\nEXIT \n0 ERRORS\n");
	Mix_CloseAudio();
	unloadGlobalSprites();
	Gamepad->Close();
	delete Gamepad;
	delete LEVEL;
//
//	
//	
// delete GOOMBA;
// delete GOOMBABIG;
// delete TURTLE;
// delete TURTLEFLY;
// delete SPINY;
// delete FIRECIRCLE;
// delete CANNON;
// delete KILLERBLUME;
// delete BRETT;
// delete PLAYER;
// delete BONUS_DYNAMIC;
// delete CRACK;
// delete GLIDDER;
// delete SCORE;
// delete FIRE;
// delete BOWSER;
// delete FIREBALL;
// delete BLOOD_GENERATOR;

//	
	exit(1);
}





