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
	 void Awake();
	 void Start();
	 void Update();
	 void LateUpdate();

public:

	void ChangeScene(SceneType type);
	shared_ptr<Scene>  GetActiveScene() { return _scene; }

private:

	shared_ptr<Scene> _scene;
	SceneType _sceneType = SceneType::NONE;

};

