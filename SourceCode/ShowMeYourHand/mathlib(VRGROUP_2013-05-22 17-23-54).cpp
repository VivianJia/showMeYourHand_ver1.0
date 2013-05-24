#include "mathlib.h"

/*
	������ת����
*/
void angleMatrix(Vector v, Matrix *m){
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
	//��m_in�������ʱ������
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
	//Ѱ����Ԫ
	for(int i = 0; i < 4; i++) {
		max = ta[i][i];
		k = i;
		for(int j = i + 1; j < 4; j++) {
			if(fabs(ta[j][i]) > fabs(max)){
				max = ta[j][i];
				k = j;
			}
		}
		//�����Ԫ�����в��ǵ�i��,�����н���
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