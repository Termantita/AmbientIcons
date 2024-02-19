#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#define TIME 100

using namespace geode::prelude;

size_t waitFor = TIME;
bool doRead = true;

class $modify(PlayLayer) {
	/*bool init(GJGameLevel* p0, bool p1, bool p2) {
		if (!PlayLayer::init(p0, p1, p2)) 
			return false;

		auto size = CCDirector::sharedDirector()->getWinSize();
		
		auto point = CCLabelBMFont::create("0", "bigFont.fnt");
		
		point->setPosition({size.width / 5, size.height / 2});

		this->addChild(point);

		return true;
	}*/

	void postUpdate(float p0) {
		if (doRead) {
			auto size = CCDirector::sharedDirector()->getWinSize();
			
			auto renderTexture = CCRenderTexture::create(static_cast<int>(size.width / 5), static_cast<int>(size.height / 2));
			
			renderTexture->beginWithClear(105, 12, 13, 100);
			CCDirector::sharedDirector()->getRunningScene()->visit();
			renderTexture->end();

			auto img = renderTexture->newCCImage();

			auto data = img->getData();

			auto color = ccColor3B(data[0], data[1], data[2]);
			delete img;

			log::info("rgb_picker({}, {}, {})", color.r, color.g, color.b);

			auto player = PlayLayer::get()->m_player1;

			if (Mod::get()->getSettingValue<bool>("change-1st-color"))
				player->setColor(color);
			
			if (Mod::get()->getSettingValue<bool>("change-2nd-color"))
				player->setSecondColor(color);

			waitFor = TIME;
		}
		waitFor--;
		doRead = waitFor == 0;
		
		PlayLayer::postUpdate(p0);
	}
};