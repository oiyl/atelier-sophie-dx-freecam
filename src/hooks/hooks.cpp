#include "hooks.h"
#include "../game/cam.h"

#include <iostream>
#include "../sdk/mem.h"

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

	if ( MH_CreateHook ( ( LPVOID )offsets::field, &camera_c_field_map::hooked, reinterpret_cast< LPVOID* > ( &camera_c_field_map::original ) ) !=
	     MH_OK ) {
		std::cout << "[error] creating field cam hook failed ";
		return false;
	}

	if ( MH_EnableHook ( ( LPVOID )offsets::field ) != MH_OK ) {
		std::cout << "[error] enabling field cam hook failed ";
		return false;
	}

	if ( MH_CreateHook ( ( LPVOID )offsets::manual, &camera_c_manual::hooked, reinterpret_cast< LPVOID* > ( &camera_c_manual::original ) ) !=
	     MH_OK ) {
		std::cout << "[error] creating manual cam hook failed ";
		return false;
	}

	if ( MH_EnableHook ( ( LPVOID )offsets::manual ) != MH_OK ) {
		std::cout << "[error] enabling manual cam hook failed ";
		return false;
	}
	if ( MH_CreateHook ( ( LPVOID )offsets::preset, &camera_c_preset::hooked, reinterpret_cast< LPVOID* > ( &camera_c_preset::original ) ) !=
	     MH_OK ) {
		std::cout << "[error] preset hook failed ";
		return false;
	}

	if ( MH_EnableHook ( ( LPVOID )offsets::preset ) != MH_OK ) {
		std::cout << "[error] preset hook failed ";
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

bool hooks::offsets::scan ( ) {
	field  = mem_scan ( str_to_bytes ( "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 57 48 8D 68 ? 48 81 EC ? ? ? ? F3 0F 10 61" ) );
	manual = mem_scan ( str_to_bytes ( "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 48 8D 68 ? 48 81 EC ? ? ? ? 48 8B D9 44 0F 29 50" ) );
	preset = mem_scan ( str_to_bytes ( "4C 8B DC 49 89 5B ? 49 89 7B" ) );

	if ( field == 0 ) {
		std::cerr << "[error] no field cam pattern" << std::endl;
		return false;
	}
	if ( manual == 0 ) {
		std::cerr << "[error] no manual cam pattern" << std::endl;
		return false;
	}
	if ( preset == 0 ) {
		std::cerr << "[error] no preset cam pattern" << std::endl;
		return false;
	}

	std::cout << "[info] all patterns found" << std::endl;
	return true;
}
