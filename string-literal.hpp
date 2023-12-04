#ifndef H_STRING_LITERAL
#define H_STRING_LITERAL

#include <cstddef>
#include <array>
#include <string_view>
#include <type_traits>

namespace aoc::utils{

	template<size_t N>
	class string_literal;

	template<typename T>
	struct get_size;

	template<size_t N>
	struct get_size<const string_literal<N>&>{
		static constexpr size_t value = N;
	};

	template<size_t N>
	struct get_size<string_literal<N>&>{
		static constexpr size_t value = N;
	};

	template<size_t N>
	struct get_size<string_literal<N>>{
		static constexpr size_t value = N;
	};

	template<size_t N>
	struct get_size<const std::array<char, N>&>{
		static constexpr size_t value = N + 1;
	};

	template<size_t N>
	struct get_size<std::array<char, N>&>{
		static constexpr size_t value = N + 1;
	};

	template<size_t N>
	struct get_size<std::array<char, N>>{
		static constexpr size_t value = N + 1;
	};

	template<size_t N>
	struct get_size<const char (&)[N]>{
		static constexpr size_t value = N;
	};

	template<typename T>
	static inline constexpr size_t get_size_v = get_size<T>::value;

	template<typename T>
	struct is_string_literal : std::false_type{};

	template<size_t N>
	struct is_string_literal<string_literal<N>&> : std::true_type{};

	template<size_t N>
	struct is_string_literal<const string_literal<N>&> : std::true_type{};

	template<size_t N>
	struct is_string_literal<string_literal<N>> : std::true_type{};

	template<size_t N>
	struct is_string_literal<std::array<char, N>> : std::true_type{};

	template<size_t N>
	struct is_string_literal<std::array<char, N>&> : std::true_type{};

	template<size_t N>
	struct is_string_literal<const std::array<char, N>&> : std::true_type{};

	template<size_t N>
	struct is_string_literal<const char (&)[N]> : std::true_type{};

	template<typename T>
	static inline constexpr bool is_string_literal_v = is_string_literal<T>::value;

	template<typename T>
	concept StringLiteral = is_string_literal_v<T>;

	template<StringLiteral... Ts>
	static constexpr string_literal<1 + (get_size_v<Ts> + ...) - sizeof...(Ts)> concat_strings(Ts&&... ts){
		return string_literal(ts...);
	}

	template<size_t N>
	class string_literal{
	public:
		static constexpr size_t total_size = N;

		constexpr string_literal() : m_str(){}
		constexpr string_literal(const char (&str)[N]) : m_str(get_array(str)) {}
		constexpr string_literal(const string_literal<N>& str) : m_str(get_array(str)) {}
		constexpr string_literal(string_literal<N>& str) : m_str(get_array(str)) {}
		constexpr string_literal(const std::array<char, N - 1>& str) : m_str(get_array(str)) {}
		constexpr string_literal(std::array<char, N - 1>& str) : m_str(get_array(str)) {}
		constexpr string_literal(StringLiteral auto&&... ts) : m_str(concat_internal(ts...)){}

		constexpr std::array<char, N> get_array() const{
			return m_str;
		}
		constexpr std::string_view get() const{
			return m_str.data();
		}
		constexpr size_t size() const{
			return N;
		}
		constexpr char operator[](size_t i) const{
			return m_str[i];
		}
		constexpr char& operator[](size_t i){
			return m_str[i];
		}
		template<StringLiteral... Ts>
		constexpr string_literal<1 + (N - 1) + (get_size_v<Ts> + ...) - sizeof...(Ts)> concat(Ts&&... ts) const {
			return concat_strings(*this, ts...);
		}

		template<StringLiteral T>
		constexpr string_literal<1 + (N - 1) + (get_size_v<T> - 1)> operator+(T&& t) const{
			return concat_strings(*this, t);
		}
	private:
		static constexpr std::array<char, N> get_array(const char (&str)[N]){
			std::array<char, N> result{};
			for(size_t i = 0; i < N; ++i){
				result[i] = str[i];
			}
			return result;
		}

		static constexpr std::array<char, N> get_array(const string_literal<N>& str){
			return str.m_str;
		}

		static constexpr std::array<char, N> get_array(const std::array<char, N - 1>& str){
			std::array<char, N> result{};
			for(size_t i = 0; i < (N - 1); ++i){
				result[i] = str[i];
			}
			result[N - 1] = '\0';
			return result;
		}

		template<StringLiteral... Ts>
		static constexpr std::array<char, 1 + (get_size_v<Ts> + ...) - sizeof...(Ts)> concat_internal(Ts&&... ts){
			std::array<char, 1 + (get_size_v<Ts> + ...) - sizeof...(Ts)> result{};
			size_t offset = 0;
			([&]{
				for(size_t i = 0; i < (get_size_v<Ts> - 1); ++i){
					result[offset++] = ts[i];
				}
			}(), ...);
			result[offset++] = '\0';

			return result;
		}

		std::array<char, N> m_str;
	};

	template<StringLiteral... Ts>
	string_literal(Ts&&...) -> string_literal<1 + (get_size_v<Ts> + ...) - sizeof...(Ts)>;

	template<StringLiteral TA, StringLiteral TB>
	static inline constexpr string_literal<1 + (get_size_v<TA> - 1) + (get_size_v<TB> - 1)> operator+(TA&& a, TB&& b){
		return concat_strings(a, b);
	}
}

#endif
