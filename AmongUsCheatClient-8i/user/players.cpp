#include "players.hpp"
using namespace app;

namespace Players {
	ImColor GetPlayersPlayerColor(PlayerControl* player) {
		auto data = GetPlayerData(player);
		return AmongUsColorToImVec4(GetPlayerColor(data->fields.ColorId));
	}

	ImColor GetPlayersPlayerColorStatus(PlayerControl* player) {
		auto data = GetPlayerData(player);
		if (data->fields.IsImpostor)
			return AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->ImpostorRed);
		else if (data->fields.IsDead)
			return AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->DisabledGrey);
		else
			return ImColor(0, 0, 0, 0);
	}

	void SquareConstraint(ImGuiSizeCallbackData* data)
	{
		data->DesiredSize = ImVec2(data->DesiredSize.x, data->DesiredSize.y);
	}

	void RenderPlayers(bool* state, float radarZoom) {
		if (IsInGame())
		{
			ImGui::SetNextWindowSize(ImVec2(256, 256));
			ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), SquareConstraint);

			ImGui::PushStyleColor(ImGuiCol_TitleBg, CCConstants::CC_GOLD);
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, CCConstants::CC_GOLD);
			ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, CCConstants::CC_GOLD);

			ImGui::Begin("Players", state, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 winpos = ImGui::GetWindowPos();
			ImVec2 winsize = ImGui::GetWindowSize();

			drawList->AddLine(ImVec2(winpos.x + winsize.x * 0.5F, winpos.y),
				ImVec2(winpos.x + winsize.x * 0.5F, winpos.y + winsize.y), ImColor(70, 70, 70, 255), 1.0F);
			drawList->AddLine(ImVec2(winpos.x, winpos.y + winsize.y * 0.5F),
				ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5F), ImColor(70, 70, 70, 255), 1.0F);

			PlayerControl* localPlayer = (*PlayerControl__TypeInfo)->static_fields->LocalPlayer;

			if (!localPlayer) {
				ImGui::End();
				return;
			}

			drawList->AddCircleFilled(ImVec2(winpos.x + winsize.x * 0.5F, winpos.y + winsize.y * 0.5F),
				CCConstants::ICON_SCALE, GetPlayersPlayerColor(localPlayer));
			drawList->AddCircle(ImVec2(winpos.x + winsize.x * 0.5F, winpos.y + winsize.y * 0.5F),
				CCConstants::ICON_SCALE + 0.5f, GetPlayersPlayerColorStatus(localPlayer), 0, 2.0F);

			Vector2 localPos = PlayerControl_GetTruePosition(localPlayer, NULL);

			for (auto player : GetAllPlayers()) {
				if (localPlayer == player)
					continue;

				Vector2 playerPos = PlayerControl_GetTruePosition(player, NULL);

				float radX = std::clamp(winpos.x + ((playerPos.x - localPos.x) * radarZoom + winsize.x * 0.5F),
					winpos.x + CCConstants::ICON_SCALE * 0.5F, winpos.x + winsize.x - CCConstants::ICON_SCALE * 0.5F);
				float radY = std::clamp(winpos.y + ((localPos.y - playerPos.y) * radarZoom + winsize.y * 0.5F),
					winpos.y + CCConstants::ICON_SCALE * 0.5F, winpos.y + winsize.y - CCConstants::ICON_SCALE * 0.5F);

				drawList->AddCircleFilled(ImVec2(radX, radY), CCConstants::ICON_SCALE, GetPlayersPlayerColor(player));
				drawList->AddCircle(ImVec2(radX, radY), CCConstants::ICON_SCALE + 0.5F, GetPlayersPlayerColorStatus(player), 0, 2.0F);
			}
		}
		ImGui::End();
	}
}