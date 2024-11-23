#include "AmbientColor.hpp"

ccColor3B AmbientColor::getRenderColor(CCSprite *bgSprite) {
	auto renderTexture = CCRenderTexture::create(1, 1);

	renderTexture->begin(); // Rendering block

	if (m_BGColorPicker && bgSprite)
		bgSprite->visit();
	else
		m_layer->visit();

	renderTexture->end(); // Rendering block

	auto img = renderTexture->newCCImage();
	auto data = img->getData();
	ccColor3B color = ccColor3B(data[0], data[1], data[2]);

	delete img;
	renderTexture->removeMeAndCleanup();

	setBGColorPicker();
	setPlayerFollowColorPicker();

	return color;
}

ccColor3B AmbientColor::getScreenColor() {
	if (!(m_changeMainColor || m_changeSecondaryColor || m_changeMainColorDual ||
		m_changeSecondaryColorDual || m_changeWaveTrail || m_changeGlowColor))
		return ccColor3B(0.f, 0.f, 0.f);

	m_pickPos = CCPoint(getRenderXPos(), getRenderYPos());

	auto size = CCDirector::sharedDirector()->getWinSize();

	auto oldPos = m_layer->getPosition();

	m_layer->setPositionX(-size.width * m_pickPos.x);
	if (m_playerFollowPicker && m_player2->getPositionX() == 0 && m_player2->getPositionY() == 0) { // Follow player picker
		auto playerPos = m_player1->getPosition();
		auto playerPosOffset = playerPos.y * (getPlayerFollowOffset() + 1.f);
		
		// Convert player level pos to player screen pos 0-1.f (related to level -> related to screen)
		auto screenPlayerPosY = playerPosOffset / size.height < 1.f ? playerPosOffset / size.height : 1.f; 

		m_layer->setPositionY(-screenPlayerPosY * size.height);
	} else {
		m_layer->setPositionY(-size.height * m_pickPos.y); // Normal/fixed position picker
	}

	auto parent = m_layer->getChildByIDRecursive("main-node");
	auto bgSprite = static_cast<CCSprite *>(parent->getChildByID("background"));


	ccColor3B color = getRenderColor(bgSprite);


	if (color == ccColor3B(0, 0, 0) && m_changeMethodWhenBlack) {

		if (bgSprite) {
			m_BGColorPicker = false;
			color = getRenderColor(bgSprite);
		}		
		
		if (m_changeMethodToPlayerFollowWhenBlack) {
			m_playerFollowPicker = true;
			getScreenColor();
		}
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