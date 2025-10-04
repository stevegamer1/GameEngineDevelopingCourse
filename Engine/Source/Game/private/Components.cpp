#include <Components.h>
#include <Vector.h>
#include <Input/InputHandler.h>

namespace GameEngine {
	Component::Component(Type concreteType) :
		ConcreteComponentType(concreteType)
	{}

	Component::Type Component::GetType() const {
		return ConcreteComponentType;
	}

	MovementComponent::MovementComponent(float leftBound, float rightBound) :
		Component{ Type::MovementComponent },
		velocity(Math::Vector3f(1.0f, 0.0f, 0.0f)), 
		leftBound(leftBound),
		rightBound(rightBound)
	{}

	PhysicsComponent::PhysicsComponent(Math::Vector3f initVelocity) :
		Component{ Type::PhysicsComponent },
		velocity(initVelocity)
	{}

	ControllableComponent::ControllableComponent() :
		Component{ Type::ControllableComponent },
		accumulatedInput(Math::Vector3f::Zero())
	{
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { accumulatedInput.x -= 1.0f; });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { accumulatedInput.x += 1.0f; });
	}
}
