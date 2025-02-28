#include <Geode/Geode.hpp>

#include "Settings.hpp"

using namespace geode::prelude;

class AmbientColor : public CCNode {
protected:
	bool init(GJBaseGameLayer* layer) {
		if (!CCNode::init())
			return false;

		this->m_layer = layer;

		// Idk why this doesn't work
		// this->m_player1 = layer->m_player1;
		// this->m_player2 = layer->m_player2;

		m_changeMethodWhenBlack =
			Mod::get()->getSettingValue<bool>("on-black:change-to-screen-picker");

		return true;
	}

private:
	GJBaseGameLayer* m_layer;

	CCPoint m_pickPos;

	// PlayerObject* m_player1;
	// PlayerObject* m_player2;

	bool m_changeMethodWhenBlack;
	bool m_isFinished = true;

	ccColor3B getRenderColor(CCSprite* bgSprite, Settings::ColorPicker picker);
	CCSprite* getPickSprite();

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

	void onChange(CCObject* sender);
	ccColor3B getScreenColor();
	void setIconColor(ccColor3B color, PlayerObject* player, bool isP2 = false);

	double getRenderXPos() {
		return Mod::get()->getSettingValue<double>("render-x-pos");
	}

	double getRenderYPos() {
		return Mod::get()->getSettingValue<double>("render-y-pos");
	}

	// Have to do this bc of CCSequence being shit
	bool isActionFinished() {
		return m_isFinished;
	}

	void onFinish(CCObject* sender) {
		m_isFinished = true;
	}
};