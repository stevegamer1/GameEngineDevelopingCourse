#include <Camera.h>
#include <Input/Controller.h>
#include <ecsControl.h>
#include <ecsBullets.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <ecsMesh.h>
#include <Geometry.h>
#include <RenderObject.h>

void RegisterEcsBulletSystems(flecs::world& world)
{
	using GameEngine::Core::array;
	using GameEngine::RenderCore::Geometry;
	using GameEngine::Math::Vector3f;

	constexpr array<Geometry::VertexType, 8> vertices =
	{
		Vector3f(-1.0f, -1.0f, -1.0f),
		Vector3f(-1.0f, +1.0f, -1.0f),
		Vector3f(+1.0f, +1.0f, -1.0f),
		Vector3f(+1.0f, -1.0f, -1.0f),
		Vector3f(-1.0f, -1.0f, +1.0f),
		Vector3f(-1.0f, +1.0f, +1.0f),
		Vector3f(+1.0f, +1.0f, +1.0f),
		Vector3f(+1.0f, -1.0f, +1.0f)
	};

	constexpr array<Geometry::IndexType, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	world.system<Position, CameraPtr, const ControllerPtr, BulletShooter>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const ControllerPtr& controller, BulletShooter& shooter)
			{
				if (controller.ptr->IsPressed("Shoot") &&
					shooter.bullets_left > 0 && shooter.seconds_till_cooldown < 0.1f && shooter.seconds_till_reload < 0.1f)
				{
					world.entity()
						.set(position)
						.set(Velocity{ camera.ptr->GetViewDir() * 20.0f })
						.set(Gravity{ GameEngine::Math::Vector3f{0, -9.813f, 0} })
						.set(DieOnTimer{5.0f})
						.set(KillOnTouch{1.0f})
						.set(BouncePlane{ GameEngine::Math::Vector4f{0, 1, 0, 0} })
						.set(Bounciness{ 1.0f })
						.set(GeometryPtr{
							std::make_shared<Geometry>(
								(Geometry::VertexType*)vertices.begin(), vertices.size(),
								(Geometry::IndexType*)indices.begin(), indices.size())
							}
						)
						.set(RenderObjectPtr{ new GameEngine::Render::RenderObject() });

					shooter.bullets_left -= 1;
					shooter.seconds_till_cooldown = shooter.current_cooldown_time;
					if (shooter.bullets_left == 0) {
						shooter.seconds_till_reload = shooter.current_reload_time;
						flecs::log::dbg("shot all shots");
					}
				}
			});

	world.system<BulletShooter>()
		.each([&](flecs::entity e, BulletShooter& shooter)
			{
				shooter.seconds_till_cooldown = std::max(shooter.seconds_till_cooldown - world.delta_time(), 0.0f);

				float old_time_till_reload = shooter.seconds_till_reload;
				shooter.seconds_till_reload = std::max(shooter.seconds_till_reload - world.delta_time(), 0.0f);
				if (old_time_till_reload > 0.1f && shooter.seconds_till_reload < 0.1f) {
					shooter.bullets_left = shooter.bullets_max;
					flecs::log::dbg("reload");
				}
			});

	world.system<DieOnTimer>()
		.each([&](flecs::entity e, DieOnTimer& death)
			{
				death.seconds_left -= world.delta_time();
				if (death.seconds_left <= 0.0f) {
					if (e.has<Position>()) {
						e.get_mut<Position>()->value = Vector3f{99999.0f, 999999.0f, 999999.0f};
					}
				}
			});

	flecs::query_builder<Position, const KillableByTouch> killableQueryBuilder =
		world.query_builder<Position, const KillableByTouch>();
	flecs::query<Position, const KillableByTouch> killableQuery = killableQueryBuilder.build();

	flecs::query_builder<BulletShooter> shooterQueryBuilder =
		world.query_builder<BulletShooter>();
	flecs::query<BulletShooter> shooterQuery = shooterQueryBuilder.build();

	world.system<Position, KillOnTouch>()
		.each([&world, killableQuery, shooterQuery](flecs::entity e, Position& position, const KillOnTouch& killer)
			{
				int bulletsToAdd = 0;

				killableQuery.each([&position, &killer, &bulletsToAdd]
					(flecs::entity v_e, Position& victim_position, const KillableByTouch& killable)
					{
						if ((position.value - victim_position.value).GetLength() < (killer.radius + killable.radius)) {
							victim_position.value = Vector3f{ 99999.0f, 999999.0f, 999999.0f };
							v_e.remove<KillableByTouch>();

							if (v_e.has<BulletsBonusOnKillByTouch>()) {
								bulletsToAdd += v_e.get<BulletsBonusOnKillByTouch>()->bullets_amount;
							}
						}
					});

				shooterQuery.each([bulletsToAdd](BulletShooter& shooter) {
						shooter.bullets_left += bulletsToAdd;
						// shooter.bullets_left = std::max(shooter.bullets_left, shooter.bullets_max);  // Should bonus override this?
					});
			});
}
