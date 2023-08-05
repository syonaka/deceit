#pragma once
#include "include.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace core
{
	inline HWND window = NULL;
	inline WNDPROC oWndProc;
	inline ID3D11Device* pDevice = NULL;
	inline ID3D11DeviceContext* pContext = NULL;
	inline ID3D11RenderTargetView* mainRenderTargetView;

	inline HRESULT WINAPI hPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef unsigned long(__stdcall* PresentFN) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	inline PresentFN oPresent{ nullptr };

	inline DWORD last_tick = 0;
	inline DWORD open_menu_tick = 0;
	inline bool init = false;
	inline bool open = false;

	inline char buf_ent_class[128] = "";
	inline char buf_ent_class_create[128] = "";
	inline char buf_ent_id[128] = "";
	inline char buf_chan_id[128] = "";

	inline float size_x = 1.0f;
	inline float size_y = 1.0f;
	inline float size_z = 1.0f;

	inline float pos_x = 0.0f;
	inline float pos_y = 1.0f;
	inline float pos_z = 1.0f;

	inline float kn_range = 1.0f;
	inline float kn_delay = 1.0f;
	inline float kn_durat = 1.0f;

	inline bool names = false;
	inline bool remove_button = false;
	inline bool remove_all_button = false;
	inline bool remove_act_button = false;
	inline bool create_button = false;
	inline bool reload_button = false;

	void Interface();
	void SetupPresent();

	void MenuLoop();
	void MainLoop();
}