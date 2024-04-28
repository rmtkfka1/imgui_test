#include "pch.h"
#include "TestScript.h"
#include "Transform.h"
void TestScript::Update()
{

	float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime();
	float _speed = 100.0f;

	vec3 pos = GetTransform()->GetLocalPosition();


	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::Q))
	{
		vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::E))
	{
		vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	GetTransform()->SetLocalPosition(pos);

	
}
