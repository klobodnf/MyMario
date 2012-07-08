/********************************************************************
  Mega Mario SRC
    created:	2005/09/18
	created:	18:9:2005   10:13
	author:		Jens Wellmann (c)
*********************************************************************/

#include "functions.h"
#include <math.h>

#define clip_xmin(surface) surface->clip_rect.x
#define clip_xmax(surface) surface->clip_rect.x+surface->clip_rect.w-1
#define clip_ymin(surface) surface->clip_rect.y
#define clip_ymax(surface) surface->clip_rect.y+surface->clip_rect.h-1


int FilledRectAlpha(SDL_Surface * surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, Uint8 alpha) // Rectangle with Alpha V.1.0
{
    Uint32 Rmask = surface->format->Rmask, Gmask =
	surface->format->Gmask, Bmask = surface->format->Bmask, Amask = surface->format->Amask;
    Uint32 R, G, B, A = 0;
    Sint16 x, y;

    switch (surface->format->BytesPerPixel) {
    case 1:{			/* Assuming 8-bpp */
	    Uint8 *row, *pixel;
	    Uint8 dR, dG, dB;

	    Uint8 sR = surface->format->palette->colors[color].r;
	    Uint8 sG = surface->format->palette->colors[color].g;
	    Uint8 sB = surface->format->palette->colors[color].b;

	    for (y = y1; y <= y2; y++) {
		row = (Uint8 *) surface->pixels + y * surface->pitch;
		for (x = x1; x <= x2; x++) {
		    pixel = row + x;

		    dR = surface->format->palette->colors[*pixel].r;
		    dG = surface->format->palette->colors[*pixel].g;
		    dB = surface->format->palette->colors[*pixel].b;

		    dR = dR + ((sR - dR) * alpha >> 8);
		    dG = dG + ((sG - dG) * alpha >> 8);
		    dB = dB + ((sB - dB) * alpha >> 8);

		    *pixel = SDL_MapRGB(surface->format, dR, dG, dB);
		}
	    }
	}
	break;

    case 2:{			/* Probably 15-bpp or 16-bpp */
	    Uint16 *row, *pixel;
	    Uint32 dR = (color & Rmask), dG = (color & Gmask), dB = (color & Bmask), dA = (color & Amask);

	    for (y = y1; y <= y2; y++) {
		row = (Uint16 *) surface->pixels + y * surface->pitch / 2;
		for (x = x1; x <= x2; x++) {
		    pixel = row + x;

		    R = ((*pixel & Rmask) + ((dR - (*pixel & Rmask)) * alpha >> 8)) & Rmask;
		    G = ((*pixel & Gmask) + ((dG - (*pixel & Gmask)) * alpha >> 8)) & Gmask;
		    B = ((*pixel & Bmask) + ((dB - (*pixel & Bmask)) * alpha >> 8)) & Bmask;
		    if (Amask)
			A = ((*pixel & Amask) + ((dA - (*pixel & Amask)) * alpha >> 8)) & Amask;

		    *pixel = R | G | B | A;
		}
	    }
	}
	break;

    case 3:{			/* Slow 24-bpp mode, usually not used */
	    Uint8 *row, *pix;
	    Uint8 dR, dG, dB, dA;
	    Uint8 rshift8 = surface->format->Rshift / 8;
	    Uint8 gshift8 = surface->format->Gshift / 8;
	    Uint8 bshift8 = surface->format->Bshift / 8;
	    Uint8 ashift8 = surface->format->Ashift / 8;

	    Uint8 sR = (color >> surface->format->Rshift) & 0xff;
	    Uint8 sG = (color >> surface->format->Gshift) & 0xff;
	    Uint8 sB = (color >> surface->format->Bshift) & 0xff;
	    Uint8 sA = (color >> surface->format->Ashift) & 0xff;

	    for (y = y1; y <= y2; y++) {
		row = (Uint8 *) surface->pixels + y * surface->pitch;
		for (x = x1; x <= x2; x++) {
		    pix = row + x * 3;

		    dR = *((pix) + rshift8);
		    dG = *((pix) + gshift8);
		    dB = *((pix) + bshift8);
		    dA = *((pix) + ashift8);

		    dR = dR + ((sR - dR) * alpha >> 8);
		    dG = dG + ((sG - dG) * alpha >> 8);
		    dB = dB + ((sB - dB) * alpha >> 8);
		    dA = dA + ((sA - dA) * alpha >> 8);

		    *((pix) + rshift8) = dR;
		    *((pix) + gshift8) = dG;
		    *((pix) + bshift8) = dB;
		    *((pix) + ashift8) = dA;
		}
	    }

	}
	break;

    case 4:{			/* Probably 32-bpp */
	    Uint32 *row, *pixel;
	    Uint32 dR = (color & Rmask), dG = (color & Gmask), dB = (color & Bmask), dA = (color & Amask);

	    for (y = y1; y <= y2; y++) {
		row = (Uint32 *) surface->pixels + y * surface->pitch / 4;
		for (x = x1; x <= x2; x++) {
		    pixel = row + x;

		    R = ((*pixel & Rmask) + ((dR - (*pixel & Rmask)) * alpha >> 8)) & Rmask;
		    G = ((*pixel & Gmask) + ((dG - (*pixel & Gmask)) * alpha >> 8)) & Gmask;
		    B = ((*pixel & Bmask) + ((dB - (*pixel & Bmask)) * alpha >> 8)) & Bmask;
		    if (Amask)
			A = ((*pixel & Amask) + ((dA - (*pixel & Amask)) * alpha >> 8)) & Amask;

		    *pixel = R | G | B | A;
		}
	    }
	}
	break;
    }

    return (0);
}

