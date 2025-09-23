#include <Window/KeyListenersKeeper.h>

namespace GameEngine::Core
{
    std::vector<KeyListenersKeeper::Listener> KeyListenersKeeper::down_listeners = {};
    std::vector<KeyListenersKeeper::Listener> KeyListenersKeeper::up_listeners = {};

    void KeyListenersKeeper::AddKeyDownListener(Listener listener) {
        down_listeners.push_back(listener);
    }

    void KeyListenersKeeper::AddKeyUpListener(Listener listener) {
        up_listeners.push_back(listener);
    }

    void KeyListenersKeeper::CallDownListeners(Key key) {
        for (auto& l : down_listeners)
        {
          l(key);
        }
    }

    void KeyListenersKeeper::CallUpListeners(Key key) {
        for (auto& l : up_listeners)
        {
          l(key);
        }
    }
}
