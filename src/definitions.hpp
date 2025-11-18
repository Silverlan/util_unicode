// SPDX-FileCopyrightText: (c) 2024 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#ifndef __UTIL_UNICODE_DEFINITIONS_HPP__
#define __UTIL_UNICODE_DEFINITIONS_HPP__

#ifdef UUNIC_STATIC
#define DLLUUNIC
#elif UUNIC_DLL
#ifdef __linux__
#define DLLUUNIC __attribute__((visibility("default")))
#else
#define DLLUUNIC __declspec(dllexport)
#endif
#else
#ifdef __linux__
#define DLLUUNIC
#else
#define DLLUUNIC __declspec(dllimport)
#endif
#endif

namespace icu_78 {
	class StringCharacterIterator;
	class UnicodeString;
};

#endif
