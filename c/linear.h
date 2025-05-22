#ifndef _LINEAR_H_
#define _LINEAR_H_

#include <stdbool.h>

#define matSet(m, r, c, v) m->data[r * m->c + c] = v

struct Matrix{
	float* data;
	unsigned short r, c;
};

// creates new matrix with r * c elems
struct Matrix matNew(unsigned short r, unsigned short c);

// creates new matrix at m with r * c elems
void matRenew(struct Matrix* m, unsigned short r, unsigned short c);

// returns if rows and cols of a and b are equal
bool matCompare(const struct Matrix* restrict a, const struct Matrix* restrict b);

// copy data from src to dst
void matCopy(const struct Matrix* restrict src, struct Matrix* restrict dest);

// assign all elems of m to v
void matFill(struct Matrix* m, float v);

// set dest equal to a + b
void matAdd(struct Matrix* dest, const struct Matrix* a, const struct Matrix* b);

// set dest equal to a - b
void matSub(struct Matrix* dest, const struct Matrix* a, const struct Matrix* b);

// set dest equal to m * v
void matScale(struct Matrix* dest, const struct Matrix* m, float v);

// return a dot b
float matDot(const struct Matrix* a, const struct Matrix* b);

// set dest equal to a cross b
void matCross(struct Matrix* restrict dest, const struct Matrix* a, const struct Matrix* b);

// return magnitude of m
float matMagnitude(const struct Matrix* m);

// set dest equal to unit m
void matUnit(struct Matrix* dest, const struct Matrix* m);

// set dest equal to m^T
void matTranspose(struct Matrix* restrict dest, const struct Matrix* restrict m);

// set dest equal to a * b
void matMult(struct Matrix* restrict dest, const struct Matrix* a, const struct Matrix* b);

// return trace of m
float matTrace(const struct Matrix* m);

// set dest equal to row r of m
void matRow(struct Matrix* restrict dest, const struct Matrix* restrict m, unsigned short r);

// set dest equal to col c of m
void matCol(struct Matrix* restrict dest, const struct Matrix* restrict m, unsigned short c);

// set m equal to its identity matrix
void matIdentity(struct Matrix* m);

// prints m
void matPrint(const struct Matrix* m);

void matInitV3(struct Matrix* m, float x, float y, float z);
void matInitV4(struct Matrix* m, float x, float y, float z, float w);

void matInitM4(struct Matrix* m,
	float aa, float ab, float ac, float ad,
	float ba, float bb, float bc, float bd,
	float ca, float cb, float cc, float cd,
	float da, float db, float dc, float dd
);

#endif
