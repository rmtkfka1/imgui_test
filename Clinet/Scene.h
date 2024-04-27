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

	//TODO LIst: 트리구조로 만들어보기
	vector<shared_ptr<GameObject>> _gameObjects;
};

