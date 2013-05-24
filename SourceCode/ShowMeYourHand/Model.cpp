/*
	Model.cpp

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <leap.h>
#include <LeapMath.h>
#include <cmath>
#include <fstream>

#include "Model.h"
#include "Lesson31.h"
#include "mathlib.h"
#include "glListener.h"
using namespace Leap;
using namespace std;

Model::Model()
{
	m_numMeshes = 0;
	m_pMeshes = NULL;
	m_numMaterials = 0;
	m_pMaterials = NULL;
	m_numTriangles = 0;
	m_pTriangles = NULL;
	m_numVertices = 0;
	m_pVertices = NULL;
	flag = 0;
}

Model::~Model()
{
	int i;
	for ( i = 0; i < m_numMeshes; i++ )
		delete[] m_pMeshes[i].m_pTriangleIndices;
	for ( i = 0; i < m_numMaterials; i++ )
		delete[] m_pMaterials[i].m_pTextureFilename;

	m_numMeshes = 0;
	if ( m_pMeshes != NULL )
	{
		delete[] m_pMeshes;
		m_pMeshes = NULL;
	}

	m_numMaterials = 0;
	if ( m_pMaterials != NULL )
	{
		delete[] m_pMaterials;
		m_pMaterials = NULL;
	}

	m_numTriangles = 0;
	if ( m_pTriangles != NULL )
	{
		delete[] m_pTriangles;
		m_pTriangles = NULL;
	}

	m_numVertices = 0;
	if ( m_pVertices != NULL )
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
}

void Model::draw() 
{
	GLboolean texEnabled = glIsEnabled( GL_TEXTURE_2D );

	// Draw by group
	for ( int i = 0; i < m_numMeshes; i++ )
	{
		int materialIndex = m_pMeshes[i].m_materialIndex;
		if ( materialIndex >= 0 )
		{
			glMaterialfv( GL_FRONT, GL_AMBIENT, m_pMaterials[materialIndex].m_ambient );
			glMaterialfv( GL_FRONT, GL_DIFFUSE, m_pMaterials[materialIndex].m_diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR, m_pMaterials[materialIndex].m_specular );
			glMaterialfv( GL_FRONT, GL_EMISSION, m_pMaterials[materialIndex].m_emissive );
			glMaterialf( GL_FRONT, GL_SHININESS, m_pMaterials[materialIndex].m_shininess );

			if ( m_pMaterials[materialIndex].m_texture > 0 )
			{
				glBindTexture( GL_TEXTURE_2D, m_pMaterials[materialIndex].m_texture );
				glEnable( GL_TEXTURE_2D );
			}
			else
				glDisable( GL_TEXTURE_2D );
		}
		else
		{
			// Material properties
			glDisable( GL_TEXTURE_2D );
		}

		glBegin( GL_TRIANGLES );
		{
			for ( int j = 0; j < m_pMeshes[i].m_numTriangles; j++ )
			{
				int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
				const Triangle* pTri = &m_pTriangles[triangleIndex];
				//glColor3f(1.0f, 0.89f, 0.76f);
				for ( int k = 0; k < 3; k++ )
				{
					/*
						Ϊ��ͬ�Ķ�����ɫ,�õ�ɫ��ѡ���ʵ�����ɫ
					*/
					
					int index = pTri->m_vertexIndices[k];
					if(m_pVertices[index].m_boneID != -1){
						switch(m_pVertices[index].m_boneID){
						case 0:glColor3f(1.0f,0.0f,0.0f);break;
						case 1:glColor3f(1.0f,0.4f,0.7f);break;
						case 2:glColor3f(176.0/255.0f,48.0/255.0f,96.0/255.0f);break;
						case 3:glColor3f(148.0/255.0f,0.0f,211.0/255.0f);break;
						case 4:glColor3f(139.0/255.0f,76.0/255.0f,57.0/255.0f);break;
						case 5:glColor3f(1.0f,165.0/255.0f,0.0f);break;
						case 6:glColor3f(238.0/255.0f, 216.0/255.0f, 174.0/255.0f);break;
						case 7:glColor3f(173.0/255.0f, 255.0/255.0f, 47.0/255.0f);break;
						case 8:glColor3f(1.0f,1.0f,0.0f);break;
						case 9:glColor3f(0.0f,0.0f,1.0f);break;
						case 10:glColor3f(0.0f,1.0f,0.0f);break;
						case 11:glColor3f(1.0f,0.5f,0.7f);break;
						case 12:glColor3f(0.0f, 218.0/255.0f, 185.0/255.0f);break;
						case 13:glColor3f(238.0/255.0f, 213.0/255.0f, 210.0/255.0f);break;
						case 14:glColor3f(205.0/255.0f, 133.0/255.0f, 63.0/255.0f);break;
						/*case 15:glColor3f(1.0f,0.5f,0.7f);break;*/

						default:glColor3f(1.0f,1.0f,1.0f);break;
						}
					}
						
					glNormal3fv( pTri->m_vertexNormals[k] );
					glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
				
					glVertex3fv( m_pVertices[index].m_location );
				}
			}
		}
		glEnd();
	}

	if ( texEnabled )
		glEnable( GL_TEXTURE_2D );
	else
		glDisable( GL_TEXTURE_2D );
}

