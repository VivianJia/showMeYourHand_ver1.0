#include "mathlib.h"
/*
	������ת����
*/
void rotationMatrix(Vector v, Matrix *m){
	//�����м����
	float cr,cp,cy,sr,sp,sy;
	float tempAngle;

	/*
		����roll��pitch��yaw�ǵ�cos��sinֵ������������ת����
	*/
	//yaw:��ʾΧ��y�����ת������ֵ��������XOZ���ϵ�ͶӰ��z���ļн�
	tempAngle = v.z;
	sy = sin(tempAngle);
	cy = cos(tempAngle);
	//pitch:��ʾΧ��x�����ת������ֵ��������YOZ���ϵ�ͶӰ��z���ļн�
	tempAngle = v.y;
	sp = sin(tempAngle);
	cp = cos(tempAngle);
	//roll:��ʾΧ��y�����ת������ֵ��������XOY���ϵ�ͶӰ��y���ļн�
	tempAngle = v.x;
	sr = sin(tempAngle);
	cr = cos(tempAngle);

	/*
		������ת����
	*/
	Vector v();
}