#include "pch.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Core.h"
#include "Texture.h"
#include "TableHeap.h"
#include "Shader.h"
void Material::Update()
{
	// CBV 업로드
	
	core->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	// SRV 업로드
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		core->GetTableHeap()->SetSRV(_textures[i]->GetCpuHandle(), reg);
	}

	// 파이프라인 세팅
	_shader->SetPipelineState();
}
