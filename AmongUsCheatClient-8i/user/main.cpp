#include "il2cpp-appdata.h"
#include "pch-il2cpp.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <thread>
#include "GameUtility.hpp"
#include "StringUtility.hpp"
#include "IterationCounter.hpp"
#include "D3D11Hooking.hpp"
#include "CCState.hpp"
#include "CCConstants.hpp"
#include "magic_enum.hpp"
#include "detours.h"
#include "helpers.h"
#include "radar.hpp"
#include "players.hpp"

using namespace app;

extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

IDXGISwapChain* SwapChain;
ID3D11Device* Device;
ID3D11DeviceContext* Ctx;
ID3D11RenderTargetView* RenderTargetView;

D3D_PRESENT_FUNCTION OriginalD3DFunction;
WNDPROC OriginalWndProcFunction;
void (*OriginalHudFunction)(MethodInfo*);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    POINT mPos;
    GetCursorPos(&mPos);
    ScreenToClient(CCState::Window, &mPos);

    ImGui::GetIO().MousePos.x = mPos.x;
    ImGui::GetIO().MousePos.y = mPos.y;

    if (uMsg == WM_KEYUP && wParam == VK_DELETE)
        CCState::ShowMenu = !CCState::ShowMenu;

    if (CCState::ShowMenu)
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    return CallWindowProcW(OriginalWndProcFunction, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall D3D_FUNCTION_HOOK(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags)
{
    if (!CCState::ImGuiInitialized)
    {
        SwapChain = pThis;
        pThis->GetDevice(__uuidof(ID3D11Device), (void**)&Device);
        Device->GetImmediateContext(&Ctx);

        DXGI_SWAP_CHAIN_DESC desc;

        pThis->GetDesc(&desc);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        //ImGui::StyleColorsDark();
        ImGui::StyleColorsClassic();

        CCState::Window = desc.OutputWindow;

        ImGui_ImplWin32_Init(CCState::Window);
        ImGui_ImplDX11_Init(Device, Ctx);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        OriginalWndProcFunction = (WNDPROC)SetWindowLongW(CCState::Window, GWLP_WNDPROC, (LONG)WndProcHook);

        ImGui::GetIO().ImeWindowHandle = CCState::Window;

        ID3D11Texture2D* pBackBuffer;
        pThis->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        Device->CreateRenderTargetView(pBackBuffer, NULL, &RenderTargetView);
        pBackBuffer->Release();

        ImGui::GetStyle().WindowRounding = 13.0F;
        ImGui::GetStyle().ChildRounding = 100.0F;
        ImGui::GetStyle().FrameRounding = 13.0F;
        ImGui::GetStyle().GrabRounding = 100.0F;
        ImGui::GetStyle().PopupRounding = 100.0F;
        ImGui::GetStyle().ScrollbarRounding = 100.0F;

        CCState::ImGuiInitialized = true;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //ImDrawList* drawList = ImGui::GetWindowDrawList();

    //Add welcome window

    if (CCState::ShowRadar) {
        Radar::RenderRadar(&CCState::ShowRadar, CCState::RadarZoom);
    }

    if (CCState::ShowPlayers) {
        Players::RenderPlayers(&CCState::ShowPlayers, CCState::PlayersZoom);
    }

    //if (CCState::ShowMenu && IsInGame())
    if (CCState::ShowMenu)
    {
        ImGui::PushStyleColor(ImGuiCol_TitleBg, CCConstants::CC_GOLD);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, CCConstants::CC_GOLD);
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, CCConstants::CC_GOLD);
        ImGui::Begin("Among Us Cheat Client", &CCState::ShowMenu);
        ImGui::PopStyleColor(2);

        ImGui::SetWindowSize(ImVec2(400, 600));

        ImGui::Text("Among Us Cheat Client");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Created by Escartem");
        ImGui::Spacing();
        ImGui::Text("github.com/Escartem/AmongUsCheatClient");
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Warning : This is a beta version of the client");
        ImGui::Text("There could be a lot of elements that are not\nworking and may crash the game");
        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Game"))
        {
            if (ImGui::Button("Call Meeting"))
                PlayerControl_CmdReportDeadBody((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL, NULL);

            ImGui::Checkbox("NoClip", &CCState::NoClip);
            ImGui::Checkbox("Mark Imposters", &CCState::MarkImposters);
            ImGui::Checkbox("Show Ghosts", &CCState::ShowGhosts);
            ImGui::Checkbox("Radar", &CCState::ShowRadar);
            ImGui::Text("Radar Zoom");
            ImGui::SliderFloat("##RadarZoom", &CCState::RadarZoom, 4.0F, 16.0F, "%.f", 1.0F);
            ImGui::Checkbox("Players", &CCState::ShowPlayers);
        }

        if (ImGui::CollapsingHeader("Ban Points"))
        {
            if ((*StatsManager__TypeInfo)->static_fields->Instance == NULL)
            {
                ImGui::Text("Stats manager is not yet initialized.\nTry connecting to the public matchmaker.");
            }
            else
            {
                if (ImGui::Button("Reset"))
                {
                    (*StatsManager__TypeInfo)->static_fields->Instance->fields.banPoints = 0.0F;
                    StatsManager_SaveStats((*StatsManager__TypeInfo)->static_fields->Instance, NULL);
                }

                ImGui::Text(("Ban Points: " + std::to_string((*StatsManager__TypeInfo)->static_fields->Instance->fields.banPoints)).c_str());
            }
        }

        if (ImGui::CollapsingHeader("Chat"))
        {
            ImGui::Checkbox("All Players Spam", &CCState::AllPlayersSpam);
            ImGui::Checkbox("Spam Chat", &CCState::SpamChat);
            ImGui::Text("Message");
            ImGui::InputText("##SpamChatMessage", CCState::SpamMessage, IM_ARRAYSIZE(CCState::SpamMessage));
            ImGui::Text("Interval");
            CCState::ChatCounter.GenerateInput("##SpamChatInverval");
        }

        //if (ImGui::CollapsingHeader("Players") && IsInGame())
        if (ImGui::CollapsingHeader("Players"))
        {
            if (GetAllPlayers().size() > 0
                && CCState::MurderQueue.empty())
            {
                if (ImGui::Button("Murder Crewmates"))
                {
                    for (auto player : GetAllPlayers())
                    {
                        if (!GetPlayerData(player)->fields.IsImpostor)
                            CCState::MurderQueue.push(player);
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button("Murder Imposters"))
                {
                    for (auto player : GetAllPlayers())
                    {
                        if (GetPlayerData(player)->fields.IsImpostor)
                            CCState::MurderQueue.push(player);
                    }
                }
                
                ImGui::Spacing();
            }

            for (auto player : GetAllPlayers())
            {
                auto data = GetPlayerData(player);
                auto name = GetUTF8StringFromNETString(data->fields.PlayerName);

                ImVec4 nameColor;

                if (data->fields.IsImpostor)
                    nameColor = AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->ImpostorRed);
                else
                    nameColor = AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->CrewmateBlue);

                if (data->fields.IsDead)
                    nameColor = AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->DisabledGrey);

                if (ImGui::Button((std::string("Kick") + std::string("##") + name).c_str()))
                {
                    CCState::KickTarget = player;
                }

                ImGui::SameLine();

                if (CCState::VoteTarget.has_value() && player == CCState::VoteTarget.value())
                {
                    if (ImGui::Button((std::string("Next Vote") + std::string("##") + name).c_str()))
                    {
                        CCState::VoteTarget = std::nullopt;
                    }
                }
                else
                {
                    if (ImGui::Button((std::string("Vote Off") + std::string("##") + name).c_str()))
                    {
                        CCState::VoteTarget = player;
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button((std::string("Teleport") + std::string("##") + name).c_str()))
                {
                    Transform* localTransform = Component_get_transform((Component*)(*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL);
                    Transform* playerTransform = Component_get_transform((Component*)player, NULL);
                    Transform_set_position(localTransform, Transform_get_position(playerTransform, NULL), NULL);
                }

                ImGui::SameLine();
                    
                if (ImGui::Button((std::string("Murder") + std::string("##") + name).c_str()))
                    CCState::MurderTarget = player;

                ImGui::SameLine();
                   
                ImGui::TextColored(nameColor, name.c_str());

                //auto data = GetPlayerData(player);

                //static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);

                //ImGui::ColorEdit4("Color", &colf.x);
                //ImGui::ColorButton("Color", &colf.x);

                //ImVec2 winpos = ImGui::GetWindowPos();
                //ImVec2 winsize = ImGui::GetWindowSize();

                //drawList->AddCircleFilled(ImVec2(winpos.x + winsize.x * 0.5F, winpos.y + winsize.y * 0.5F),
                    //CCConstants::ICON_SCALE, AmongUsColorToImVec4(GetPlayerColor(GetPlayerData(player)->fields.ColorId)));

                //drawList->AddCircleFilled(ImVec2(0, 0), CCConstants::ICON_SCALE, AmongUsColorToImVec4(GetPlayerColor(data->fields.ColorId)));
            }
        }

        if (ImGui::CollapsingHeader("Doors"))
        {
            if (CCState::PinDoors)
            {
                if (ImGui::Button("Select All"))
                {
                    for (auto entry : magic_enum::enum_values<SystemTypes__Enum>())
                    {
                        CCState::PinnedDoors.insert(entry);
                    }
                }
                
                ImGui::SameLine();

                if (ImGui::Button("Unselect All"))
                {
                    CCState::PinnedDoors.clear();
                }

                ImGui::Spacing();
            }

            ImGui::Checkbox("Keep Doors Closed", &CCState::PinDoors);

            if (CCState::PinDoors)
                ImGui::Indent(16.0F);

            for (auto entry : magic_enum::enum_entries<SystemTypes__Enum>())
            {
                if (CCState::PinDoors)
                {
                    bool checked = CCState::PinnedDoors.find(entry.first) != CCState::PinnedDoors.end();

                    ImGui::Checkbox((std::string(entry.second.data() + sizeof("SystemTypes__Enum")) + std::string("##Checkbox")).c_str(), &checked);

                    if (!checked)
                        CCState::PinnedDoors.erase(entry.first);
                    else
                        CCState::PinnedDoors.insert(entry.first);
                }
                else
                {
                    if (ImGui::Button((std::string(entry.second.data() + sizeof("SystemTypes__Enum")) + std::string("##Button")).c_str()))
                    {
                        ShipStatus_RpcCloseDoorsOfType((*ShipStatus__TypeInfo)->static_fields->Instance, entry.first, NULL);
                    }
                }
            }

            if (CCState::PinDoors)
                ImGui::Unindent(16.0F);
        }

        if (ImGui::CollapsingHeader("Repair"))
        {
            // yes i hardcoded the systems
            for (auto entry : CCConstants::SABOTAGE_SYSTEMS)
            {
                if (ImGui::Button((std::string(magic_enum::enum_name<SystemTypes__Enum>((SystemTypes__Enum)entry).data() + sizeof("SystemTypes__Enum")) + std::string("##RepairButton")).c_str()))
                {
                    ShipStatus_RepairSystem((*ShipStatus__TypeInfo)->static_fields->Instance, (SystemTypes__Enum)entry,
                        (*PlayerControl__TypeInfo)->static_fields->LocalPlayer, 0x80, NULL);
                }
            }
        }

        if (ImGui::CollapsingHeader("Sabotage"))
        {
            for (auto entry : CCConstants::SABOTAGE_SYSTEMS)
            {
                if (ImGui::Button((std::string(magic_enum::enum_name<SystemTypes__Enum>((SystemTypes__Enum)entry).data() + sizeof("SystemTypes__Enum")) + std::string("##SabotageButton")).c_str()))
                {
                    // yeah dude "repair system" my ass
                    ShipStatus_RpcRepairSystem((*ShipStatus__TypeInfo)->static_fields->Instance, SystemTypes__Enum_Sabotage, entry, NULL);
                }
            }
        }

        if ((*PlayerControl__TypeInfo)->static_fields->LocalPlayer != NULL
            && GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer) != NULL
            && GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.Tasks != NULL
            && ImGui::CollapsingHeader("Tasks"))
        {
            List_1_GameData_TaskInfo_* tasks = GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.Tasks;

            if (List_1_GameData_TaskInfo__get_Count(tasks, NULL) > 0 
                && ImGui::Button("Complete All Tasks"))
            {
                for (int i = 0; i < List_1_GameData_TaskInfo__get_Count(tasks, NULL); i++)
                {
                    GameData_TaskInfo* task = List_1_GameData_TaskInfo__get_Item(tasks, i, NULL);

                    if (!task->fields.Complete)
                        PlayerControl_RpcCompleteTask((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, task->fields.Id, NULL);
                }
            }

            ImGui::Spacing();
            
            for (int i = 0; i < List_1_GameData_TaskInfo__get_Count(tasks, NULL); i++)
            {
                GameData_TaskInfo* task = List_1_GameData_TaskInfo__get_Item(tasks, i, NULL);

                if (ImGui::Button(("Complete##Button" + std::to_string(task->fields.Id)).c_str()) && !task->fields.Complete)
                {
                    PlayerControl_RpcCompleteTask((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, task->fields.Id, NULL);
                }

                ImGui::SameLine();
                
                ImGui::TextColored(task->fields.Complete
                    ? ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
                    : AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->DisabledGrey) 
                    , (std::string("Task #") + std::to_string(task->fields.Id)).c_str());
            }
        }

        if (ImGui::CollapsingHeader("Modifiers"))
        {
            ImGui::Text("Light Mod Modifier");
            ImGui::Checkbox("##LightModModifierCheckbox", &CCState::ModifyLight);
            ImGui::SameLine();
            ImGui::SliderFloat("##LightModModifier", &CCState::LightModifier, 0, 10.0F);

            ImGui::Text("Speed Modifier");
            ImGui::Checkbox("##SpeedModifierCheckbox", &CCState::ModifySpeed);
            ImGui::SameLine();
            ImGui::SliderFloat("##SpeedModifier", &CCState::SpeedModifier, 0, 10);

            ImGui::Text("Kill Cooldown Modifier");
            ImGui::Checkbox("##KillCooldownModifierCheckbox", &CCState::ModifyKillCooldown);
            ImGui::SameLine();
            ImGui::SliderFloat("##KillCooldown", &CCState::KillCooldownModifier, 0.001F, 60);

            ImGui::Text("Kill Distance Modifier");
            ImGui::Checkbox("##KillDistanceModifierCheckbox", &CCState::ModifyKillDistance);

            String__Array* killDistancesNames = (*GameOptionsData__TypeInfo)->static_fields->KillDistanceStrings;

            for (int i = 0; i < CCConstants::KILL_DISTANCES_LENGTH; i++)
            {
                ImGui::SameLine();
                ImGui::RadioButton(GetUTF8StringFromNETString(killDistancesNames->vector[i]).c_str(), &CCState::KillDistanceModifier, i);
            }
        }

        ImGui::End();
    }
    
    ImGui::EndFrame();
    ImGui::Render();

    Ctx->OMSetRenderTargets(1, &RenderTargetView, NULL);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return OriginalD3DFunction(pThis, SyncInterval, Flags);
}

void HudHook(MethodInfo* m)
{
    OriginalHudFunction(m);

    if (CCState::VoteTarget.has_value()
        && (*MeetingHud__TypeInfo)->static_fields->Instance != NULL)
    {       
        // Class appears to be 4 bytes off?
        // here we are checking vote state which appears to be in VoteEndingSound
        MeetingHud_VoteStates__Enum state =
            (MeetingHud_VoteStates__Enum)((intptr_t)(*MeetingHud__TypeInfo)->static_fields->Instance->fields.VoteEndingSound);

        if (state == MeetingHud_VoteStates__Enum_NotVoted
            || state == MeetingHud_VoteStates__Enum_Voted)
        {
            for (auto player : GetAllPlayers())
            {
                MeetingHud_CmdCastVote((*MeetingHud__TypeInfo)->static_fields->Instance,
                    GetPlayerData(player)->fields.PlayerId,
                    GetPlayerData(CCState::VoteTarget.value())->fields.PlayerId,
                    NULL);
            }

            CCState::VoteTarget = std::nullopt;
        }
    }

    if (CCState::AllClothesCounter.ProcessAction() && CCState::ShiftAllClothes)
    {
        for (auto player : GetAllPlayers())
        {
            // would write way to find max bound for these two
            // but im too lazy 
            PlayerControl_RpcSetHat(player, randi(0, 10), NULL);
            PlayerControl_RpcSetSkin(player, randi(0, 10), NULL);
        }
    }

    if (CCState::ChatCounter.ProcessAction() && CCState::SpamChat)
    {
        if (CCState::AllPlayersSpam)
        {
            for (auto player : GetAllPlayers())
                PlayerControl_RpcSendChat(player, CreateNETStringFromANSI(CCState::SpamMessage), NULL);
        }
        else
        {
            PlayerControl_RpcSendChat((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CreateNETStringFromANSI(CCState::SpamMessage), NULL);
        }
    };

    if (CCState::ColorCounter.ProcessAction() && CCState::ColorShift)
    {
        CCState::CurrentColor = GetNextColor(CCState::CurrentColor);

        while (!CheckColorAvailable(CCState::CurrentColor))
            CCState::CurrentColor = GetNextColor(CCState::CurrentColor);

        PlayerControl_CmdCheckColor((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CCState::CurrentColor, NULL);
    }

    if (CCState::AllColorsCounter.ProcessAction() && CCState::ShiftAllColors)
    {
        for (auto player : GetAllPlayers())
        {
            auto data = GetPlayerData(player);
            auto playerColorId = data->fields.ColorId;
            playerColorId = GetNextColor(playerColorId);

            while (!CheckColorAvailable(playerColorId))
                playerColorId = GetNextColor(playerColorId);

            PlayerControl_CmdCheckColor(player, playerColorId, NULL);
        }
    }

    if (CCState::KickTarget.has_value())
    {
        VoteBanSystem* vbSystem = (*VoteBanSystem__TypeInfo)->static_fields->Instance;
        InnerNetClient* net = (InnerNetClient*)(*AmongUsClient__TypeInfo)->static_fields->Instance;
        int32_t victimId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)CCState::KickTarget.value(), NULL);
        int32_t oldClientId = net->fields.ClientId;

        for (auto player : GetAllPlayers())
        {
            net->fields.ClientId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)player, NULL);
            VoteBanSystem_CmdAddVote((*VoteBanSystem__TypeInfo)->static_fields->Instance, victimId, NULL);
        }

        net->fields.ClientId = oldClientId;

        CCState::KickTarget = std::nullopt;
    }

    if (CCState::PinDoors)
    {
        for (auto entry : CCState::PinnedDoors)
        {
            ShipStatus_RpcCloseDoorsOfType((*ShipStatus__TypeInfo)->static_fields->Instance, entry, NULL);
        }
    }

    if (CCState::MurderTarget.has_value())
    {
        if (GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.IsImpostor)
        {
            PlayerControl_RpcMurderPlayer((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CCState::MurderTarget.value(), NULL);
        }
        else
        {
            for (auto player : GetAllPlayers())
            {
                if (GetPlayerData(player)->fields.IsImpostor && !GetPlayerData(player)->fields.IsDead)
                {
                    PlayerControl_RpcMurderPlayer(player, CCState::MurderTarget.value(), NULL);
                    break;
                }
            }
        }
        
        CCState::MurderTarget = std::nullopt;
    }

    if (CCState::ColorTarget.has_value())
    {
        PlayerControl_CmdCheckColor((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CCState::ColorTarget.value(), NULL);
        CCState::ColorTarget = std::nullopt;
    }

    while (!CCState::MurderQueue.empty())
    {
        auto front = CCState::MurderQueue.front();
        if (GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.IsImpostor)
        {
            PlayerControl_RpcMurderPlayer((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, front, NULL);
        }
        else
        {
            for (auto player : GetAllPlayers())
            {
                if (GetPlayerData(player)->fields.IsImpostor && !GetPlayerData(player)->fields.IsDead)
                {
                    PlayerControl_RpcMurderPlayer(player, front, NULL);
                    break;
                }
            }
        }
        CCState::MurderQueue.pop();
    }
    
    if (CCState::ModifySpeed)
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.PlayerSpeedMod = CCState::SpeedModifier;

    if (CCState::ModifyKillCooldown)
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.KillCooldown = CCState::KillCooldownModifier;

    if (CCState::ModifyKillDistance)
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.KillDistance = CCState::KillDistanceModifier;

    if (CCState::ModifyLight)
    {
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.ImpostorLightMod = CCState::LightModifier;
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.CrewLightMod = CCState::LightModifier;
    }

    if (CCState::NoClip)
    {
        auto comp = Component_get_gameObject((Component*)(*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL);
        GameObject_set_layer(comp, LayerMask_NameToLayer(CreateNETStringFromANSI("Ghost"), NULL), NULL);
    }

    if (CCState::ShowGhosts) {


        for (auto player : GetAllPlayers())
        {
            auto data = GetPlayerData(player);
            if (!PlayerControl_get_Visible(player, NULL) && data->fields.IsDead)
                PlayerControl_set_Visible(player, true, NULL);
        }
    }

    if (CCState::MarkImposters)
        for (auto player : GetAllPlayers())
        {
            auto data = GetPlayerData(player);

            // Interestingly enough, RemainingEmergencies is actually a pointer to the PlayerControl field nameText...
            TextRenderer* nameText = (TextRenderer*)(player->fields.RemainingEmergencies);

            nameText->fields.Color = data->fields.IsImpostor
                ? (*Palette__TypeInfo)->static_fields->ImpostorRed
                : (*Palette__TypeInfo)->static_fields->CrewmateBlue;
        }
    
    //auto hudManager = (HudManager*)DestroyableSingleton_1_InnerNet_InnerNetServer__get_Instance(*(MethodInfo**)(GetBaseAddress() + 0x00E2577C));
}

bool HookFunction(PVOID* original, PVOID detour, const char* funcName)
{
    LONG err = DetourAttach(original, detour);
    if (err != 0)
    {
        std::cout << "Failed to hook a function: " << funcName << "\n";
        return false;
    }
    else
    {
        std::cout << "Successfully hooked a function: " << funcName << "\n";
        return true;
    }
}

void Run()
{
    NewConsole();

    std::cout << "Initializing...\n";

    OriginalHudFunction = KeyboardJoystick_HandleHud;
    OriginalD3DFunction = GetD3D11PresentFunction();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if (!HookFunction(&(PVOID&)OriginalHudFunction, HudHook, "KeyboardJoystick_HandleHud"))
        return;

    if (OriginalD3DFunction == NULL || !HookFunction(&(PVOID&)OriginalD3DFunction, D3D_FUNCTION_HOOK, "D3D11Present"))
        return;

    DetourTransactionCommit();

    std::cout << "Initialization Complete\n";

    system("cls");

    //std::cout << CCConstants::ASCII;
    std::cout << "Among Us Cheat Client | v.0.0.3-beta2";
    std::cout << "\nCreated by Escartem";
    std::cout << "\nPress del to open the menu";
    std::cout << "\nClosing this window will close the game";
}