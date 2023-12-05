#ifndef H_SCANNER
#define H_SCANNER

#include "string-literal.hpp"
#include "token.hpp"
#include <vector>
#include <string>
#include <iostream>

template<size_t N>
class scanner{
public:
	constexpr scanner(aoc::utils::string_literal<N> input) : m_input(input){}

	constexpr token<N> scan_token(){
		if(is_at_end())
			return token<N>::create(token_type::eof, m_currentPos, 0, m_line, m_input);

		char c = advance();
		switch(c){
			case ':':
				return token<N>::create(token_type::colon, m_currentPos, 1, m_line, m_input);
			case ',':
				return token<N>::create(token_type::comma, m_currentPos, 1, m_line, m_input);
			case ';':
				return token<N>::create(token_type::semicolon, m_currentPos, 1, m_line, m_input);
			case ' ':
			case '\n':
			case '\r':
			case '\t':
				m_line++;
				return scan_token();
			case '\0':
				return token<N>::create(token_type::eof, m_currentPos, 0, m_line, m_input);
			default:
				if(is_digit(c))
					return scan_number(c);

				std::optional<token<N>> tryScanGameResult = try_scan_game(c);
				if(tryScanGameResult.has_value())
					return tryScanGameResult.value();

				std::optional<token<N>> tryScanColorResult = try_scan_color(c);
				if(tryScanColorResult.has_value())
					return tryScanColorResult.value();

				std::cout << "Unexpected character: '" << c << "' at location " << m_currentPos << "\n";
				throw;
		}
	}

private:
	static constexpr uint64_t pow10(size_t amount, uint64_t start = 1){
		if(amount == 0)
			return start;

		return pow10(amount - 1, start * 10);
	}

	static constexpr uint8_t digit_value(char c){
		return c - '0';
	}

	constexpr std::optional<token<N>> try_scan_game(char startChar){
		if(startChar != 'G')
			return std::nullopt;
		if(peek(0) != 'a')
			return std::nullopt;
		if(peek(1) != 'm')
			return std::nullopt;
		if(peek(2) != 'e')
			return std::nullopt;

		size_t startPosition = m_currentPos;
		consume(3);
		return token<N>::create(token_type::game, startPosition, 4, m_line, m_input);
	}

	constexpr std::optional<token<N>> try_scan_color(char startChar){
		size_t startPosition = m_currentPos;
		if(startChar == 'r' && peek(0) == 'e' && peek(1) == 'd'){
			consume(2);
			return token<N>::create_color(cube_color::red, startPosition, 3, m_line, m_input);
		}
		if(startChar == 'g' && peek(0) == 'r' && peek(1) == 'e' && peek(2) == 'e' && peek(3) == 'n'){
			consume(4);
			return token<N>::create_color(cube_color::green, startPosition, 5, m_line, m_input);
		}
		if(startChar == 'b' && peek(0) == 'l' && peek(1) == 'u' && peek(2) == 'e'){
			consume(3);
			return token<N>::create_color(cube_color::blue, startPosition, 4, m_line, m_input);
		}
		return std::nullopt;
	}

	constexpr token<N> scan_number(char startChar){
		size_t startPosition = m_currentPos;
		std::vector<char> numberChars{startChar};

		while(is_digit(peek())){
			numberChars.push_back(advance());
		}

		uint64_t numberValue = 0;
		for(size_t i = 0; i < numberChars.size(); ++i){
			char c = numberChars[i];
			numberValue += (digit_value(c) * pow10(numberChars.size() - i - 1));
		}

		return token<N>::create_number(numberValue, startPosition, m_currentPos - startPosition + 1, m_line, m_input);
	}

	static constexpr bool is_digit(char c){
		return c >= '0' && c <= '9';
	}

	constexpr bool is_at_end() const{
		return m_currentPos >= N || m_input[m_currentPos] == '\0';
	}

	constexpr char advance(){
		return m_input[m_currentPos++];
	}

	constexpr char peek(size_t n = 0) const{
		if(is_at_end())
			return '\0';
		return m_input[m_currentPos + n];
	}

	constexpr void consume(size_t amount = 1){
		m_currentPos += amount;
	}

	aoc::utils::string_literal<N> m_input;
	size_t m_currentPos = 0;
	size_t m_line = 0;
};

template<aoc::utils::StringLiteral T>
scanner(T&&) -> scanner<aoc::utils::get_size_v<T>>;
#endif
