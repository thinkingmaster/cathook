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
	union {
		char 	 d8[16];
		uint16_t d16[8];
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
		for (int i = 0; i < 8; i++) {
			d16[i] = other.d16[i];
		}
	}

	inline uuid_t(const std::string& string) {
		assign(string);
	}

	inline void assign(const std::string& string) {
		sscanf(string.c_str(), "%04hx%04hx-%04hx-%04hx-%04hx-%04hx%04hx%04hx",
			&d16[0], &d16[1], &d16[2], &d16[3],
			&d16[4], &d16[5], &d16[6], &d16[7]);
	}

	inline bool operator==(const uuid_t& other) const {
		for (int i = 0; i < 8; i++) {
			if (d16[i] != other.d16[i]) return false;
		}
		return true;
	}

	inline operator std::string() const {
		char string[37];
		sprintf(string, "%04hx%04hx-%04hx-%04hx-%04hx-%04hx%04hx%04hx",
			d16[0], d16[1], d16[2], d16[3],
			d16[4], d16[5], d16[6], d16[7]);
		return std::string(string);
	}
};

