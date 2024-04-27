#include "pch.h"
#include "Converter.h"
#include <filesystem>
#include <iostream>
#include "tinyxml2.h"
#include "FileUtils.h"
Converter::Converter()
{
	_importer = make_shared<Assimp::Importer>();
}

Converter::~Converter()
{

}

void Converter::ReadAssetFile(wstring file)
{
	wstring fileStr = _assetPath + file;

	auto p = std::filesystem::path(fileStr);
	assert(std::filesystem::exists(p));

	_scene = _importer->ReadFile(
		Helper::Tostring(fileStr),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate|
		aiProcess_GenUVCoords|
		aiProcess_GenNormals|
		aiProcess_CalcTangentSpace
	);

	assert(_scene != nullptr);
}

void Converter::ExportModelData(wstring savePath)
{
	wstring finalPath = _modelPath + savePath + L".mesh";

	//SRT 정보
	ReadModelData(_scene->mRootNode, -1, -1);
	WriteModelData(finalPath);

}

void Converter::ExportMaterialData(wstring savePath)
{
	wstring finalPath = _texturePath +savePath+ L".xml";
	ReadMaterialData();
	WriteMaterialData(finalPath);
}

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	//이떄 매트릭스 월드매트릭스가 아님 월드 매트릭스는 우리가 인게임에 배치하는것에따라 달라짐
	//루트를 기준으로 하는것이냐 ?  vs 직속상관(부모를)을 기준으로 하는것이냐? => 직속상관(relative Transform)

	//직속상관 매트릭스
	Matrix transform(node->mTransformation[0]); //4x4 matrix 복사
	bone->transform = transform.Transpose();

	//루트 매트릭스 구하기 이떄 matParent 는 Root Matrix
	Matrix matParent = Matrix::Identity;

	if (parent >= 0)
		matParent = _bones[parent]->transform;

	//루트 매트릭스
	bone->transform = bone->transform * matParent;

	_bones.push_back(bone);


	//MeshData 호출 
	ReadMeshData(node, index);


	///////////재귀적으로 호출/// 트리구조이므로

	for (uint32 i = 0; i < node->mNumChildren; ++i)
	{
		ReadModelData(node->mChildren[i], _bones.size(), index);
	}
}

void Converter::ReadMeshData(aiNode* node, int32 bone)
{
	if (node->mNumMeshes < 1)
		return;

	shared_ptr<asMesh> mesh = make_shared<asMesh>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		uint32 index = node->mMeshes[i]; //인덱스번호를 추출함
		const aiMesh* srcMesh = _scene->mMeshes[index]; //씬에서 들고있는 메쉬에접근

		// Material Name
		const aiMaterial* material = _scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		const uint32 startVertex = mesh->vertices.size();

		for (uint32 v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex
			VertexType vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(vec3));

			// UV
			if (srcMesh->HasTextureCoords(0))
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(vec2));

			// Normal
			if (srcMesh->HasNormals())
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(vec3));

			mesh->vertices.push_back(vertex);
		}

		// Index
		for (uint32 f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (uint32 k = 0; k < face.mNumIndices; k++)
				mesh->indices.push_back(face.mIndices[k] + startVertex);
		}
	}

	_meshes.push_back(mesh);
}

void Converter::WriteModelData(wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	// 폴더가 없으면 만든다.
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	// Bone Data
	file->Write<uint32>(_bones.size());
	for (shared_ptr<asBone>& bone : _bones)
	{
		file->Write<int32>(bone->index);
		file->Write<string>(bone->name);
		file->Write<int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}

	// Mesh Data
	file->Write<uint32>(_meshes.size());
	for (shared_ptr<asMesh>& meshData : _meshes)
	{
		file->Write<string>(meshData->name);
		file->Write<int32>(meshData->boneIndex);
		file->Write<string>(meshData->materialName);

		// Vertex Data
		file->Write<uint32>(meshData->vertices.size());
		file->Write(&meshData->vertices[0], sizeof(VertexType) * meshData->vertices.size());

		// Index Data
		file->Write<uint32>(meshData->indices.size());
		file->Write(&meshData->indices[0], sizeof(uint32) * meshData->indices.size());
	}

}

void Converter::ReadMaterialData()
{

	for (uint32 i = 0; i < _scene->mNumMaterials; ++i)
	{
		aiMaterial* srcMaterial = _scene->mMaterials[i];

		shared_ptr<asMaterial> material = make_shared<asMaterial>();

		material->name = srcMaterial->GetName().C_Str(); //나만의 자료형에 이름저장
		
		aiColor3D color;

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Color(color.r, color.g, color.b, 1.0f);
		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->emissive = Color(color.r, color.g, color.b, 1.0f);


		//아래는 옵션
		aiString file;

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->diffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->specularFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->normalFile = file.C_Str();

		_materials.push_back(material);

	}
}

void Converter::WriteMaterialData(wstring finalPath)
{
	//폴더가없으면 생성
	auto path = std::filesystem::path(finalPath);

	filesystem::create_directory(path.parent_path());
	
	string folder = path.parent_path().string();

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	for (shared_ptr<asMaterial> material : _materials)
	{
		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		tinyxml2::XMLElement* element = nullptr;

		element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->normalFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->emissive.x);
		element->SetAttribute("G", material->emissive.y);
		element->SetAttribute("B", material->emissive.z);
		element->SetAttribute("A", material->emissive.w);
		node->LinkEndChild(element);
	}

	document->SaveFile(Helper::Tostring(finalPath).c_str());


}

string Converter::WriteTexture(string saveFolder, string file)
{

	//옵션//

	string fileName = filesystem::path(file).filename().string();
	string folderName = filesystem::path(saveFolder).filename().string();

	//fbx 파일안에 텍스쳐파일이 포함되어있는경우



	string originStr = (filesystem::path(_assetPath) / folderName / file).string();
	Helper::Replace(originStr, "\\", "/");

	string pathStr = (filesystem::path(saveFolder) / fileName).string();
	Helper::Replace(pathStr, "\\", "/");

	::CopyFileA(originStr.c_str(), pathStr.c_str(), false);


	return fileName;

};
