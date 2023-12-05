#ifndef H_PARSER
#define H_PARSER

#include "string-literal.hpp"
#include "game.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include <exception>
#include <optional>
#include <span>

/*
input := game* eof
game := GAME NUMBER COLON set SEMICOLON set SEMICOLON set
set := cube (COMMA cube)? (COMMA cube)?
cube := NUMBER COLOR
*/

template<size_t N>
class parser{
public:
	constexpr parser(aoc::utils::string_literal<N> input) : m_tokens(scan_all(input)){}
	constexpr std::vector<game> parse_games(){
		std::vector<game> games{};
		while(!is_at_end()){
			games.push_back(parse_game());
		}
		return games;
	}

private:
	static void throw_error(std::string_view message, size_t location, std::string_view lexeme){
		std::cout << message << "(at location: " << location << ", lexeme: " << lexeme << ")\n";
		throw;
	}

	constexpr game parse_game(){
		if(!match(token_type::game))
			throw_error("expected game", m_currentPos, peek().lexeme());
		if(!match(token_type::number))
			throw_error("expected number", m_currentPos, peek().lexeme());

		int64_t number = previous().number_literal().value();

		if(!match(token_type::colon))
			throw_error("expected colon", m_currentPos, peek().lexeme());

		std::vector<cube_set> sets{};
		sets.push_back(parse_set());
		while(peek().type() == token_type::semicolon){
			if(!match(token_type::semicolon))
				throw_error("expected semicolon", m_currentPos, peek().lexeme());
			sets.push_back(parse_set());
		}

		return game(number, sets);
	}

	constexpr cube_set parse_set(){
		cube_set set{};
		cube cube = parse_cube();

		if(cube.color() == cube_color::red)
			set.redCubes += cube.amount();
		if(cube.color() == cube_color::green)
			set.greenCubes += cube.amount();
		if(cube.color() == cube_color::blue)
			set.blueCubes += cube.amount();

		while(peek().type() == token_type::comma){
			m_currentPos++;
			cube = parse_cube();

			if(cube.color() == cube_color::red)
				set.redCubes += cube.amount();
			if(cube.color() == cube_color::green)
				set.greenCubes += cube.amount();
			if(cube.color() == cube_color::blue)
				set.blueCubes += cube.amount();
		}

		return set;
	}

	constexpr cube parse_cube(){
		if(!match(token_type::number))
			throw_error("expected number", m_currentPos, peek().lexeme());

		int64_t number = previous().number_literal().value();
		
		if(!match(token_type::color))
			throw_error("expected color", m_currentPos, peek().lexeme());

		cube_color color = previous().color_literal().value();

		return cube(color, number);
	}

	constexpr bool match(std::span<token_type> tokenTypes){
		for(token_type type : tokenTypes){
			if(check(type)){
				advance();
				return true;
			}
		}

		return false;
	}
	constexpr bool match(token_type type){
		if(check(type)){
			advance();
			return true;
		}

		return false;
	}

	constexpr bool check(token_type type){
		if(is_at_end())
			return false;
		return peek().type() == type;
	}

	constexpr const token<N>& advance(){
		if(!is_at_end())
			m_currentPos++;
		return previous();
	}

	constexpr bool is_at_end() const{
		return peek().type() == token_type::eof;
	}

	constexpr const token<N>& previous(size_t n = 1) const{
		return m_tokens[m_currentPos - n];
	}

	constexpr const token<N>& peek() const{
		return m_tokens[m_currentPos];
	}

	static constexpr std::vector<token<N>> scan_all(aoc::utils::string_literal<N> input){
		std::vector<token<N>> tokens;
		scanner scan(input);
		size_t i = 0;
		token token{scan.scan_token()};
		while(token.type() != token_type::eof){
			tokens.push_back(token);
			token = scan.scan_token();
		}
		tokens.push_back(token);
		return tokens;
	}

	std::vector<token<N>> m_tokens;
	size_t m_currentPos = 0;
};

template<aoc::utils::StringLiteral T>
parser(T&&) -> parser<aoc::utils::get_size_v<T>>;

#endif
