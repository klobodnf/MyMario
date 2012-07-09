/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/



#include "Global.h"

//#include <Math.h>
#define M_PI 3.141592654


#define JUMPS 12
#define STEP_DEFAULT 4


int counta=0;
cPlayer::cPlayer(int LEFT,int RIGHT,int UP,int DOWN,int B,int A,int _speed_jump_height)
{
	
	x = 200;
	y = 3000;
	
	incincible = 0;
	Surface_R[0][0] = LoadIMG(DATADIR "gfx/characters/small/player1r.png");
	Surface_R[0][1] = LoadIMG(DATADIR "gfx/characters/small/player2r.png");
	Surface_R[0][2] = LoadIMG(DATADIR "gfx/characters/small/player3r.png");
	Surface_L[0][0] = LoadIMG(DATADIR "gfx/characters/small/player1l.png");
	Surface_L[0][1] = LoadIMG(DATADIR "gfx/characters/small/player2l.png");
	Surface_L[0][2] = LoadIMG(DATADIR "gfx/characters/small/player3l.png");
	Surface_R[0][3] = LoadIMG(DATADIR "gfx/characters/small/downr.png");
	Surface_L[0][3] = LoadIMG(DATADIR "gfx/characters/small/downl.png");

	Surface_R[1][0] = LoadIMG(DATADIR "gfx/characters/big/player1r.png");
	Surface_R[1][1] = LoadIMG(DATADIR "gfx/characters/big/player2r.png");
	Surface_R[1][2] = LoadIMG(DATADIR "gfx/characters/big/player3r.png");
	Surface_L[1][0] = LoadIMG(DATADIR "gfx/characters/big/player1l.png");
	Surface_L[1][1] = LoadIMG(DATADIR "gfx/characters/big/player2l.png");
	Surface_L[1][2] = LoadIMG(DATADIR "gfx/characters/big/player3l.png");
	Surface_R[1][3] = LoadIMG(DATADIR "gfx/characters/big/downr.png");
	Surface_L[1][3] = LoadIMG(DATADIR "gfx/characters/big/downl.png");

	Surface_R[2][0] = LoadIMG(DATADIR "gfx/characters/fire/player1r.png");
	Surface_R[2][1] = LoadIMG(DATADIR "gfx/characters/fire/player2r.png");
	Surface_R[2][2] = LoadIMG(DATADIR "gfx/characters/fire/player3r.png");
	Surface_L[2][0] = LoadIMG(DATADIR "gfx/characters/fire/player1l.png");
	Surface_L[2][1] = LoadIMG(DATADIR "gfx/characters/fire/player2l.png");
	Surface_L[2][2] = LoadIMG(DATADIR "gfx/characters/fire/player3l.png");
	Surface_R[2][3] = LoadIMG(DATADIR "gfx/characters/fire/downr.png");
	Surface_L[2][3] = LoadIMG(DATADIR "gfx/characters/fire/downl.png");

	K_LEFT = LEFT;
	K_RIGHT = RIGHT;
	K_UP = UP;
	K_DOWN = DOWN;

	speed_jump_height = _speed_jump_height;
	
	K_B = B;
	K_A = A;

	// 刚开始、无敌时间就设置为关闭、也就是1
	STARPOWER_OVER = 1;

   // 所谓的walkcount就是动画帧、在这里
	// 0代表站立动画
	// 1代表小步行走动画
	// 2代表大步行走动画
	// 3代表蹲下动画
	walkcount = 0;
	walkcount2 = 0;

	// 这里的walkadd代表walkcount的趋势、可能是正数也可以是负数、
	// 以此来决定人物下一个动画是大步行走还是站立
	walkadd = 1;
	
	y_speed = 0;
	x_speed = 0;
	JUMPcount = 400;

	dead = 0;

	STARPOWER = 0;
	stat = 0;
	W = 38;
	H = 42;
	Aed=0;
	Bed=0;
	invincible=200;
}
#define CAMRANGE 300
#define CAMRANGE_H 200
void cPlayer::movecam()
{
	if(cam_x>0&&x<cam_x+CAMRANGE)
		cam_x = x-CAMRANGE;
	else if(x>cam_x+WIDTH-CAMRANGE)
		cam_x = x-WIDTH+CAMRANGE;
	
	if(y<cam_y+CAMRANGE_H)
		cam_y = y-CAMRANGE_H;
	else if(y>cam_y+HEIGHT-CAMRANGE_H)
		cam_y = y-HEIGHT+CAMRANGE_H;

	
	if(cam_x<40)
		cam_x = 40;
	else
		if(cam_x>16000 - WIDTH)
			cam_x = 16000 - WIDTH;
	if(cam_y<40)
		cam_y = 40;
	else
		if(cam_y > 4000 - HEIGHT)
			cam_y = 4000 - HEIGHT;
 
}

