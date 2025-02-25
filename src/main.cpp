#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

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
	struct Fields {
		Ref<AmbientColor> m_ambientChanger;
	};
	
  	void onExit() {
    	PlayLayer::onExit();
  	}
	
	void resetLevel() {
		PlayLayer::resetLevel();
	}

  	bool init(GJGameLevel* p0, bool p1, bool p2) {
    	if (!PlayLayer::init(p0, p1, p2))
      		return false;
    	m_fields->m_ambientChanger = AmbientColor::create(this);
    	return true;
  	}


	void postUpdate(float p0) {
		PlayLayer::postUpdate(p0);

		auto seq = CCSequence::create(
			CCDelayTime::create(globalInterval),
			CCCallFunc::create(m_fields->m_ambientChanger, callfunc_selector(AmbientColor::onChange)),
			nullptr
		);

		this->runAction(seq);
	}
};

struct AmbientLevelEditorLayer : Modify<AmbientLevelEditorLayer, LevelEditorLayer> {
	struct Fields {
		Ref<AmbientColor> m_ambientChanger;
	};

	bool init(GJGameLevel* p0, bool p1) {
		if (!LevelEditorLayer::init(p0, p1))
			return false;
		m_fields->m_ambientChanger = AmbientColor::create(this);
		return true;
	}

	void postUpdate(float p0) {
		LevelEditorLayer::postUpdate(p0);

		auto seq = CCSequence::create(
			CCDelayTime::create(globalInterval),
			CCCallFunc::create(m_fields->m_ambientChanger, callfunc_selector(AmbientColor::onChange)),
			nullptr
		);

		this->runAction(seq);
	}
};