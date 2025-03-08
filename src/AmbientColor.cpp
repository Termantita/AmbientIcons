#include "AmbientColor.hpp"

void AmbientColor::onChange(float dt) {
	auto layer = m_layer.lock();

	if (!layer) {
		// this means the user most likely exited the level
		return;
	}

	auto color = getScreenColor(layer);

	setIconColor(color, layer->m_player1);
	setIconColor(color, layer->m_player2, true);
}

ccColor3B AmbientColor::getRenderColor(GJBaseGameLayer* layer, CCSprite* bgSprite, Settings::ColorPicker picker) {
	if (!m_renderTexture) {
		m_renderTexture = CCRenderTexture::create(1, 1);
	}

	m_renderTexture->begin(); // Rendering block

	if (picker == Settings::BG && bgSprite)
		bgSprite->visit();
	else
		layer->visit();

	m_renderTexture->end(); // Rendering block

	auto img = m_renderTexture->newCCImage();
	auto data = img->getData();
	ccColor3B color = ccColor3B{data[0], data[1], data[2]};

	delete img;

	return color;
}

CCSprite* AmbientColor::getPickSprite(GJBaseGameLayer* layer) {
	auto parent = layer->getChildByIDRecursive("main-node");
	CCSprite* bgSprite = nullptr;

	if (parent->getChildByID("background"))
		bgSprite = static_cast<CCSprite *>(parent->getChildByID("background"));
	else {
		bgSprite = parent->getChildByType<CCSprite>(0);
	}
	return bgSprite;
}

ccColor3B AmbientColor::getScreenColor(GJBaseGameLayer* layer) {
	m_pickPos = Settings::renderPosition;

	auto size = CCDirector::sharedDirector()->getWinSize();

	auto oldPos = layer->getPosition();

	// For getting the exact position on the screen, we will move the layer itself, because of the RenderTexture capabilities
	// For m_layer (which is the layer), we need negative numbers to move the layer off screen and get the exact position
	layer->setPositionX(-size.width * m_pickPos.x);
	layer->setPositionY(-size.height * m_pickPos.y);

	if (Settings::drawRenderPosition) {
		if (auto spr = layer->getChildByIDRecursive("render-pos-circle"_spr))
			spr->removeMeAndCleanup();

		auto circleSpr = CCSprite::create("circle.png");
		circleSpr->setPosition({static_cast<float>(size.width * m_pickPos.x), static_cast<float>(size.height * m_pickPos.y)});
		circleSpr->setID("render-pos-circle"_spr);

		layer->addChild(circleSpr);
	}


	auto pickSprite = getPickSprite(layer);
	ccColor3B color = getRenderColor(layer, pickSprite, Settings::colorPicker);

	if (Settings::debugColor)
		log::info("RGB: {} {} {}", color.r, color.g, color.b);

	if (color == ccColor3B{0, 0, 0} && m_changeMethodWhenBlack) {
		color = getRenderColor(layer, pickSprite, Settings::ColorPicker::SCREEN);
	}

	layer->setPosition(oldPos);

	return color;
}

void AmbientColor::setIconColor(ccColor3B color, PlayerObject* player, bool isP2) {
	// TODO: Fix bad colors after dashing without having both colors enabled
	switch (isP2 ? Settings::player2Color : Settings::player1Color) {
		case Settings::BOTH:
			player->setColor(color);
			player->setSecondColor(color);
			break;
		case Settings::MAIN:
			player->setColor(color);
			break;
		case Settings::SECONDARY:
			player->setSecondColor(color);
			break;
	}

	switch (isP2 ? Settings::player2Extra : Settings::player1Extra) {
		case Settings::GLOW:
			player->m_glowColor = color;
			player->updateGlowColor();
			break;
		case Settings::WAVE_TRAIL:
			if (player->m_isDart)
				player->m_waveTrail->setColor(color);
			break;
		case Settings::NONE:
			break;
	}
}