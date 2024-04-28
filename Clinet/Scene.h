#pragma once

class GameObject;

class Scene
{

public:
	Scene();
	virtual ~Scene();


	virtual void Init();
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();


	void AddGameObject(shared_ptr<GameObject> object);
	vector<shared_ptr<GameObject>>& GetGameObject() { return _gameObjects; }

protected:

	//TODO LIst: Ʈ�������� ������
	vector<shared_ptr<GameObject>> _gameObjects;
};

