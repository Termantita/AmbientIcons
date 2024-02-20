#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/loader/SettingEvent.hpp>

using namespace geode::prelude;

int globalInterval = Mod::get()->getSettingValue<int64_t>("update-time");

$execute {
    listenForSettingChanges("update-time", +[](int64_t value) {
			globalInterval = value;
    });
}

void changeColor(std::chrono::steady_clock::time_point* lastExecutionTime, std::chrono::steady_clock::time_point* currentTime) {
  auto size = CCDirector::sharedDirector()->getWinSize();
            
  auto renderTexture = CCRenderTexture::create(static_cast<int>(size.width / 2), static_cast<int>(size.height / 2));

  auto player = PlayLayer::get()->m_player1;
  auto player2 = PlayLayer::get()->m_player2;
            
  renderTexture->begin();
  player->m_gameLayer->visit();
  renderTexture->end();
            
  auto img = renderTexture->newCCImage();
  auto data = img->getData();
  auto color = ccColor3B(data[0], data[1], data[2]);
            
  if (Mod::get()->getSettingValue<bool>("change-main-color"))
    player->setColor(color);
            
  if (Mod::get()->getSettingValue<bool>("change-secondary-color"))
    player->setSecondColor(color);
            
  if (Mod::get()->getSettingValue<bool>("change-main-color-dual"))
    player2->setColor(color);
            
  if (Mod::get()->getSettingValue<bool>("change-secondary-color-dual"))
    player2->setSecondColor(color);
            
  delete img;
  *lastExecutionTime = *currentTime;

	return;
}

bool firstTime = true;
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
    static bool executeCode = true;
    
		static auto lastExecutionTime = std::chrono::steady_clock::now();
		
    std::chrono::milliseconds interval(globalInterval);
    
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);
        
    if ((executeCode && elapsedTime >= interval) || firstTime) { 
			changeColor(&lastExecutionTime, &currentTime);

			if (firstTime)
				firstTime = false;
    }
        
    executeCode = !executeCode;
    
    PlayLayer::postUpdate(p0);
	}
};
