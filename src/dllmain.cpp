#include "hooks/hooks.h"

#include <iostream>
#include <thread>

auto WINAPI inject ( LPVOID lp_param ) -> DWORD {
	auto mod = static_cast< HMODULE > ( lp_param );

	hooks::init ( );

	while ( !GetAsyncKeyState ( VK_END ) ) {
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 100 ) );
		if (GetAsyncKeyState(VK_F6)) {
			hooks::unload();
			std::this_thread::sleep_for(std::chrono::seconds(1));

			hooks::init();
		}
	}


	FreeLibraryAndExitThread ( mod, 0 );
}

auto uninject ( ) -> void {
	hooks::unload ( );
}

BOOL APIENTRY DllMain ( HMODULE mod, DWORD reason, LPVOID reserved ) {
	switch ( reason ) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls ( mod );
		if ( auto handle = CreateThread ( nullptr, 0, inject, mod, 0, nullptr ) ) {
			CloseHandle ( handle );
		}
		break;
	case DLL_PROCESS_DETACH:
		uninject ( );
		break;
	}
	return TRUE;
}
