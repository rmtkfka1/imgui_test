#pragma once

class RootSignature
{
public:

	void Init();
	ComPtr<ID3D12RootSignature> GetRootSignature() { return _rootSignature; }

private:
	ComPtr<ID3D12RootSignature> _rootSignature;
	D3D12_STATIC_SAMPLER_DESC _samplerDesc;


};