void cPlayer::update()
{
	// 开启无敌模式、然后便不断的减少时间
	if(invincible)
		invincible--;

	if(x<100)
		x = 100;
	
	if(x>15500)
		x = 15500;

	// 设置死亡边界、超过4000就掛了
	if(y > 4000)
		this->die();

	movecam();

	if(!touch_now)
		down_touch  = LEVEL->posValid(x+3,y+3+H) + LEVEL->posValid(x+W-3,y+3+H);
	else
		down_touch = 1;
	top_touch   = LEVEL->posValid(x+3,y) + LEVEL->posValid(x+W-3,y);
	left_touch  = LEVEL->posValid(x+x_speed-1,y+H/1.25) + LEVEL->posValid(x+x_speed-1,y+H/4);
	right_touch = LEVEL->posValid(x+W+x_speed+1,y+H/1.25) + LEVEL->posValid(x+W+x_speed+1,y+H/4);
	touch_now = 0;

	if(LEVEL->posValid(x,y+3+H)==33||(LEVEL->posValid(x+W,y+3+H)==33))
		if(!right_touch)x+=3;	
	if(LEVEL->posValid(x+20,y+3+H)==32||(LEVEL->posValid(x+W,y+3+H)==32))
		if(!left_touch)x-=3;

	
	if(bonus_activ<0)
		LEVEL->BonusCheck(x+5,y,x+W-5,y);
	else bonus_activ--;

	if(y_speed>=0)
	{
		if(!down_touch)
		{
			y += y_speed;
			if(y_speed<speed_jump_height)
			y_speed+=0.5;
					
		}
		else
		{
			y_speed = 0;
			JUMPcount = 400;
			if(stat==0)
				y = (((int)((y-20)/40))*40)+80-H;
			
		}
	}
	else
	{
		// 如果没碰到地面、则MARIO的所在高度会随垂直速度而变化
		// 但总会不断的下降、这里是模拟重力、直到碰到地面了
		if(!top_touch)
		{
			y += y_speed;
			y_speed+=0.5;
		}
		else y_speed = 0;
	}

	///SHOOOOOOOOOT N RUUUUUUUUUUUUUUUUN
	if(keys[K_B]||Gamepad->Button_B)
	{		
		max_speed = 8;
		if(!Bed&&stat==2)///////NEW FIREBALL!!!!!!!!!!!!!
		{
			#define FIRESPEED 3
			// 方向向右、则火球的速度方向向右、否则向左
			if(dir) 
			{
				if(!FIREBALL[0]->init(x,y+35,FIRESPEED))
					FIREBALL[1]->init(x,y+35,FIRESPEED);
			}
				else
					if(!FIREBALL[0]->init(x,y+35,-FIRESPEED))
						FIREBALL[1]->init(x,y+35,-FIRESPEED);
		}
		Bed=1;
	}
	else
	{max_speed = 4;Bed=0;}
	
	///JUUUUUUUUMP
	if(keys[K_A]||Gamepad->Button_A)
	{
		if(Aed==0&&JUMPcount==400&&down_touch)
		{
			// 这里跳跃的高度将会受到水平速度的影响
			jumpheight = JUMPS+abs(x_speed*1.4);
			PLAYSOUND0(S_JUMP);
			JUMPcount = 0;
		}
		
		if(JUMPcount<jumpheight)
		{
			if(top_touch)
			{
				JUMPcount = 410;
				Aed=0;
			}
			y_speed=-8;		
			JUMPcount++;
		}
		Aed=1;
	}else
	{
		if(Aed)
		{
			JUMPcount = 400;
			Aed=0;
		}
	}

	if(keys[K_DOWN]||Gamepad->down)
	{
		// 如果尙未蹲下、则执行、
		// 人物高度下降、并且把动画帧指向3、也就是蹲下的动画
		if(!KeyDown_down)
		{
			y+=H-34;
			H = 34;
			walkcount = 3;
		}
		
		KeyDown_down = 1;
	}
	else
	{
		// 如果玩家已经释放了蹲下键、则人物高度恢复、重新站起来
		if(KeyDown_down)
		{
			walkcount = 0;
			// 这里还需要检查一下MARIO的状态、是否为小MARIO、如果是
			// 高度不变、因为小MARIO的高度本来就等于大MARIO蹲下的高度
			H = (stat==0) ? 42 : 75;
			y-=H-34;
			KeyDown_down = 0;
		}		
	}	
	if((keys[K_LEFT]||Gamepad->left)&&!KeyDown_down)
	{		
		// 设置方向向左、和速度
		x_speed -= 0.2;
		dir = 0;
		MakeMyWalk();
	}
	else if((keys[K_RIGHT]||Gamepad->right)&&!KeyDown_down)
	{		
		// 设置方向向右
		x_speed += 0.2;
		dir = 1;
		MakeMyWalk();
	}
	else
	{
		max_speed = 4;
		if(x_speed<-0.04)
		{
			x_speed+=0.05;
			if(!keys[K_DOWN]&&!Gamepad->down)MakeMyWalk();
		}
		else
			if(x_speed>0.04)
			{
				x_speed-=0.05;
				if(!keys[K_DOWN]&&!Gamepad->down)MakeMyWalk();
			}
			// 只要MARIO不是蹲下的状态、就站立
			else if(walkcount!=3)walkcount=0;
	}


	if(x_speed>=max_speed)
		x_speed = max_speed;
	else if(x_speed<=-max_speed)
		x_speed = -max_speed;

	
	if(x_speed<0)
			if(!left_touch)
				x += x_speed;
			else x_speed = 0;
		else
			if(!right_touch)
				x += x_speed;
			else x_speed = 0;

			
	rect.x = (int)x - cam_x;
	rect.y = (int)y - cam_y;

	// STARPOWER不仅仅是无敌标志、还是无敌时间、这个值由bonus.cpp那里赋予
	// 当玩家吃到星星的时候、自动增加500个单位的时间
	if(STARPOWER)
	{
		// 如果是6的整数倍、才刷新一新时间
		if(STARPOWER%6 == 0)
			// 显示倒计时的矩形、
			GLIDDER->init(x/40,y/40 +1,0,-10);
		
		STARPOWER--;
		// 如果吃了星星、那么星星时间就是无敌时间
		invincible = STARPOWER;

				
	for(int i=0;i<GOOMBAcount;i++)
		if(!GOOMBA[i]->DEATHcount&&!GOOMBA[i]->dying && collision(&rect,&GOOMBA[i]->rect))
		{			
			GOOMBA[i]->die();
		}
	
	for(int i=0;i<TURTLEFLYcount;i++)
		if(!TURTLEFLY[i]->dead&&!TURTLEFLY[i]->dying && collision(&rect,&TURTLEFLY[i]->rect))
		{			
			TURTLEFLY[i]->die();
		}
	for(int i=0;i<TURTLEcount;i++)
		if(!TURTLE[i]->dead&&!TURTLE[i]->dying && collision(&rect,&TURTLE[i]->rect))
		{			
			TURTLE[i]->die();
		}
	for(int i=0;i<SPINYcount;i++)
		if(!SPINY[i]->dead&&!SPINY[i]->dying && collision(&rect,&SPINY[i]->rect))
		{			
			SPINY[i]->die();
		}
	for(int i=0;i<KILLERBLUMEcount;i++)
		if(!KILLERBLUME[i]->dead&& collision(&rect,&KILLERBLUME[i]->rect))
		{		
			// 吃星星后、挡莪者死、
			KILLERBLUME[i]->dead=1;
			// 碰到食人花、会加分
			SCORE->init(KILLERBLUME[i]->x,KILLERBLUME[i]->y,250);
			// 出血
			BLOOD_GENERATOR->newBlood(KILLERBLUME[i]->x,KILLERBLUME[i]->y);
			PLAYSOUND2(S_KICK);			
		}

		// 只要STARPOWER还在、这个值就一直为0、也就是无敌时间还未结束
		// 如果哪天STARTPOWER已经为0了、那么就自动跳到下面的else语句了
		// 然后STARTPOWER_OVER就为0了
		STARPOWER_OVER = 0;
	}else{
		if(!STARPOWER_OVER)
		{
			if(nosound==2)
				playMusic("megamusic.mid");
			STARPOWER_OVER=1;
		}
	}

	for(int a=0;a<5;a++)
	{
			tmp_rect.x=LEVEL->EXITPOINTS[a].x - cam_x + 5;
			tmp_rect.y=LEVEL->EXITPOINTS[a].y - cam_y;
			tmp_rect.w = 10;
			tmp_rect.h = 20;
			if(LEVEL->EXITPOINTS[a].type == 1 )
				if(!keys[K_DOWN]&&!Gamepad->down)
					continue;

			if(collision(&rect,&tmp_rect))
			{
				PLAYSOUND2(S_PIPE);
				//rect.x = H =tmp_rect.x-10;
				for(int n=0;n<30;n++)
				{
					SDL_FillRect(screen,0,LEVEL->BGCOLOR);
					LEVEL->drawBackground();
					if(LEVEL->EXITPOINTS[a].type == 1)//DOWN
						PLAYER->y+=2;
					else if(LEVEL->EXITPOINTS[a].type==2)//RIGHT
						PLAYER->x+=2;
					else if(LEVEL->EXITPOINTS[a].type==3)//LEFT
						PLAYER->x-=2;
					else if(LEVEL->EXITPOINTS[a].type==4)//UP
						PLAYER->y-=2;

						tmp_rect.x = PLAYER->x-cam_x;
						tmp_rect.y = PLAYER->y-cam_y;
						SDL_UpperBlit(PLAYER->Surface_R[PLAYER->stat][0],0,screen,&tmp_rect);
						LEVEL->drawLevel();
						drawGLscreen();
						SDL_Delay(10);
				}
				LEVEL->loadLevel(LEVEL->EXITPOINTS[a].level,LEVEL->EXITPOINTS[a].startpoint);
			}
	
	}
}

