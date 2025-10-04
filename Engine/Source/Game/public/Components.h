#pragma once

#include <Vector.h>
#include <typeindex>

namespace GameEngine {
	// Base of all components.
	struct Component {
		enum class Type {
			Invalid = 0,
			MovementComponent = 1,
			PhysicsComponent = 2,
			ControllableComponent = 3
		};
	protected:
		Type ConcreteComponentType = Type::Invalid;
	public:

		Component(Type concreteType);
		Type GetType() const;
		virtual ~Component() = default;
	};

	// Move back and forth.
	class MovementComponent : public Component {
	public:
		MovementComponent(float, float);
		Math::Vector3f velocity;
		float leftBound;
		float rightBound;
	};

	// Jump on the ground.
	class PhysicsComponent : public Component {
	public:
		const Type ConcreteComponentType = Type::PhysicsComponent;
		PhysicsComponent(Math::Vector3f);
		Math::Vector3f velocity;
	};

	// Be controlled by input.
	class ControllableComponent : public Component {
	public:
		const Type ConcreteComponentType = Type::ControllableComponent;
		ControllableComponent();
		Math::Vector3f accumulatedInput;
	};
}