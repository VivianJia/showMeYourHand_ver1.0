#include "mathlib.h"

/*
	构造旋转矩阵
*/
void angleMatrix(Vector v, Matrix *m){
	//定义中间变量
	float cr,cp,cy,sr,sp,sy;
	float tempAngle;

	/*
		计算roll，pitch，yaw角的cos，sin值，用来构造旋转矩阵
	*/
	//yaw:表示围绕y轴的旋转，但数值是向量在XOZ面上的投影与z负的夹角
	tempAngle = v.z;
	sy = sin(tempAngle);
	cy = cos(tempAngle);
	//pitch:表示围绕x轴的旋转，但数值是向量在YOZ面上的投影与z负的夹角
	tempAngle = v.y;
	sp = sin(tempAngle);
	cp = cos(tempAngle);
	//roll:表示围绕y轴的旋转，但数值是向量在XOY面上的投影与y正的夹角
	tempAngle = v.x;
	sr = sin(tempAngle);
	cr = cos(tempAngle);

	/*
		构造旋转矩阵
	*/
	Vector xBasis(cp*cy,sr*sp*cy+cr*-sy,(cr*sp*cy+-sr*-sy));
	Vector yBasis(cp*sy,sr*sp*sy+cr*cy,(cr*sp*sy+-sr*cy));
	Vector zBasis(-sp,sr*cp,cr*cp);
	//Vector origin(0.0,0.0,0.0);

	m->xBasis = xBasis;
	m->yBasis = yBasis;
	m->zBasis = zBasis;
	/*m->origin = origin;*/
}

/*
	Inverse a matrix
	@param 
	@returns 
*/
void InverseMatrix(Matrix *m_in,Matrix *m_out){
	FloatArray temp;
	float max,mid;
	float ta[4][4],tb[4][4];
	int k;
	//将m_in存放在临时矩阵中
	temp = m_in->toArray4x4();
	for(unsigned int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < 4; j++){
			ta[i][j] = temp[j+i*4];
		}
	}

	temp = m_out->toArray4x4();
	for(unsigned int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < 4; j++){
			tb[i][j] = temp[j+i*4];
		}
	}
	//寻找主元
	for(int i = 0; i < 4; i++) {
		max = ta[i][i];
		k = i;
		for(int j = i + 1; j < 4; j++) {
			if(fabs(ta[j][i]) > fabs(max)){
				max = ta[j][i];
				k = j;
			}
		}
		//如果主元所在行不是第i行,进行行交换
		if(k != i) {
			for(int j = 0; j < 4; j++) {
				mid = ta[i][j];
				ta[i][j] = ta[k][j];
				ta[k][j] = mid;

				mid = tb[i][j];
				tb[i][j] = tb[k][j];
				tb[k][j] = mid;
			}
		}
		
	}
}