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
	// properties
	Toastr toastr;
	float windowRounding = 6.0f;
	bool windowBorder = false;
	float fadeInDuration = 0.4f;
	float fadeOutDuration = 0.4f;
	float ghostDuration = 0.3f;
	char message[256] = "Test Notification";
	float dismissTime = 4.0f;
	size_t textWidth = 30;

	// support functions
	void randomBurst();
};