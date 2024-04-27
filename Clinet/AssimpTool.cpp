#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"



void AssimpTool::Init()
{

	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		//FBX -> MEMORY
		converter->ReadAssetFile(L"House/House.fbx");

		//MEMORY -> CustomData
		converter->ExportMaterialData(L"Houes/House");
		converter->ExportModelData(L"Houes/House");

	}

}

void AssimpTool::Update()
{
}

void AssimpTool::Render()
{
}
