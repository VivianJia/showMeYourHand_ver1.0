#ifndef MATHLIB_H
#define MATHLIB_H
#include <Leap.h>
#include <math.h>
using namespace Leap; 
/*
	¹¹ÔìÐý×ª¾ØÕó
*/
void angleMatrix(Vector v, Matrix *m);

/*
	Inverse a matrix
	@param matrix to be inversed,inversed matrix
	@returns void
*/
void InverseMatrix(Matrix *m_in,Matrix *m_out);

void VectorIRotate(Vector *vIn, Matrix *mat, Vector *vOut);

void VectorITransform(Vector *vIn, Matrix *mat, Vector *vOut);
void tranverse(Matrix *relativeMatrix);
#endif