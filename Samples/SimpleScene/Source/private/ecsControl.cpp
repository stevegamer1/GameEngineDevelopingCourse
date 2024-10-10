#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Geometry.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <RenderObject.h>
#include <Vector.h>
#include <DefaultGeometry.h>


using namespace GameEngine;

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<Position, CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller)
	{
		Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
		if (controller.ptr->IsPressed("GoLeft"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoRight"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoBack"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetViewDir();
		}
		if (controller.ptr->IsPressed("GoForward"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetViewDir();
		}
		position.value = position.value + currentMoveDir.Normalized() * speed * world.delta_time();
		camera.ptr->SetPosition(position.value);
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.value.x * pos.value.x + plane.value.y * pos.value.y + plane.value.z * pos.value.z < plane.value.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.value.y = jump.value;
			}
		}
	});

	world.system<const Position, const CameraPtr, const ControllerPtr>()
		.each([&world](const Position& pos, const CameraPtr& camera, const ControllerPtr& controller)
	{
		if (controller.ptr->IsPressed("Shoot"))
		{
			GameEngine::Math::Vector3f viewDir = camera.ptr->GetViewDir();
			GameEngine::Core::Timer dieTimer = GameEngine::Core::Timer();
			dieTimer.Stop();
			world.entity()
				.set(Position{ pos.value + viewDir * 0.1f })
				.set(Velocity{ viewDir * 50.0f })
				.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
				.set(DieOnTimerAfterBounce{ dieTimer, 5.0f })
				.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
				.set(Bounciness{ 1.f })
				.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
				.set(RenderObjectPtr{ new Render::RenderObject() });
		}
	});
}

