/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UTIL_UNICODE_HPP__
#define __UTIL_UNICODE_HPP__

#include "util_unicode_definitions.hpp"
#include <memory>
#include <string>
#include <ostream>

namespace icu_75 {
	class StringCharacterIterator;
	class UnicodeString;
};

namespace util {
	class Utf8String;
	class BaseUtf8String;
	class DLLUUNIC UnicodeStringIterator {
	  public:
		friend Utf8String;
		using value_type = char16_t;
		UnicodeStringIterator(const BaseUtf8String &str);
		UnicodeStringIterator(const UnicodeStringIterator &it);
		~UnicodeStringIterator();

		bool operator<(const UnicodeStringIterator &other) const;
		bool operator>(const UnicodeStringIterator &other) const;

		char16_t operator*() const;
		UnicodeStringIterator &operator++();
		UnicodeStringIterator &operator--();
		UnicodeStringIterator operator+(int32_t idx) const;
		UnicodeStringIterator &operator+=(int32_t idx);
		bool operator!=(const UnicodeStringIterator &other) const;
		UnicodeStringIterator &operator=(const UnicodeStringIterator &other);
	  private:
		friend BaseUtf8String;
		UnicodeStringIterator();
		std::unique_ptr<icu_75::StringCharacterIterator> iterator;
		bool done;
	};

	class DLLUUNIC BaseUtf8String {
	  public:
		static constexpr inline size_t npos = std::string::npos;
		friend UnicodeStringIterator;
		~BaseUtf8String();
		std::string cpp_str() const;
		size_t find(char c, size_t startPos = 0) const;
		size_t find(char16_t c, size_t startPos = 0) const;
		size_t find(const char *str, size_t startPos = 0) const;
		size_t find(const std::string &str, size_t startPos = 0) const;
		size_t find(const BaseUtf8String &str, size_t startPos = 0) const;
		UnicodeStringIterator begin() const;
		UnicodeStringIterator end() const;
		const char16_t *data();
		const char16_t *c_str();
		size_t length() const;
		size_t size() const;
		bool empty() const { return length() == 0; }
		uint16_t at(size_t idx) const;
		uint16_t front() const;
		uint16_t back() const;

		void toUpper();
		void toLower();

		bool operator==(const BaseUtf8String &other) const;
		bool operator!=(const BaseUtf8String &other) const;
		bool operator==(const std::string &other) const;
		bool operator!=(const std::string &other) const;

		bool operator<(const BaseUtf8String &other) const;
		bool operator>(const BaseUtf8String &other) const;

		operator std::string() const;
	  protected:
		BaseUtf8String();
		BaseUtf8String(std::unique_ptr<icu_75::UnicodeString> &&str);
		icu_75::UnicodeString &GetIcuString();
		const icu_75::UnicodeString &GetIcuString() const;
		std::unique_ptr<icu_75::UnicodeString> m_string;
	};

	class Utf8StringView;
	class Utf8StringArg;
	class DLLUUNIC Utf8String : public BaseUtf8String {
	  public:
		friend Utf8StringView;
		Utf8String();
		Utf8String(char16_t c);
		Utf8String(const Utf8String &str);
		Utf8String(const std::string &str);
		Utf8String(const char *str);
		Utf8String(const char *str, size_t count);
		Utf8String(const char16_t *str);
		Utf8String(const char16_t *str, size_t count);
		Utf8String &operator=(const Utf8String &str);
		Utf8String &operator=(const Utf8StringView &str);
		Utf8String &operator=(const std::string &str);
		Utf8String &operator=(const char *str);
		Utf8String &operator=(const char16_t *str);

		Utf8String operator+(const Utf8String &str) const;
		Utf8String operator+(const Utf8StringView &str) const;
		Utf8String operator+(const std::string &str) const;
		Utf8String operator+(const char *str) const;
		Utf8String operator+(const char16_t *str) const;
		Utf8String operator+(char c) const;
		Utf8String operator+(char16_t c) const;

		Utf8String &operator+=(const Utf8String &str);
		Utf8String &operator+=(const Utf8StringView &str);
		Utf8String &operator+=(const std::string &str);
		Utf8String &operator+=(const char *str);
		Utf8String &operator+=(const char16_t *str);
		Utf8String &operator+=(char c);
		Utf8String &operator+=(char16_t c);

		Utf8String &replace(int32_t start, int32_t len, const Utf8StringArg &str);
		UnicodeStringIterator erase(const UnicodeStringIterator &it, size_t count = 1);
		UnicodeStringIterator erase(const UnicodeStringIterator &it, const UnicodeStringIterator &itEnd);
		void insert(const UnicodeStringIterator &it, const Utf8String &str);
		Utf8String substr(size_t start, size_t count = std::numeric_limits<size_t>::max()) const;
		void clear();
	};

	class DLLUUNIC Utf8StringView : public BaseUtf8String {
	  public:
		friend Utf8String;
		Utf8StringView();
		Utf8StringView(const Utf8String &str);
		Utf8StringView(const Utf8StringView &str);
		~Utf8StringView();
		Utf8StringView &operator=(const Utf8String &str);
		Utf8StringView &operator=(const Utf8StringView &str);
		Utf8StringView substr(size_t start, size_t count = std::numeric_limits<size_t>::max()) const;
		Utf8String to_str() const;
	};

	class DLLUUNIC Utf8StringArg {
	  public:
		Utf8StringArg(const char *text);
		Utf8StringArg(const std::string &text);
		Utf8StringArg(const util::Utf8String &text);
		Utf8StringArg(const util::Utf8StringView &text);
		~Utf8StringArg();
		const util::Utf8StringView &operator*() const;
		const util::Utf8StringView *operator->() const;
	  private:
		std::unique_ptr<util::Utf8String> m_cpy;
		util::Utf8StringView m_view;
	};
};

DLLUUNIC std::ostream &operator<<(std::ostream &out, const util::Utf8String &str);
DLLUUNIC std::ostream &operator<<(std::ostream &out, const util::Utf8StringView &str);

#endif