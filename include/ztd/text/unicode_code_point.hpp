// =============================================================================
//
// ztd.text
// Copyright © 2021 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact: opensource@soasis.org
//
// Commercial License Usage
// Licensees holding valid commercial ztd.text licenses may use this file in
// accordance with the commercial license agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// a written agreement between you and Shepherd's Oasis, LLC.
// For licensing terms and conditions see your agreement. For
// further information contact opensource@soasis.org.
//
// Apache License Version 2 Usage
// Alternatively, this file may be used under the terms of Apache License
// Version 2.0 (the "License") for non-commercial use; you may not use this
// file except in compliance with the License. You may obtain a copy of the 
// License at
//
//		http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// =============================================================================

#pragma once

#ifndef ZTD_TEXT_UNICODE_CODE_POINT_HPP
#define ZTD_TEXT_UNICODE_CODE_POINT_HPP

#include <ztd/text/version.hpp>

#include <utility>
#if ZTD_TEXT_IS_ON(ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE_I_)
#include <string>
#include <cstdint>
#endif

namespace ztd { namespace text {
	ZTD_TEXT_INLINE_ABI_NAMESPACE_OPEN_I_

#if ZTD_TEXT_IS_ON(ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE_I_)
	//////
	/// @brief A 32-bit value that is within the allowed 21 bits of Unicode. Can be one of the surrogate values.
	///
	//////
	class unicode_code_point {
	public:
		//////
		/// @brief Constructs a code point value of indeterminate value (if no parentheses/brackets are provided) or
		/// with the value 0 (if parentheses/brackets are provided for intentional value initialization).
		///
		//////
		constexpr unicode_code_point() noexcept = default;

		//////
		/// @brief Constructs a code point value with the given code point value.
		///
		/// @remarks
		//////
		constexpr unicode_code_point(char32_t __code_point) noexcept : _M_scalar(__code_point) {
#if ZTD_TEXT_IS_ON(ZTD_TEXT_UNICODE_SCALAR_VALUE_INVARIANT_ABORT_I_)
			if (__detail::__is_surrogate(this->_M_scalar) || (this->_M_scalar > __detail::__last_code_point)) {
				::std::abort();
			}
#else
			ZTD_TEXT_ASSERT_MESSAGE_I_("The code point value must be a valid code point.",
				(this->_M_scalar <= __detail::__last_code_point));
#endif
		}

		//////
		/// @brief An explicit conversion to a typical char32_t value, bit-compatible with a normal code point value.
		///
		//////
		constexpr explicit operator char32_t() const noexcept {
			return this->_M_scalar;
		}

		//////
		/// @brief Retrieves the underlying value.
		///
		//////
		constexpr const char32_t& value() const& noexcept {
			return this->_M_scalar;
		}

		//////
		/// @brief Retrieves the underlying value.
		///
		//////
		constexpr char32_t& value() & noexcept {
			return this->_M_scalar;
		}

		//////
		/// @brief Retrieves the underlying value.
		///
		//////
		constexpr char32_t&& value() && noexcept {
			return ::std::move(this->_M_scalar);
		}

	private:
		char32_t _M_scalar;
	};
#else
	// TODO: what are the merits of a strong type here?
	using unicode_code_point = char32_t;
#endif

	ZTD_TEXT_INLINE_ABI_NAMESPACE_CLOSE_I_
}} // namespace ztd::text

#if ZTD_TEXT_IS_ON(ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE_I_)
namespace std {

	template <>
	class char_traits<::ztd::text::unicode_code_point> {
		using char_type  = ::ztd::text::unicode_code_point;
		using int_type   = ::std::int_least32_t;
		using pos_type   = ::std::streampos;
		using off_type   = ::std::streamoff;
		using state_type = ::std::mbstate_t;

		static /*constexpr*/ char_type* copy(
		     char_type* __dest, const char_type* __src, ::std::size_t __count) noexcept {
			// TODO: constexpr. right now illegal cast
			return reinterpret_cast<char_type*>(::std::char_traits<char32_t>::copy(
			     reinterpret_cast<char32_t*>(__destination), reinterpret_cast<const char32_t*>(__source), __count));
		}

		static /*constexpr*/ char_type* move(
		     char_type* __destination, const char_type* __source, std::size_t __count) noexcept {
			// TODO: constexpr. right now illegal cast
			return reinterpret_cast<char_type*>(::std::char_traits<char32_t>::move(
			     reinterpret_cast<char32_t*>(__destination), reinterpret_cast<const char32_t*>(__source), __count));
		}

		ZTD_TEXT_NODISCARD_I_ static /*constexpr*/ int compare(
		     const char_type* __left, const char_type* __right, std::size_t __count) noexcept {
			// TODO: constexpr. right now illegal cast
			return ::std::char_traits<char32_t>::compare(
			     reinterpret_cast<const char32_t*>(__left), reinterpret_cast<const char32_t*>(__right), __count);
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr size_t length(const char_type* __it) noexcept {
			size_t __count = 0;
			const char_type __null_value {};
			while (*__it != __null_value) {
				++__count;
				++__it;
			}
			return __count;
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr const char_type* find(
		     const char_type* __it, size_t __count, const char_type& __c) noexcept {
			for (; 0 < __count; --__count, (void)++__it) {
				if (*__it == __c) {
					return __it;
				}
			}
			return nullptr;
		}

		static constexpr char_type* assign(char_type* __first, size_t __count, const char_type __c) noexcept {
			for (char_type* __it = __first; __count > 0; --__count, (void)++__it) {
				*__it = __c;
			}
			return __first;
		}

		static constexpr void assign(char_type& __left, const char_type& __right) noexcept {
			__left = __right;
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr bool eq(const char_type& __left, const char_type& __right) noexcept {
			return __left == __right;
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr bool lt(const char_type& __left, const char_type& __right) noexcept {
			return __left < __right;
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr char_type to_char_type(const int_type& __c_as_int) noexcept {
			return char_type(static_cast<char32_t>(__c_as_int));
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr int_type to_int_type(const char_type& __c) noexcept {
			return static_cast<int_type>(__c.value());
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr bool eq_int_type(
		     const int_type& __left, const int_type& __right) noexcept {
			return __left == __right;
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr int_type not_eof(const int_type& __c_as_int) noexcept {
			return __c_as_int != eof() ? __c_as_int : !eof();
		}

		ZTD_TEXT_NODISCARD_I_ static constexpr int_type eof() noexcept {
			return static_cast<int_type>(EOF);
		}
	};
} // namespace std
#endif

#endif // ZTD_TEXT_UNICODE_CODE_POINT_HPP