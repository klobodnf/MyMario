/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


int a=0;
#include "Global.h"
#define cam_speed 5


void keyInput(void)
{
	// 传入NULL参数将返回所有已按的按键的一个指针数组、迩可以
	// 遍历这个数组然后检测迩想检测的按键是否已经按下去了
	keys = SDL_GetKeyState(NULL);

	if(Gamepad->Button_Start)
	{	
		showMenu();
	}

	if(keys[SDLK_f])
	{
		delete [] cheatcode;
		cheatcode = new char[10];

		strcpy(cheatcode,"f");		
		keys[SDLK_f] = 0;
	}
	if(keys[SDLK_i])
	{
		if(strlen(cheatcode)<10)
			strcat(cheatcode,"i");	
		keys[SDLK_i] = 0;
	}
	if(keys[SDLK_r])
	{
		if(strlen(cheatcode)<10)
			strcat(cheatcode,"r");
		keys[SDLK_r] = 0;
	}

	// 1、 假如输入fir再输入e、那么会掉一个花出来奖励给玩家、如果再按
	// 会再出现、最多只能出现5朵花
   // 2、假如输入了di再输入e、那么会自杀
	// e就是执行的意思
	if(keys[SDLK_e])
	{
		if(cheats_enabled)
		if(!strcmp(cheatcode,"fir"))
		{
			keys[SDLK_e] = 0;
		
			for(int i=0;i<5;i++)
				if(!BONUS_DYNAMIC[i]->online)
				{
					cheater = 1;
					BONUS_DYNAMIC[i]->init(PLAYER->x/40,(PLAYER->y-80)/40,1);
					break;
				}
		}
		if(!strcmp(cheatcode,"di"))
			PLAYER->die();
	}
	else
	if(keys[SDLK_b])
	{
		delete [] cheatcode;
		cheatcode = new char[10];

		strcpy(cheatcode,"b");		
		keys[SDLK_b] = 0;
	}

	// 如果输入了bi再输入g、就长出蘑菇、和前面长出花是一样道理的
	if(keys[SDLK_g])
	{
	if(cheats_enabled)
		if(!strcmp(cheatcode,"bi"))
		{	keys[SDLK_g] = 0;
			for(int i=0;i<5;i++)
				if(!BONUS_DYNAMIC[i]->online)
				{
					cheater = 1;
					BONUS_DYNAMIC[i]->init(PLAYER->x/40,(PLAYER->y-80)/40,0);
					break;
				}
		}
	}
	else
	if(keys[SDLK_d])
	{
		delete [] cheatcode;
		cheatcode = new char[10];

		strcpy(cheatcode,"d");		
		keys[SDLK_d] = 0;
	}
		else
	if(keys[SDLK_s])
	{
		delete [] cheatcode;
		cheatcode = new char[10];

		strcpy(cheatcode,"s");		
		keys[SDLK_s] = 0;
	}

	// 先输入s再输入l可以实现超级跳
	if(keys[SDLK_l])
	{
	if(cheats_enabled)
		if(!strcmp(cheatcode,"s"))
			PLAYER->changeStat(0);
	}
	if(keys[SDLK_p])
	{
		PLAYSOUND0(S_PAUSE);
		showTip("  Game Paused"," "," ","   The journey goes on"," "," "," ");
	}

	// 如果同时输入5和1、MARIO会飞
	if(cheats_enabled)
	if(keys[SDLK_5]&&keys[SDLK_1])
	{
		cheater = 1;
		PLAYER->y_speed = -5;
	}

	
}
