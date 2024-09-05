#pragma once
#pragma comment( lib, "libMinHook.x64.lib" )

#include <MinHook.h>
#include <Xinput.h>

#define FUNCTION_PROTOTYPE( name, t )                                                                                                                \
	struct name {                                                                                                                                    \
		using fn = t;                                                                                                                                \
		static fn hooked;                                                                                                                            \
		static fn* original;                                                                                                                         \
	};                                                                                                                                               \
	inline name::fn* name::original;

FUNCTION_PROTOTYPE(xinput_get_state, auto(WINAPI) (DWORD, XINPUT_STATE*)->DWORD);
FUNCTION_PROTOTYPE ( camera_c_field_map, auto( __fastcall ) ( uintptr_t, uintptr_t, uintptr_t )->char );
FUNCTION_PROTOTYPE ( camera_c_manual, auto( __fastcall ) ( uintptr_t, uintptr_t, uintptr_t )->char );
FUNCTION_PROTOTYPE ( camera_c_preset, auto( __fastcall ) ( uintptr_t, uintptr_t, uintptr_t )->char );

namespace hooks {

	auto init ( ) -> bool;
	auto unload ( ) -> void;
}