int CharacterColor(SDL_Surface * dst, Sint16 x, Sint16 y, char c, Uint32 color) //Buchstabe mit Clipping V.1.0
{
    Sint16 left, right, top, bottom;
    Sint16 x1, y1, x2, y2;
    SDL_Rect srect;
    SDL_Rect drect;
    int result;
    int ix, iy, k;
    unsigned char *charpos;
    unsigned char bits[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    unsigned char *bitpos;
    Uint8 *curpos;
    int forced_redraw;

    /*
     * Get clipping boundary 
     */
    left = dst->clip_rect.x;
    right = dst->clip_rect.x + dst->clip_rect.w - 1;
    top = dst->clip_rect.y;
    bottom = dst->clip_rect.y + dst->clip_rect.h - 1;

    /*
     * Test if bounding box of character is visible 
     */
    x1 = x;
    x2 = x + 8;
    y1 = y;
    y2 = y + 8;
    if ((x1<left) && (x2<left)) {
     return(0);
    } 
    if ((x1>right) && (x2>right)) {
     return(0);
    } 
    if ((y1<top) && (y2<top)) {
     return(0);
    } 
    if ((y1>bottom) && (y2>bottom)) {
     return(0);
    } 

    /*
     * Setup source rectangle for 8x8 bitmap 
     */
    srect.x = 0;
    srect.y = 0;
    srect.w = 8;
    srect.h = 8;

    /*
     * Setup destination rectangle for 8x8 bitmap 
     */
    drect.x = x;
    drect.y = y;
    drect.w = 8;
    drect.h = 8;

    /*
     * Create new 8x8 bitmap surface if not already present 
     */
    if (GfxFont[(unsigned char) c] == NULL) {
	GfxFont[(unsigned char) c] =
	    SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_HWSURFACE | SDL_SRCALPHA, 8, 8,
				 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	/*
	 * Check pointer 
	 */
	if (GfxFont[(unsigned char) c] == NULL) {
	    return (-1);
	}
	/*
	 * Definitely redraw 
	 */
	forced_redraw = 1;
    } else {
	forced_redraw = 0;
    }

    /*
     * Check if color has changed 
     */
    if ((GfxFontColor[(unsigned char) c] != color) || (forced_redraw)) {
	/*
	 * Redraw character 
	 */
	SDL_SetAlpha(GfxFont[(unsigned char) c], SDL_SRCALPHA, 255);
	GfxFontColor[(unsigned char) c] = color;

	/*
	 * Variable setup 
	 */
	k = (unsigned char) c;
	k *= 8;
	charpos = gfxFontdata;
	charpos += k;

	/*
	 * Clear bitmap 
	 */
	curpos = (Uint8 *) GfxFont[(unsigned char) c]->pixels;
	memset(curpos, 0, 8 * 8 * 4);

	/*
	 * Drawing loop 
	 */
	for (iy = 0; iy < 8; iy++) 
	{
	    bitpos = bits;
	    for (ix = 0; ix < 8; ix++)
		{
		if ((*charpos & *bitpos) == *bitpos) 
		{
		    memcpy(curpos, &color, 4);
		}
		bitpos++;
		curpos += 4;
	    }
	    charpos++;
	}
    }

    /*
     * Draw bitmap onto destination surface 
     */
    result = SDL_BlitSurface(GfxFont[(unsigned char) c], &srect, dst, &drect);

    return (result);
}

// Buchstabe mit Clipping und Alpha
int CharacterRGBA(SDL_Surface * dst, Sint16 x, Sint16 y, char c, Uint8 r, Uint8 g, Uint8 b, Uint8 a)// Buchstabe mit Clipping und Alpha V.1.0
{
    /*
     * Draw 
     */
    return (CharacterColor(dst, x, y, c, ((Uint32) r << 24) | ((Uint32) g << 16) | ((Uint32) b << 8) | (Uint32) a));
}
// String
int StringColor(SDL_Surface * dst, Sint16 x, Sint16 y, char *c, Uint32 color) // String V.1.0
{
    int result;
    int i, length;
    char *curchar;
    int curx;

    length = strlen(c);
    curchar = c;
    curx = x;
    result = 0;
    for (i = 0; i < length; i++) {
	result |= CharacterColor(dst, curx, y, *curchar, color);
	curchar++;
	curx += 8;
    }

    return (result);
}
int FastPixelColorNolock(SDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color) // Fast Pixel Set V.1.0
{
    int bpp;
    Uint8 *p;

    /*
     * Honor clipping setup at pixel level 
     */
    if ((x >= clip_xmin(dst)) && (x <= clip_xmax(dst)) && (y >= clip_ymin(dst)) && (y <= clip_ymax(dst))) {

	/*
	 * Get destination format 
	 */
	bpp = dst->format->BytesPerPixel;
	p = (Uint8 *) dst->pixels + y * dst->pitch + x * bpp;
	switch (bpp) {
	case 1:
	    *p = color;
	    break;
	case 2:
	    *(Uint16 *) p = color;
	    break;
	case 3:
	    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		p[0] = (color >> 16) & 0xff;
		p[1] = (color >> 8) & 0xff;
		p[2] = color & 0xff;
	    } else {
		p[0] = color & 0xff;
		p[1] = (color >> 8) & 0xff;
		p[2] = (color >> 16) & 0xff;
	    }
	    break;
	case 4:
	    *(Uint32 *) p = color;
	    break;
	}			/* switch */


    }

    return (0);
}

