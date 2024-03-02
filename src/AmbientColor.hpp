#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AmbientColor {
private:
	GJBaseGameLayer* layer;
	CCPoint pickPos;

	PlayerObject* player1;
	PlayerObject* player2;

	bool changeMainColor;
	bool changeSecondaryColor;
	bool changeMainColorDual;
	bool changeSecondaryColorDual;
	bool changeWaveTrail;
	bool changeGlowColor;
  
	bool pickBGColor;
public:
  AmbientColor(GJBaseGameLayer* layer, CCPoint pos)
    : layer{layer}, pickPos{pos} {
	  	player1 = layer->m_player1;
      player2 = layer->m_player2;
        
      changeMainColor = Mod::get()->getSettingValue<bool>("change-main-color");
      changeSecondaryColor = Mod::get()->getSettingValue<bool>("change-secondary-color");
      changeMainColorDual = Mod::get()->getSettingValue<bool>("change-main-color-dual");
      changeSecondaryColorDual = Mod::get()->getSettingValue<bool>("change-secondary-color-dual");
      changeWaveTrail = Mod::get()->getSettingValue<bool>("change-wave-trail");
      changeGlowColor = Mod::get()->getSettingValue<bool>("change-glow-color");

      pickBGColor = Mod::get()->getSettingValue<bool>("pick-bg-color");

    };
  
  
  ccColor3B getScreenColor();
  void setIconColor(ccColor3B color);
  void setGlobedIconColor(ccColor3B color);
};