//ת������
Vector Model::floatToVector(const float *buffer) {
	Vector v(buffer[0],buffer[1],buffer[2]);
	return v;
}

int Model::findJointByName(char *name){
	for(int i = 0; i < m_numJoints; i++) {
		if(strcmp(name,m_pJoints[i].m_name) == 0)
			return i;
	}
}

//�󶨹���������ؽڵ�ĳ�ʼ�ֲ����󡢳�ʼȫ�־���
void Model::SetupJointMatrices(){
	/*
		Ϊÿ���ؽڵ������ĸ�����ÿ�������Ƿ���任����
		��ʼ�ֲ����󡢳�ʼȫ�־��󡢵�ǰ�ֲ����󡢵�ǰȫ�־���
	*/
	int i;
	for(i = 0; i < m_numJoints; i ++) {
		Joint *joint =& m_pJoints[i];
		/*
			�����ʼ�ֲ�����
		*/
		Matrix relativeMatrix;
		Vector vTemp(joint->m_rotation[0],joint->m_rotation[1],joint->m_rotation[2]);
		
		//�趨�������ת
		angleMatrix(vTemp,&relativeMatrix);

		//�趨�����ƽ��
		relativeMatrix.origin = Vector (joint->m_translation[0],joint->m_translation[1],joint->m_translation[2]);

		//Ϊ�ؽڵ�ĳ�ʼ�ֲ�����ֵ
		joint->matStaticLocal = relativeMatrix;

		/*
			���ʼȫ�־���
		*/
		//����Ǹ��ڵ�,��ֲ�������Ϊȫ�־���
		if(strcmp(joint->m_parentName,"") == 0){
			joint->matStaticGlobal = joint->matStaticLocal;
		}
		else{//������Ǹ��ڵ�
			//�ҵ����ڵ�
			joint->parentIndex = findJointByName(joint->m_parentName);
			Joint *parentJoint = &m_pJoints[joint->parentIndex];
			joint->matStaticGlobal = parentJoint->matStaticGlobal * joint->matStaticLocal;
		}
	}

	//�󶨶��㣬��ʼ��������������
	SetupVertices();
}

//�󶨶��㣺���㲢���涥��ĳ�ʼ�ֲ����ꡢ���ں���ȫ������ļ���
void Model::SetupVertices(){
	for(int i = 0; i < m_numVertices; i ++) {
		Vertex *mVt;
		mVt = & m_pVertices[i];
		Vector v, v1, v2;
		Matrix matrix, matrix2;
		//������㱻�󶨵�������
		if(mVt->m_boneID >= 0) {
			//��ȡ����󶨵Ľڵ�ĳ�ʼȫ�־���
			matrix = m_pJoints[mVt->m_boneID].matStaticGlobal;

			//����ȫ����������
			v = Vector (mVt->m_location[0],mVt->m_location[1],mVt->m_location[2]);

			//��þ���������
			/*InverseMatrix(&matrix,  &matrix2);*/
			//m_pJoints[mVt->m_boneID].InverseStaticGlobal = matrix2;
			matrix2 = matrix.rigidInverse();

			////��ö����������꣬��任
			//v2 = matrix2.transformDirection(v);
			v2 = matrix2.transformPoint(v);
			/*VectorITransform(&v,&matrix,&v2);*/

			//���涥����������
			mVt->v_relative = v2;
		}
	}
}

Vector Model::getDataFromFile(){
	/*
		�ֽ׶ο��ǵ��Ѷ�,�Ȳ�ͬLeap����,���ļ��ж�ȡ��ǰ�ɼ�������
		����:1.txt
	*/
	//istream myfile;
	return Vector(0.0,0.0,0.0);
}

Vector Model::getPitch(Vector v){
	/*
		ÿ��Joint�Ľ���IK����;
		pip = (3/2)dip
		mp = (1/2)pip
	*/
	return Vector(0.0,0.0,0.0);
	
}

