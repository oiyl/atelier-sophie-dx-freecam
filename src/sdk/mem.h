#include <vector>
#include <string>
#include <sstream>
#include <iostream>

auto str_to_bytes ( const std::string& str ) -> std::vector< int8_t > {
	std::istringstream ss ( str );

	std::vector< int8_t > result;
	std::string token;

	while ( ss >> token ) {
		if ( token == "?" ) {
			result.push_back ( -1 );
		} else {
			result.push_back ( std::stoi ( token, nullptr, 16 ) );
		}
	}

	return result;
}

bool compare_mem ( const unsigned char* data, const std::vector< int8_t >& pattern ) {
	for ( size_t i = 0; i < pattern.size ( ); i++ ) {
		if ( pattern[ i ] != -1 && data[ i ] != static_cast< unsigned char > ( pattern[ i ] ) ) {
			return false;
		}
	}
	return true;
}

uintptr_t mem_scan ( std::vector< int8_t > pattern ) {
	auto base = GetModuleHandleW ( nullptr );

	MEMORY_BASIC_INFORMATION mbi;
	void* address = nullptr;

	while ( VirtualQuery ( address, &mbi, sizeof ( mbi ) ) ) {
		if ( mbi.Protect & PAGE_EXECUTE_READ && mbi.State == MEM_COMMIT ) {
			unsigned char* start = static_cast< unsigned char* > ( mbi.BaseAddress );
			unsigned char* end   = start + mbi.RegionSize;

			for ( unsigned char* ptr = start; ptr < end; ++ptr ) {
				if ( compare_mem ( ptr, pattern ) ) {
					std::cout << "[info] pattern found: " << static_cast< void* > ( ptr ) << std::endl;
					return reinterpret_cast< uintptr_t > ( ptr );
				}
			}
		}

		address = static_cast< unsigned char* > ( mbi.BaseAddress ) + mbi.RegionSize;
	}

	std::cout << "[error] pattern scan failed" << std::endl;
	return 0;
}