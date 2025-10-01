#pragma once

#include <Vector.h>
#include <typeindex>

namespace GameEngine {
	// Base of all components.
	struct Component {
		enum class Type {
			MovementComponent = 0,
			PhysicsComponent = 1,
			ControllableComponent = 2
		};

		virtual Component::Type GetConcreteComponentType() const = 0;
		virtual ~Component(){}
	};

	// Move back and forth.
	class MovementComponent : public Component {
	public:
		MovementComponent(float, float);
		Math::Vector3f velocity;
		float leftBound;
		float rightBound;

		Component::Type GetConcreteComponentType() const override;
	};

	// Jump on the ground.
	class PhysicsComponent : public Component {
	public:
		PhysicsComponent(Math::Vector3f);
		Math::Vector3f velocity;

		Component::Type GetConcreteComponentType() const override;
	};

	// Be controlled by input.
	class ControllableComponent : public Component {
	public:
		ControllableComponent();
		Math::Vector3f accumulatedInput;

		Component::Type GetConcreteComponentType() const override;
	};
}