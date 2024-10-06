/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

module;

#include "definitions.hpp"
#include <memory>
#include <string>
#include <ostream>
#include <limits>

export module pragma.string.unicode:utf8;

export namespace pragma::string {
	class Utf8String;
	class BaseUtf8String;
	using Char8 = char;
	using Char16 = char16_t;
	using Char32 = int32_t;
	class DLLUUNIC UnicodeStringIterator {
	  public:
		friend Utf8String;
		using value_type = Char32;
		UnicodeStringIterator(const BaseUtf8String &str);
		UnicodeStringIterator(const UnicodeStringIterator &it);
		~UnicodeStringIterator();

		bool operator<(const UnicodeStringIterator &other) const;
		bool operator>(const UnicodeStringIterator &other) const;
		bool operator==(const UnicodeStringIterator &other) const;
		bool operator!=(const UnicodeStringIterator &other) const;

		Char32 operator*() const;
		UnicodeStringIterator &operator++();
		UnicodeStringIterator &operator--();
		UnicodeStringIterator operator+(int32_t idx) const;
		UnicodeStringIterator &operator+=(int32_t idx);
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
		virtual ~BaseUtf8String();
		std::string cpp_str() const;
		size_t find(Char8 c, size_t startPos = 0) const;
		size_t find(Char16 c, size_t startPos = 0) const;
		size_t find(Char32 c, size_t startPos = 0) const;
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
		Char32 at(size_t idx) const;
		Char32 front() const;
		Char32 back() const;

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
		Utf8String(Char16 c);
		Utf8String(Char32 c);
		Utf8String(const Utf8String &str);
		Utf8String(const Utf8StringView &str);
		Utf8String(const std::string &str);
		Utf8String(const char *str);
		Utf8String(const char *str, size_t count);
		Utf8String(const char16_t *str);
		Utf8String(const char16_t *str, size_t count);
		Utf8String(const int32_t *str, size_t count);
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
		Utf8String operator+(Char8 c) const;
		Utf8String operator+(Char16 c) const;
		Utf8String operator+(Char32 c) const;

		Utf8String &operator+=(const Utf8String &str);
		Utf8String &operator+=(const Utf8StringView &str);
		Utf8String &operator+=(const std::string &str);
		Utf8String &operator+=(const char *str);
		Utf8String &operator+=(const char16_t *str);
		Utf8String &operator+=(Char8 c);
		Utf8String &operator+=(Char16 c);
		Utf8String &operator+=(Char32 c);

		Utf8String &replace(int32_t start, int32_t len, const Utf8StringArg &str);
		UnicodeStringIterator erase(const UnicodeStringIterator &it, size_t count = 1);
		UnicodeStringIterator erase(const UnicodeStringIterator &it, const UnicodeStringIterator &itEnd);
		void insert(const UnicodeStringIterator &it, const Utf8StringArg &str);
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
	  private:
		const icu_75::UnicodeString *m_underlyingString = nullptr;
		size_t m_start = 0;
		size_t m_length = std::numeric_limits<size_t>::max();
	};

	class DLLUUNIC Utf8StringArg {
	  public:
		Utf8StringArg(const char *text);
		Utf8StringArg(const std::string &text);
		Utf8StringArg(const Utf8String &text);
		Utf8StringArg(const Utf8StringView &text);
		~Utf8StringArg();
		const Utf8StringView &operator*() const;
		const Utf8StringView *operator->() const;
	  private:
		std::unique_ptr<Utf8String> m_cpy;
		Utf8StringView m_view;
	};
};

export DLLUUNIC std::ostream &operator<<(std::ostream &out, const pragma::string::Utf8String &str);
export DLLUUNIC std::ostream &operator<<(std::ostream &out, const pragma::string::Utf8StringView &str);
