#pragma once


enum class SceneType
{
	NONE,
	STAGE1,
	STAGE2,

};
class Scene;

class SceneManger
{

public:

	static SceneManger* GetInstance()
	{

		static SceneManger instance;
		return &instance;
	}


public:
	void Init();
	void Update();
	void Render();

public:

	void ChangeScene(SceneType type);


private:

	shared_ptr<Scene> _scene;
	SceneType _sceneType = SceneType::NONE;

};

