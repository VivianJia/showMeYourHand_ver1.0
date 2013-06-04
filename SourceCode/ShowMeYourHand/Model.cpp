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

	m_numJoints= 0;
	if ( m_pJoints != NULL )
	{
		delete[] m_pJoints;
		m_pJoints = NULL;
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
						为不同的顶点着色,用调色板选择适当的颜色
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

//转换函数
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

//绑定骨骼：计算关节点的初始局部矩阵、初始全局矩阵
void Model::SetupJointMatrices(){
	/*
		为每个关节点设置四个矩阵，每个矩阵都是仿射变换矩阵：
		初始局部矩阵、初始全局矩阵、当前局部矩阵、当前全局矩阵
	*/
	int i;
	for(i = 0; i < m_numJoints; i ++) {
		Joint *joint =& m_pJoints[i];
		/*
			构造初始局部矩阵
		*/
		Matrix relativeMatrix;
		Vector vTemp(joint->m_rotation[0],joint->m_rotation[1],joint->m_rotation[2]);
		
		//设定矩阵的旋转
		angleMatrix(vTemp,&relativeMatrix);

		//设定矩阵的平移
		relativeMatrix.origin = Vector (joint->m_translation[0],joint->m_translation[1],joint->m_translation[2]);

		//为关节点的初始局部矩阵赋值
		joint->matStaticLocal = relativeMatrix;

		/*
			求初始全局矩阵
		*/
		//如果是根节点,则局部矩阵则为全局矩阵
		if(strcmp(joint->m_parentName,"") == 0){
			joint->matStaticGlobal = joint->matStaticLocal;
		}
		else{//如果不是根节点
			//找到父节点
			joint->parentIndex = findJointByName(joint->m_parentName);
			Joint *parentJoint = &m_pJoints[joint->parentIndex];
			joint->matStaticGlobal = parentJoint->matStaticGlobal * joint->matStaticLocal;
		}
	}

	//绑定顶点，初始化顶点的相对坐标
	SetupVertices();
}

//绑定顶点：计算并保存顶点的初始局部坐标、用于后续全局坐标的计算
void Model::SetupVertices(){
	for(int i = 0; i < m_numVertices; i ++) {
		Vertex *mVt;
		mVt = & m_pVertices[i];
		Vector v, v1, v2;
		Matrix matrix, matrix2;
		//如果顶点被绑定到骨骼上
		if(mVt->m_boneID >= 0) {
			//获取顶点绑定的节点的初始全局矩阵
			matrix = m_pJoints[mVt->m_boneID].matStaticGlobal;

			//构造全局坐标向量
			v = Vector (mVt->m_location[0],mVt->m_location[1],mVt->m_location[2]);

			//求该矩阵的逆矩阵
			matrix2 = matrix.rigidInverse();

			////求该顶点的相对坐标，点变换
			//v2 = matrix2.transformDirection(v);
			v2 = matrix2.transformPoint(v);
			/*VectorITransform(&v,&matrix,&v2);*/

			//保存顶点的相对坐标
			mVt->v_relative = v2;
		}
	}
}

Vector Model::getDataFromFile(){
	/*
		现阶段考虑到难度,先不同Leap连接,从文件中读取提前采集的数据
		输入:1.txt
	*/
	//istream myfile;
	return Vector(0.0,0.0,0.0);
}

Vector Model::getPitch(Vector v){
	/*
		每个Joint的近似IK解算;
		pip = (3/2)dip
		mp = (1/2)pip
	*/
	return Vector(0.0,0.0,0.0);
	
}

//animate
void Model::animation (float *pitches, float *rolls, float *yaws){
	/*
		1 实际阶段:当获取到一个Leap的更新帧, 利用帧数据进行刷新
		2 模拟阶段:利用已有的数据进行骨骼的重置
	*/


#pragma region extent
	float fix = 0.0f;
	/*
		wrist
	*/
	Joint *joint1 = &m_pJoints[0];
	joint1->vCurrent.x = pitches[0];
	joint1->vCurrent.y = rolls[0];
	joint1->vCurrent.z = yaws[0];

	/*
		Thumb finger
	*/
	Joint *joint3 = & m_pJoints[2];				//Thumb DIP
	joint3->vCurrent.x = pitches[2];
	//joint6->vCurrent.z = rolls[2] ;
	joint3->vCurrent.y = yaws[2];

	Joint *joint2 = & m_pJoints[1];				//Index MP
	joint2->vCurrent.x = pitches[2] ;
	//joint4->vCurrent.z = rolls[2] ;
	joint2->vCurrent.y = yaws[2] * (1.0/2.0);
	
	/*
		Index finger
	*/
	Joint *joint6 = & m_pJoints[5];				//Index DIP
	joint6->vCurrent.x = pitches[5] ;
	//joint6->vCurrent.z = rolls[5] ;
	joint6->vCurrent.y = -yaws[5] + fix ;

	Joint *joint5 = & m_pJoints[4];				//Index PIP
	joint5->vCurrent.x = pitches[5] ;
	//joint5->vCurrent.z = rolls[5] ;
	joint5->vCurrent.y = -yaws[5] * (3.0/2.0);

	Joint *joint4 = & m_pJoints[3];				//Index MP
	joint4->vCurrent.x = -pitches[5];
	//joint4->vCurrent.z = rolls[5] ;1
	joint4->vCurrent.y = yaws[5]  * (1.0/2.0);

	/*
		Middle finger
	*/
	Joint *joint9 = & m_pJoints[8];				//Middle DIP
	joint9->vCurrent.x = pitches[8];
	//joint9->vCurrent.z = rolls[8];
	joint9->vCurrent.y = yaws[8]+ fix;

	Joint *joint8 = & m_pJoints[7];				//Middle PIP
	joint8->vCurrent.x = pitches[8];
	//joint8->vCurrent.z = rolls[8];
	joint8->vCurrent.y = yaws[8] * (3.0/2.0);

	Joint *joint7 = & m_pJoints[6];				//Middle MP
	joint7->vCurrent.x = pitches[8] ;
	//joint7->vCurrent.z = rolls[8];
	joint7->vCurrent.y = yaws[8] * (1.0/2.0);

	/*
		Ring finger
	*/
	Joint *joint12 = & m_pJoints[11];				//Ring DIP
	joint12->vCurrent.x = pitches[11] ;
	//joint12->vCurrent.z = rolls[11];
	joint12->vCurrent.y = -yaws[11]+ fix;

	Joint *joint11 = & m_pJoints[10];				//Ring PIP
	joint11->vCurrent.x = pitches[11] ;
	//joint11->vCurrent.z = rolls[11];
	joint11->vCurrent.y = -yaws[11] * (3.0/2.0);

	Joint *joint10 = & m_pJoints[9];				//Ring MP
	joint10->vCurrent.x = pitches[11];
	//joint10->vCurrent.z = rolls[11];
	joint10->vCurrent.y = yaws[11] * (1.0/2.0);

	/*
		Little finger
	*/
	Joint *joint15 = & m_pJoints[14];				//Little DIP
	joint15->vCurrent.x = pitches[14];
	//joint15->vCurrent.z = rolls[14];
	joint15->vCurrent.y = -yaws[14] + fix;

	Joint *joint14 = & m_pJoints[13];				//Little PIP
	joint14->vCurrent.x = pitches[14];
	//joint14->vCurrent.z = rolls[14];
	joint14->vCurrent.y = -yaws[14] * (3.0/2.0);
	
	Joint *joint13 = & m_pJoints[12];				//Little MP
	joint13->vCurrent.x = pitches[14];
	//joint13->vCurrent.z = rolls[14];
	joint13->vCurrent.y = yaws[14] * (1.0/2.0);

	for(int i = 0; i < m_numJoints; i++) {
		Matrix motionMatrix;
		Joint *joint = & m_pJoints[i];
		//构造旋转矩阵
		angleMatrix(joint->vCurrent, &motionMatrix);

		//当前局部矩阵 = 初始局部矩阵*动作矩阵，动画矩阵是相对于初始矩阵状态的变换
		joint->matCurrentLocal = joint->matStaticLocal * motionMatrix; 

		/*
			求当前全局矩阵
		*/
		//如果是根节点,则局部矩阵则为全局矩阵
		if(strcmp(joint->m_parentName,"") == 0){
			joint->matCurrentGlobal = joint->matCurrentLocal;
		}
		else{//如果不是根节点
			//找到父节点
			Joint *parentJoint = & m_pJoints[joint->parentIndex];
			joint->matCurrentGlobal = parentJoint->matCurrentGlobal * joint->matCurrentLocal;
		}
	}


#pragma endregion extent
	/*	
		更新顶点位置
		重绘
	*/
	updateVertices();
}

/*
	更新顶点位置
*/
void Model::updateVertices(){
	for(int i = 0; i < m_numVertices; i ++) {
		Vertex *mVt;
		mVt = & m_pVertices[i];
		Vector v, v1, v2;
		Matrix matrix, matrix2;
		//如果顶点被绑定到骨骼上
		if(mVt->m_boneID >= 0) {
			//获取顶点绑定的节点的当前全局矩阵
			matrix = m_pJoints[mVt->m_boneID].matCurrentGlobal;

			//获取相对坐标
			v = mVt->v_relative;
			//变换 当前绝对坐标 = 相对坐标 * 当前全局矩阵
			mVt->v_absolute = matrix.transformPoint(v);
			mVt->m_location[0] = mVt->v_absolute.x;
			mVt->m_location[1] = mVt->v_absolute.y;
			mVt->m_location[2] = mVt->v_absolute.z;
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


