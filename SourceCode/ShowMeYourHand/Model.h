/*
	Model.h

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#ifndef MODEL_H
#define MODEL_H
#include<leap.h>
typedef unsigned char byte;
typedef unsigned short word;
using namespace Leap;
class Model
{
	public:
		//	Mesh
		struct Mesh
		{
			int m_materialIndex;
			int m_numTriangles;
			int *m_pTriangleIndices;
		};

		//	Material properties
		struct Material
		{
			float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
			float m_shininess;
			GLuint m_texture;
			char *m_pTextureFilename;
		};

		//	Triangle structure
		struct Triangle
		{
			float m_vertexNormals[3][3];
			float m_s[3], m_t[3];
			int m_vertexIndices[3];
		};

		//	Vertex structure
		struct Vertex
		{
			char m_boneID;										// for skeletal animation
			float m_location[3];								//��ʼȫ������
			//float m_relativeLocaltion[3];
			Vector v_relative;								    //���joint������
			Vector v_absolute;									//������º��ȫ������
		};
		//֡�ṹ����
		struct KeyFrame;
		//Joint information for this app
		struct Joint 
		{
			byte m_flags;
			char m_name[32];															//��Joint������	
			char m_parentName[32];														//����������
			
			float m_rotation[3];														//��ʼ��ת����
			float m_translation[3];														//��ʼƽ������
			
			word m_numRotationKeyframes;												//��ת֡������
			word m_numTranslationKeyframes;												//ƽ��֡������
			//KeyFrame *m_rotationKeyframes;											//���е���ת֡
			//KeyFrame *m_TranslationKeyframes;											//���е�ƽ��֡

			Vector vCurrent;
			
			//used for rendering
			int parentIndex;									//���ڵ�����
			
			Matrix matStaticLocal;
			Matrix matStaticGlobal;
			Matrix matCurrentLocal;
			Matrix matCurrentGlobal;
		};

		struct KeyFrame 
		{
			float m_fTime;										//֡������ʱ�䣬��λΪ��
			float m_fParam[3];									//ƽ��ֵ����ŷ����
		};

	public:
		/*	Constructor. */
		Model();

		/*	Destructor. */
		virtual ~Model();

		/*	
			Load the model data into the private variables. 
				filename			Model filename
		*/
		virtual bool loadModelData( const char *filename ) = 0;

		/*
			��ʼ������
		*/
		void SetupJointMatrices();

		/*
			��ʼ�����������joint�����꣬�൱��Ϊÿ�������ҵ���󶨵�joint
		*/
		void SetupVertices();


		/*
			��������ȡ�ڵ�
		*/
		void GetJointAt(int i, Joint & joint);

		/*
			��float����ת��Ϊ����
		*/
		Vector floatToVector(const float *buffer) ;

		/*
			�������ҽڵ�����
		*/
		int findJointByName(char *name);

		/*
			Draw the model.
		*/
		void draw();

		/*
			��Leap��ȡָ���ȫ�ַ�λ�Ǻ�ȫ��λ������,���쵱ǰȫ�־���
		*/
		Vector getPitch(Vector v);

		/*
			���ļ��л�ȡ��Ҫ����Ϣ
		*/
		Vector getDataFromFile();

		/*
			����֡
		*/
		void animation(float *pitches, float *rolls, float *yaws);
		/*
			���¶���ľ���λ��
		*/
		void updateVertices();

		/*
			���ݶ���ľ���λ���ػ涥��λ��(�����ǲ����)
		*/
		void reDraw();

		/*
			Called if OpenGL context was lost and we need to reload textures, display lists, etc.
		*/
		void  reloadTextures ();

		int flag;

	protected:
		//	Meshes used
		int m_numMeshes;
		Mesh *m_pMeshes;

		//	Materials used
		int m_numMaterials;
		Material *m_pMaterials;

		//	Triangles used
		int m_numTriangles;
		Triangle *m_pTriangles;

		//	Vertices Used 
		int m_numVertices;
		Vertex *m_pVertices;

		//  Joint used
		int m_numJoints;
		Joint *m_pJoints;

		//  Keyframe used
		int m_numKeyframes;
		KeyFrame *m_pKeyframes;

};

#endif 
