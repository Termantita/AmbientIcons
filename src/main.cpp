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
		Ref<CCSequence> seq;
	};

	bool init() {
		if (!GJBaseGameLayer::init())
			return false;

		if (!Mod::get()->getSettingValue<bool>("mod-enabled"))
			return true;

		m_fields->m_ambientChanger = AmbientColor::create(this);
		
		auto ambient = m_fields->m_ambientChanger;
		m_fields->seq = CCSequence::create(
			CCCallFunc::create(ambient, callfunc_selector(AmbientColor::onChange)),
			CCDelayTime::create(static_cast<float>(globalInterval) / 1000),
			CCCallFunc::create(ambient, callfunc_selector(AmbientColor::onFinish)),
			nullptr
		);

		return true;
	}

	void update(float p0) {
		GJBaseGameLayer::update(p0);

		if (!Mod::get()->getSettingValue<bool>("mod-enabled"))
			return;

		if (m_fields->m_ambientChanger->isActionFinished()) this->runAction(m_fields->seq);
	}
};