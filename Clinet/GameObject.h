#pragma once

class Component;
class MonoBehaviour;
class MeshRenderer;
class Camera;

#include "Component.h"

class GameObject : public enable_shared_from_this<GameObject>
{

public:
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Transform> GetTransform();

	void AddComponent(shared_ptr<Component> component);
	
private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;

};