SDL_Surface *LoadIMG(const char *filename)
{

	SDL_Surface *image,*temp=0;


	cout << "  * load image = " << filename << flush <<endl;

	temp = IMG_Load(filename);
    if(temp == NULL)
    {
        cout << SDL_GetError() << '\n' << flush;
		cout << filename<<" , fehlt." << '\n' << flush <<endl;
		temp = IMG_Load(DATADIR "gfx/editor/error");
		return 0;
    }

	image = SDL_DisplayFormat(temp);
	
	SDL_FreeSurface(temp);

	SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, magenta);

	return image;
}


bool collision(SDL_Rect *r1, SDL_Rect *r2)
{
	if (r1->x + r1->w <= r2->x)
		return false;
	if (r1->x >= r2->x + r2->w)
		return false;
	if (r1->y + r1->h <= r2->y)
		return false;
	if (r1->y >= r2->y + r2->h)
		return false;
	return true;
}
bool pointCollision(SDL_Rect *r1, int x, int y)
{
	if(x<r1->x)
	return false;
	if(x>r1->x+r1->w)
	return false;
	if(y<r1->y)
	return false;
	if(y>r1->y+r1->h)
	return false;
	
	return true;
}


void initMusic()
{
	MusicArchive.allocatecount=0;

	for(int i=0;i<MAXSONGS;i++)
	{
		MusicArchive.song[i] = 0;
		MusicArchive.name[i] = 0;
	}
	MusicArchive.currentsong = new char[50];
}

