/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __SHIFT_JIS_HPP__
#define __SHIFT_JIS_HPP__

#include "util_unicode_definitions.hpp"
#include <string>
#include <cinttypes>

namespace util {
	DLLUUNIC void init_shift_jis(const std::string &dataFilePath);
	DLLUUNIC uint32_t validate_shift_jis(const std::string &data);
	DLLUUNIC std::string shift_jis_to_utf8(const std::string &input, bool validate = false);
};

#endif
