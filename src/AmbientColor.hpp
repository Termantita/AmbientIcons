#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AmbientColor {
public:
  GJBaseGameLayer* layer;
  CCPoint pickPos;
  PlayerObject* player1 = layer->m_player1;
  PlayerObject* player2 = layer->m_player2;
  
  bool changeMainColor = Mod::get()->getSettingValue<bool>("change-main-color");
  bool changeSecondaryColor = Mod::get()->getSettingValue<bool>("change-secondary-color");
  bool changeMainColorDual = Mod::get()->getSettingValue<bool>("change-main-color-dual");
  bool changeSecondaryColorDual = Mod::get()->getSettingValue<bool>("change-secondary-color-dual");
  bool changeWaveTrail = Mod::get()->getSettingValue<bool>("change-wave-trail");
  bool changeGlowColor = Mod::get()->getSettingValue<bool>("change-glow-color");

  bool pickBGColor = Mod::get()->getSettingValue<bool>("pick-bg-color");
  
  AmbientColor(GJBaseGameLayer* layer, CCPoint pos)
    : layer{layer}, pickPos{pos} {};
  
  
  ccColor3B getScreenColor();
  void setIconColor(ccColor3B color);
  void setGlobedIconColor(ccColor3B color);
};