void cPlayer::MakeMyWalk()
{
		walkcount2++;
		if(walkcount2 >= 10 - (int)(abs((int)x_speed)))
		{
			// 如果人物是在大步跑的话、就恢复正常步伐
			if(walkcount==2)
				walkadd = -1;
			else
				// 如果人物是在站立的话、就恢复正常步伐
				if(walkcount ==  0)
					walkadd = 1;

			// 这里为的就是使MARIO恢复正常步伐
			walkcount+=walkadd;
			walkcount2=0;
		}
}
#define DELAYTIME 100
void cPlayer::changeStat(int staty)
{	
	// 假如已经在无敌状态、则吃啥都不会改变MARIO的形态
	if(STARPOWER)
		return;

	y-=40;

	// 如果stat为0那么一定小于或等于staty、这样的话就长大成人
	if(stat<=staty)
	{
		PLAYSOUND1(S_GROW);
		invincible = 0;
	}

	// 否则的话、便是变小了、但是给MARIO他400秒的时间
	else
	{
		invincible = 400;
		PLAYSOUND1(S_SHRINK);
	}

	stat=staty;
	// 判断MARIO是否已经长大了、如果stat为0则是小MARIO
	// 否则的话、变成大MARIO
	if(stat==0)
	{
		W = 38;
		H = 42;
	}
	else
	{
		W = 38;
		H = 75;

		// 长大的话就会搞点特效、不断的有星星冒出来
		GLIDDER->init(this->x/40,this->y/40,5,3);
		GLIDDER->init(this->x/40,this->y/40,0,0);
		GLIDDER->init(this->x/40,this->y/40+1,0,0);
		GLIDDER->init(this->x/40,this->y/40+1,0,0);
		GLIDDER->init(this->x/40,this->y/40+1,0,0);
	}

}


