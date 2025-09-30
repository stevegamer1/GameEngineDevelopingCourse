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

	const std::type_index& MovementComponent::GetConcreteComponentTypeID() const {
		return ConcreteType;
	}

	const std::type_index& PhysicsComponent::GetConcreteComponentTypeID() const {
		return ConcreteType;
	}

	const std::type_index& ControllableComponent::GetConcreteComponentTypeID() const {
		return ConcreteType;
	}

	const std::type_index MovementComponent::ConcreteType = typeid(MovementComponent);
	const std::type_index PhysicsComponent::ConcreteType = typeid(PhysicsComponent);
	const std::type_index ControllableComponent::ConcreteType = typeid(ControllableComponent);
}
