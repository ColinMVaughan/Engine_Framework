#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <ECS.h>
#include <PhysicsSystem.h>
#include <Renderer.h>

#include <glm\mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>




class CameraComponent
{
public:
	COMPONENT_SERIALIZE(m_CameraID)

	Camera m_Camera;
	int m_CameraID;
};

class SetEditorCameraEvent : public IEvent
{
public:
	SetEditorCameraEvent() = default;
	SetEditorCameraEvent(CameraComponent* cam):camera(cam) {}
	CameraComponent* camera;
};

//-----------------------------------------------------------------------------
//					CameraSystem
//
//	CameraSystem sends the entitie's transform matrix to the Camera component
//	once per frame.
//	
//	Required Components: Camera, Transform
//-----------------------------------------------------------------------------

class CameraSystem : public ECS::System<CameraComponent, Transform>
{
public:
	CameraSystem(ECS::ComponentManager* a_CompManager, EventManager& a_EveManager)
		:System(a_CompManager, a_EveManager) 
	{
		//std:std::function<void(SetEditorCameraEvent&)> f2 = [this](SetEditorCameraEvent& eve) {SetEditorCamera(eve); };
		//a_EveManager.AddListner<SetEditorCameraEvent>(f2);
		REGISTER_EVENT_LISTNER(SetEditorCameraEvent,SetEditorCamera,a_EveManager)
	}

	// This Updates the Camera Component's transform menmber to the Entities global transform.
	// This Will most likley be refactored later.
	void Update(double deltaTime, ECS::Entity& entity) override
	{
		//Get Required Components
		auto camera		= entity.GetComponent<CameraComponent>();
		auto transform  = entity.GetComponent<Transform>();

		//Set the Camera's Transform Component to the Transform of the Entity
		camera->m_Camera.m_Transform = glm::make_mat4(transform->GetGlobalTransformMatrix().front());
	}

	void Start(ECS::Entity& entity) override
	{
		
	}

	void Stop(ECS::Entity& entity) override
	{

	}

	void EntityRegistered(ECS::Entity& entity) override
	{
		auto cam = entity.GetComponent<CameraComponent>();
		
		//set default projection
		cam->m_Camera.m_Projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		
		//if there is no editor camera set, use this camera
		if (m_EditorCamera == nullptr)
		{
			m_EditorCamera = cam;
			m_Renderer->SetCamera(&cam->m_Camera);
		}

		//if there is no main camera set, use this one
		if (m_MainCamera == nullptr)
			m_MainCamera = cam;
	}

	void SetEditorCamera(SetEditorCameraEvent& a_event)
	{
		m_EditorCamera = a_event.camera;
		m_Renderer->SetCamera(&m_EditorCamera->m_Camera);
	}

	void SetRendrer(Renderer* rendPtr)
	{
		m_Renderer = rendPtr;
	}
private:
	Renderer * m_Renderer;
	CameraComponent* m_EditorCamera = nullptr;
	CameraComponent* m_MainCamera = nullptr;

};


//---------------------------------------------------------------------------------------
//						Debug Camera Control
//
//	Debug Camera Control is used for controlling the camera without being connected to the player
//	or gravity & collisions.
//
//	Required Components: DebugControl, Transform, Camera
//---------------------------------------------------------------------------------------

struct DebugControl
{
public:
	COMPONENT_SERIALIZE(bla)
private:
	short int bla;
};

class DebugCameraControlSystem : public ECS::System<DebugControl, Transform, CameraComponent>
{
public:

	DebugCameraControlSystem(ECS::ComponentManager* a_cmanager, EventManager& a_eManager);

	void PreUpdate(double deltaTime) override;
	void Update(double deltaTime, ECS::Entity& entity) override;
	void PostUpdate(double deltaTime) override;

	void KeyDown(unsigned char key) override;
	void KeyUp(unsigned char key) override;

	void MouseMoved(float xVelocity, float yVelocity);

private:
	PxVec3 MovementDirection;

	float speed = 0.3f;
	PxVec3 MoveDirection;
	PxQuat rotation;

	PxVec2 rotations;
};

#endif