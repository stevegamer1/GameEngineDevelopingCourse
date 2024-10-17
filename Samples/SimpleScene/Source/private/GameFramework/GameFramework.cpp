// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#define GAME_FRAMEWORK

#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ECS/ecsSystems.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>
#include <flecs.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterComponents();
	RegisterSystems();

	flecs::entity cubeControl = m_World.entity()
		.set(Position{ -2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 10.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 0.3f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ 2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 3.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 1.f })
		.set(Collider{ 0.5, 0.5, 0.5 })
		.set(DieOnCollision{true})
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity camera = m_World.entity()
		.set(Position{ 0.0f, 12.0f, -10.0f })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });
}

void GameFramework::RegisterComponents()
{
	// Exposing these components for the lua system
	ECS_META_COMPONENT(m_World, Position);
	ECS_META_COMPONENT(m_World, Velocity);
	ECS_META_COMPONENT(m_World, Gravity);
	ECS_META_COMPONENT(m_World, BouncePlane);
	ECS_META_COMPONENT(m_World, Bounciness);
	ECS_META_COMPONENT(m_World, ShiverAmount);
	ECS_META_COMPONENT(m_World, FrictionAmount);
	ECS_META_COMPONENT(m_World, Speed);
	ECS_META_COMPONENT(m_World, DestructionTimer);
	ECS_META_COMPONENT(m_World, Collider);
	ECS_META_COMPONENT(m_World, KillOnCollision);
	ECS_META_COMPONENT(m_World, DieOnCollision);
}

void GameFramework::RegisterSystems()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);

	m_World.system<const Position, const CameraPtr, const ControllerPtr>()
		.each([&](const Position& position, const CameraPtr& cameraPtr, const ControllerPtr& controller)
			{
				if (controller.ptr->IsPressed("Shoot")) {
					Math::Vector3 velocity = cameraPtr.ptr->GetViewDir() * 4.0f;
					m_World.entity()
						.set(Position{ position })
						.set(Velocity{ velocity.x, velocity.y, velocity.z })
						.set(Gravity{ 0.f, -9.8065f, 0.f })
						.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
						.set(Bounciness{ 0.2f })
						.set(DestructionTimer{ 10.0f })
						.set(Collider{0.5, 0.5, 0.5})
						.set(KillOnCollision{true})
						.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
						.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });
				}
			});
}

void GameFramework::Update(float dt)
{

}