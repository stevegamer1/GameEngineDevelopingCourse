#pragma once

#include <Vector.h>
#include <typeindex>

namespace GameEngine {
	// Base of all components.
	struct Component {
		virtual const std::type_index& GetConcreteComponentTypeID() const = 0;
		virtual ~Component(){}
	};

	// Move back and forth.
	class MovementComponent : public Component {
	public:
		static const std::type_index ConcreteType;
		MovementComponent(float, float);
		Math::Vector3f velocity;
		float leftBound;
		float rightBound;

		const std::type_index& GetConcreteComponentTypeID() const override;
	};

	// Jump on the ground.
	class PhysicsComponent : public Component {
	public:
		static const std::type_index ConcreteType;
		PhysicsComponent(Math::Vector3f);
		Math::Vector3f velocity;

		const std::type_index& GetConcreteComponentTypeID() const override;
	};

	// Be controlled by input.
	class ControllableComponent : public Component {
	public:
		static const std::type_index ConcreteType;
		ControllableComponent();
		Math::Vector3f accumulatedInput;

		const std::type_index& GetConcreteComponentTypeID() const override;
	};
}