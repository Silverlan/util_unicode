// SPDX-FileCopyrightText: (c) 2024 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "definitions.hpp"
#include <string>
#include <cinttypes>

export module pragma.string.unicode:shift_jis;

export namespace pragma::string::unicode {
	DLLUUNIC void init_shift_jis(const std::string &dataFilePath);
	DLLUUNIC uint32_t validate_shift_jis(const std::string &data);
	DLLUUNIC std::string shift_jis_to_utf8(const std::string &input, bool validate = false);
};
