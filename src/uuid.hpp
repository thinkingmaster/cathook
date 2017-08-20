/*
 * uuid.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdint.h>

class uuid_t {
public:
	class hash {
	public:
		std::size_t operator()(const uuid_t& uuid) {
			return (uuid.d32[0]) ^ (uuid.d32[1])
				 ^ (uuid.d32[2]) ^ (uuid.d32[3]);
		}
	};
public:
	union {
		char 	 d8[16];
		uint16_t d16[8];
		uint32_t d32[4];
	};

	inline uuid_t() {
		d16[0] = uint16_t(rand());
		d16[1] = uint16_t(rand());
		d16[2] = uint16_t(rand());
		d16[3] = 0x4000 | (uint16_t(rand()) & 0x0FFF);
		d16[4] = 0x8000 + (uint16_t(rand()) % 0x3FFF);
		d16[5] = uint16_t(rand());
		d16[6] = uint16_t(rand());
		d16[7] = uint16_t(rand());
	}

	inline uuid_t(const uuid_t& other) {
		for (int i = 0; i < 4; i++) {
			d32[i] = other.d32[i];
		}
	}

	inline uuid_t(const std::string& string) {
		assign(string);
	}

	inline void assign(const std::string& string) {
		sscanf(string.c_str(), "%08x-%04hx-%04hx-%04hx-%04hx%08x",
			&d32[0], &d16[2], &d16[3], &d16[4], &d16[5], &d32[3]);
	}

	inline uuid_t& operator=(const uuid_t& other) {
		for (int i = 0; i < 4; i++) {
			d32[i] = other.d32[i];
		}
	}

	inline bool operator==(const uuid_t& other) const {
		for (int i = 0; i < 4; i++) {
			if (d32[i] != other.d32[i]) return false;
		}
		return true;
	}

	inline operator std::string() const {
		char string[37];
		sprintf(string, "%08x-%04hx-%04hx-%04hx-%04hx%08x",
				d32[0], d16[2], d16[3], d16[4], d16[5], d32[3]);
		return std::string(string);
	}
};

