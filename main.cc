#include <iostream>
#include <array>

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

template<size_t R, size_t C, typename T = double>
class Matrix{
	std::array<T, R * C> data;

public:
	Matrix<R, C, T>() : data({}) {}

	Matrix<R, C, T>(std::initializer_list<T> list) : data({}){
		std::copy(list.begin(), list.end(), data.begin());
	}

	constexpr size_t getR() const noexcept{
		return R;
	}

	constexpr size_t getC() const noexcept{
		return C;
	}

	T& get(size_t r, size_t c){
		if(r >= R || c >= C){
			std::cerr << "MATRIX DIM MISMATCH\n";
			exit(EXIT_FAILURE);
		}
		return data[r + c * R];
	}
	
	const T& get(size_t r, size_t c) const{
		if(r >= R || c >= C){
			std::cerr << "MATRIX DIM MISMATCH\n";
			exit(EXIT_FAILURE);
		}
		return data[r + c * R];
	}

	template<typename T2>
	Matrix<R, C, T> operator+(const Matrix<R, C, T2>& rhs) const{
		Matrix<R, C, T> ret;
		for(size_t a = 0; a < R * C; ++a){
			ret.data[a] = data[a] + rhs.data[a];
		}
		return ret;
	}

	template<typename T2>
	Matrix<R, C, T> operator-(const Matrix<R, C, T2>& rhs) const{
		Matrix<R, C, T> ret;
		for(size_t a = 0; a < R * C; ++a){
			ret.data[a] = data[a] - rhs.data[a];
		}
		return ret;
	}

	template<size_t R2, size_t C2, typename T2>
	Matrix<R, C2, T> operator*(const Matrix<R2, C2, T2>& rhs) const{
		static_assert(C == R2);
		Matrix<R, C2, T> ret;
		for(int c = 0; c < C2; ++c){
			for(int r = 0; r < R; ++r){
				double dot = 0;
				for(int a = 0; a < C; ++a){
					dot += get(r, a) * rhs.get(a, c);
				}
				ret.set(r, c, dot);
			}
		}
		return ret;
	}

	Matrix<R, C, T> operator*(const T& v){
		Matrix<R, C, T> ret;
		for(int a = 0; a < R * C; ++a){
			ret[a] = data[a] * v;
		}
		return ret;
	}
	
	T& operator[](const size_t& idx){
		return data[idx];
	}

	const T& operator[](const size_t& idx) const{
		return data[idx];
	}

	static Matrix<R, C, T> identity(){
		static_assert(R == C);
		Matrix<R, C, T> ret;
		for(int a = 0; a < R; ++a){
			ret.set(a, a, 1);
		}
		return ret;
	}

	void print() const{
		for(int r = 0; r < R; ++r){
			for(int c = 0; c < C; ++c){
				std::cout << get(r, c) << " ";
			}
			std::cout << "\n";
		}
	}
};

using Vec3 = Matrix<3, 1>;
using Vec4 = Matrix<4, 1>;

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

	while(true){
		quitTest();
		double dt = jayden::dt();
		std::cout << (int)(1. / dt) << " fps\n";
		
		update();
	}
}
