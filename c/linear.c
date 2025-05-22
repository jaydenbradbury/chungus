#include "linear.h"

#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>

#ifdef MATRIX_SAFE_MODE
#define TEST(cond, msg) \
	if(!(cond)){ \
		fprintf(stderr, "%s: %s\n", __func__, msg); \
		exit(EXIT_FAILURE); \
	}
#else
#define TEST(cond, msg)
#endif

struct Matrix matNew(unsigned short r, unsigned short c){
	return (struct Matrix){.r = r, .c = c, .data = malloc(r * c * sizeof(float))};
}

void matRenew(struct Matrix* m, unsigned short r, unsigned short c){
	m->r = r;
	m->c = c;
	free(m->data);
	m->data = malloc(r * c * sizeof(float));
}

bool matCompare(const struct Matrix* restrict a, const struct Matrix* restrict b){
	return a->r == b->r && a->c == b->c;
}

void matCopy(const struct Matrix* restrict src, struct Matrix* restrict dest){
	TEST(matCompare(src, dest), "matrix dimensions not equal")
	memcpy(dest->data, src->data, src->r * src->c * sizeof(float));
}

void matFill(struct Matrix* m, float v){
	for(unsigned short a = 0; a < m->r * m->c; ++a){
		m->data[a] = v;
	}
}

void matAdd(struct Matrix* dest, const struct Matrix* a, const struct Matrix* b){
	TEST(matCompare(a, b) && matCompare(dest, a), "matrix dimensions not equal")
	for(unsigned short i = 0; i < dest->r * dest->c; ++i){
		dest->data[i] = a->data[i] + b->data[i];
	}
}

void matSub(struct Matrix* dest, const struct Matrix* a, const struct Matrix* b){
	TEST(matCompare(a, b) && matCompare(dest, a), "matrix dimensions not equal")
	for(unsigned short i = 0; i < dest->r * dest->c; ++i){
		dest->data[i] = a->data[i] - b->data[i];
	}
}

void matScale(struct Matrix* dest, const struct Matrix* m, float v){
	TEST(matCompare(dest, m), "matrix dimensions not equal")
	for(unsigned short a = 0; a < dest->r * dest->c; ++a){
		dest->data[a] = m->data[a] * v;
	}
}

float matDot(const struct Matrix* a, const struct Matrix* b){
	TEST(matCompare(a, b) && a->c == 1, "dot product matrices have wrong dimensions")
	float v = 0;
	for(unsigned int i = 0; i < a->r; ++i){
		v += a->data[i] * b->data[i];
	}
	return v;
}

void matCross(struct Matrix* restrict dest, const struct Matrix* a, const struct Matrix* b){
	TEST(dest->c == 1 && dest->r == 3 && matCompare(dest, a) && matCompare(a, b), "cross product matricies have wrong dimensions")
	dest->data[0] = a->data[1] * b->data[2] - a->data[2] * b->data[1];
	dest->data[1] = a->data[2] * b->data[0] - a->data[0] * b->data[2];
	dest->data[2] = a->data[0] * b->data[1] - a->data[1] * b->data[0];
}

float matMagnitude(const struct Matrix* m){
	TEST(m->c == 1, "can not take magnitude of multicolumn matrix")
	float mag = 0;
	for(unsigned short a = 0; a < m->r; ++a){
		mag += m->data[a] * m->data[a];
	}
	return sqrtf(mag);
}

void matUnit(struct Matrix* dest, const struct Matrix* m){
	TEST(dest->c == 1 && matCompare(dest, m), "can not unit multicolumn matrix")
	float scale = 1 / matMagnitude(m);
	matScale(dest, m, scale);
}

void matTranspose(struct Matrix* restrict dest, const struct Matrix* restrict m){
	TEST(dest->r == m->c && dest->c == m->r, "transpose destination has wrong dimensions")
	for(unsigned short r = 0; r < dest->r; ++r){
		for(unsigned short c = 0; c < dest->c; ++c){
			dest->data[r * dest->c + c] = m->data[c * m->c + r];
		}
	}
}

void matMult(struct Matrix* restrict dest, const struct Matrix* a, const struct Matrix* b){
	TEST(dest->r == a->r && dest->c == b->c && a->c == b->r, "multiplication matricies have wrong dimensions")
	for(unsigned short c = 0; c < b->c; ++c){
		for(unsigned short r = 0; r < a->r; ++r){
			float dot = 0;
			for(unsigned short i = 0; i < a->c; ++i){
				dot += a->data[r * a->c + i] * b->data[i * b->c + c];
			}
			dest->data[r * dest->c + c] = dot;
		}
	}
}

float matTrace(const struct Matrix* m){
	TEST(m->r == m->c, "can not take trace of non square matrix")
	float trace = 0;
	for(int a = 0; a < m->r; ++a){
		trace += m->data[a * m->c + a];
	}
	return trace;
}

void matRow(struct Matrix* restrict dest, const struct Matrix* restrict m, unsigned short r){
	TEST(dest->r == 1 && dest->c == m->c, "row get destination has wrong dimensions");
	TEST(r < m->r, "row get has invalid index");
	for(int a = 0; a < m->c; ++a){
		dest->data[a] = m->data[r * m->c + a];
	}
}

void matCol(struct Matrix* restrict dest, const struct Matrix* restrict m, unsigned short c){
	TEST(dest->c == 1 && dest->r == m->r, "col get destination has wrong dimensions");
	TEST(c < m->c, "col get has invalid index");
	for(int a = 0; a < m->r; ++a){
		dest->data[a] = m->data[a * m->c + c];
	}
}

void matIdentity(struct Matrix* m){
	TEST(m->r == m->c, "can not make identity matrix from non square matrix")
	matFill(m, 0);
	for(int a = 0; a < m->r; ++a){
		m->data[a * m->c + a] = 1;
	}
}

void matPrint(const struct Matrix* m){
	for(int r = 0; r < m->r; ++r){
		for(int c = 0; c < m->c; ++c){
			printf("%.2f ", m->data[r * m->c + c]);
		}
		puts("");
	}
}

void matInitV3(struct Matrix* m, float x, float y, float z){
	TEST(m->c == 1 && m->r == 3, "init matrix not vector 3")
	m->data[0] = x;
	m->data[1] = y;
	m->data[2] = z;
}

void matInitV4(struct Matrix* m, float x, float y, float z, float w){
	TEST(m->c == 1 && m->r == 4, "init matrix not vector 4")
	m->data[0] = x;
	m->data[1] = y;
	m->data[2] = z;
	m->data[3] = w;
}

void matInitM4(struct Matrix* m,
	float aa, float ab, float ac, float ad,
	float ba, float bb, float bc, float bd,
	float ca, float cb, float cc, float cd,
	float da, float db, float dc, float dd
){
	TEST(m->c == 4 && m->r == 4, "init matrix not 4x4")
	m->data[0] = aa;
	m->data[1] = ab;
	m->data[2] = ac;
	m->data[3] = ad;
	m->data[4] = ba;
	m->data[5] = bb;
	m->data[6] = bc;
	m->data[7] = bd;
	m->data[8] = ca;
	m->data[9] = cb;
	m->data[10] = dc;
	m->data[11] = cd;
	m->data[12] = da;
	m->data[13] = db;
	m->data[14] = dc;
	m->data[15] = dd;
}
