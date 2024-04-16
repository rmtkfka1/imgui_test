#include "pch.h"
#include "RootSignature.h"
#include "Core.h"
void RootSignature::Init()
{

	//D3D12_ROOT_PARAMETER param[1];

		//param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	//param[0].Constants.Num32BitValues = 4;
	//param[0].Constants.ShaderRegister = 0;
	//param[0].Constants.RegisterSpace = 0;
	//param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	CD3DX12_ROOT_PARAMETER param[2];

	param[0].InitAsConstants(4, 0);
	param[1].InitAsConstantBufferView(1);


	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	desc.NumParameters = _countof(param);
	desc.pParameters = param;
	desc.NumStaticSamplers = 0;
	desc.pStaticSamplers = NULL;
	desc.Flags = d3dRootSignatureFlags;



	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);

	core->GetDevice();
	core->GetDevice()->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));

}
