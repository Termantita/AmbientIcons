#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ColorWrapper : public ccColor3B {
public:
	ColorWrapper(GLubyte r, GLubyte g, GLubyte b) {
		this->r = r;
		this->g = g;
		this->b = b; 
	}

	ColorWrapper(ccColor3B color3b) {
		this->r = color3b.r;
		this->g = color3b.g;
		this->b = color3b.b;
	}

	ColorWrapper operator-(ColorWrapper const& color) {
		GLubyte newR = abs(this->r - color.r);
		GLubyte newG = abs(this->g - color.g);
		GLubyte newB = abs(this->b - color.b);
		return ColorWrapper(newR, newG, newB);
	}

	ColorWrapper invert() {
		return ColorWrapper(255 - r, 255 - g, 255 - b);
	}

	ccColor3B toColor3B() {
		return ccColor3B(r, g, b);
	}
};
