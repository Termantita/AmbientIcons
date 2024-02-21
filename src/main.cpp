#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include <Geode/loader/SettingEvent.hpp>

#include <thread>

using namespace geode::prelude;

int globalInterval = Mod::get()->getSettingValue<int64_t>("update-time");

$execute {
    listenForSettingChanges("update-time", +[](int64_t value) {
			globalInterval = value;
    });
}

ccColor3B getScreenColor() {
  auto size = CCDirector::sharedDirector()->getWinSize();
            
  auto renderTexture = CCRenderTexture::create(static_cast<int>(size.width / 2), static_cast<int>(size.height / 2));


  renderTexture->begin();
  CCDirector::sharedDirector()->getRunningScene()->visit();
  renderTexture->end();

  auto img = renderTexture->newCCImage();
  auto data = img->getData();
  auto color = ccColor3B(data[0], data[1], data[2]);
  
  delete img;

  return color;
}

void changeColor(ccColor3B color) {
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

	return;
}

bool globalFirstTime = true;
class $modify(PlayLayer) {
  void onExit() {
    globalFirstTime = true;
    PlayLayer::onExit();
  }

	void postUpdate(float p0) {
    PlayLayer::postUpdate(p0);

    static bool executeCode = true;
    ccColor3B color;
    
		static auto lastExecutionTime = std::chrono::steady_clock::now();
		
    std::chrono::milliseconds interval(globalInterval);
    
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);

    if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			changeColor(getScreenColor());
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
