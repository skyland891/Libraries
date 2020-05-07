#pragma once

#ifndef _POLY_H_

#define _POLY_H_

#include <stdio.h>

// polynom's type
typedef struct {
	int N; // polynom's degree
	int* factor; // coef-s
}poly_t;

// PolyCreate: creates a polynomial for a given array
poly_t PolyCreate(int* arr, int size);

// PolyDestroy: clears the memory allocated under the polynomial
void PolyDestroy(poly_t* polynomial);

// PolyFromStr: creating a polynomial from a specific type
poly_t PolyFromInt(int N, ...);

// PolyParse: creates a polynomial from formatted string
poly_t PolyParse(const char*  str);

// PolyFormat: creates a string for a given polynomial
char* PolyFormat(poly_t* polynomial);

//PolyRead: reads polynomial from file
poly_t PolyRead(FILE* file);

//PolyWrite: writes polynomial to file
void PolyWrite(poly_t polynomial, FILE* fp);

//PolyAdd: add two polynomials
poly_t PolyAdd(poly_t poly1, poly_t poly2);

// PolySub: subtraction two polynomials
poly_t PolySub(poly_t poly1, poly_t poly2);

//PolyMul: multiplies two polynomials
poly_t PolyMul(poly_t poly1, poly_t poly2);

//PolyMulInt: multiplies polynomials and number(int)
poly_t PolyMulInt(poly_t poly1, int num);

//PolyEvaluate: considers the value of a polynomial at a point
int PolyEvaluate(poly_t polynomial, int x);

// PolyElement: finds the i - th element of the polynomial
int PolyElement(poly_t polynomial, int i);

#endif // !POLY_H_
