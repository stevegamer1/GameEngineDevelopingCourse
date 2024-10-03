#pragma once

#include <Vector.h>

namespace GameEngine {
	// Base of all components.
	struct Component {
		virtual ~Component(){}
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
		PhysicsComponent(Math::Vector3f);
		Math::Vector3f velocity;
	};

	// Be controlled by input.
	class ControllableComponent : public Component {
	public:
		ControllableComponent();
		Math::Vector3f accumulatedInput;
	};
}