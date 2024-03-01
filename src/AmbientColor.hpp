#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AmbientColor {
public:
  GJBaseGameLayer* layer;
  CCPoint pickPos;
  PlayerObject* player1 = layer->m_player1;
  PlayerObject* player2 = layer->m_player2;
  
  AmbientColor(GJBaseGameLayer* layer, CCPoint pos)
    : layer{layer}, pickPos{pos} {};
  
  
  ccColor3B getScreenColor();
  void setIconColor(ccColor3B color);
};