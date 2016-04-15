#pragma once

struct Options {
	unsigned int DisplayHeight, DisplayWidth;
	int WindowType;

	Options() {
		DisplayHeight = 600;
		DisplayWidth = 800;
		WindowType = SW_SHOWMAXIMIZED;
	}
};