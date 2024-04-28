#pragma once
#include "Component.h"

class Transform : public Component
{
public:


	virtual void Update() override;
	void PushData();

public:
	Transform();
	virtual ~Transform();
	// Parent 기준
	const vec3& GetLocalPosition() { return _localPosition; }
	const vec3& GetLocalRotation() { return _localRotation; }
	const vec3& GetLocalScale() { return _localScale; }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	const vec3& GetWorldPosition() { return _matWorld.Translation(); }

	vec3 GetRight() { return _matWorld.Right(); }
	vec3 GetUp() { return _matWorld.Up(); }
	vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const vec3& position) { _localPosition = position; }
	void SetLocalRotation(const vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const vec3& scale) { _localScale = scale; }

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

private:
	// Parent 기준
	vec3 _localPosition = {0.0f,0.0f,0.0f};
	vec3 _localRotation = {0.0f,0.0f,0.0f};
	vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal = {};
	Matrix _matWorld = {};

	weak_ptr<Transform> _parent;
};

