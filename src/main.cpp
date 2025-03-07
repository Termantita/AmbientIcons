#include <Geode/Geode.hpp>

#include <Geode/modify/GJBaseGameLayer.hpp>

#include "AmbientColor.hpp"

using namespace geode::prelude;

struct AmbientGJBGL : Modify<AmbientGJBGL, GJBaseGameLayer> {
	struct Fields {
		Ref<AmbientColor> m_ambientChanger;
	};

	bool init() {
		if (!GJBaseGameLayer::init())
			return false;

		Settings::updateSettings();
		
		if (!Settings::enabled)
			return true;

		auto ambient = AmbientColor::create(this);
		m_fields->m_ambientChanger = ambient;

		ambient->schedule(
			schedule_selector(AmbientColor::onChange),
			static_cast<float>(Settings::updateTime) / 1000
		);
		ambient->onEnter();

		return true;
	}
};
