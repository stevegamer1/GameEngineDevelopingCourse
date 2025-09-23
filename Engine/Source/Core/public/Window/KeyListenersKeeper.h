#pragma once

#include <Window/IWindow.h>

namespace GameEngine::Core
{
	class CORE_API KeyListenersKeeper
    {
    public:
        enum class Key
        {
            W,
            A,
            S,
            D
        };
        using Listener = std::function<void(Key)>;

    private:
        static std::vector<Listener> down_listeners;
        static std::vector<Listener> up_listeners;

    public:
        static void AddKeyDownListener(Listener listener);

        static void AddKeyUpListener(Listener listener);

        static void CallDownListeners(Key key);

        static void CallUpListeners(Key key);
    };
}
