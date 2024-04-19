#pragma once

class GameObject;

class Scene
{

public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void Update();
	virtual void Render();

	void AddGameObject(shared_ptr<GameObject> object);


protected:
	vector<shared_ptr<GameObject>> _gameObjects;
};

