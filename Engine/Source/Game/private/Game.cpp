#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>
#include <Components.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		std::srand(m_GameTimer.GetTotalTime());
		for (int i = 0; i < 100; ++i)
		{
			m_Objects.push_back(new GameObject());
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
			
			float x = (static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f) * 10.0f;
			float z = (static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f) * 10.0f;
			m_Objects.back()->SetPosition(Math::Vector3f(x, 0.0f, z), m_renderThread->GetMainFrame());

			int obj_type = std::rand() % 3;
			if (obj_type == 0) {
				float leftBound = m_Objects.back()->GetPosition().x - 5.0f;
				float rightBound = m_Objects.back()->GetPosition().x + 5.0f;
				m_Objects.back()->components.push_back(new MovementComponent(leftBound, rightBound));
			}
			else if (obj_type == 1) {
				float velocityY = static_cast<float>(std::rand()) / RAND_MAX * 10.0f;
				m_Objects.back()->components.push_back(new PhysicsComponent(Math::Vector3f(0.0f, velocityY, 0.0f)));
			}
			else {
				m_Objects.back()->components.push_back(new ControllableComponent());
			}
		}

		// Movement system.
		m_Systems.push_back([](Game& game, GameObject& obj,  float dt) {
			for (const auto& comp : obj.components) {
				MovementComponent* movement = dynamic_cast<MovementComponent*>(comp);
				if (movement != nullptr) {
					obj.SetPosition(obj.GetPosition() + movement->velocity * dt, game.m_renderThread->GetMainFrame());

					if (obj.GetPosition().x > movement->rightBound) {
						movement->velocity = Math::Vector3f(-1.0f, 0.0f, 0.0f);
					}

					if (obj.GetPosition().x < movement->leftBound) {
						movement->velocity = Math::Vector3f(1.0f, 0.0f, 0.0f);
					}
				}
			}
		});

		// Physics system.
		m_Systems.push_back([](Game& game, GameObject& obj, float dt) {
			for (const auto& comp : obj.components) {
				PhysicsComponent* physics = dynamic_cast<PhysicsComponent*>(comp);
				if (physics != nullptr) {
					Math::Vector3 gravity = Math::Vector3f(0.0f, -2.0f, 0.0f);

					// Movement.
					obj.SetPosition(obj.GetPosition() + physics->velocity * dt + gravity * dt * dt * 0.5f,
						game.m_renderThread->GetMainFrame());

					// Acceleration.
					physics->velocity = physics->velocity + gravity * dt;

					// Bounce.
					if (obj.GetPosition().y < 0.0f) {
						Math::Vector3f newPos = obj.GetPosition();
						newPos.z = 0.0f;
						obj.SetPosition(newPos, game.m_renderThread->GetMainFrame());
						physics->velocity = -physics->velocity;
					}
				}
			}
		});

		// Controlled movement system.
		m_Systems.push_back([](Game& game, GameObject& obj, float dt) {
			for (const auto& comp : obj.components) {
				ControllableComponent* control = dynamic_cast<ControllableComponent*>(comp);
				if (control != nullptr) {
					obj.SetPosition(obj.GetPosition() + control->accumulatedInput * dt, game.m_renderThread->GetMainFrame());
					control->accumulatedInput = Math::Vector3f::Zero();
				}
			}
		});

		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		for (const auto& sys : m_Systems) {
			for (const auto& obj : m_Objects) {
				sys(*this, *obj, dt);
			}
		}
	}
}