void cPlayer::draw()
{

	if(STARPOWER)
	{
		SDL_LockSurface(screen);
		for(int i=100;i<100+STARPOWER/2;i++)
		{
			for(int x=100;x<110;x++)
				FastPixelColorNolock(screen,i,x,8394261);
		}
		SDL_UnlockSurface(screen);
	}

	if(invincible%3)
	{
//		if(!(invincible%5))
//			BLOOD_GENERATOR->newBlood(x,y);
		
		return;
	}

	if(down_touch)
	{
		// 如果方向向右时渲染右边的图案
		if(dir)
		{
			SDL_UpperBlit(Surface_R[stat][walkcount],0,screen,&rect);
		}
		else
		{
			SDL_UpperBlit(Surface_L[stat][walkcount],0,screen,&rect);		
		}
	}
	else
		if(dir)
		{
			SDL_UpperBlit(Surface_R[stat][2],0,screen,&rect);
		}
		else
		{
			SDL_UpperBlit(Surface_L[stat][2],0,screen,&rect);		
		}

	
}

void cPlayer::die()
{
	if(y < 4000)
	{
	if(invincible)
		return;

	invincible = 30;

	if(incincible)
		return;
	}
	BLOOD_GENERATOR->reset();
	
	PLAYSOUND1(S_DIE);

	LEVEL->drawBackground();
	LEVEL->drawLevel();

	int speed = 8;
	if(show_blood)
		for(int i=0;i<200;i++)
		{
			BLOOD_GENERATOR->oneNewBlood(x,y);
			BLOOD_GENERATOR->oneNewBlood(x,y);
			BLOOD_GENERATOR->oneNewBlood(x,y);
			BLOOD_GENERATOR->oneNewBlood(x,y);
			BLOOD_GENERATOR->update();

			drawGLscreen();
			framerate();
		}
	else speed = 4;


	for(int  i=0;i<HEIGHT/2;i+=speed)
	{
		SDL_LockSurface(screen);
		for(int n=0;n<WIDTH;n++)
		{
			for(int x=i;x<i+speed;x+=rand()%6)
				FastPixelColorNolock(screen,n,x,8394261);
		}
		for( int n=0;n<WIDTH;n++)
		{
			for(int x=HEIGHT-i;x<HEIGHT-i+speed;x+=rand()%6)
				FastPixelColorNolock(screen,n,x,8394261);
		}
		SDL_UnlockSurface(screen);

		drawGLscreen();
		SDL_LockSurface(screen);
		for(int  n=0;n<WIDTH;n++)
		{
			for(int x=i;x<i+speed;x+=rand()%6)
				FastPixelColorNolock(screen,n,x,8394261);
		}
		for( int n=0;n<WIDTH;n++)
		{
			for(int x=HEIGHT-i;x<HEIGHT-i+speed;x+=rand()%6)
				FastPixelColorNolock(screen,n,x,8394261);
		}
		SDL_UnlockSurface(screen);
		drawGLscreen();

		framerate();
	}

	dead = 1;

}

