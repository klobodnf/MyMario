
#include "Global.h"


void InitOpenGL( void )
{


	

	// viewport should cover the whole screen
	glViewport( 0, 0, GL_Width, GL_Height );

	// Camera projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	// Set Up An Ortho Screen
	glOrtho( 0, (double)GL_Width, (double)GL_Height, 0, -100, 100 );


	glLoadIdentity();
	// Smooth Shading
	glShadeModel( GL_SMOOTH );
	// Line Smoothing
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

	// clear color
	
	// Depth Buffer Setup
	//glClearDepth( 1.0 );

	glEnable( GL_BLEND );
	// Blending function
	
	// Alpha
	//glEnable( GL_ALPHA_TEST );
	// Alpha function
	//glAlphaFunc( GL_GREATER, 0.1f );
	// Perspective Calculations
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	// back-facing polygons
	glDisable( GL_CULL_FACE );

	
}






unsigned int Get_GLsize( unsigned int size )
{
	unsigned int value = 1;

	while( value < size )
	{
		value <<= 1;
	}

	return value;
}

void drawGLscreen()
{
#ifdef Use_OPENGL

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	
	unsigned int image_num = 0;
	unsigned int width = Get_GLsize( screen->w );
	unsigned int height = Get_GLsize( screen->h );




	// create one texture
	glGenTextures( 1, &image_num );
	// use the generated texture
	glBindTexture( GL_TEXTURE_2D, image_num );
	// affect how this texture is drawn later on
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );



	// copies the sdl surface into the opengl texture
	glPixelStorei( GL_UNPACK_ROW_LENGTH, screen->pitch / screen->format->BytesPerPixel );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, screen->w, screen->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, screen->pixels );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );



	glEnable( GL_TEXTURE_2D );
	// texture id
	glBindTexture( GL_TEXTURE_2D, image_num );

	glScaled(0.003f,-0.004f,0);
	

	// rectangle
	glBegin( GL_QUADS );
	// top left
	glTexCoord2f( 0, 0 );
	glVertex3d( -(int)width / 2, -(int)height / 2, 0 );
	// top right
	glTexCoord2f( 1, 0 );
	glVertex3d( width / 2, -(int)height / 2, 0 );
	// bottom right
	glTexCoord2f( 1, 1 );
	glVertex3d( width / 2, height / 2, 0 );
	// bottom left
	glTexCoord2f( 0, 1 );
	glVertex3d( -(int)width / 2, height / 2, 0 );
	glEnd();

	glDisable( GL_TEXTURE_2D );
	SDL_GL_SwapBuffers();
	
	glDeleteTextures(1,&image_num );

#else

SDL_Flip(screen);

#endif

}
