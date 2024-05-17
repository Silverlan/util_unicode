/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "util_unicode.hpp"
#include <unicode/unistr.h>
#include <unicode/schriter.h>

using namespace util;

BaseUtf8String::BaseUtf8String() : m_string {std::make_unique<icu::UnicodeString>()} {}
BaseUtf8String::BaseUtf8String(std::unique_ptr<icu_75::UnicodeString> &&str) : m_string {std::move(str)} {}
BaseUtf8String::~BaseUtf8String() {}
icu_75::UnicodeString &BaseUtf8String::GetIcuString() { return *m_string; }
const icu_75::UnicodeString &BaseUtf8String::GetIcuString() const { return *m_string; }
size_t BaseUtf8String::length() const { return m_string->length(); }
size_t BaseUtf8String::size() const { return m_string->length(); }

size_t BaseUtf8String::find(Char8 c, size_t startPos) const
{
	auto pos = m_string->indexOf(c);
	return (pos >= 0) ? pos : std::string::npos;
}
size_t BaseUtf8String::find(Char16 c, size_t startPos) const
{
	auto pos = m_string->indexOf(c);
	return (pos >= 0) ? pos : std::string::npos;
}
size_t BaseUtf8String::find(Char32 c, size_t startPos) const
{
	auto pos = m_string->indexOf(c);
	return (pos >= 0) ? pos : std::string::npos;
}
size_t BaseUtf8String::find(const char *str, size_t startPos) const
{
	auto pos = m_string->indexOf(str);
	return (pos >= 0) ? pos : std::string::npos;
}
size_t BaseUtf8String::find(const std::string &str, size_t startPos) const { return find(str.data(), startPos); }
size_t BaseUtf8String::find(const BaseUtf8String &strU8, size_t startPos) const { return m_string->indexOf(*strU8.m_string, startPos); }
Char32 BaseUtf8String::at(size_t idx) const { return m_string->char32At(idx); }
Char32 BaseUtf8String::front() const { return m_string->char32At(0); }
Char32 BaseUtf8String::back() const { return m_string->char32At(m_string->length() - 1); }
void BaseUtf8String::toUpper() { m_string->toUpper(); }
void BaseUtf8String::toLower() { m_string->toLower(); }
UnicodeStringIterator BaseUtf8String::begin() const { return *this; }
UnicodeStringIterator BaseUtf8String::end() const
{
	UnicodeStringIterator it {*this};
	it.done = true;
	return it;
}
std::string BaseUtf8String::cpp_str() const
{
	std::string utf8String;
	m_string->toUTF8String(utf8String);
	return utf8String;
}
const char16_t *BaseUtf8String::data() { return m_string->getTerminatedBuffer(); }
const char16_t *BaseUtf8String::c_str() { return data(); }
BaseUtf8String::operator std::string() const { return cpp_str(); }
bool BaseUtf8String::operator==(const BaseUtf8String &other) const { return *m_string == *other.m_string; }
bool BaseUtf8String::operator!=(const BaseUtf8String &other) const { return !operator==(other); }
bool BaseUtf8String::operator==(const std::string &other) const { return operator==(Utf8String {other}); }
bool BaseUtf8String::operator!=(const std::string &other) const { return !operator==(other); }
bool BaseUtf8String::operator<(const BaseUtf8String &other) const { return m_string < other.m_string; }
bool BaseUtf8String::operator>(const BaseUtf8String &other) const { return m_string > other.m_string; }

////////////////////

