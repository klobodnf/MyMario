#include "initfiles.h"

#include <Windows.h>

#define SETFILE "settings.ini"

void loadSettings()
{
	cout<<"LOAD SETTINGS"<<endl;
	DEBUG_MODE = 0;

	ifstream ifs(SETFILE, ios::in);

		char *contents = new char[200];
	
	for (int i=0; ifs.getline(contents, 200); i++)
		if ( contents[0] != '#' && strlen(contents) > 2)
		{
			interpreteLine(contents);
		}

	ifs.close();
	delete(contents);
}

void interpreteLine(char *command)
{
	int count = 0;
	int len = 0, i = 0;
	char *str = command;
	
	str += strspn(str," ");
	
	while (*str)
	{
		count++;
		str += strcspn(str," ");
		str += strspn(str," ");
	}
	
	str = command;
	
	char **parts = new char*[ count ];
	
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

	for(i=0;i<count;i++)
		cout<<parts[i]<<" ";
		cout<<endl;
		
	if ( strcmp (parts[0], "DebugMode") == 0)
	{
		if(atoi(parts[1])!=0)DEBUG_MODE = 1;
	}
	else if ( strcmp (parts[0], "framedisplay") == 0)
	{
		if(atoi(parts[1])!=1)NoFrameCheck = 0;
	}
	else if ( strcmp (parts[0], "sound") == 0)
	{
		if(atoi(parts[1])!=1)sound = 0;
	}

		 
	//SCREENFLAGS:
		else if ( strcmp (parts[0], "I_WANT_FULLSCREEN") == 0)
		{
			Screen_Flags = SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_SRCCOLORKEY | SDL_RLEACCEL | SDL_SRCALPHA | SDL_HWSURFACE | SDL_HWSURFACE;
		}
		else if ( strcmp (parts[0], "GIMME_DOUBLEBUF") == 0)
		{
			Screen_Flags |= SDL_DOUBLEBUF;
		}
		else if ( strcmp (parts[0], "USE_COLORKEY") == 0)
		{
			Screen_Flags |= SDL_SRCCOLORKEY;
		}
		else if ( strcmp (parts[0], "I_NEED_MORE_RLESPEED") == 0)
		{
			Screen_Flags |= SDL_RLEACCEL;	
		}
		else if ( strcmp (parts[0], "ASYNCBLIT_ROXXX") == 0)
		{
			Screen_Flags |= SDL_ASYNCBLIT;
		}
		else if ( strcmp (parts[0], "THERE_MUST_BE_SRCALPHA") == 0)
		{
			Screen_Flags |= SDL_SRCALPHA;
		}
		else if ( strcmp (parts[0], "NO_HARDWAREACCEL_WOULD_BE_BAD") == 0)
		{
			Screen_Flags |= SDL_HWACCEL;
		}
		else if ( strcmp (parts[0], "NO_HARDWARESURFACE_WOULD_BE_VERY_BAD") == 0)
		{
			Screen_Flags |= SDL_HWSURFACE;
		}

	
	for (i = 0; i < count ;i++)
		delete(parts[i]);

	delete(parts);
}
