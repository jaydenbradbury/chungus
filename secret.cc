#include "secret.hh"

#include <iostream>

#include <SDL2/SDL.h>

#ifndef	SCREEN_WIDTH
#define	SCREEN_WIDTH	800
#endif

#ifndef	SCREEN_HEIGHT
#define	SCREEN_HEIGHT	800
#endif

static jayden::BYTE* pixels = nullptr;
static SDL_Window* jwindow;
static SDL_Renderer* jrenderer;
static SDL_Texture* jtexture;

void jayden::init(){
	pixels = new jayden::BYTE[3 * SCREEN_WIDTH * SCREEN_HEIGHT];

	SDL_Init(SDL_INIT_VIDEO);
	jwindow = SDL_CreateWindow("jayden", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
	jrenderer = SDL_CreateRenderer(jwindow, -1, SDL_RENDERER_ACCELERATED);
	jtexture = SDL_CreateTexture(jrenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void jayden::quitTest(){
	static SDL_Event event;
	SDL_PollEvent(&event);
	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q){
		finish();
	}
}

void jayden::setPixel(unsigned x, unsigned y, BYTE r, BYTE g, BYTE b){
	if(x >= SCREEN_WIDTH){
		std::cerr << __func__ << " x value out of range (" << x << " >= " << SCREEN_WIDTH << ")\n";
		return;
	}
	if(y >= SCREEN_HEIGHT){
		std::cerr << __func__ << " y value out of range (" << y << " >= " << SCREEN_HEIGHT << ")\n";
		return;
	}
	pixels[((SCREEN_HEIGHT - 1 - y) * SCREEN_WIDTH + x) * 3] = r;
	pixels[((SCREEN_HEIGHT - 1 - y) * SCREEN_WIDTH + x) * 3 + 1] = g;
	pixels[((SCREEN_HEIGHT - 1 - y) * SCREEN_WIDTH + x) * 3 + 2] = b;
}

void jayden::update(){
	SDL_UpdateTexture(jtexture, NULL, pixels, 3 * SCREEN_WIDTH);
	SDL_RenderCopy(jrenderer, jtexture, NULL, NULL);
	SDL_RenderPresent(jrenderer);
}

[[noreturn]] void jayden::finish(){
	SDL_DestroyTexture(jtexture);
	SDL_DestroyRenderer(jrenderer);
	SDL_DestroyWindow(jwindow);
	SDL_Quit();
	delete pixels;
	exit(EXIT_SUCCESS);
}

double jayden::dt(){
	static unsigned long now = 0, last = 0;
	now = SDL_GetPerformanceCounter();
	double time = (now - last) / (double)SDL_GetPerformanceFrequency();
	last = now;
	return time;
}
