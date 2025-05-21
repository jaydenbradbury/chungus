#include <iostream>
#include <array>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	800
#include "secret.hh"
#include "Matrix.hh"

using namespace jayden;

struct ScreenPoint{
	int x, y;
};

struct Color{
	double r, g, b;
};

// Z into screen
// CW

Vec4 topx(Vec4 v){
	Vec4 ret;
	ret[0] = (v[0] + 1) / 2 * SCREEN_WIDTH;
	ret[1] = (v[1] + 1) / 2 * SCREEN_HEIGHT;
	return ret;
}

struct Vertex{
	Vec4 pos;
	Color col;
};

Vertex a{.pos = {100, 100, 0}, .col = {1, 0, 0}};
Vertex b{.pos = {150, 400, 0}, .col = {0, 1, 0}};
Vertex c{.pos = {200, 200, 0}, .col = {0, 0, 1}};

double triArea(Vertex base, Vertex edge, Vertex test){
	return (test.pos[0] - base.pos[0]) * (edge.pos[1] - base.pos[1]) - (test.pos[1] - base.pos[1]) * (edge.pos[0] - base.pos[0]);
}

bool edgeTest(Vertex base, Vertex edge, Vertex test){
	return (test.pos[0] - base.pos[0]) * (edge.pos[1] - base.pos[1]) - (test.pos[1] - base.pos[1]) * (edge.pos[0] - base.pos[0]) > 0;
}

bool inTri(Vertex a, Vertex b, Vertex c, Vertex pt){
	return edgeTest(a, b, pt) && edgeTest(b, c, pt) && edgeTest(c, a, pt);
}

int main(){
	init();


	while(true){
		quitTest();
		double dt = jayden::dt();
		std::cout << (int)(1. / dt) << " fps\n";
	double ABC = triArea(a, b, c);

	for(int x = 0; x < SCREEN_WIDTH; ++x){
		for(int y = 0; y < SCREEN_HEIGHT; ++y){
			Vertex pt{.pos = {(double)x, (double)y, 0}, .col = {0, 0, 0}};

			double ABP = triArea(a, b, pt);
			double BCP = triArea(b, c, pt);
			double CAP = triArea(c, a, pt);
			
			double weightA = BCP / ABC;
			double weightB = CAP / ABC;
			double weightC = ABP / ABC;

		
			if(inTri(a, b, c, pt)){
				double R = a.col.r* weightA + b.col.r * weightB + c.col.r * weightC;
				double G = a.col.g* weightA + b.col.g * weightB + c.col.g * weightC;
				double B = a.col.b* weightA + b.col.b * weightB + c.col.b * weightC;

				setPixel(x, y, R * 255, G * 255, B * 255);
			} else{
				setPixel(x, y, 0, 0, 0);
			}
		}
	}
		
		update();
	}
}
