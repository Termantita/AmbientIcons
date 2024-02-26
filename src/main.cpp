#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include <Geode/loader/SettingEvent.hpp>

using namespace geode::prelude;

int globalInterval = Mod::get()->getSettingValue<int64_t>("update-time");
float globalXPos = Mod::get()->getSettingValue<double>("render-x-pos");

$execute {
  listenForSettingChanges("update-time", +[](int64_t value) {
		globalInterval = value;
  });
}

$execute {
  listenForSettingChanges("render-x-pos", +[](double value) {
		globalXPos = value;
  });
}

ccColor3B getScreenColor() {
  auto size = CCDirector::sharedDirector()->getWinSize();

  auto renderTexture = CCRenderTexture::create(1, 1);
  auto layer = PlayLayer::get();
  auto oldPos = layer->getPosition();
  
  layer->setPosition({size.width * globalXPos, -size.height / 2});

  renderTexture->begin();
  layer->visit();
  renderTexture->end();

  layer->setPosition(oldPos);

  auto img = renderTexture->newCCImage();
  auto data = img->getData();
  ccColor3B color = ccColor3B(data[0], data[1], data[2]);

  delete img;
  renderTexture->removeMeAndCleanup();

  return color;
}

void changeColor(ccColor3B color, bool onLevelEditorLayer = false) {
  PlayerObject* player;
  PlayerObject* player2;
  
  if (!onLevelEditorLayer) {
    player = PlayLayer::get()->m_player1;
    player2 = PlayLayer::get()->m_player2;
  } else {
    player = LevelEditorLayer::get()->m_player1;
    player2 = LevelEditorLayer::get()->m_player2;
  }

  if (Mod::get()->getSettingValue<bool>("change-main-color"))
    player->setColor(color);

  if (Mod::get()->getSettingValue<bool>("change-secondary-color"))
    player->setSecondColor(color);

  if (Mod::get()->getSettingValue<bool>("change-main-color-dual"))
    player2->setColor(color);

  if (Mod::get()->getSettingValue<bool>("change-secondary-color-dual"))
    player2->setSecondColor(color);

  if (Mod::get()->getSettingValue<bool>("change-wave-trail")) {
    if (player->m_isDart) {
      player->m_waveTrail->setColor(color);
    }
    if (player2->m_isDart) {
      player2->m_waveTrail->setColor(color);
    }
  }

  if (Mod::get()->getSettingValue<bool>("change-glow-color")) {
    player->m_glowColor = color;
    player->updateGlowColor();

    player2->m_glowColor = color;
    player2->updateGlowColor();
  }

	return;
}

bool globalFirstTime = true;
class $modify(PlayLayer) {
  bool init(GJGameLevel* p0, bool p1, bool p2) {
    if (!PlayLayer::init(p0, p1, p2))
      return false;

    auto container = GameObject::create();
    container->setPosition(CCDirector::sharedDirector()->getWinSize() / 2);
    container->setID("frame-container"_spr);

    this->addChild(container);
    
    return true;
  }

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

class $modify(LevelEditorLayer) {
  void postUpdate(float p0) {
    LevelEditorLayer::postUpdate(p0);

    static bool executeCode = true;
    ccColor3B color;
    
		static auto lastExecutionTime = std::chrono::steady_clock::now();
		
    std::chrono::milliseconds interval(globalInterval);
    
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);

    if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			changeColor(getScreenColor(), true);
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
