#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include <Geode/loader/SettingEvent.hpp>

using namespace geode::prelude;

int globalInterval = Mod::get()->getSettingValue<int64_t>("update-time");

float globalXPos = Mod::get()->getSettingValue<double>("render-x-pos");
float globalYPos = Mod::get()->getSettingValue<double>("render-y-pos");

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

$execute {
  listenForSettingChanges("render-y-pos", +[](double value) {
		globalYPos = value;
  });
}

bool modSettingsDisabled() {
  auto mod = Mod::get();

  auto mColor = mod->getSettingValue<bool>("change-main-color");
  auto sColor = mod->getSettingValue<bool>("change-secondary-color");
  auto mColorD = mod->getSettingValue<bool>("change-main-color-dual");
  auto sColorD = mod->getSettingValue<bool>("change-secondary-color-dual");
  auto waveTrail = mod->getSettingValue<bool>("change-wave-trail");
  auto playerGlow = mod->getSettingValue<bool>("change-glow-color");

  return !(mColor || sColor || mColorD || sColorD || waveTrail || playerGlow);
}

ccColor3B getScreenColor(bool onLevelEditorLayer = false) {
  if (modSettingsDisabled())
    return ccColor3B(0.f, 0.f, 0.f);

  auto size = CCDirector::sharedDirector()->getWinSize();

  auto renderTexture = CCRenderTexture::create(1, 1);
  CCLayer* layer;
  if (!onLevelEditorLayer)
    layer = PlayLayer::get();
  else 
    layer = LevelEditorLayer::get();
  
  auto oldPos = layer->getPosition();
  
  layer->setPosition({-size.width * globalXPos, -size.height * globalYPos});

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
}

void getScreenColorAndChange(bool onLevelEditorLayer = false) {
  auto color = getScreenColor(onLevelEditorLayer);
  changeColor(color, onLevelEditorLayer);
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
    
		
    std::chrono::milliseconds interval(globalInterval);
    
    auto currentTime = std::chrono::steady_clock::now();
		
    static auto lastExecutionTime = currentTime;
    
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);

    if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			getScreenColorAndChange();
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
    
		
    std::chrono::milliseconds interval(globalInterval);
    
    auto currentTime = std::chrono::steady_clock::now();
    
    static auto lastExecutionTime = currentTime;
		
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime);

    if ((executeCode && elapsedTime >= interval || globalFirstTime)) { 
			getScreenColorAndChange(true);
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
