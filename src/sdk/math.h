#include <cmath>

struct vec2_t {
	float x, y;

	vec2_t ( ) : x ( 0 ), y ( 0 ) { }
	vec2_t ( float x, float y ) : x ( x ), y ( y ) { }
};

struct vec3_t {
	float x, y, z;

	vec3_t ( ) : x ( 0 ), y ( 0 ), z ( 0 ) { }
	vec3_t ( float x, float y, float z ) : x ( x ), y ( y ), z ( z ) { }

	void normalize ( ) {
		float magnitude = sqrt ( ( x * x ) + ( y * y ) + ( z * z ) );
		if ( magnitude != 0 ) {
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
	}

	vec3_t cross ( const vec3_t& other ) const {
		return vec3_t ( y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x );
	};

	vec3_t operator- ( const vec3_t& value ) {
		return vec3_t ( x - value.x, y - value.y, z - value.z );
	}

	vec3_t operator+ ( const vec3_t& value ) {
		return vec3_t ( x + value.x, y + value.y, z + value.z );
	}

	vec3_t operator* ( const float& value ) {
		return vec3_t ( x * value, y * value, z * value );
	}

	vec3_t operator* ( const vec3_t& vec ) {
		return vec3_t ( x * vec.x, y * vec.y, z * vec.z );
	}

	void operator+= ( const float& value ) {
		x += value;
		y += value;
		z += value;
	}
	void operator+= ( const vec3_t& vec ) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void operator-= ( const float& value ) {
		x -= value;
		y -= value;
		z -= value;
	}
	void operator-= ( const vec3_t& vec ) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	void operator*= ( const float& value ) {
		x *= value;
		y *= value;
		z *= value;
	}

	void operator*= ( const vec3_t& vec ) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}
};