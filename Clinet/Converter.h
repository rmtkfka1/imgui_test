#pragma once
#include "AsTypes.h"
class Converter
{

public:

	Converter();
	~Converter();

public:
	void ReadAssetFile(wstring file); //경로읽기
	void ExportModelData(wstring savePath);
	void ExportMaterialData(wstring savePath);


private:
	void ReadModelData(aiNode* node, int32 index, int32 parent);
	void ReadMeshData(aiNode* node, int32 bone);
	void WriteModelData(wstring finalPath);

	void ReadMaterialData();
	void WriteMaterialData(wstring finalPath);
	string WriteTexture(string saveFolder, string file);

private:
	wstring _assetPath = L"../Resources/Asset/";
	wstring _modelPath = L"../Resources/Model/";
	wstring _texturePath = L"../Resources/Texture/";

private:
	shared_ptr<Assimp::Importer> _importer;

	const aiScene* _scene; // import 한 구조체의 형식 / 루트노드 , 메쉬 , 머테리얼 등등 정보를가지고있음

private:

	vector<shared_ptr<asBone>> _bones;
	vector<shared_ptr<asMaterial>> _materials;
	vector<shared_ptr<asMesh>> _meshes;



};

