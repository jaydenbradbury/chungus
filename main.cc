#include <iostream>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	800
#include "secret.hh"

using namespace jayden;

struct ScreenPoint{
	int x, y;
};

struct Color{
	double r, g, b;
};

struct Position{
	double x, y, z;
};

struct Vertex{
	Position pos;
	Color col;
};

struct Matrix{
	int r, c;
	double* data;

	Matrix(int r, int c) : r(r), c(c){
		data = new double[r * c];
	}

	~Matrix(){
		delete[] data;
	}

	void set(int r, int c, double d){
		if(r >= this->r || c >= this->c){
			return;
		}
		data[r * this->c + c] = d;
	}

	double get(int r, int c){
		if(r >= this->r || c >= this->c){
			return;
		}
		data[r * this->c + c] = d;
	}
};

Vertex a{.pos = {100, 100, 0}, .col = {1, 0, 0}};
Vertex b{.pos = {150, 400, 0}, .col = {0, 1, 0}};
Vertex c{.pos = {200, 200, 0}, .col = {0, 0, 1}};

double triArea(Vertex base, Vertex edge, Vertex test){
	return (test.pos.x - base.pos.x) * (edge.pos.y - base.pos.y) - (test.pos.y - base.pos.y) * (edge.pos.x - base.pos.x);
}

bool edgeTest(Vertex base, Vertex edge, Vertex test){
	return (test.pos.x - base.pos.x) * (edge.pos.y - base.pos.y) - (test.pos.y - base.pos.y) * (edge.pos.x - base.pos.x) > 0;
}

bool inTri(Vertex a, Vertex b, Vertex c, Vertex pt){
	return edgeTest(a, b, pt) && edgeTest(b, c, pt) && edgeTest(c, a, pt);
}

int main(){
	init();
	double ABC = triArea(a, b, c);

	for(int x = 0; x < SCREEN_WIDTH; ++x){
		for(int y = 0; y < SCREEN_HEIGHT; ++y){
			Vertex pt{.pos = {x, y, 0}, .col = {0, 0, 0}};

			double ABP = triArea(a, b, pt);
			double BCP = triArea(b, c, pt);
			double CAP = triArea(c, a, pt);
			
			double weightA = BCP / ABC;
			double weightB = CAP / ABC;
			double weightC = ABP / ABC;

		
			if(inTri(a, b, c, pt)){
				double R = a.col.r* weightA + b.col.r * weightB + c.col.r * weightC;
				double B = a.col.g* weightA + b.col.g * weightB + c.col.g * weightC;
				double G = a.col.b* weightA + b.col.b * weightB + c.col.b * weightC;

				setPixel(x, y, R*255, B*255, G*255);
			} else{
				setPixel(x, y, 0, 0, 0);
			}
		}
	}
	
	while(true){
		quitTest();
		double dt = jayden::dt();
		std::cout << (int)(1. / dt) << " fps\n";
		
		update();
	}
}
