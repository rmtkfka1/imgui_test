#include "pch.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Core.h"
#include "TableHeap.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Shader.h"
void GameObject::Init()
{
}

void GameObject::Update()
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::D))
	{
		_transform.offset.x += 0.3f * dt;

	}
	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::A))
	{
		_transform.offset.x -= 0.3f * dt;

	}
	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::W))
	{
		_transform.offset.y += 0.3f * dt;

	}
	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::S))
	{
		_transform.offset.y -= 0.3f * dt;

	}
}

void GameObject::Render()
{

	assert(_mesh);
	assert(_texture);


	_shader->SetPipelineState();

	core->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	core->GetCmdList()->IASetVertexBuffers(0, 1, &_mesh->GetVertexView()); // Slot: (0~15)
	core->GetCmdList()->IASetIndexBuffer(&_mesh->GetIndexView());

	core->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&_transform, sizeof(_transform));
	core->GetTableHeap()->SetSRV(_texture->GetCpuHandle(), SRV_REGISTER::t0);

	core->GetTableHeap()->CommitTable();
	core->GetCmdList()->DrawIndexedInstanced(_mesh->GetIndexCount(), 1, 0, 0, 0);

}
