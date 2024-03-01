#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include <Geode/loader/SettingEvent.hpp>

#include "AmbientColor.hpp"

using namespace geode::prelude;

int globalInterval = Mod::get()->getSettingValue<int64_t>("update-time");

$execute {
  listenForSettingChanges("update-time", +[](int64_t value) {
		globalInterval = value;
  });
}

// void getScreenColorAndChange(bool onLevelEditorLayer = false) {
//   auto color = getScreenColor(onLevelEditorLayer);
//   changeColor(color, onLevelEditorLayer);
// }

bool globalFirstTime = true;
class $modify(PlayLayer) {
  void onExit() {
    globalFirstTime = true;
    PlayLayer::onExit();
  }

	void postUpdate(float p0) {
    PlayLayer::postUpdate(p0);

    static bool executeCode = true;
    
		
    std::chrono::milliseconds interval(globalInterval);
    
    auto currentTime = std::chrono::steady_clock::now();
		
    static auto lastExecutionTime = currentTime;
    
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);

    AmbientColor changer(this, CCPoint(Mod::get()->getSettingValue<double>("render-x-pos"), Mod::get()->getSettingValue<double>("render-y-pos")));
    
    if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			changer.setIconColor(changer.getScreenColor());
      lastExecutionTime = currentTime;

			if (globalFirstTime)
				globalFirstTime = false;
    }

    executeCode = !executeCode;
	}
};

class $modify(LevelEditorLayer) {
  void postUpdate(float p0) {
    LevelEditorLayer::postUpdate(p0);

    static bool executeCode = true;
    
		
    std::chrono::milliseconds interval(globalInterval);
    
    auto currentTime = std::chrono::steady_clock::now();
    
    static auto lastExecutionTime = currentTime;
		
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);

    AmbientColor changer(this, CCPoint(Mod::get()->getSettingValue<double>("render-x-pos"), Mod::get()->getSettingValue<double>("render-y-pos")));

    if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			changer.setIconColor(changer.getScreenColor());
      lastExecutionTime = currentTime;

			if (globalFirstTime)
				globalFirstTime = false;
    }

    executeCode = !executeCode;
  }
};

class $modify(PlayerObject) {
  void flashPlayer(float p0, float p1, ccColor3B mainColor, ccColor3B secondColor) {
    if (Mod::get()->getSettingValue<bool>("player-flash"))
      PlayerObject::flashPlayer(p0, p1, mainColor, secondColor);
  }
};
