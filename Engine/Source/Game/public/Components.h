#pragma once

#include <Vector.h>

namespace GameEngine {
	// Base of all components.
	struct Component {
		virtual const std::type_info& GetConcreteComponentTypeID() const = 0;
		virtual ~Component(){}
	};

	// Move back and forth.
	class MovementComponent : public Component {
	public:
		MovementComponent(float, float);
		Math::Vector3f velocity;
		float leftBound;
		float rightBound;

		const std::type_info& GetConcreteComponentTypeID() const override;
	};

	// Jump on the ground.
	class PhysicsComponent : public Component {
	public:
		PhysicsComponent(Math::Vector3f);
		Math::Vector3f velocity;

		const std::type_info& GetConcreteComponentTypeID() const override;
	};

	// Be controlled by input.
	class ControllableComponent : public Component {
	public:
		ControllableComponent();
		Math::Vector3f accumulatedInput;

		const std::type_info& GetConcreteComponentTypeID() const override;
	};
}