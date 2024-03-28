#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include <Geode/loader/SettingEvent.hpp>

#include "AmbientColor.hpp"

using namespace geode::prelude;

int globalInterval = Mod::get()->getSettingValue<int64_t>("update-time");

$execute {
  listenForSettingChanges("update-time", +[](int64_t value) {
		globalInterval = value;
  });
}

bool globalFirstTime = true;
struct AmbientPlayLayer : Modify<AmbientPlayLayer, PlayLayer> {
	Ref<AmbientColor> m_ambientChanger;
  	void onExit() {
    	globalFirstTime = true;
    	PlayLayer::onExit();
  	}
	
	void resetLevel() {
		PlayLayer::resetLevel();
		globalFirstTime = true;
	}

  	bool init(GJGameLevel* p0, bool p1, bool p2) {
    	if (!PlayLayer::init(p0, p1, p2))
      		return false;
    	m_fields->m_ambientChanger = AmbientColor::create(this);
    	return true;
  	}


	void postUpdate(float p0) {
		PlayLayer::postUpdate(p0);

    	static bool executeCode = true;
    
    	std::chrono::milliseconds interval(globalInterval);
    
    	auto currentTime = std::chrono::steady_clock::now();
		
    	static auto lastExecutionTime = currentTime;
    	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);

    
    	if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			m_fields->m_ambientChanger->setIconColor(m_fields->m_ambientChanger->getScreenColor());
      		lastExecutionTime = currentTime;

			if (globalFirstTime)
				globalFirstTime = false;
    	}

    	executeCode = !executeCode;
    
	}
};

struct AmbientLevelEditorLayer : Modify<AmbientLevelEditorLayer, LevelEditorLayer> {
	Ref<AmbientColor> m_ambientChanger;

	bool init(GJGameLevel* p0, bool p1) {
		if (!LevelEditorLayer::init(p0, p1))
			return false;
		m_fields->m_ambientChanger = AmbientColor::create(this);
		return true;
	}

	void postUpdate(float p0) {
		LevelEditorLayer::postUpdate(p0);

		static bool executeCode = true;
		
		std::chrono::milliseconds interval(globalInterval);
		
		auto currentTime = std::chrono::steady_clock::now();
			
		static auto lastExecutionTime = currentTime;
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);


		if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			m_fields->m_ambientChanger->setIconColor(m_fields->m_ambientChanger->getScreenColor());
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
