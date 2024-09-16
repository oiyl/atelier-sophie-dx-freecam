#include "hooks/hooks.h"

#include <iostream>
#include <thread>

#ifdef _DEBUG
auto open_console ( ) -> void {
	AllocConsole ( );

	FILE* fp;
	freopen_s ( &fp, "CONOUT$", "w", stdout );
	freopen_s ( &fp, "CONOUT$", "w", stderr );
	freopen_s ( &fp, "CONIN$", "r", stdin );

	SetConsoleTitle ( L"debug console" );
}

auto close_console ( ) -> void {
	fclose ( stdin );
	fclose ( stdout );
	fclose ( stderr );

	FreeConsole ( );
}
#endif

auto WINAPI inject ( LPVOID lp_param ) -> DWORD {
	auto mod = static_cast< HMODULE > ( lp_param );
#ifdef _DEBUG
	open_console ( );
#endif
	hooks::init ( );

	while ( !GetAsyncKeyState ( VK_END ) ) {
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 100 ) );

		/* fix bugged camera */
		if ( GetAsyncKeyState ( VK_F6 ) ) {
			hooks::unload ( );
			/* weird game edge case where it gets messed up i guess */
			std::this_thread::sleep_for ( std::chrono::seconds ( 5 ) );

			hooks::init ( );
		}
	}

	FreeLibraryAndExitThread ( mod, 0 );
}

auto uninject ( ) -> void {
	hooks::unload ( );
#ifdef _DEBUG
	close_console ( );
#endif
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
