//	Toastr - A notification system for Dear ImGui.
//	Copyright (c) 2026 Johan A. Goossens. All rights reserved.
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.


//
//	Include files
//

#include <iomanip>
#include <random>
#include <sstream>

#include "notification.h"


//
//	selectorEnum
//

template <typename T>
static bool selectorEnum(const char* label, T* value, const char* const names[], size_t count) {
	auto changed = false;

	if (ImGui::BeginCombo(label, names[static_cast<size_t>(*value)])) {
		for (size_t i = 0; i < count; i++) {
			if (ImGui::Selectable(names[i], static_cast<size_t>(*value) == i)) {
				if (static_cast<size_t>(*value) != i) {
					*value = static_cast<T>(i);
					changed = true;
				}
			}

			if (i == static_cast<size_t>(*value)) {
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	return changed;
}


//
//	action
//

static bool action(const char* title, float hue) {

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(hue, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(hue, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(hue, 0.8f, 0.8f));
	bool result = ImGui::Button(title);
	ImGui::PopStyleColor(4);
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

	if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Alt | ImGuiKey_I, ImGuiInputFlags_RouteAlways)) {
		showDebugWindow = !showDebugWindow;
	}

	// configuration
	ImGui::Spacing();
	ImGui::SeparatorText("Configuration:");
	ImGui::Spacing();

	ImGui::Checkbox("Window Border",&windowBorder);
	ImGui::SliderFloat("Window Rounding", &windowRounding, 0.0f, 20.0f);

	if (selectorEnum("Dear ImGui Palette", &imGuiPalette, imGuiPalettes, imGuiPaletteCount)) {
		switch (imGuiPalette) {
			case ImGuiPalette::dark: ImGui::StyleColorsDark(); break;
			case ImGuiPalette::light: ImGui::StyleColorsLight(); break;
		}
	}

	if (selectorEnum("Toastr Palette", &toastrPalette, toastrPalettes, toastrPaletteCount)) {
		switch (toastrPalette) {
			case ToastrPalette::dark: toastr.SetPalette(toastr.GetDarkPalette()); break;
			case ToastrPalette::saturated: toastr.SetPalette(toastr.GetSaturatedPalette()); break;
			case ToastrPalette::pastel: toastr.SetPalette(toastr.GetPastelPalette()); break;
			case ToastrPalette::custom: toastr.SetPalette(customPalette); break;
		}
	}

	int width = static_cast<int>(textWidth);

	if (ImGui::SliderInt("Text Width (in glyphs)", &width, 0, 200)) {
		textWidth = static_cast<size_t>(width);
	}

	ImGui::SliderFloat("Fade in Duration", &fadeInDuration, 0.0f, 4.0f);
	ImGui::SliderFloat("Fade out Duration", &fadeOutDuration, 0.0f, 4.0f);
	ImGui::SliderFloat("Ghost Duration", &ghostDuration, 0.0f, 4.0f);

	// custom palette editor
	if (toastrPalette == ToastrPalette::custom) {
		ImGui::Spacing();
		ImGui::SeparatorText("Custom Palette:");
		ImGui::Spacing();

		if (ImGui::Button("Edit")) {
			ImGui::OpenPopup("CustomPaletteEditor");
		}

		ImGui::SameLine();

		if (ImGui::Button("Copy from Dark")) {
			customPalette = Toastr::GetDarkPalette();
			toastr.SetPalette(customPalette);
		}

		ImGui::SameLine();

		if (ImGui::Button("Copy from Saturated")) {
			customPalette = Toastr::GetSaturatedPalette();
			toastr.SetPalette(customPalette);
		}

		ImGui::SameLine();

		if (ImGui::Button("Copy from Pastel")) {
			customPalette = Toastr::GetPastelPalette();
			toastr.SetPalette(customPalette);
		}
	}

	if (ImGui::BeginPopup("CustomPaletteEditor")) {
		if (ImGui::BeginTable("palette", 3)) {
			auto cellWidth = ImGui::CalcTextSize("#").x * 30.0f;
#define W() ImGui::SetNextItemWidth(cellWidth)

			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextUnformatted("");
			ImGui::TableNextColumn(); W(); ImGui::TextUnformatted("Message");
			ImGui::TableNextColumn(); W(); ImGui::TextUnformatted("Icon");

			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextUnformatted("Success");
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::successBackground);
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::successIcon);

			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextUnformatted("Warning");
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::warningBackground);
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::warningIcon);

			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextUnformatted("Error");
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::errorBackground);
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::errorIcon);

			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextUnformatted("Info");
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::infoBackground);
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::infoIcon);

			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextUnformatted("Text");
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::messageColor);
			ImGui::TableNextColumn(); W(); editColor(Toastr::Color::iconColor);
