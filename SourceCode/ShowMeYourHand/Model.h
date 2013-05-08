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
			float m_location[3];
		};
		//帧结构声明
		struct KeyFrame;
		//Joint information for this app
		struct Joint 
		{
			byte m_flags;
			char m_name[32];															//本Joint的名字	
			char m_parentName[32];														//父级的名字
			
			float m_rotation[3];														//初始旋转向量
			float m_translation[3];														//初始平移向量
			
			word m_numRotationKeyframes;												//旋转帧的数量
			word m_numTranslationKeyframes;												//平移帧的数量
			//KeyFrame *m_rotationKeyframes;											//所有的旋转帧
			//KeyFrame *m_TranslationKeyframes;											//所有的平移帧

			//20 joints from joint1~joint21(skip joint13)
			
			//used for rendering
			int parentIndex;									//父节点索引
			
			//float matStaticLocal[4][4];							//初始骨骼局部矩阵
			//float matStaticGlobal[4][4];						//初始骨骼全局矩阵
			//float matCurrentLocal[4][4];						//当前局部矩阵
			//float matCurrentGlobal[4][4];						//当前全局矩阵

			Leap::Matrix matStaticLocal;
			Leap::Matrix matStaticGlobal;
			Leap::Matrix matCurrentLocal;
			Leap::Matrix matCurrentGlobal;
		};

		struct KeyFrame 
		{
			float m_fTime;										//帧所处的时间，单位为秒
			float m_fParam[3];									//平移值或者欧拉角
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
			初始化骨骼和绑定的顶点
		*/
		void SetupJointMatrices(Model *pModel);

		/*
			从索引读取节点
		*/
		void GetJointAt(int i, Joint & joint);

		/*
			将float数组转化为向量
		*/
		//Vector floatToVector(const float *buffer) ;
		/*
			Draw the model.
		*/
		void draw();

		/*
			Called if OpenGL context was lost and we need to reload textures, display lists, etc.
		*/
		void  reloadTextures ();

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

		//Joint used
		int m_numJoints;
		Joint *m_pJoints;

		//Keyframe used
		int m_numKeyframes;
		KeyFrame *m_pKeyframes;
};

#endif // ndef MODEL_H