void _playCurrentMusic()
{
	playMusic(MusicArchive.currentsong);
}
void playMusic(char* filename)
{ 
	if(nosound!=2)
		return;

	static bool errors = 1;
	errors = 1;


	for(int i=0;i<MusicArchive.allocatecount;i++)
	{
		if(!strcmp(MusicArchive.name[i],filename))
		{
			if(MusicArchive.song[i])
			{
				Mix_HaltMusic();
				strcpy(MusicArchive.currentsong,filename);
				Mix_PlayMusic(MusicArchive.song[i],20);
				Mix_HookMusicFinished(_playCurrentMusic);
				errors = 0;
				Mix_VolumeMusic(musicvolume);
			}
		}
	}
	if(errors==0)
		return;

	MusicArchive.name[MusicArchive.allocatecount] = new char[strlen(filename)+3];
	strcpy(MusicArchive.name[MusicArchive.allocatecount],filename);
	strcpy(MusicArchive.currentsong,filename);

	char* tmp = new char[100];
	sprintf(tmp,  DATADIR "sfx/music/%s",filename);
	MusicArchive.song[MusicArchive.allocatecount] = Mix_LoadMUS(tmp);
	delete[](tmp);

	
	Mix_HaltMusic();
	
	Mix_PlayMusic(MusicArchive.song[MusicArchive.allocatecount],1);
	Mix_HookMusicFinished(_playCurrentMusic);
	
	MusicArchive.allocatecount++;
	
	Mix_VolumeMusic(musicvolume);
}





/* Turn a normal gamma value into an appropriate gamma ramp */
void CalculateGamma(double gamma, Uint16 *ramp)
{
	int i, value;

	gamma = 1.0 / gamma;
	for ( i=0; i<256; ++i ) {
		value = (int)(pow((double)i/256.0, gamma)*65535.0 + 0.5);
		if ( value > 65535 ) {
			value = 65535;
		}
		ramp[i] = (Uint16)value;
	}
}





Uint32 getpixel(SDL_Surface *surface, int x, int y)
//Returns da Color of the pixel on da position! Yo , nice!
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) 
	{
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
			else
			return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;
	}
}


SDL_Surface *Pixelate(SDL_Surface *surface,int sizeW,int sizeH)
{
	int W = surface->w;
	int H = surface->h;
	SDL_Surface *temp = SDL_CreateRGBSurface(surface->flags,W,H,32,surface->format->Rmask,surface->format->Gmask,surface->format->Bmask,surface->format->Amask);
	
	SDL_LockSurface(surface); 
	SDL_LockSurface(temp); 
	

	int color=0;
	for(int Y=0;Y<H;Y+=sizeH)
		for(int X=0;X<W;X+=sizeW)
		{			
			color = getpixel(surface,X,Y);
			for(int YY=0;YY<sizeH;YY++)
				for(int XX=0;XX<sizeW;XX++)
				{
					FastPixelColorNolock(temp,X+XX,Y+YY,color);
				}
		}
	SDL_UnlockSurface(temp);
	SDL_UnlockSurface(surface);
	
	return temp;
}

void changescreen()
{
	int WW=2;
	SDL_Surface *I,*screen2;
	
	screen2 = SDL_CreateRGBSurface(screen->flags,WIDTH,HEIGHT,32,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);
	
	SDL_UpperBlit(screen,0,screen2,0);
	for(WW=2;WW<screen->w;WW*=2)
	{
		I = Pixelate(screen2,WW,WW);
		SDL_UpperBlit(I,0,screen,0);
		drawGLscreen();
		SDL_FreeSurface(I);
		SDL_Delay(30);
	}
	SDL_FillRect(screen,0,LEVEL->BGCOLOR);
	PLAYER->update();
	LEVEL->drawBackground();
	LEVEL->drawLevel();		
	FIRE->update();
	for(int i=0;i<GOOMBAcount;i++)
		GOOMBA[i]->update();
	for(int i=0;i<TURTLEcount;i++)
		TURTLE[i]->update();
	for(int i=0;i<SPINYcount;i++)
		SPINY[i]->update();
	for(int i=0;i<FIRECIRCLEcount;i++)
		FIRECIRCLE[i]->update();
	for(int i=0;i<CANNONcount;i++)
		CANNON[i]->update();

	SDL_UpperBlit(screen,0,screen2,0);
	for(WW=WW;WW>=2;WW/=2)
	{
		I = Pixelate(screen2,WW,WW);
		SDL_UpperBlit(I,0,screen,0);
		drawGLscreen();
		SDL_Delay(30);
		SDL_FreeSurface(I);
	}
	SDL_FreeSurface(screen2);
}


