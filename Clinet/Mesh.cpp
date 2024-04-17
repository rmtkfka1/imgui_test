#include "pch.h"
#include "Mesh.h"
#include "Core.h"
#include "ConstantBuffer.h"
#include "TableHeap.h"
#include "Texture.h"
Mesh::Mesh() 
{
}
Mesh::~Mesh()
{
}
void Mesh::Init(vector<Vertex>& vec, vector<uint32>& index)
{
	CreateVertxBuffer(vec);
	CreateIndexBuffer(index);


	test.offset = { 0.0,0.0,0.0,0.0 };
	test2.offset = { 50.0f,0,0,0 };


}

void Mesh::Render()
{

	float dt = TimeManager::GetInstance()->GetDeltaTime();

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::D))
	{
		test.offset.x += 0.3f* dt;
		
	}
	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::A))
	{
		test.offset.x -= 0.3f * dt;
	
	}
	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::W))
	{
		test.offset.y += 0.3f * dt;
		
	}
	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::S))
	{
		test.offset.y -= 0.3f * dt;
	
	}


	core->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	core->GetCmdList()->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	core->GetCmdList()->IASetIndexBuffer(&_indexBufferView);

	core->GetConstantBuffer(CBV_REGISTER::b0)->SetData(&test2, sizeof(test2));
	core->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&test, sizeof(test));

	core->GetTableHeap()->SetSRV(_tex->GetCpuHandle(), SRV_REGISTER::t0);
	core->GetTableHeap()->CommitTable();

	//core->GetCmdList()->SetGraphicsRoot32BitConstants(5, 4, &test2, 0);


	core->GetCmdList()->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}

void Mesh::CreateVertxBuffer(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	core->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	

}

void Mesh::CreateIndexBuffer(vector<uint32>& buffer)
{
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	core->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, buffer.data(), bufferSize);
	_indexBuffer->Unmap(0, nullptr);

	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	_indexBufferView.SizeInBytes = bufferSize;


}
