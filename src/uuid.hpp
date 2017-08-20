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

class uuid {
public:
	union {
		char 	 d8[16];
		uint16_t d32[8];
	};

	inline uuid() {
		d32[0] = uint16_t(rand());
		d32[1] = uint16_t(rand());
		d32[2] = uint16_t(rand());
		d32[3] = 0x4000 | (uint16_t(rand()) & 0x0FFF);
		d32[4] = 0x8000 + (uint16_t(rand()) % 0x3FFF);
		d32[5] = uint16_t(rand());
		d32[6] = uint16_t(rand());
		d32[7] = uint16_t(rand());
	}

	inline uuid(const std::string& string) {
		sscanf(string.c_str(), "%04hx%04hx-%04hx-%04hx-%04hx-%04hx%04hx%04hx",
			&d32[0], &d32[1], &d32[2], &d32[3],
			&d32[4], &d32[5], &d32[6], &d32[7]);
	}

	inline bool operator==(const uuid& other) const {
		for (int i = 0; i < 8; i++) {
			if (d32[i] != other.d32[i]) return false;
		}
		return true;
	}

	inline operator std::string() const {
		char string[37];
		sprintf(string, "%04hx%04hx-%04hx-%04hx-%04hx-%04hx%04hx%04hx",
			d32[0], d32[1], d32[2], d32[3],
			d32[4], d32[5], d32[6], d32[7]);
		return std::string(string);
	}
};