//animate
void Model::animation (int *pitches,int *rolls,int *yaws){
	/*
		1 ʵ�ʽ׶�:����ȡ��һ��Leap�ĸ���֡, ����֡���ݽ���ˢ��
		2 ģ��׶�:�������е����ݽ��й���������
		  �������ݰ���:joint3,7,11,15,19��rpy��λ��,�����ɶ�
		  ����:���������ÿ���ڵ�ķ�λ��,�����ÿһ�ڵ�ĵ�ǰȫ�־���,��������������㷨.
		  �����˶�ѧ:����DIP��ǰȫ�־���,�𼶼��㸸�ڵ�ĵ�ǰȫ�־�����ô��ǰ�ֲ�����������
		  getDataFromFile();
	*/

	/* 
		3 ��opencv��trackbar���ƻ��������ƽǶ�  
	*/
	int i;
	for(i = 0; i < m_numJoints; i ++) {
		Joint *joint = & m_pJoints[i];
		/*
			��ǰ�������󣬸þ�����ڳ�ʼ�ֲ��任
		*/
		Matrix motionMatrix;
		Vector vTemp;
		//�޸Ĺؽڽ�
		vTemp.y = pitches[i] / 100.0;
		vTemp.x = rolls[i] / 100.0;
		vTemp.z = yaws[i] / 100.0;
		
		//�趨�������ת
		angleMatrix(vTemp,&motionMatrix);

		//�趨�����ƽ��
		//motionMatrix.origin = Vector (joint->m_translation[0],joint->m_translation[1],joint->m_translation[2]);

		//��ǰ�ֲ����� = ��ʼ�ֲ�����*�������󣬳����趨�ı任�ǻ��ڳ�ʼ�ֲ�
		joint->matCurrentLocal =joint->matStaticLocal * motionMatrix;

		/*
			��ǰȫ�־���
		*/
		//����Ǹ��ڵ�,��ֲ�������Ϊȫ�־���
		if(strcmp(joint->m_parentName,"") == 0){
			joint->matCurrentGlobal = joint->matCurrentLocal;
		}
		else{//������Ǹ��ڵ�
			//�ҵ����ڵ�
			Joint *parentJoint = & m_pJoints[joint->parentIndex];
			joint->matCurrentGlobal = parentJoint->matCurrentGlobal * joint->matCurrentLocal;
		}
	}

	/*	
		���¶���λ��
		�ػ�
	*/
	updateVertices();
//	reDraw();

}

/*
	���¶���λ��
*/
void Model::updateVertices(){
	for(int i = 0; i < m_numVertices; i ++) {
		Vertex *mVt;
		mVt = & m_pVertices[i];
		Vector v, v1, v2;
		Matrix matrix, matrix2;
		//������㱻�󶨵�������
		if(mVt->m_boneID >= 0) {
			//��ȡ����󶨵Ľڵ�ĵ�ǰȫ�־���
			matrix = m_pJoints[mVt->m_boneID].matCurrentGlobal;

			//��ȡ�������
			v = mVt->v_relative;
			//�任 ��ǰ�������� = ������� * ��ǰȫ�־���
			mVt->v_absolute = matrix.transformPoint(v);
			mVt->m_location[0] = mVt->v_absolute.x;
			mVt->m_location[1] = mVt->v_absolute.y;
			mVt->m_location[2] = mVt->v_absolute.z;
		}
	}
}

//���º��ػ�
void Model::reDraw() 
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear The Screen And The Depth Buffer
	//glLoadIdentity();												// Reset The Modelview Matrix

	GLboolean texEnabled = glIsEnabled( GL_TEXTURE_2D );
	// Draw by group
	for ( int i = 0; i < m_numMeshes; i++ )
	{
		int materialIndex = m_pMeshes[i].m_materialIndex;
		if ( materialIndex >= 0 )
		{
			glMaterialfv( GL_FRONT, GL_AMBIENT, m_pMaterials[materialIndex].m_ambient );
			glMaterialfv( GL_FRONT, GL_DIFFUSE, m_pMaterials[materialIndex].m_diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR, m_pMaterials[materialIndex].m_specular );
			glMaterialfv( GL_FRONT, GL_EMISSION, m_pMaterials[materialIndex].m_emissive );
			glMaterialf( GL_FRONT, GL_SHININESS, m_pMaterials[materialIndex].m_shininess );

			if ( m_pMaterials[materialIndex].m_texture > 0 )
			{
				glBindTexture( GL_TEXTURE_2D, m_pMaterials[materialIndex].m_texture );
				glEnable( GL_TEXTURE_2D );
			}
			else
				glDisable( GL_TEXTURE_2D );
		}
		else
		{
			// Material properties?
			glDisable( GL_TEXTURE_2D );
		}

		glBegin( GL_TRIANGLES );
		{
			for ( int j = 0; j < m_pMeshes[i].m_numTriangles; j++ )
			{
				int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
				const Triangle* pTri = &m_pTriangles[triangleIndex];
				
				glColor3f(1.0f, 0.89f, 0.76f);
				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];

					glNormal3fv( pTri->m_vertexNormals[k] );
					glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
					//�ػ��ǻ��ڱ任���ȫ�������,����һ����ʱ����,�����������ֵ
					float absolute[3];
					//���������Ǵ�ÿһ֡���������.
					absolute[0] = m_pVertices[index].v_absolute.x;
					absolute[1] = m_pVertices[index].v_absolute.y;
					absolute[2] = m_pVertices[index].v_absolute.z;

					glVertex3fv( absolute );
				}
			}
		}
		glEnd();
	}

	if ( texEnabled )
		glEnable( GL_TEXTURE_2D );
	else
		glDisable( GL_TEXTURE_2D );
}


void Model::reloadTextures()
{
	for ( int i = 0; i < m_numMaterials; i++ )
		if ( strlen( m_pMaterials[i].m_pTextureFilename ) > 0 )
			m_pMaterials[i].m_texture = LoadGLTexture( m_pMaterials[i].m_pTextureFilename );
		else
			m_pMaterials[i].m_texture = 0;
}


