#include <Components.h>
#include <Vector.h>
#include <Input/InputHandler.h>

namespace GameEngine {
	MovementComponent::MovementComponent(float leftBound, float rightBound) :
		velocity(Math::Vector3f(1.0f, 0.0f, 0.0f)), 
		leftBound(leftBound),
		rightBound(rightBound)
	{}

	PhysicsComponent::PhysicsComponent(Math::Vector3f initVelocity) : velocity(initVelocity) {}

	ControllableComponent::ControllableComponent() : accumulatedInput(Math::Vector3f::Zero()) {
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { accumulatedInput.x -= 1.0f; });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { accumulatedInput.x += 1.0f; });
	}
}
