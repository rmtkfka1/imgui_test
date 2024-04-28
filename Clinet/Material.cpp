#include "pch.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Core.h"
#include "Texture.h"
#include "TableHeap.h"
#include "Shader.h"
void Material::Update()
{
	// CBV ���ε�
	
	core->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	// SRV ���ε�
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		core->GetTableHeap()->SetSRV(_textures[i]->GetCpuHandle(), reg);
	}

	// ���������� ����
	_shader->SetPipelineState();
}
