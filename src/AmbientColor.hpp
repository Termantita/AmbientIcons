#pragma once

#include <Geode/Geode.hpp>

#include "Settings.hpp"
#include "ColorWrapper.hpp"

using namespace geode::prelude;

class AmbientColor : public CCNode {
protected:
	bool init(GJBaseGameLayer* layer) {
		if (!CCNode::init())
			return false;

		this->m_layer = layer;

		// // Idk why this doesn't work
		// this does not work because the player objects are created in PlayLayer::init and we hook GJBaseGameLayer::init !
		// this->m_player1 = layer->m_player1;
		// this->m_player2 = layer->m_player2;

		return true;
	}

private:
	WeakRef<GJBaseGameLayer> m_layer;
	Ref<CCRenderTexture> m_renderTexture;

	CCPoint m_pickPos;

	// PlayerObject* m_player1;
	// PlayerObject* m_player2;

	ColorWrapper getRenderColor(GJBaseGameLayer* layer, CCSprite* bgSprite, Settings::ColorPicker picker);
	CCSprite* getPickSprite(GJBaseGameLayer* layer);

public:
	static AmbientColor* create(GJBaseGameLayer* layer) {
		auto ret = new AmbientColor();
		if (ret && ret->init(layer)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	void onChange(float dt);
	ColorWrapper getScreenColor(GJBaseGameLayer* layer);
	void setIconColor(ColorWrapper color, PlayerObject* player, bool isP2 = false);
};