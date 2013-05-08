#include "mathlib.h"
/*
	构造旋转矩阵
*/
void rotationMatrix(Vector v, Matrix *m){
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
	Vector v();
}