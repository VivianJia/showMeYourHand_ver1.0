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
			float m_location[3];								//初始全局坐标
			//float m_relativeLocaltion[3];
			Vector v_relative;								    //相对joint的坐标
			Vector v_absolute;									//保存更新后的全局坐标
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

			Vector vCurrent;
			
			//used for rendering
			int parentIndex;									//父节点索引
			
			Matrix matStaticLocal;
			Matrix matStaticGlobal;
			Matrix matCurrentLocal;
			Matrix matCurrentGlobal;
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
			初始化骨骼
		*/
		void SetupJointMatrices();

		/*
			初始化顶点相对其joint的坐标，相当于为每个顶点找到其绑定的joint
		*/
		void SetupVertices();


		/*
			从索引读取节点
		*/
		void GetJointAt(int i, Joint & joint);

		/*
			将float数组转化为向量
		*/
		Vector floatToVector(const float *buffer) ;

		/*
			按名称找节点索引
		*/
		int findJointByName(char *name);

		/*
			Draw the model.
		*/
		void draw();

		/*
			从Leap获取指尖的全局方位角和全局位置坐标,构造当前全局矩阵
		*/
		Vector getPitch(Vector v);

		/*
			从文件中获取所要的信息
		*/
		Vector getDataFromFile();

		/*
			动作帧
		*/
		void animation(float *pitches, float *rolls, float *yaws);
		/*
			更新顶点的绝对位置
		*/
		void updateVertices();

		/*
			根据顶点的绝对位置重绘顶点位置(索引是不变的)
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
