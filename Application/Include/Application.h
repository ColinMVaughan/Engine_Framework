#pragma once
#include<ECS.h>
#include <Renderer.h>

class Application
{
public:
	Application();
	~Application();

	void Initalize();
	void Update();

protected:

	void PreInitalize();
	virtual void DoInitalize() = 0;
	void PostInitalize();

	void PreUpdate();
	virtual void DoUpdate() = 0;
	void PostUpdate();


protected:

	ComponentManager* m_ComponentManager;
	SystemManager* m_SystemManager;
	ECS* m_ECS;

	Renderer* m_Renderer;
};

