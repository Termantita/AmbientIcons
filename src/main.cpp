#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

#include "AmbientColor.hpp"

using namespace geode::prelude;

int globalInterval = Mod::get()->getSettingValue<int64_t>("update-time");

$execute {
	listenForSettingChanges("update-time", +[](int64_t value) { globalInterval = value; });
}

struct AmbientGJBGL : Modify<AmbientGJBGL, GJBaseGameLayer> {
	struct Fields {
		Ref<AmbientColor> m_ambientChanger;
	};

	bool init() {
		if (!GJBaseGameLayer::init())
			return false;
		m_fields->m_ambientChanger = AmbientColor::create(this);
		return true;
	}

	void update(float p0) {
		GJBaseGameLayer::update(p0);

		auto seq = CCSequence::create(
			CCDelayTime::create(globalInterval),
			CCCallFunc::create(m_fields->m_ambientChanger, callfunc_selector(AmbientColor::onChange)),
			nullptr
		);

		this->runAction(seq);
	}
};