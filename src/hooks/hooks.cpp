#include "hooks.h"
#include "../game/cam.h"

#include <iostream>

auto __fastcall camera_c_field_map::hooked ( uintptr_t a1, uintptr_t a2, uintptr_t a3 ) -> char {
	return g_cam->run ( a1, a2, a3, original );
}

auto __fastcall camera_c_manual::hooked ( uintptr_t a1, uintptr_t a2, uintptr_t a3 ) -> char {
	return g_cam->run ( a1, a2, a3, original );
}

auto __fastcall camera_c_preset::hooked ( uintptr_t a1, uintptr_t a2, uintptr_t a3 ) -> char {
	return g_cam->run ( a1, a2, a3, original );
}

auto WINAPI xinput_get_state::hooked ( DWORD dwUserIndex, XINPUT_STATE* pState ) -> DWORD {
	if ( game::freecam_enabled ) {
		return ERROR_DEVICE_NOT_CONNECTED;
	}

	return xinput_get_state::original ( dwUserIndex, pState );
}

/* minhook in current year 2025 <3 */
auto hooks::init ( ) -> bool {
	if ( MH_Initialize ( ) != MH_OK ) {
		std::cout << "[error] hook initialization failed \n";
		return false;
	}

	if ( MH_CreateHook ( ( LPVOID )0x14037DAD0, &camera_c_field_map::hooked, reinterpret_cast< LPVOID* > ( &camera_c_field_map::original ) ) !=
	     MH_OK ) {
		std::cout << "[error] creating cam hook failed ";
		return false;
	}

	if ( MH_EnableHook ( ( LPVOID )0x14037DAD0 ) != MH_OK ) {
		std::cout << "[error] enabling cam hook failed ";
		return false;
	}

	if ( MH_CreateHook ( ( LPVOID )0x14037EAE0, &camera_c_manual::hooked, reinterpret_cast< LPVOID* > ( &camera_c_manual::original ) ) != MH_OK ) {
		std::cout << "[error] creating cam hook failed ";
		return false;
	}

	if ( MH_EnableHook ( ( LPVOID )0x14037EAE0 ) != MH_OK ) {
		std::cout << "[error] enabling cam hook failed ";
		return false;
	}
	if ( MH_CreateHook ( ( LPVOID )0x14037EEF0, &camera_c_preset::hooked, reinterpret_cast< LPVOID* > ( &camera_c_preset::original ) ) != MH_OK ) {
		std::cout << "[error] test hook failed ";
		return false;
	}

	if ( MH_EnableHook ( ( LPVOID )0x14037EEF0 ) != MH_OK ) {
		std::cout << "[error] test hook failed ";
		return false;
	}

	HMODULE xinput = GetModuleHandle ( L"xinput1_3.dll" );
	if ( xinput ) {
		void* xinput_get_state_ptr = GetProcAddress ( xinput, "XInputGetState" );

		if ( MH_CreateHook ( xinput_get_state_ptr, &xinput_get_state::hooked, reinterpret_cast< LPVOID* > ( &xinput_get_state::original ) ) !=
		     MH_OK ) {
			std::cerr << "[error] creating xinput get state hook failed";
			return false;
		}

		if ( MH_EnableHook ( xinput_get_state_ptr ) != MH_OK ) {
			std::cerr << "[error] enabling xinput get state hook failed";
			return false;
		}
	}

	return true;
}

auto hooks::unload ( ) -> void {
	MH_DisableHook ( MH_ALL_HOOKS );
	MH_Uninitialize ( );
}