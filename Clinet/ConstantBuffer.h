#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg,uint32 size, uint32 count);

	void Clear();
	void PushData(void* buffer, uint32 size);
	void SetData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);


private:
	void CreateBuffer();
	void CreateView();


private:
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE* _mappedBuffer = nullptr;
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	uint32								_handleIncrementSize = 0;


	uint32					_currentIndex = 0;
	CBV_REGISTER			_reg = {};

};

