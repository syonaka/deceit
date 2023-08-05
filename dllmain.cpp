#include "include.h"

VOID WINAPI MainThread(HMODULE hModule)
{
	AllocConsole();
	freopen_s((_iobuf**)(__acrt_iob_func(0)), "conin$", "r", (_iobuf*)(__acrt_iob_func(0)));
	freopen_s((_iobuf**)(__acrt_iob_func(1)), "conout$", "w", (_iobuf*)(__acrt_iob_func(1)));

	{
		core::Interface();
		core::SetupPresent();
	}

	while (!GetAsyncKeyState(VK_F4));
	TerminateProcess(GetCurrentProcess(), 1);
}

BOOL WINAPI DllMain(HMODULE hModule, uint32_t reason, LPVOID)
{
	switch (reason)
	{
	case TRUE:
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, nullptr);
	case FALSE:
		FreeConsole();
	}

	return TRUE;
}
