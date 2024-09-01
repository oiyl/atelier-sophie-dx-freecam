#include "Windows.h"
#include "game.h"
#include <memory>

class c_cam {
public:

	auto is_valid ( ) const -> bool {
		return m_ptr != 0;
	}

	auto init ( vec3_t& pos, vec3_t& look_at ) const -> void;

	auto set_position ( const vec3_t& new_position ) -> void;
	auto set_look_at ( const vec3_t& new_position ) -> void;

	auto calc_pos ( ) -> void;
	auto calc_dir ( ) -> void;

	template< typename t >
	inline auto run ( uintptr_t a1, uintptr_t a2, uintptr_t a3, t original ) -> char;

	uintptr_t m_ptr;
	vec3_t m_pos;
	vec3_t m_look_at;
};

inline auto g_cam = std::make_unique< c_cam > ( );

template< typename t >
auto c_cam::run ( uintptr_t a1, uintptr_t a2, uintptr_t a3, t original ) -> char {
	if ( GetAsyncKeyState ( VK_F7 ) & 1 ) {
		game::freecam_enabled ^= 1;
	}

	if ( g_cam->m_ptr != a3 ) {
		g_cam->m_ptr = a3;
	}

	g_cam->init ( g_cam->m_pos, g_cam->m_look_at );

	if ( game::freecam_enabled ) {
		g_cam->calc_pos ( );
		g_cam->calc_dir ( );
		return 1;
	} else {
		return original ( a1, a2, a3 );
	}
}