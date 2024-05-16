/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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

#endif