void showTip(char* caption,char* l1,char* l2,char* l3,char* l4,char* l5,char* l6)
{
	SDL_LockSurface(screen);
	int rect_W = 0;
	for(int i=0;i<30;i++)
	{	
		FilledRectAlpha(screen,100+rect_W,100+rect_W,WIDTH-(100+rect_W),HEIGHT-(100+rect_W),554334,50);
		rect_W+=5;
	}
	SDL_UnlockSurface(screen);
	
	SDL_Surface *Text_surf;
	SDL_Color COLORUS;
	COLORUS.r = 0;
	COLORUS.g = 0;
	COLORUS.b = 0;
	
	tmp_rect.x=150;
	tmp_rect.y=140;

	for(int i=0;i<strlen(caption);i++)
		if(caption[i]=='_')
			caption[i]=' ';
	for(int i=0;i<strlen(l1);i++)
		if(l1[i]=='_')
			l1[i]=' ';
	for(int i=0;i<strlen(l2);i++)
		if(l2[i]=='_')
			l2[i]=' ';
	for(int i=0;i<strlen(l3);i++)
		if(l3[i]=='_')
			l3[i]=' ';
	for(int i=0;i<strlen(l4);i++)
		if(l4[i]=='_')
			l4[i]=' ';
	for(int i=0;i<strlen(l5);i++)
		if(l5[i]=='_')
			l5[i]=' ';
	for(int i=0;i<strlen(l6);i++)
		if(l6[i]=='_')
			l6[i]=' ';

	Text_surf = TTF_RenderText_Blended(font,caption,COLORUS);
	tmp_rect.y+=20;
	SDL_BlitSurface(Text_surf,0,screen,&tmp_rect);
	Text_surf = TTF_RenderText_Blended(font,l1,COLORUS);
	tmp_rect.y+=40;
	SDL_BlitSurface(Text_surf,0,screen,&tmp_rect);
	Text_surf = TTF_RenderText_Blended(font,l2,COLORUS);
	tmp_rect.y+=20;
	SDL_BlitSurface(Text_surf,0,screen,&tmp_rect);
	Text_surf = TTF_RenderText_Blended(font,l3,COLORUS);
	tmp_rect.y+=20;
	SDL_BlitSurface(Text_surf,0,screen,&tmp_rect);
	Text_surf = TTF_RenderText_Blended(font,l4,COLORUS);
	tmp_rect.y+=20;
	SDL_BlitSurface(Text_surf,0,screen,&tmp_rect);
	Text_surf = TTF_RenderText_Blended(font,l5,COLORUS);
	tmp_rect.y+=20;
	SDL_BlitSurface(Text_surf,0,screen,&tmp_rect);
	Text_surf = TTF_RenderText_Blended(font,l6,COLORUS);
	tmp_rect.y+=20;
	SDL_BlitSurface(Text_surf,0,screen,&tmp_rect);

	drawGLscreen();
	bool ACDC=1;
	while(ACDC)
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exitos();
					break;
				case SDL_KEYDOWN:
					ACDC = 0;
				break;
			}
			Gamepad->Handle_Event();
			if(Gamepad->Button_A||Gamepad->Button_B||Gamepad->Button_Start)
				ACDC = 0;
		}
}



bool playerinited=0;
void resetPlayer()
{
	if(playerinited)
		return;
	
	int xa = PLAYER->x;int ya = PLAYER->y;
	delete(PLAYER);
	PLAYER = new cPlayer(SDLK_a,SDLK_d,SDLK_w,SDLK_s,SDLK_j,SDLK_k,12);

	//PLAYER = new cPlayer(SDLK_LEFT,SDLK_RIGHT,SDLK_UP,SDLK_DOWN,SDLK_LCTRL,SDLK_SPACE,8);
	PLAYER->x = xa;PLAYER->y = ya;
	
	playerinited=1;
}
