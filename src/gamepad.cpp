/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


#include "Global.h"


void cGamepad::init()
{
	Joystick = NULL;

	left = 0;
	right = 0;
	up = 0;
	down = 0;
	
	if(SDL_NumJoysticks()<0)
    {
		enabled=0;	
		return;
    }
	

	SDL_JoystickEventState( SDL_ENABLE );


	Joystick = SDL_JoystickOpen( 0 );

	if( !Joystick )
	{
		enabled = 0;
		return;
	}

	enabled = 1;
}

void cGamepad :: Close(  )
{
	if( enabled )
	{
		SDL_JoystickClose( Joystick );
		Joystick = NULL;
		enabled = 0;
	}
}


void cGamepad::reset()
{
	Button_A = 0;
	Button_B = 0;
	Button_Start = 0;
	left = 0;
	right = 0;
	up = 0;
	down = 0;
}

#define Threshold 3000
void cGamepad :: Handle_Event( void )
{
	if( enabled == 0)
	{
		return;
	}
	

	if(event.type == SDL_JOYAXISMOTION)
	{
	
	if(event.jaxis.axis== 1) 
		{
			if( event.jaxis.value < -Threshold ) // Up
			{
				up = 1;
				down = 0;
			}
			else if( event.jaxis.value > Threshold ) //down
			{
			
				up = 0;
				down = 1;
			}
			else
			{
				up = 0;
				down = 0;
			}

		}
	else if(event.jaxis.axis== 0) //  right left
		{ 
			if( event.jaxis.value < -Threshold ) // Left
			{
			
				left = 1;
				right = 0; 
			}
			else if( event.jaxis.value > Threshold ) // Right
			{
				left = 0;
				right = 1; 
			}
			else 
			{		
				left = 0;
				right = 0; 
			}
		}
	}

	if(event.type ==SDL_JOYBUTTONDOWN)
	{

			if( event.jbutton.button == 0 )
			{
				Button_A = 1;
			}
			else if( event.jbutton.button == 1 )
			{
				Button_B = 1;
			}
			else if( event.jbutton.button == 2 )
			{
				Button_Start = 1;
			}
		
	}
	else if(event.type ==SDL_JOYBUTTONUP)	{
	

		if( event.jbutton.button == 0 )
		{
			Button_A = 0;
		}
		if( event.jbutton.button == 1 )
		{
			Button_B = 0;
		}
		if( event.jbutton.button == 2 )
		{
			Button_Start = 0;
		}
	}

	
}

void cGamepad :: drawStats()
{
	
	if(left)
		StringColor(screen,50,200,"LINKS",-134631460);
	if(right)
		StringColor(screen,50,150,"RECHTS",-134631460);
	if(up)
		StringColor(screen,50,100,"HOCH",-134631460);
	if(down)
		StringColor(screen,50,250,"RUNTER",-134631460);
	if(Button_A)
		StringColor(screen,50,300,"AAA",-134631460);
	if(Button_B)
		StringColor(screen,50,350,"BBB",-134631460);
	if(Button_Start)
		StringColor(screen,50,400,"Start",-134631460);
}
