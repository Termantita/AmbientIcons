#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Settings {
public:
	enum PlayerColor {
		COLOR_MAIN,
		COLOR_SECONDARY,
		COLOR_BOTH
	};

	enum ColorPicker {
		PICKER_BG,
		PICKER_SCREEN
	};

	enum Extra {
		EXTRA_GLOW,
		EXTRA_WAVE_TRAIL,
		EXTRA_NONE
	};

	enum InvertColor {
		INVERT_NONE,
		INVERT_P1,
		INVERT_P2,
		INVERT_BOTH
	};

	static inline bool enabled;

	static inline PlayerColor p1Color;
	static inline PlayerColor p2Color;
	static inline Extra p1Extra;
	static inline Extra p2Extra;
	static inline InvertColor invertColor;

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
				return COLOR_MAIN;
			} else if (value == "Secondary") {
				return COLOR_SECONDARY;
			} else if (value == "Both") {
				return COLOR_BOTH;
			}

			return COLOR_MAIN; // shouldn't happen
		};

		auto parseChangeExtra = [](const std::string& key) {
			auto value = Mod::get()->getSettingValue<std::string>(key);
			if (value == "None") {
				return EXTRA_NONE;
			} else if (value == "Wave Trail") {
				return EXTRA_WAVE_TRAIL;
			} else {
				return EXTRA_GLOW;
			}
		};

		auto parseInvertColor = [](const std::string& key) {
			auto value = Mod::get()->getSettingValue<std::string>(key);
			if (value == "None") {
				return INVERT_NONE;
			} else if (value == "P1") {
				return INVERT_P1;
			} else if (value == "P2") {
				return INVERT_P2;
			} else {
				return INVERT_BOTH;
			}
		};

		p1Color = parseChangeColor("change-p1-color");
		p2Color = parseChangeColor("change-p2-color");
		p1Extra = parseChangeExtra("change-p1-extra");
		p2Extra = parseChangeExtra("change-p2-extra");
		invertColor = parseInvertColor("invert-color");

		auto colorp = Mod::get()->getSettingValue<std::string>("color-picker");
		if (colorp == "Background") {
			colorPicker = PICKER_BG;
		} else {
			colorPicker = PICKER_SCREEN;
		}

		changeMethodWhenBlack = Mod::get()->getSettingValue<bool>("change-to-screen-picker");
		updateTime = Mod::get()->getSettingValue<int64_t>("update-time");

		renderPosition = CCPoint{
			static_cast<float>(Mod::get()->getSettingValue<double>("render-x-pos")),
			static_cast<float>(Mod::get()->getSettingValue<double>("render-y-pos"))
		};

		drawRenderPosition = Mod::get()->getSettingValue<bool>("draw-pos");
		debugColor = Mod::get()->getSettingValue<bool>("debug-color");
	}
};
