#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AmbientColor : public CCNode {
protected:
    bool init(GJBaseGameLayer* layer) {
        if (!CCNode::init())
            return false;
        
		this->m_layer = layer;

		this->m_player1 = layer->m_player1;
      	this->m_player2 = layer->m_player2;
        
      	m_changeMainColor = Mod::get()->getSettingValue<bool>("change-main-color");
      	m_changeSecondaryColor = Mod::get()->getSettingValue<bool>("change-secondary-color");
      	m_changeMainColorDual = Mod::get()->getSettingValue<bool>("change-main-color-dual");
      	m_changeSecondaryColorDual = Mod::get()->getSettingValue<bool>("change-secondary-color-dual");
      	m_changeWaveTrail = Mod::get()->getSettingValue<bool>("change-wave-trail");
      	m_changeGlowColor = Mod::get()->getSettingValue<bool>("change-glow-color");

      	m_pickBGColor = Mod::get()->getSettingValue<bool>("pick-bg-color");
		m_changeMethodWhenBlack = Mod::get()->getSettingValue<bool>("change-method-when-black");
		m_playerFollowPicker = Mod::get()->getSettingValue<bool>("player-follow-picker");

        return true;
    }
private:
	GJBaseGameLayer* m_layer;

	CCPoint m_pickPos;

	PlayerObject* m_player1;
	PlayerObject* m_player2;

	bool m_changeMainColor;
	bool m_changeSecondaryColor;
	bool m_changeMainColorDual;
	bool m_changeSecondaryColorDual;
	bool m_changeWaveTrail;
	bool m_changeGlowColor;
  
	bool m_pickBGColor;
	bool m_changeMethodWhenBlack;

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

	ccColor3B getScreenColor();
	void setIconColor(ccColor3B color);
	void setGlobedIconColor(ccColor3B color);

	void setPickBGColor() {
		m_pickBGColor = Mod::get()->getSettingValue<bool>("pick-bg-color");
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