//	Toastr - A notification system for Dear ImGui.
//	Copyright (c) 2026 Johan A. Goossens. All rights reserved.
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.


#pragma once


//
//	Include files
//

#include "../Toastr.h"


//
//  Notification
//

class Notification {
public:
	// render a frame
	void render();

private:
	// palette type
	enum class ImGuiPalette {
		dark,
		light
	};

	static constexpr const char* imGuiPalettes[] = {
		"Dark",
		"Light"
	};

	static constexpr size_t imGuiPaletteCount = sizeof(imGuiPalettes) / sizeof(*imGuiPalettes);

	enum class ToastrPalette {
		dark,
		saturated,
		pastel,
		custom
	};

	static constexpr const char* toastrPalettes[] = {
		"Dark",
		"Saturated",
		"Pastel",
		"Custom"
	};

	static constexpr size_t toastrPaletteCount = sizeof(toastrPalettes) / sizeof(*toastrPalettes);

	// properties
	Toastr toastr;
	ImGuiPalette imGuiPalette = ImGuiPalette::dark;
	ToastrPalette toastrPalette = ToastrPalette::dark;
	float windowRounding = 6.0f;
	bool windowBorder = false;
	float fadeInDuration = 0.4f;
	float fadeOutDuration = 0.4f;
	float ghostDuration = 0.3f;
	bool iconVisible = true;
	bool customIconRenderer = false;
	char message[256] = "Test Notification";
	float displayTime = 10.0f;
	size_t textWidth = 30;
	Toastr::Palette customPalette = Toastr::GetDarkPalette();
	bool showDebugWindow = false;
	size_t lastNotice = 0;

	// support functions
	void editColor(Toastr::Color color);
	void oneOfEach();
	void randomBurst();
};