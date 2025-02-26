#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Settings {
public:
	enum PlayerColor {
		MAIN,
		SECONDARY,
		BOTH
	};

	enum ColorPicker {
		BG,
		SCREEN
	};

	enum Extra {
		GLOW,
		WAVE_TRAIL
	};

	static ColorPicker getPicker() {
		if (Mod::get()->getSettingValue<std::string>("color-picker") == "Background")
			return ColorPicker::BG;
		return ColorPicker::SCREEN;
	}

	static Extra getExtra() {
		if (Mod::get()->getSettingValue<std::string>("change-extra") == "Glow")
			return Extra::GLOW;
		return Extra::WAVE_TRAIL;
	}

	static PlayerColor getPlayerPreference(bool isP2 = false) {
		auto setting = isP2 ? "change-p2-color" : "change-p1-color";
		auto preference = Mod::get()->getSettingValue<std::string>(setting);

		if (preference == "MAIN")
			return PlayerColor::MAIN;
		if (preference == "SECONDARY")
			return PlayerColor::SECONDARY;
		return PlayerColor::BOTH;
	}
};
