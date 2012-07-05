/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/


class cGamepad
{
public:


	void init();
	
	void Close( void );

	bool OpenStick( unsigned int index );

	void CloseStick( void );
	void reset();
	void Handle_Event();

	void drawStats();

	SDL_Joystick *Joystick;

	bool enabled;

	bool left, right, up, down;
	bool Button_A,Button_B,Button_Start;
};