#undef W

			ImGui::EndTable();
		}

		ImGui::EndPopup();
	}

	// update configuration
	toastr.SetWindowRounding(windowRounding);
	toastr.SetWindowBorder(windowBorder);
	toastr.SetTextWidth(textWidth);
	toastr.SetFadeInDuration(fadeInDuration);
	toastr.SetFadeOuDuration(fadeOutDuration);
	toastr.SetGhostDuration(ghostDuration);

	if (ImGui::Checkbox("Show Progress Bar", &progressVisible)) {
		toastr.SetProgressVisible(progressVisible);
	}

	if (ImGui::Checkbox("Show Icon", &iconVisible)) {
		toastr.SetIconVisible(iconVisible);
	}

	ImGui::SameLine();

	if (ImGui::Checkbox("Custom Icon Renderer", &customIconRenderer)) {
		if (customIconRenderer) {
			toastr.SetCustomIconRenderer([](Toastr::CustomIcon& data) {
				auto radius = data.size * 0.5f;
				ImU32 color = 0;
				char letter = ' ';

				switch (data.type) {
					case Toastr::NotificationType::success:
						color = ImGui::GetColorU32(data.palette->get(Toastr::Color::successIcon), data.alpha * 0.5f);
						letter = 'S';
						break;

					case Toastr::NotificationType::warning:
						color = ImGui::GetColorU32(data.palette->get(Toastr::Color::warningIcon), data.alpha * 0.5f);
						letter = 'W';
						break;

					case Toastr::NotificationType::error:
						color = ImGui::GetColorU32(data.palette->get(Toastr::Color::errorIcon), data.alpha * 0.5f);
						letter = 'E';
						break;

					case Toastr::NotificationType::info:
						color = ImGui::GetColorU32(data.palette->get(Toastr::Color::infoIcon), data.alpha * 0.5f);
						letter = 'I';
						break;
				}

				data.drawList->AddCircleFilled(data.center, radius, color);
				ImGui::PushFont(nullptr, radius);
				auto textSize = ImGui::CalcTextSize("#");
				auto pos = data.center - ImVec2(textSize.x * 0.5f, textSize.y * 0.5f);
				color = ImGui::GetColorU32(data.palette->get(Toastr::Color::iconColor), data.alpha);
				data.drawList->AddText(pos, color, &letter, (&letter + 1));
				ImGui::PopFont();
			});

		} else {
			toastr.ClearCustomIconRenderer();
		}
	}

	// show examples
	ImGui::Spacing();
	ImGui::SeparatorText("Example Notification:");
	ImGui::Spacing();

	toastr.RenderSuccessSample(); ImGui::SameLine();
	toastr.RenderWarningSample();
	toastr.RenderErrorSample(); ImGui::SameLine();
	toastr.RenderInfoSample();

	// add notification messages
	ImGui::Spacing();
	ImGui::SeparatorText("Add Notifications:");
	ImGui::Spacing();

	ImGui::InputText("Message", message, sizeof(message));
	ImGui::SliderFloat("Display Time", &displayTime, 0.0f, 60.0f);
	ImGui::PopItemWidth();

	if (action("Success", 2.0f / 7.0f)) { lastNotice = toastr.Success(message, displayTime); } ImGui::SameLine();
	if (action("Warning", 1.0f / 7.0f)) { lastNotice = toastr.Warning(message, displayTime); } ImGui::SameLine();
	if (action("Error", 0.0f / 7.0f)) { lastNotice = toastr.Error(message, displayTime); } ImGui::SameLine();
	if (action("Info", 4.0f / 7.0f)) { lastNotice = toastr.Info(message, displayTime); }
	if (ImGui::Button("One of Each")) { oneOfEach(); } ImGui::SameLine();
	if (ImGui::Button("Random Burst")) { randomBurst(); }

	// remove notification messages
	ImGui::Spacing();
	ImGui::SeparatorText("Remove Notifications:");
	ImGui::Spacing();

	if (ImGui::Button("Cancel Last")) { toastr.Delete(lastNotice); } ImGui::SameLine();
	if (ImGui::Button("Delete All")) { toastr.DeleteAll(); }

	ImGui::End();

	// show Dear ImGui metrics (if required)
	if (showDebugWindow) {
		ImGui::ShowMetricsWindow();
	}
	// render notifications at the bottom right side of the window
	auto mainWindowSize = ImGui::GetMainViewport()->Size;
	auto mainWindowPos = ImGui::GetMainViewport()->Pos;
	auto anchor = mainWindowPos + mainWindowSize - ImGui::GetStyle().ItemSpacing;

	// render notifications
	toastr.Render(anchor, Toastr::AnchorType::bottomRight);
}


//
//	Notification::editColor
//

void Notification::editColor(Toastr::Color color) {
	std::stringstream ss;
	ss << "##color" << static_cast<size_t>(color);
	auto label = ss.str();

	ImVec4 color4 = ImGui::ColorConvertU32ToFloat4(customPalette.get(color));

	if (ImGui::ColorEdit4(label.c_str(), (float*) &color4)) {
		customPalette.set(color, ImGui::ColorConvertFloat4ToU32(color4));
		toastr.SetPalette(customPalette);
	}
}


//
//	Notification::oneOfEach
//

void Notification::oneOfEach() {
	lastNotice = toastr.Success("Success", displayTime);
	lastNotice = toastr.Warning("Warning", displayTime);
	lastNotice = toastr.Error("Error", displayTime);
	lastNotice = toastr.Info("Info", displayTime);
}


//
//	Notification::randomBurst
//

void Notification::randomBurst() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> notificationType(1, 4);
	std::uniform_int_distribution<int> messageType(0, 5);
	std::uniform_real_distribution<float> notificationDuration(1.0f, displayTime);

	for (int i = 0; i < 8; i++) {
		auto randomType = notificationType(gen);
		auto randomMessage = messageType(gen);
		auto randomDuration = notificationDuration(gen);

		switch (randomType) {
			case 1:  {
				static const char* const messages[] = {
					"Process completed successfully",
					"An unbelievable buzzer-beater",
					"Task finished with zero errors",
					"All systems go! Process done",
					"All tests passed! Your code is officially bulletproof",
					"Payment accepted. Enjoy your item"
				};

				lastNotice = toastr.Success(messages[randomMessage], randomDuration);
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

				lastNotice = toastr.Warning(messages[randomMessage], randomDuration);
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

				lastNotice = toastr.Error(messages[randomMessage], randomDuration);
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

				lastNotice = toastr.Info(messages[randomMessage], randomDuration);
				break;
			}
		}
	}
}
