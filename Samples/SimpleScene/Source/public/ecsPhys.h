#pragma once

#include <flecs.h>
#include <Vector.h>
#include <Timer.h>

struct Position
{
	GameEngine::Math::Vector3f value;
};

struct Velocity
{
	GameEngine::Math::Vector3f value;
};

struct Gravity
{
	GameEngine::Math::Vector3f value;
};

struct BouncePlane
{
	GameEngine::Math::Vector4f value;
};

struct Bounciness
{
	float value;
};

struct ShiverAmount
{
	float value;
};

struct FrictionAmount
{
	float value;
};

struct DieOnTimerAfterBounce {
	GameEngine::Core::Timer timer;
	float death_time;
};

struct KillOnTouch { bool iHateYou = true; /*just don't touch this field, empty structs are prohibited*/ };

struct KilledOnTouch { bool iHateMyself = true; /*just don't touch this field, empty structs are prohibited*/ };

using Speed = float;

void RegisterEcsPhysSystems(flecs::world& world);

