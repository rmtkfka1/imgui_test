#pragma once

using VertexType = VertexTextureNormalTangentBlendData;

struct asBone
{
	string name;
	int32 index = -1;
	int32 parent = -1;

	Matrix transform;

};

struct asMesh
{
	string name;
	aiMesh* mesh;
	vector<VertexType> vertices;
	vector<uint32> indices;

	int32 boneIndex;
	string materialName;

};

struct asMaterial
{
	string name; //이름
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;

	string diffuseFile;		 //파일경로
	string specularFile;     //파일경로
	string normalFile;		 //파일경로
};


