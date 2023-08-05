#include "core.h"

#define valid(buf) std::string(buf).c_str()

void core::Interface()
{
	auto base = (DWORD64)(GetModuleHandleA("Deceit.exe"));
	common::pGameFramework = *(str::IGameFramework**)(base + 0x283B8C0);
	common::pEntitySystem = *(str::IEntitySystem**)(base + 0x283B8D0);
	common::pRender = *(str::IRender**)(base + 0x283B920);
}

void core::SetupPresent()
{
	MH_Initialize();

	kiero::init(kiero::RenderType::D3D11);
	kiero::bind(8, (void**)&core::oPresent, &core::hPresent);

	MH_EnableHook(MH_ALL_HOOKS);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(core::oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT WINAPI core::hPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!core::init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&core::pDevice)))
		{
			DXGI_SWAP_CHAIN_DESC sd;
			core::pDevice->GetImmediateContext(&core::pContext);
			pSwapChain->GetDesc(&sd);
			core::window = sd.OutputWindow;

			RECT rect;
			GetWindowRect(core::window, &rect);
			trash::RenderWidth = rect.right - rect.left;
			trash::RenderHeight = rect.bottom - rect.top;

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			core::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &core::mainRenderTargetView);
			pBackBuffer->Release();

			core::oWndProc = (WNDPROC)SetWindowLongPtrA(core::window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			core::init = true;
		}
		else
		{
			return core::oPresent(pSwapChain, SyncInterval, Flags);
		}

		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = NULL;
			io.LogFilename = NULL;
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(core::window);
			ImGui_ImplDX11_Init(core::pDevice, core::pContext);
		}
	}

	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	
		core::last_tick = GetTickCount64();

		if (GetAsyncKeyState(VK_INSERT))
		{
			if (core::last_tick - core::open_menu_tick > 200) {
				core::open = !core::open;
			}
			core::open_menu_tick = GetTickCount64();
		}

		core::MenuLoop();
		core::MainLoop();

		ImGui::Render();
		core::pContext->OMSetRenderTargets(1, &core::mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return core::oPresent(pSwapChain, SyncInterval, Flags);
}

void core::MenuLoop()
{
	if (core::open)
	{
		ImGui::Begin("meow -> <-");
		
		ImGui::Checkbox("Id-Class", &core::names);
		ImGui::InputText("EntityId", core::buf_ent_id, sizeof(core::buf_ent_id));
		ImGui::InputText("ChannelId", core::buf_chan_id, sizeof(core::buf_chan_id));
		ImGui::InputText("Class", core::buf_ent_class, sizeof(core::buf_ent_class));
		ImGui::InputText("Class Create", core::buf_ent_class_create, sizeof(core::buf_ent_class_create));
		ImGui::Spacing();
		ImGui::SliderFloat("SizeX", &core::size_x, -1.f, 10.f, "%.0f");
		ImGui::SliderFloat("SizeY", &core::size_y, -1.f, 10.f, "%.0f");
		ImGui::SliderFloat("SizeZ", &core::size_z, -1.f, 10.f, "%.0f");
		ImGui::Spacing();
		ImGui::SliderFloat("PosFromPlayerX", &core::pos_x, -10.f, 10.f, "%.0f");
		ImGui::SliderFloat("PosFromPlayerY", &core::pos_y, -10.f, 10.f, "%.0f");
		ImGui::SliderFloat("PosFromPlayerZ", &core::pos_z, -10.f, 10.f, "%.0f");
		ImGui::Spacing();
		ImGui::SliderFloat("Knife Range", &core::kn_range, -1.f, 100.f, "%.0f");
		ImGui::SliderFloat("Knife Delay", &core::kn_delay, -1.f, 5.f, "%.0f");
		ImGui::SliderFloat("Knife Duratio", &core::kn_durat, -1.f, 5.f, "%.0f");
		ImGui::Spacing();
		if (ImGui::Button("Remove All Entities by class"))
		{
			core::remove_all_button = true;
		}
		if (ImGui::Button("Remove Entity by id"))
		{
			core::remove_button = true;
		}
		if (ImGui::Button("Remove Actor by id"))
		{
			core::remove_act_button = true;
		}
		if (ImGui::Button("Create Entity by chid, class"))
		{
			core::create_button = true;
		}
		if (ImGui::Button("Reload EntitySystem"))
		{
			core::reload_button = true;
		}

		ImGui::End();
	}
}

