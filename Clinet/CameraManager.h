#pragma once
enum class PROJECTION_TYPE
{
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영
};

struct VPMatrix
{
	Matrix _matView = {};
	Matrix _matProjection = {};
};

class CameraManager
{

public:
	static CameraManager* GetInstance()
	{
		static CameraManager instance;
		return &instance;
	}

	void Update();

	
private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 0.1f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _scale = 1.f;

	VPMatrix vpMatrix;

public:

	static VPMatrix svpMatrix;


};

