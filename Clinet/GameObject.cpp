#include "pch.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Core.h"
#include "TableHeap.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Transform.h"
void GameObject::Init()
{
	_transform = make_shared<Transform>();
}

void GameObject::Update()
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::W))
	{
		vec3 pos  =_transform->GetLocalPosition();
		pos.z += 0.3f * dt;
		_transform->SetLocalPosition(pos);
	}


	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::S))
	{
		vec3 pos = _transform->GetLocalPosition();
		pos.z -= 0.3f * dt;
		_transform->SetLocalPosition(pos);
	}

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::D))
	{
		vec3 pos = _transform->GetLocalPosition();
		pos.x += 0.3f * dt;
		_transform->SetLocalPosition(pos);
	}

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::A))
	{
		vec3 pos = _transform->GetLocalPosition();
		pos.x -= 0.3f * dt;
		_transform->SetLocalPosition(pos);
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

	_transform->Update();
	core->GetTableHeap()->BindTexture(_texture->GetCpuHandle(), SRV_REGISTER::t0);

	core->GetTableHeap()->CommitTable();
	core->GetCmdList()->DrawIndexedInstanced(_mesh->GetIndexCount(), 1, 0, 0, 0);

}
