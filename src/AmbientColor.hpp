#include <Geode/Geode.hpp>

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


		// Color pickers
		m_changeMethodWhenBlack =
			Mod::get()->getSettingValue<bool>("change-method-when-black");

		m_changeMethodToPlayerFollowWhenBlack =
			Mod::get()->getSettingValue<bool>("change-to-follow-method-when-black");
		m_playerFollowPicker =
			Mod::get()->getSettingValue<bool>("player-follow-picker");

		return true;
	}

private:
	GJBaseGameLayer* m_layer;

	CCPoint m_pickPos;

	// PlayerObject* m_player1;
	// PlayerObject* m_player2;

	// Color pickers
	bool m_changeMethodWhenBlack;

	bool m_changeMethodToPlayerFollowWhenBlack;
	bool m_playerFollowPicker;

	ccColor3B getRenderColor(CCSprite* bgSprite);

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
	void setIconColor(ccColor3B color);

	void setPlayerFollowColorPicker() {
		m_playerFollowPicker =
			Mod::get()->getSettingValue<bool>("player-follow-picker");
	}

	double getRenderXPos() {
		return Mod::get()->getSettingValue<double>("render-x-pos");
	}

	double getRenderYPos() {
		return Mod::get()->getSettingValue<double>("render-y-pos");
	}

	double getPlayerFollowOffset() {
		return Mod::get()->getSettingValue<double>("player-follow-offset");
	}
};