Utf8String::Utf8String() : BaseUtf8String {} {}
Utf8String::Utf8String(Char16 c) : Utf8String {} { *this += c; }
Utf8String::Utf8String(Char32 c) : Utf8String {} { *this += c; }
Utf8String::Utf8String(const Utf8String &str) : BaseUtf8String {std::make_unique<icu::UnicodeString>(*str.m_string)} {}
Utf8String::Utf8String(const Utf8StringView &str) : BaseUtf8String {std::make_unique<icu::UnicodeString>(*str.m_string)} {}
Utf8String::Utf8String(const std::string &str) : BaseUtf8String {std::make_unique<icu::UnicodeString>(icu::UnicodeString::fromUTF8(str))} {}
Utf8String::Utf8String(const char *str) : BaseUtf8String {std::make_unique<icu::UnicodeString>(icu::UnicodeString::fromUTF8(icu::StringPiece {str}))} {}
Utf8String::Utf8String(const char *str, size_t count) : BaseUtf8String {std::make_unique<icu::UnicodeString>(icu::UnicodeString::fromUTF8(icu::StringPiece {str, static_cast<int32_t>(count)}))} {}
Utf8String::Utf8String(const char16_t *str) : BaseUtf8String {std::make_unique<icu::UnicodeString>(str)} {}
Utf8String::Utf8String(const char16_t *str, size_t count) : BaseUtf8String {std::make_unique<icu::UnicodeString>(str, count)} {}
Utf8String &Utf8String::operator=(const Utf8String &str)
{
	m_string = std::make_unique<icu::UnicodeString>(*str.m_string);
	return *this;
}
Utf8String &Utf8String::operator=(const Utf8StringView &str)
{
	m_string = std::make_unique<icu::UnicodeString>(*str.m_string);
	return *this;
}
Utf8String &Utf8String::operator=(const std::string &str)
{
	m_string = std::make_unique<icu::UnicodeString>(icu::UnicodeString::fromUTF8(icu::StringPiece {str}));
	return *this;
}
Utf8String &Utf8String::operator=(const char *str)
{
	m_string = std::make_unique<icu::UnicodeString>(icu::UnicodeString::fromUTF8(icu::StringPiece {str}));
	return *this;
}
Utf8String &Utf8String::operator=(const char16_t *str)
{
	m_string = std::make_unique<icu::UnicodeString>(str);
	return *this;
}

Utf8String Utf8String::operator+(const Utf8String &str) const
{
	auto cpy = *this;
	cpy.operator+=(str);
	return cpy;
}
Utf8String Utf8String::operator+(const Utf8StringView &str) const
{
	auto cpy = *this;
	cpy.operator+=(str);
	return cpy;
}
Utf8String Utf8String::operator+(const std::string &str) const
{
	auto cpy = *this;
	cpy.operator+=(str);
	return cpy;
}
Utf8String Utf8String::operator+(const char *str) const
{
	auto cpy = *this;
	cpy.operator+=(str);
	return cpy;
}
Utf8String Utf8String::operator+(const char16_t *str) const
{
	auto cpy = *this;
	cpy.operator+=(str);
	return cpy;
}
Utf8String Utf8String::operator+(Char8 c) const
{
	auto cpy = *this;
	cpy.operator+=(c);
	return cpy;
}
Utf8String Utf8String::operator+(Char16 c) const
{
	auto cpy = *this;
	cpy.operator+=(c);
	return cpy;
}
Utf8String Utf8String::operator+(Char32 c) const
{
	auto cpy = *this;
	cpy.operator+=(c);
	return cpy;
}

Utf8String &Utf8String::operator+=(const Utf8String &str)
{
	*m_string += *str.m_string;
	return *this;
}
Utf8String &Utf8String::operator+=(const Utf8StringView &str)
{
	*m_string += *str.m_string;
	return *this;
}
Utf8String &Utf8String::operator+=(const std::string &str) { return operator+=(Utf8String {str}); }
Utf8String &Utf8String::operator+=(const char *str) { return operator+=(Utf8String {str}); }
Utf8String &Utf8String::operator+=(const char16_t *str) { return operator+=(Utf8String {str}); }
Utf8String &Utf8String::operator+=(Char8 c)
{
	*m_string += c;
	return *this;
}
Utf8String &Utf8String::operator+=(Char16 c)
{
	*m_string += c;
	return *this;
}
Utf8String &Utf8String::operator+=(Char32 c)
{
	*m_string += c;
	return *this;
}
Utf8String &Utf8String::replace(int32_t start, int32_t len, const Utf8StringArg &str)
{
	m_string->replace(start, len, str->GetIcuString());
	return *this;
}
UnicodeStringIterator Utf8String::erase(const UnicodeStringIterator &it, size_t count)
{
	auto idx = it.iterator->getIndex();
	m_string->removeBetween(idx, idx + count);

	UnicodeStringIterator newIt {};
	if(m_string->isEmpty()) {
		newIt.iterator = std::make_unique<icu::StringCharacterIterator>(*m_string);
		newIt.done = true;
	}
	else
		newIt.iterator = std::make_unique<icu::StringCharacterIterator>(*m_string, idx);
	return newIt;
}
UnicodeStringIterator Utf8String::erase(const UnicodeStringIterator &it, const UnicodeStringIterator &itEnd)
{
	auto idx0 = it.iterator->getIndex();
	auto idx1 = itEnd.iterator->getIndex();
	if(idx1 <= idx0)
		return it;
	return erase(it, (idx1 - idx0) + 1);
}
void Utf8String::insert(const UnicodeStringIterator &it, const Utf8StringArg &str) { m_string->insert(it.iterator->getIndex(), str->GetIcuString()); }
Utf8String Utf8String::substr(size_t start, size_t count) const
{
	if(count > INT32_MAX)
		count = INT32_MAX;
	auto sub = m_string->tempSubString(start, count);
	return Utf8String {sub.getTerminatedBuffer()};
}
void Utf8String::clear() { m_string->remove(); }

