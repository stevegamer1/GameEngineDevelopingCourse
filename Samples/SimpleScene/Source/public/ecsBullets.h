#pragma once

#include <flecs.h>

struct BulletShooter {
	uint32_t bullets_left = 6;
	uint32_t bullets_max = 6;
	float current_cooldown_time = 1.0f;
	float current_reload_time = 5.0f;
	float seconds_till_cooldown = 0.0f;
	float seconds_till_reload = 0.0f;
};

struct KillOnTouch {
	float radius = 1.0f;
};

struct KillableByTouch {
	float radius = 1.0f;
};

struct BulletsBonusOnKillByTouch {
	uint32_t bullets_amount = 10;
};

struct DieOnTimer {
	float seconds_left = 1.0f;
};

void RegisterEcsBulletSystems(flecs::world& world);
