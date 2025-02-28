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
		WAVE_TRAIL,
		NONE
	};

	static ColorPicker getPicker() {
		if (Mod::get()->getSettingValue<std::string>("color-picker") == "Background")
			return ColorPicker::BG;
		return ColorPicker::SCREEN;
	}

	static Extra getExtra(bool isP2 = false) {
		auto setting = isP2 ? "change-p2-extra" : "change-p1-extra";
		auto option = Mod::get()->getSettingValue<std::string>(setting);
		
		if (option == "Wave Trail")
			return Extra::WAVE_TRAIL;
		if (option == "None")
			return Extra::NONE;
		return Extra::GLOW;
	}

	static PlayerColor getPlayerPreference(bool isP2 = false) {
		auto setting = isP2 ? "change-p2-color" : "change-p1-color";
		auto preference = Mod::get()->getSettingValue<std::string>(setting);

		if (preference == "Main")
			return PlayerColor::MAIN;
		if (preference == "Secondary")
			return PlayerColor::SECONDARY;
		return PlayerColor::BOTH;
	}
};