void core::MainLoop()
{
	ImGui::Begin("", 0, 128 | 32 | 16 | 8 | 4 | 2 | 1 | 0);

	str::IActorSystem* actor_sys = common::pGameFramework->GetActorSystem();
	auto local_player = common::pGameFramework->GetClientEntity();

	if (!local_player)
		return;

	if (core::create_button)
	{
		auto pl = local_player->Position;

		Quat rot;
		rot.v.x = 0;
		rot.v.y = 0;
		rot.v.z = 0;
		rot.w = 1;

		Vector3 pos;
		pos.x = pl.x + core::pos_x;
		pos.y = pl.y + core::pos_y;
		pos.z = pl.z + core::pos_z;

		if (!std::string(buf_ent_class_create).empty() && !std::string(buf_chan_id).empty())
		{
			auto name = gen_random_str(9).c_str();
			auto entid = gen_random_int(5);

			auto act = actor_sys->CreateActor(
				std::stoi(std::string(core::buf_chan_id)),
				name,
				valid(buf_ent_class_create),
				pos,
				rot,
				{ core::size_x, core::size_y, core::size_z },
				entid
			);

			actor_sys->AddActor(entid, act);
		}

		core::create_button = false;
	}

	if (core::reload_button)
	{
		common::pEntitySystem->Reload();
		core::reload_button = false;
	}

	if (core::remove_all_button)
	{
		if (!std::string(core::buf_ent_class).empty()) 
		{
			str::IEntityIt* it = *(str::IEntityIt**)common::pEntitySystem->GetEntityIterator();
			if (it)
			{
				for (; str::IEntity * entity = it->Next();)
				{
					if (!entity)
						continue;

					if (entity == local_player)
						continue;

					if (valid(core::buf_ent_class) == entity->GetClass()) {
						common::pEntitySystem->RemoveEntity(entity->GetId(), true);
					}
				}
			}
		}
		core::remove_all_button = false;
	}

	if (core::remove_button)
	{
		if (!std::string(core::buf_ent_id).empty()) {
			common::pEntitySystem->RemoveEntity(std::stoi(std::string(core::buf_ent_id)), true);
		}
		core::remove_button = false;
	}
	
	if (core::remove_act_button)
	{
		if (!std::string(core::buf_ent_id).empty()) {
			actor_sys->RemoveActor(std::stoi(std::string(core::buf_ent_id)));
		}
		core::remove_act_button = false;
	}

	str::IEntityIt* it = *(str::IEntityIt**)common::pEntitySystem->GetEntityIterator();
	if (it)
	{
		for (; str::IEntity * entity = it->Next();)
		{
			if (!entity)
				continue;

			if (entity == local_player)
				continue;

			if (valid(core::buf_ent_class) == entity->GetClass())
			{
				entity->SetScale({ core::size_x, core::size_y, core::size_z });
			}

			if (core::names)
			{
				Vector3 out = common::pRender->ProjectToScreen(entity->Position);
				if (out.x > 1 || out.y > 1) {
					ImGui::GetBackgroundDrawList()->AddText(0, 13, ImVec2(out.x, out.y), 0xFFFFFFFF, entity->GetClass().c_str());
					ImGui::GetBackgroundDrawList()->AddText(0, 13, ImVec2(out.x, out.y + 20), 0xFFFFFFFF, std::to_string(entity->GetId()).c_str());
				}
			}
		}
	}

	auto item = actor_sys->GetActor(local_player->GetId())->GetCurrentItem();
	if (item)
	{
		auto item_name = item->GetName();
		auto weapon = item->GetWeapon();

		if (std::string(item_name).find("DeceitPistol") != std::string::npos)
		{
			
		}

		if (std::string(item_name).find("DeceitKnife") != std::string::npos)
		{
			auto spizdil_meow = weapon->Melee->MeleeParam;
			spizdil_meow->Delay = core::kn_delay;
			spizdil_meow->Range = core::kn_range;
			spizdil_meow->Duration = core::kn_durat;
			

		}
	}

	ImGui::End();
}