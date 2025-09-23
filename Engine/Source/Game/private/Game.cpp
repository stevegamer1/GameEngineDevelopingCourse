#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>

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

		// When the user minimizes the window, the keys stay "pressed", but fixing this would require catching window minimization.
		using Key = Core::KeyListenersKeeper::Key;
		Core::KeyListenersKeeper::AddKeyDownListener([this](Key key){
			this->is_key_pressed[key] = true;
		});
		Core::KeyListenersKeeper::AddKeyUpListener([this](Key key){
			this->is_key_pressed[key] = false;
		});

		m_renderThread = std::make_unique<Render::RenderThread>();

		// How many objects do we want to create
		for (int i = 0; i < 3; ++i)
		{
			m_Objects.push_back(new GameObject());
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}
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

			Update(dt);
			
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

			m_renderThread->OnEndFrame();
		}
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			Math::Vector3f pos = m_Objects[i]->GetPosition();

			// Showcase
			if (i == 0)
			{
				pos.x += 0.5f * dt;
			}
			else if (i == 1)
			{
				pos.y -= 0.5f * dt;
			}
			else if (i == 2)
			{
				pos.x += 0.5f * dt;
				pos.y -= 0.5f * dt;
			}
			m_Objects[i]->SetPosition(pos, m_renderThread->GetMainFrame());
		}


		const float speed = 10.0f;
		Math::Vector3f direction(0.0f, 0.0f, 0.0f);
		using Key = Core::KeyListenersKeeper::Key;
		Math::Vector3f forward = Core::g_MainCamera->GetViewDir();
		Math::Vector3f right = Math::Vector3f(0.0f, 1.0f, 0.0f).CrossProduct(forward);

		if (is_key_pressed[Key::D])
		{
			direction = direction + right;
		}

		if (is_key_pressed[Key::A])
		{
			direction = direction - right;
		}

		if (is_key_pressed[Key::W])
		{
			direction = direction + forward;
		}

		if (is_key_pressed[Key::S])
		{
			direction = direction - forward;
		}

		Math::Vector3f offset = direction * dt * speed;
		Core::g_MainCamera->GetViewDir();
		Math::Vector3f pos = Core::g_MainCamera->GetPosition();
		Core::g_MainCamera->SetPosition(pos + offset);
	}
}