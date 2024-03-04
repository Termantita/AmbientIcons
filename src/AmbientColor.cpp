#include <Geode/Geode.hpp>

#include <Geode/loader/SettingEvent.hpp>

#include "AmbientColor.hpp"

double globalXPos = Mod::get()->getSettingValue<double>("render-x-pos");
double globalYPos = Mod::get()->getSettingValue<double>("render-y-pos");

$execute {
  listenForSettingChanges("render-x-pos", +[](double value) {
		globalXPos = value;
  });
  
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


ccColor3B AmbientColor::getScreenColor() {
  if (modSettingsDisabled())
    return ccColor3B(0.f, 0.f, 0.f);

  pickPos = CCPoint(globalXPos, globalYPos);

  auto size = CCDirector::sharedDirector()->getWinSize();

  auto renderTexture = CCRenderTexture::create(1, 1);
  auto oldPos = layer->getPosition();
  
  layer->setPosition({-size.width * pickPos.x, -size.height * pickPos.y});
  
  CCNode* parentBG = nullptr;
  CCSprite* bgSprite = nullptr;
	if (typeinfo_cast<PlayLayer* >(layer)) // Get from ShaderLayer (enabled when there are shaders in the level)
    parentBG = typeinfo_cast<ShaderLayer* >(layer->getChildren()->objectAtIndex(3));
  else
    parentBG = typeinfo_cast<ShaderLayer* >(layer->getChildren()->objectAtIndex(4));
    

	if (!parentBG) { 
		parentBG = static_cast<CCNode* >(layer->getChildren()->objectAtIndex(0)); // Get from normal CCNode

		if (Loader::get()->getLoadedMod("geode.node-ids")) // Get from Node IDs
			parentBG = layer->getChildByIDRecursive("main-node");
	} else
		parentBG = static_cast<CCNode* >(parentBG->getChildren()->objectAtIndex(1));
  
	bgSprite = getChildOfType<CCSprite>(parentBG, 0);

  renderTexture->begin(); // Rendering block
  
	if (pickBGColor && parentBG && bgSprite)
    bgSprite->visit();
  else
    layer->visit();
  
  renderTexture->end(); // Rendering block

  layer->setPosition(oldPos);

  auto img = renderTexture->newCCImage();
  auto data = img->getData();
  ccColor3B color = ccColor3B(data[0], data[1], data[2]);

  delete img;
  renderTexture->removeMeAndCleanup();

  return color;
}

void AmbientColor::setIconColor(ccColor3B color) {
  if (changeMainColor)
    player1->setColor(color);

  if (changeSecondaryColor)
    player1->setSecondColor(color);

  if (changeMainColorDual)
    player2->setColor(color);

  if (changeSecondaryColorDual)
    player2->setSecondColor(color);

  if (changeWaveTrail) {
    if (player1->m_isDart) {
      player1->m_waveTrail->setColor(color);
    }
    if (player2->m_isDart) {
      player2->m_waveTrail->setColor(color);
    }
  }

  if (changeGlowColor) {
    player1->m_glowColor = color;
    player1->updateGlowColor();

    player2->m_glowColor = color;
    player2->updateGlowColor();
  }

  if (!(layer->m_level->isPlatformer()) && Loader::get()->isModLoaded("dankmeme.globed2") && Mod::get()->getSettingValue<bool>("change-globed-icon") && typeinfo_cast<PlayLayer* >(layer))
    setGlobedIconColor(color);
}

void AmbientColor::setGlobedIconColor(ccColor3B color) {
  auto progressBarPlayer = layer->getChildByIDRecursive("dankmeme.globed2/self-player-progress");
  
  if (!progressBarPlayer)
    return;
    
  auto globedPlayer = static_cast<SimplePlayer* >(progressBarPlayer->getChildren()->objectAtIndex(1));

  if (changeMainColor)
    globedPlayer->setColor(color);

  if (changeSecondaryColor)
    globedPlayer->setSecondColor(color);

  if (changeGlowColor)
    globedPlayer->setGlowOutline(color);
}