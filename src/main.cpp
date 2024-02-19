#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#define TIME 100

using namespace geode::prelude;

size_t waitFor = TIME;
bool doRead = true;

class $modify(PlayLayer) {
	void postUpdate(float p0) {
		if (doRead) { // Evits fps drops
			// get color screen
			auto size = CCDirector::sharedDirector()->getWinSize();
			
			auto renderTexture = CCRenderTexture::create(static_cast<int>(size.width / 5), static_cast<int>(size.height / 2));
			
			renderTexture->begin();
			CCDirector::sharedDirector()->getRunningScene()->visit();
			renderTexture->end();

			auto img = renderTexture->newCCImage();

			auto data = img->getData();

			auto color = ccColor3B(data[0], data[1], data[2]);
			delete img;

			// Change player color
			auto player = PlayLayer::get()->m_player1;
			auto player2 = PlayLayer::get()->m_player2;

			if (Mod::get()->getSettingValue<bool>("change-main-color"))
				player->setColor(color);
			
			if (Mod::get()->getSettingValue<bool>("change-secondary-color"))
				player->setSecondColor(color);
			
			if (Mod::get()->getSettingValue<bool>("change-main-color-dual"))
				player2->setColor(color);
			
			if (Mod::get()->getSettingValue<bool>("change-secondary-color-dual"))
				player2->setSecondColor(color);
			
			waitFor = TIME;
		}
		waitFor--;
		doRead = waitFor == 0;
		
		PlayLayer::postUpdate(p0);
	}
};