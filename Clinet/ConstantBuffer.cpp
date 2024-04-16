#include "pch.h"
#include "ConstantBuffer.h"
#include "Core.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);

		_cbvBuffer = nullptr;
	}
}



void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// 상수 버퍼는 256 바이트 배수로 만들어야 한다
	// 0 256 512 768
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
	CreateView();
	CreateTableHeap(256);

}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	core->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

void ConstantBuffer::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;

	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;
	desc.NumDescriptors = _elementCount;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	core->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_cbvHeap));

	_cbvHeapBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	_handleIncrementSize = core->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	for (uint32 i= 0; i < _elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle;
		handle.ptr = _cbvHeapBegin.ptr + i * _handleIncrementSize;

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint32>(_elementSize);
		desc.SizeInBytes = _elementSize;

		core->GetDevice()->CreateConstantBufferView(&desc, handle);

	}


}

void ConstantBuffer::CreateTableHeap(uint32 count)
{
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * 3;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	core->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_tableHeap));

	_groupSize = _handleIncrementSize * 3;


}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
	_currentGroupIndex = 0;
}





void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	assert(_currentIndex < _elementCount);

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE _cbvCurrentHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_cbvHeapBegin, _currentIndex * _handleIncrementSize);

	//D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);

	//core->GetCmdList()->SetGraphicsRootConstantBufferView(rootParamIndex, address);

	_currentIndex++;
}



void ConstantBuffer::PushTable(uint32 regi)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = _tableHeap->GetCPUDescriptorHandleForHeapStart();

	destHandle.ptr += _currentGroupIndex * _groupSize;
	destHandle.ptr += regi * _handleIncrementSize;

	uint32 destRange = 1;
	uint32 srcRange = 1;
	core->GetDevice()->CopyDescriptors(1, &destHandle, &destRange, 1, &_cbvCurrentHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	D3D12_GPU_DESCRIPTOR_HANDLE handle = _tableHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	core->GetCmdList()->SetGraphicsRootDescriptorTable(0, handle);
	_currentGroupIndex++;

}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}