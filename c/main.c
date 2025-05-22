#include <stdlib.h>

#include <SDL2/SDL.h>

#define M_PI 3.14159265358979323846
#include <math.h>

#include "linear.h"

typedef unsigned char BYTE;

#ifndef	SCREEN_WIDTH
#define	SCREEN_WIDTH	800
#endif

#ifndef	SCREEN_HEIGHT
#define	SCREEN_HEIGHT	800
#endif

static BYTE* pixels = NULL;
static SDL_Window* jwindow;
static SDL_Renderer* jrenderer;
static SDL_Texture* jtexture;

void init(void){
	pixels = malloc(sizeof(BYTE) * 3 * SCREEN_WIDTH * SCREEN_HEIGHT);

	SDL_Init(SDL_INIT_VIDEO);
	jwindow = SDL_CreateWindow("jayden", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
	jrenderer = SDL_CreateRenderer(jwindow, -1, SDL_RENDERER_ACCELERATED);
	jtexture = SDL_CreateTexture(jrenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void finish(void) __attribute__((noreturn));

void quitTest(){
	static SDL_Event event;
	SDL_PollEvent(&event);
	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q){
		finish();
	}
}

void setPixel(unsigned x, unsigned y, BYTE r, BYTE g, BYTE b){
	if(x >= SCREEN_WIDTH){
		fprintf(stderr, "%s x value out of range %u >= %u\n", __func__, x, SCREEN_WIDTH);
		exit(EXIT_FAILURE);
		return;
	}
	if(y >= SCREEN_HEIGHT){
		fprintf(stderr, "%s y value out of range %u >= %u\n", __func__, y, SCREEN_HEIGHT);
		exit(EXIT_FAILURE);
		return;
	}
	pixels[((SCREEN_HEIGHT - 1 - y) * SCREEN_WIDTH + x) * 3] = r;
	pixels[((SCREEN_HEIGHT - 1 - y) * SCREEN_WIDTH + x) * 3 + 1] = g;
	pixels[((SCREEN_HEIGHT - 1 - y) * SCREEN_WIDTH + x) * 3 + 2] = b;
}

void update(void){
	SDL_UpdateTexture(jtexture, NULL, pixels, 3 * SCREEN_WIDTH);
	SDL_RenderCopy(jrenderer, jtexture, NULL, NULL);
	SDL_RenderPresent(jrenderer);
}

void __attribute__((noreturn)) finish(void){
	SDL_DestroyTexture(jtexture);
	SDL_DestroyRenderer(jrenderer);
	SDL_DestroyWindow(jwindow);
	SDL_Quit();
	free(pixels);
	exit(EXIT_SUCCESS);
}

double dt(void){
	static unsigned long now = 0, last = 0;
	now = SDL_GetPerformanceCounter();
	double time = (now - last) / (double)SDL_GetPerformanceFrequency();
	last = now;
	return time;
}

#define NEAR 1.0
#define FAR 10.0
#define VIEW_ANGLE 90.0

float VIEW_X, VIEW_Y;

void thing(struct Matrix* out, struct Matrix* projmat, struct Matrix* in){
	matMult(out, projmat, in);
	matScale(out, out, 1 / out->data[3]);
	out->data[0] = (out->data[0] + 1) / 2 * SCREEN_WIDTH;
	out->data[1] = (out->data[1] + 1) / 2 * SCREEN_HEIGHT;
}

bool edgeTest(struct Matrix* base, struct Matrix* edge, struct Matrix* test){
	return (test->data[0] - base->data[0]) * (edge->data[1] - base->data[1]) > (test->data[1] - base->data[1]) * (edge->data[0] - base->data[0]);
}

bool inTri(struct Matrix* a, struct Matrix* b, struct Matrix* c, struct Matrix* pt){
	return edgeTest(a, b, pt) && edgeTest(b, c, pt) && edgeTest(c, a, pt);
}

int main(void){
	VIEW_X = tan(VIEW_ANGLE / 180.0 * M_PI / 2) * NEAR;
	VIEW_Y = VIEW_X;
	init();
	struct Matrix a = matNew(4, 1), b = matNew(4, 1), c = matNew(4, 1);
	matInitV4(&a, -1, -1, 2, 1);
	matInitV4(&b, 0, 3, 3, 1);
	matInitV4(&c, 2, -1, 4, 1);

	struct Matrix projmat = matNew(4, 4);
	matInitM4(&projmat,
		NEAR / VIEW_X, 0, 0, 0,
		0, NEAR / VIEW_Y, 0, 0,
		0, 0, FAR / (NEAR - FAR), NEAR * FAR / (NEAR - FAR),
		0, 0, 1, 0
	);
	while(1){
		quitTest();
		double delta = dt();
		printf("%d fps\n", (int)(1. / delta));

		struct Matrix na = matNew(4, 1);
		thing(&na, &projmat, &a);
		struct Matrix nb = matNew(4, 1);
		thing(&nb, &projmat, &b);
		struct Matrix nc = matNew(4, 1);
		thing(&nc, &projmat, &c);

		struct Matrix pt = matNew(4, 1);
		for(int x = 0; x < SCREEN_WIDTH; ++x){
			for(int y = 0; y < SCREEN_HEIGHT; ++y){
				pt.data[0] = x;
				pt.data[1] = y;
				pt.data[2] = 0;
				if(inTri(&na, &nb, &nc, &pt)){
					setPixel(x, y, 255, 255, 255);
				}else{
					setPixel(x, y, 0, 0, 0);
				}
			}
		}

		update();
	}
	return EXIT_SUCCESS;
}
