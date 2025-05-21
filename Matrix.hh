#ifndef _MATRIX_HH_
#define _MATRIX_HH_

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

#endif
