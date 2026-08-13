//	Toastr - A notification system for Dear ImGui.
//	Copyright (c) 2026 Johan A. Goossens. All rights reserved.
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.


//
//	Include files
//

#include <format>
#include <iomanip>
#include <random>
#include <sstream>

#include "notification.h"


//
//	action
//

static bool action(const char* title, float hue) {
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(hue, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(hue, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(hue, 0.8f, 0.8f));
	bool result = ImGui::Button(title);
	ImGui::PopStyleColor(3);
	return result;
}


//
//	Notification::render
//

void Notification::render() {
	// start window
	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
	ImGui::Begin("Main Window", nullptr, windowFlags);
	ImGui::PushItemWidth(300.0f);

	// configuration
	ImGui::Spacing();
	ImGui::SeparatorText("Configuration:");
	ImGui::Spacing();

	ImGui::Checkbox("Window Border",&windowBorder);
	ImGui::SliderFloat("Window Rounding", &windowRounding, 0.0f, 20.0f);

	int width = static_cast<int>(textWidth);

	if (ImGui::SliderInt("Text Width", &width, 0, 200)) {
		textWidth = static_cast<size_t>(width);
	}

	ImGui::SliderFloat("Fade in Duration", &fadeInDuration, 0.0f, 4.0f);
	ImGui::SliderFloat("Fade out Duration", &fadeOutDuration, 0.0f, 4.0f);
	ImGui::SliderFloat("Ghost Duration", &ghostDuration, 0.0f, 4.0f);

	ImGui::Spacing();
	ImGui::SeparatorText("Notification Details");
	ImGui::Spacing();

	ImGui::InputText("Message", message, sizeof(message));
	ImGui::SliderFloat("Dismiss Time", &dismissTime, 0.0f, 60.0f);
	ImGui::PopItemWidth();

	// add notification messages
	ImGui::Spacing();
	ImGui::SeparatorText("Add Notification:");
	ImGui::Spacing();

	if (action("Success", 2.0f / 7.0f)) { toastr.Success(message, dismissTime); } ImGui::SameLine();
	if (action("Warning", 1.0f / 7.0f)) { toastr.Warning(message, dismissTime); } ImGui::SameLine();
	if (action("Error", 0.0f / 7.0f)) { toastr.Error(message, dismissTime); } ImGui::SameLine();
	if (action("Info", 4.0f / 7.0f)) { toastr.Info(message, dismissTime); } ImGui::SameLine();
	if (action("Random Burst", 6.0f / 7.0f)) { randomBurst(); }

	ImGui::End();

	// render notifications at the bottom right side of the window
	auto mainWindowSize = ImGui::GetMainViewport()->Size;
	auto mainWindowPos = ImGui::GetMainViewport()->Pos;
	auto anchor = mainWindowPos + mainWindowSize - ImGui::GetStyle().ItemSpacing;

	toastr.SetWindowRounding(windowRounding);
	toastr.SetWindowBorder(windowBorder);
	toastr.SetTextWidth(textWidth);
	toastr.SetFadeInDuration(fadeInDuration);
	toastr.SetFadeOuDuration(fadeOutDuration);
	toastr.SetGhostDuration(ghostDuration);
	toastr.Render(anchor, Toastr::AnchorType::bottomRight);
}


//
//	Notification::randomBurst
//

void Notification::randomBurst() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> notificationType(1, 4);
	std::uniform_int_distribution<int> messageType(0, 5);
	std::uniform_real_distribution<float> notificationDuration(1.0f, dismissTime);

	for (int i = 0; i < 8; i++) {
		auto type = notificationType(gen);
		auto message = messageType(gen);
		auto duration = notificationDuration(gen);

		static int id = 1;
		std::stringstream ss;
		ss << "Notification " << id++ << ": " << std::fixed << std::setprecision(2) << duration << "s";
		auto text = ss.str();

		switch (type) {
			case 1:  {
				static const char* const messages[] = {
					"Process completed successfully",
					"An unbelievable buzzer-beater",
					"Task finished with zero errors",
					"All systems go! Process done",
					"All tests passed! Your code is officially bulletproof",
					"Payment accepted. Enjoy your item"
				};

				toastr.Success(messages[message], duration);
				break;
			}

			case 2: {
				static const char* const messages[] = {
					"Low disk space detected",
					"High CPU temperature. Check cooling system",
					"Connection timeout. Retrying...",
					"Unrecognized device connected",
					"Memory usage exceeds 85%",
					"Security certificate expires soon"
				};

				toastr.Warning(messages[message], duration);
				break;
			}

			case 3: {
				static const char* const messages[] = {
					"Resource not found",
					"Internal server malfunction",
					"Access Denied: Invalid permissions",
					"Timeout: The operation took too long to complete",
					"Memory Allocation Failure: Out of system memory",
					"Invalid Input: Parameter format mismatch"
				};

				toastr.Error(messages[message], duration);
				break;
			}

			case 4:  {
				static const char* const messages[] = {
					"System is running smoothly",
					"Bring your own reusable bags",
					"New update is available",
					"50% off all fresh fruit today",
					"Buy one, get one free on milk",
					"Free coffee samples in aisle 4"
				};

				toastr.Info(messages[message], duration);
				break;
			}
		}
	}
}
