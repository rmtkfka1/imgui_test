#pragma once
#include "Scene.h"
class Stage1 : public Scene
{
	using Super = Scene;

public:
	Stage1();
	virtual ~Stage1();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

};

