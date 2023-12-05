#ifndef H_TOKEN
#define H_TOKEN

#include "string-literal.hpp"
#include "game.hpp"
#include <cstdint>
#include <cstddef>
#include <optional>
#include <string_view>
#include <iostream>

enum class token_type{
	game,
	number,
	colon,
	color,
	comma,
	semicolon,
	eof
};

template<size_t N>
class token{
public:
	constexpr token(const token&) = default;
	constexpr token(token&&) = default;
	constexpr token& operator=(const token&) = default;
	constexpr token& operator=(token&&) = default;

	static constexpr token create(
		token_type type,
		size_t startPosition,
		size_t size,
		size_t line,
		aoc::utils::string_literal<N> source
	){
		return token(type, std::nullopt, std::nullopt, startPosition, size, line, source);
	}

	static constexpr token create_number(
		int64_t numberLiteral,
		size_t startPosition,
		size_t size,
		size_t line,
		aoc::utils::string_literal<N> source
	){
		return token(token_type::number, numberLiteral, std::nullopt, startPosition, size, line, source);
	}

	static constexpr token create_color(
		cube_color colorLiteral,
		size_t startPosition,
		size_t size,
		size_t line,
		aoc::utils::string_literal<N> source
	){
		return token(token_type::color, std::nullopt, colorLiteral, startPosition, size, line, source);
	}

	constexpr token_type type() const{
		return m_type;
	}

	constexpr std::string_view lexeme() const{
		return std::string_view(m_source.get()).substr(m_startPosition - 1, m_size);
	}

	constexpr std::optional<int64_t> number_literal() const{
		return m_numberLiteral;
	}

	constexpr std::optional<cube_color> color_literal() const{
		return m_colorLiteral;
	}

	constexpr size_t start_position() const{
		return m_startPosition;
	}

	constexpr size_t size() const{
		return m_size;
	}

	constexpr size_t line() const{
		return m_line;
	}
private:
	constexpr token(
		token_type type,
		std::optional<int64_t> numberLiteral,
		std::optional<cube_color> colorLiteral,
		size_t startPosition,
		size_t size,
		size_t line,
		aoc::utils::string_literal<N> source
	)
		: m_type(type),
		m_numberLiteral(numberLiteral),
		m_colorLiteral(colorLiteral),
		m_startPosition(startPosition),
		m_size(size),
		m_line(line),
		m_source(source){
	}

	token_type m_type;
	std::optional<int64_t> m_numberLiteral;
	std::optional<cube_color> m_colorLiteral;
	size_t m_startPosition;
	size_t m_size;
	size_t m_line;
	aoc::utils::string_literal<N> m_source;
};

#endif