cFireball::cFireball()
{
	FIREBALLS[0] = LoadIMG(DATADIR "gfx/fire1.png");
	FIREBALLS[1] = LoadIMG(DATADIR "gfx/fire2.png");
	FIREBALLS[2] = LoadIMG(DATADIR "gfx/fire3.png");
	FIREBALLS[3] = LoadIMG(DATADIR "gfx/fire4.png");
	online = 0;
}

bool cFireball::init(int _x, int _y,int dir)
{
	if(online)
		return 0;

	PLAYSOUND1(S_FIREBALL);
	x = _x;
	y = _y;
	online = 1;
	x_speed = dir*3;
	y_speed = 3;
	ani = 0;	
	return 1;
}
void cFireball::update()
{
	if(!online)
		return;

	ani+=0.2;
	if(ani>=4)
		ani = 0;

	center_touch   = LEVEL->posValid(x,y+2) + LEVEL->posValid(x+20,y+2);
	down_touch  = LEVEL->posValid(x+10,y+20);

	x += x_speed;
	y += y_speed;

	if(down_touch)
		y_speed=-5;

	y_speed+=0.5;

	rect.x = x - cam_x;
	rect.y = y - cam_y;

	if(center_touch||rect.x<-30||rect.x>WIDTH+50)
		online = 0;

	SDL_UpperBlit(FIREBALLS[(int)ani],0,screen,&rect);
	if(BOWSER!=0)
		if(collision(&rect,&BOWSER->rect))
		{			
			BOWSER->hit();
			online=0;
		}

	for(int i=0;i<GOOMBAcount;i++)
		if(!GOOMBA[i]->DEATHcount&&!GOOMBA[i]->dying && collision(&rect,&GOOMBA[i]->rect))
		{			
			GOOMBA[i]->die();
			online=0;
		}
	
	for(int i=0;i<TURTLEFLYcount;i++)
		if(!TURTLEFLY[i]->dead&&!TURTLEFLY[i]->dying && collision(&rect,&TURTLEFLY[i]->rect))
		{			
			TURTLEFLY[i]->die();
			online=0;
		}
	for(int i=0;i<TURTLEcount;i++)
		if(!TURTLE[i]->dead&&!TURTLE[i]->dying && collision(&rect,&TURTLE[i]->rect))
		{			
			TURTLE[i]->die();
			online=0;
		}
	for(int i=0;i<SPINYcount;i++)
		if(!SPINY[i]->dead&&!SPINY[i]->dying && collision(&rect,&SPINY[i]->rect))
		{			
			SPINY[i]->die();
			online=0;
		}
	for(int i=0;i<KILLERBLUMEcount;i++)
		if(!KILLERBLUME[i]->dead&& collision(&rect,&KILLERBLUME[i]->rect))
		{			
			KILLERBLUME[i]->dead=1;
			SCORE->init(KILLERBLUME[i]->x,KILLERBLUME[i]->y,250);
			BLOOD_GENERATOR->newBlood(KILLERBLUME[i]->x,KILLERBLUME[i]->y);
			online=0;
			PLAYSOUND2(S_KICK);			
		}
}
