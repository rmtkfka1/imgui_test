#include "pch.h"
#include "CameraManager.h"
#include "Core.h"
#include "ConstantBuffer.h"

VPMatrix CameraManager::svpMatrix;


void CameraManager::Update()
{

	vpMatrix._matView = Matrix::Identity;

	float width = static_cast<float>(core->GetWindowInfo().width);
	float height = static_cast<float>(core->GetWindowInfo().height);


	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		vpMatrix._matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	else
		vpMatrix._matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);

	svpMatrix._matView = vpMatrix._matView;
	svpMatrix._matProjection = vpMatrix._matProjection;

	core->GetConstantBuffer(CBV_REGISTER::b2)->BindTransform(&vpMatrix, sizeof(vpMatrix));
}