////////////////////

Utf8StringView::Utf8StringView() : BaseUtf8String {} {}
Utf8StringView::Utf8StringView(const Utf8String &str) : BaseUtf8String {} { m_string->fastCopyFrom(*str.m_string); }
Utf8StringView::Utf8StringView(const Utf8StringView &str) : BaseUtf8String {} { m_string->fastCopyFrom(*str.m_string); }
Utf8StringView::~Utf8StringView() {}
Utf8StringView &Utf8StringView::operator=(const Utf8String &str)
{
	m_string->fastCopyFrom(*str.m_string);
	return *this;
}
Utf8StringView &Utf8StringView::operator=(const Utf8StringView &str)
{
	m_string->fastCopyFrom(*str.m_string);
	return *this;
}
Utf8StringView Utf8StringView::substr(size_t start, size_t count) const
{
	if(count > INT32_MAX)
		count = INT32_MAX;
	Utf8StringView sub {};
	sub.m_string->fastCopyFrom(m_string->tempSubString(start, count));
	return sub;
}
Utf8String Utf8StringView::to_str() const { return {*this}; }

////////////////////

UnicodeStringIterator::UnicodeStringIterator() : iterator {}, done {false} {}
UnicodeStringIterator::UnicodeStringIterator(const BaseUtf8String &str) : iterator {std::make_unique<icu::StringCharacterIterator>(str.GetIcuString())}, done {false} {}
UnicodeStringIterator::UnicodeStringIterator(const UnicodeStringIterator &it) : iterator {std::make_unique<icu::StringCharacterIterator>(*it.iterator)}, done {false} {}

UnicodeStringIterator::~UnicodeStringIterator() {}

bool UnicodeStringIterator::operator<(const UnicodeStringIterator &other) const { return iterator->current32() < other.iterator->current32(); }
bool UnicodeStringIterator::operator>(const UnicodeStringIterator &other) const { return iterator->current32() > other.iterator->current32(); }

Char32 UnicodeStringIterator::operator*() const { return iterator->current32(); }

UnicodeStringIterator &UnicodeStringIterator::operator++()
{
	auto c = iterator->next32();
	if(c == icu::CharacterIterator::DONE)
		done = true;
	return *this;
}

UnicodeStringIterator &UnicodeStringIterator::operator--()
{
	auto c = iterator->previous32();
	if(c == icu::CharacterIterator::DONE)
		done = true;
	return *this;
}

UnicodeStringIterator UnicodeStringIterator::operator+(int32_t idx) const
{
	auto newIt = *this;
	newIt += idx;
	return newIt;
}

UnicodeStringIterator &UnicodeStringIterator::operator+=(int32_t idx)
{
	if(idx > 0) {
		for(int32_t i = 0; i < idx; ++i)
			operator++();
		return *this;
	}
	for(int32_t i = 0; i < idx; ++i)
		operator--();
	return *this;
}

bool UnicodeStringIterator::operator!=(const UnicodeStringIterator &other) const { return !done; }

UnicodeStringIterator &UnicodeStringIterator::operator=(const UnicodeStringIterator &other)
{
	iterator = std::make_unique<icu_75::StringCharacterIterator>(*other.iterator);
	done = other.done;
	return *this;
}

////////////////////

Utf8StringArg::Utf8StringArg(const char *text) : m_cpy {std::make_unique<Utf8String>(text)}, m_view {*m_cpy} {}
Utf8StringArg::Utf8StringArg(const std::string &text) : m_cpy {std::make_unique<Utf8String>(text)}, m_view {*m_cpy} {}
Utf8StringArg::Utf8StringArg(const util::Utf8String &text) : m_cpy {std::make_unique<Utf8String>(text)}, m_view {*m_cpy} {}
Utf8StringArg::Utf8StringArg(const util::Utf8StringView &text) : m_view {text} {}
Utf8StringArg::~Utf8StringArg() {}
const util::Utf8StringView &Utf8StringArg::operator*() const { return m_view; }
const util::Utf8StringView *Utf8StringArg::operator->() const { return &m_view; }

//////////////////////////

std::ostream &operator<<(std::ostream &out, const util::Utf8String &str)
{
	out << str.cpp_str();
	return out;
}
std::ostream &operator<<(std::ostream &out, const util::Utf8StringView &str)
{
	out << str.cpp_str();
	return out;
}
