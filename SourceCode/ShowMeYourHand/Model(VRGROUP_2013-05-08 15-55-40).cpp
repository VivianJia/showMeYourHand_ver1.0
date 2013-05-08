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
#include "LeapMath.h"


#include "Model.h"
#include "Lesson31.h"

using namespace Leap;

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
			// Material properties?
			glDisable( GL_TEXTURE_2D );
		}

		glBegin( GL_TRIANGLES );
		{
			for ( int j = 0; j < m_pMeshes[i].m_numTriangles; j++ )
			{
				int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
				const Triangle* pTri = &m_pTriangles[triangleIndex];
				//�Բ�ͬ�Ĺؽڿ��Ʋ��ֽ�����ɫ���ٶ�����Ĺ������Ʋ�ͬ���ֵĵ�
				//���ҳ���ͬ�Ĳ��ֲ����кϲ�����ɫ��ͬʱ������Щ��
				//if(j < 90){
				//	glColor3f(1.0f, 0.0f, 0.0f);
				//}
				//else if(j >= 90 && j < 100){
				//	glColor3f(1.0f, 0.89f, 0.76f);
				//}
				glColor3f(1.0f, 0.89f, 0.76f);
				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];

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
Vector floatToVector(const float *buffer) {
	Vector v(buffer[0],buffer[1],buffer[2]);
	return v;
}

//�󶨹����������ʼ�ֲ����󡢳�ʼȫ�־���
void Model::SetupJointMatrices(Model *model){
	/*Ϊÿ���ؽڵ������ĸ�����
	��ʼ�ֲ����󡢳�ʼȫ�־��󡢵�ǰ�ֲ����󡢵�ǰȫ��
	*/
	int i;
	//step1 �����ʼ�ֲ�����
	for(i = 0; i < model->m_numJoints; i ++) {
		//����ؽ�i�Ĺ����ʼ�ֲ�����
		Matrix relativeMatrix;

		//��������������
		Vector vx(model->m_pJoints[i].m_rotation[0],0.0,0.0);
		Vector vy(0.0,model->m_pJoints[i].m_rotation[1],0.0);
		Vector vz(0.0,0.0,model->m_pJoints[i].m_rotation[2]);
		Vector vt(model->m_pJoints[i].m_translation[0],model->m_pJoints[i].m_translation[1],model->m_pJoints[i].m_translation[2]);
		
		//�趨��ʼ�ֲ�����ĳ�ʼֵ
		relativeMatrix.xBasis = vx;
		relativeMatrix.yBasis = vy;
		relativeMatrix.zBasis = vz;
		relativeMatrix.origin = vt;
		
		/*��ؽ�i�ĳ�ʼȫ�־���*/
		//�ж��ǲ��Ǹ��ڵ�,���ڵ�ĸ��ڵ�������һ���մ�
		if(strcmp(model->m_pJoints[i].parentIndex,"") == 0){
			model->m_pJoints[i].matStaticGlobal[]
		}
		

	}
}

void Model::reloadTextures()
{
	for ( int i = 0; i < m_numMaterials; i++ )
		if ( strlen( m_pMaterials[i].m_pTextureFilename ) > 0 )
			m_pMaterials[i].m_texture = LoadGLTexture( m_pMaterials[i].m_pTextureFilename );
		else
			m_pMaterials[i].m_texture = 0;
}


