#include "cam.h"

#include <algorithm>
#include <stdexcept>
#include <cmath>

auto c_cam::init ( vec3_t& pos, vec3_t& look_at ) const -> void {
	if ( !is_valid ( ) ) {
		throw std::runtime_error ( "invalid ptr accessed" );
	}

	pos.x = *reinterpret_cast< float* > ( m_ptr + 0x08 );
	pos.y = *reinterpret_cast< float* > ( m_ptr + 0xC );
	pos.z = *reinterpret_cast< float* > ( m_ptr + 0x10 );

	look_at.x = *reinterpret_cast< float* > ( m_ptr + 0x18 );
	look_at.y = *reinterpret_cast< float* > ( m_ptr + 0x1C );
	look_at.z = *reinterpret_cast< float* > ( m_ptr + 0x20 );
}

auto c_cam::set_position ( const vec3_t& new_position ) -> void {
	if ( !is_valid ( ) ) {
		throw std::runtime_error ( "invalid ptr accessed" );
	}
	*reinterpret_cast< float* > ( m_ptr + 0x08 ) = new_position.x;
	*reinterpret_cast< float* > ( m_ptr + 0xC )  = new_position.y;
	*reinterpret_cast< float* > ( m_ptr + 0x10 ) = new_position.z;
}

auto c_cam::set_look_at ( const vec3_t& new_position ) -> void {
	if ( !is_valid ( ) ) {
		throw std::runtime_error ( "invalid ptr accessed" );
	}
	*reinterpret_cast< float* > ( m_ptr + 0x18 ) = new_position.x;
	*reinterpret_cast< float* > ( m_ptr + 0x1C ) = new_position.y;
	*reinterpret_cast< float* > ( m_ptr + 0x20 ) = new_position.z;
}

float speed = 20;

auto c_cam::calc_pos ( ) -> void {
	vec3_t direction = m_look_at - m_pos;
	direction.normalize ( );

	vec3_t up { 0, 1, 0 };
	vec3_t right = direction.cross ( up );
	right.normalize ( );

	if ( GetAsyncKeyState ( 'W' ) & 0x8000 ) {
		m_pos += direction * speed;
		m_look_at += direction * speed;
	}

	if ( GetAsyncKeyState ( 'S' ) & 0x8000 ) {
		m_pos -= direction * speed;
		m_look_at -= direction * speed;
	}

	if ( GetAsyncKeyState ( 'A' ) & 0x8000 ) {
		m_pos -= right * speed;
		m_look_at -= right * speed;
	}

	if ( GetAsyncKeyState ( 'D' ) & 0x8000 ) {
		m_pos += right * speed;
		m_look_at += right * speed;
	}

	if ( GetAsyncKeyState ( VK_LCONTROL ) & 0x8000 ) {
		m_pos += up * speed;
		m_look_at += up * speed;
	}

	if ( GetAsyncKeyState ( VK_SHIFT ) & 0x8000 ) {
		m_pos -= up * speed;
		m_look_at -= up * speed;
	}

	set_position ( m_pos );
}

auto spherical_to_cartesian ( float r, float theta, float phi ) -> vec3_t {
	return { r * sin ( phi ) * sin ( theta ), r * cos ( phi ), r * sin ( phi ) * cos ( theta ) };
}

auto c_cam::calc_dir ( ) -> void {
	vec3_t dis = m_look_at - m_pos;

	float radius = sqrt ( dis.x * dis.x + dis.y * dis.y + dis.z * dis.z );
	float yaw    = atan2 ( dis.x, dis.z );
	float pitch  = acos ( dis.y / radius );

	if ( GetAsyncKeyState ( 'Z' ) & 0x8000 ) {
		yaw += 0.01f;
		yaw = std::fmod ( yaw, 2.0f * 3.1415f );
	}

	if ( GetAsyncKeyState ( 'X' ) & 0x8000 ) {
		yaw -= 0.01f;
		yaw = std::fmod ( yaw, 2.0f * 3.1415f );
	}

	if ( GetAsyncKeyState ( 'F' ) & 0x8000 ) {
		pitch += 0.01f;
		pitch = std::clamp ( pitch, 0.0f, 3.1415f );
	}

	if ( GetAsyncKeyState ( 'T' ) & 0x8000 ) {
		pitch -= 0.01f;
		pitch = std::clamp ( pitch, 0.0f, 3.1415f );
	}

	m_look_at = m_pos + spherical_to_cartesian ( radius, yaw, pitch );

	set_look_at ( m_look_at );
}