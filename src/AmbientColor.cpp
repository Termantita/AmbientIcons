#include "AmbientColor.hpp"

void AmbientColor::onChange(float dt) {
	auto layer = m_layer.lock();

	if (!layer) {
		// this means the user most likely exited the level
		return;
	}

	auto color = getScreenColor(layer);

	auto p1Color = color;
	auto p2Color = color;
	auto invertedColor = color.invert();

	if (Settings::invertColor == Settings::INVERT_BOTH) {
		p1Color = invertedColor;
		p2Color = invertedColor;
	} else if (Settings::invertColor == Settings::INVERT_P1) {
		p1Color = invertedColor;
	} else if (Settings::invertColor == Settings::INVERT_P2) {
		p2Color = invertedColor;
	}

	setIconColor(p1Color, layer->m_player1);
	setIconColor(p2Color, layer->m_player2, true);
}

ColorWrapper AmbientColor::getRenderColor(GJBaseGameLayer* layer, CCSprite* bgSprite, Settings::ColorPicker picker) {
	if (!m_renderTexture) {
		m_renderTexture = CCRenderTexture::create(1, 1);
	}

	m_renderTexture->begin(); // Rendering block

	if (picker == Settings::PICKER_BG && bgSprite)
		bgSprite->visit();
	else
		layer->visit();

	m_renderTexture->end(); // Rendering block

	auto img = m_renderTexture->newCCImage();
	auto data = img->getData();
	ColorWrapper color = ColorWrapper(data[0], data[1], data[2]);

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

ColorWrapper AmbientColor::getScreenColor(GJBaseGameLayer* layer) {
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
	ColorWrapper color = getRenderColor(layer, pickSprite, Settings::colorPicker);

	if (Settings::debugColor) {
		ColorWrapper invertedColor = color.invert();
		log::info("Normal: ({} {} {}) | Inverted: ({} {} {})", color.r, color.g, color.b, invertedColor.r, invertedColor.g, invertedColor.b);
	}

	if (color == ColorWrapper(0, 0, 0) && Settings::changeMethodWhenBlack) {
		color = getRenderColor(layer, pickSprite, Settings::ColorPicker::PICKER_SCREEN);
	}

	layer->setPosition(oldPos);

	return color;
}



void AmbientColor::setIconColor(ColorWrapper color, PlayerObject* player, bool isP2) {
	// TODO: Fix bad colors after dashing without having both colors enabled

	switch (isP2 ? Settings::p2Color : Settings::p1Color) {
		case Settings::COLOR_BOTH:
			player->setColor(color);
			player->setSecondColor(color);
			break;
		case Settings::COLOR_MAIN:
			player->setColor(color);
			break;
		case Settings::COLOR_SECONDARY:
			player->setSecondColor(color);
			break;
	}

	switch (isP2 ? Settings::p2Extra : Settings::p1Extra) {
		case Settings::EXTRA_GLOW:
			player->m_glowColor = color;
			player->updateGlowColor();
			break;
		case Settings::EXTRA_WAVE_TRAIL:
			if (player->m_isDart)
				player->m_waveTrail->setColor(color);
			break;
		case Settings::EXTRA_NONE:
			break;
	}
}