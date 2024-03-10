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

ccColor3B AmbientColor::getRenderColor(CCSprite* bgSprite) {
	auto renderTexture = CCRenderTexture::create(1, 1);

	renderTexture->begin(); // Rendering block
  
	if (m_pickBGColor && bgSprite)
		bgSprite->visit();
	else
		m_layer->visit();
  
  	renderTexture->end(); // Rendering block


  	auto img = renderTexture->newCCImage();
  	auto data = img->getData();
  	ccColor3B color = ccColor3B(data[0], data[1], data[2]);
	
  	delete img;
  	renderTexture->removeMeAndCleanup();

	setPickBGColor();

	return color;
}

ccColor3B AmbientColor::getScreenColor() {
	if (!(m_changeMainColor || m_changeSecondaryColor || m_changeMainColorDual || m_changeSecondaryColorDual || m_changeWaveTrail || m_changeGlowColor))
		return ccColor3B(0.f, 0.f, 0.f);

  	m_pickPos = CCPoint(globalXPos, globalYPos);

  	auto size = CCDirector::sharedDirector()->getWinSize();

  	auto oldPos = m_layer->getPosition();
  
  	m_layer->setPosition({-size.width * m_pickPos.x, -size.height * m_pickPos.y});
  
	CCSprite* bgSprite = nullptr;
	if (typeinfo_cast<PlayLayer* >(m_layer)) {
		auto parent = m_layer->getChildByIDRecursive("main-node");
		bgSprite = getChildOfType<CCSprite>(parent, 0);
	} else {
		auto parent = m_layer->getChildByIDRecursive("main-node");
		bgSprite = static_cast<CCSprite* >(parent->getChildByID("background"));
	}
	
	ccColor3B color = getRenderColor(bgSprite);
  	
	if (color == ccColor3B(0, 0, 0) && bgSprite && m_changeMethodWhenBlack) {
		m_pickBGColor = false;
		color = getRenderColor(bgSprite);
	}
	
	m_layer->setPosition(oldPos);

  	return color;
}

void AmbientColor::setIconColor(ccColor3B color) {
  	if (m_changeMainColor)
		m_player1->setColor(color);

  	if (m_changeSecondaryColor)
		m_player1->setSecondColor(color);

  	if (m_changeMainColorDual)
		m_player2->setColor(color);

  	if (m_changeSecondaryColorDual)
		m_player2->setSecondColor(color);

  	if (m_changeWaveTrail) {
		if (m_player1->m_isDart) {
	  		m_player1->m_waveTrail->setColor(color);
		}
		if (m_player2->m_isDart) {
	  		m_player2->m_waveTrail->setColor(color);
		}
  	}

  	if (m_changeGlowColor) {
		m_player1->m_glowColor = color;
		m_player1->updateGlowColor();

		m_player2->m_glowColor = color;
		m_player2->updateGlowColor();
  	}
}