#include "pch.h"
#include "TableHeap.h"
#include "Core.h"

void TableHeap::Init(uint32 count)
{
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * (REGISTER_COUNT-1);
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	core->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = core->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * (REGISTER_COUNT-1);
}

void TableHeap::Clear()
{
	_currentGroupIndex = 0;
}

void TableHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	core->GetDevice()->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	core->GetDevice()->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	core->GetCmdList()->SetGraphicsRootDescriptorTable(1, handle);

	_currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint8>(reg));
}


D3D12_CPU_DESCRIPTOR_HANDLE TableHeap::GetCPUHandle(uint32 reg)
{
	assert(reg > 0);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	handle.ptr += (reg-1) * _handleSize;
	return handle;
}