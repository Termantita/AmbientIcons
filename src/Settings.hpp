#pragma once
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

	static inline bool enabled;
	static inline PlayerColor player1Color;
	static inline PlayerColor player2Color;
	static inline Extra player1Extra;
	static inline Extra player2Extra;
	static inline ColorPicker colorPicker;
	static inline bool changeMethodWhenBlack;
	static inline int updateTime;
	static inline CCPoint renderPosition;
	static inline bool drawRenderPosition;
	static inline bool debugColor;

	static void updateSettings() {
		enabled = Mod::get()->getSettingValue<bool>("mod-enabled");

		auto parseChangeColor = [](const std::string& key) {
			auto value = Mod::get()->getSettingValue<std::string>(key);
			if (value == "Main") {
				return MAIN;
			} else if (value == "Secondary") {
				return SECONDARY;
			} else if (value == "Both") {
				return BOTH;
			}

			return MAIN; // shouldn't happen
		};

		auto parseChangeExtra = [](const std::string& key) {
			auto value = Mod::get()->getSettingValue<std::string>(key);
			if (value == "None") {
				return NONE;
			} else if (value == "Wave Trail") {
				return WAVE_TRAIL;
			} else {
				return GLOW;
			}
		};

		player1Color = parseChangeColor("change-p1-color");
		player2Color = parseChangeColor("change-p2-color");
		player1Extra = parseChangeExtra("change-p1-extra");
		player2Extra = parseChangeExtra("change-p2-extra");

		auto colorp = Mod::get()->getSettingValue<std::string>("color-picker");
		if (colorp == "Background") {
			colorPicker = BG;
		} else {
			colorPicker = SCREEN;
		}

		changeMethodWhenBlack = Mod::get()->getSettingValue<bool>("on-black:change-to-screen-picker");
		updateTime = Mod::get()->getSettingValue<int64_t>("update-time");

		renderPosition = CCPoint{
			static_cast<float>(Mod::get()->getSettingValue<double>("render-x-pos")),
			static_cast<float>(Mod::get()->getSettingValue<double>("render-y-pos"))
		};

		drawRenderPosition = Mod::get()->getSettingValue<bool>("draw-pos");
		debugColor = Mod::get()->getSettingValue<bool>("debug-color");
	}
};
