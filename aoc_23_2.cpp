#include "parser.hpp"
#include "string-literal.hpp"
#include "scanner.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <iostream>

static constexpr aoc::utils::string_literal test_input =
"Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n"
"Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n"
"Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n"
"Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n"
"Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green";
;

static constexpr aoc::utils::string_literal input = "input...";

template<size_t N>
constexpr size_t aoc_23_2_1(aoc::utils::string_literal<N> input){
	cube_set availableCubes{};
	availableCubes.redCubes = 12;
	availableCubes.greenCubes = 13;
	availableCubes.blueCubes = 14;

	parser parser(input);
	auto games = parser.parse_games();
	size_t sum = 0;
	for(const auto& game : games){
		bool gamePossible = true;
		for(const auto& set : game.cube_sets()){
			if(set.redCubes > availableCubes.redCubes)
				gamePossible = false;
			if(set.greenCubes > availableCubes.greenCubes)
				gamePossible = false;
			if(set.blueCubes > availableCubes.blueCubes)
				gamePossible = false;
		}
		if(gamePossible)
			sum += game.id();
	}
	return sum;
}

template<size_t N>
constexpr size_t aoc_23_2_2(aoc::utils::string_literal<N> input){
	parser parser(input);
	auto games = parser.parse_games();
	size_t sum = 0;
	for(const auto& game : games){
		cube_set minCubes{};
		for(const auto& set : game.cube_sets()){
			if(set.redCubes > minCubes.redCubes)
				minCubes.redCubes = set.redCubes;
			if(set.greenCubes > minCubes.greenCubes)
				minCubes.greenCubes = set.greenCubes;
			if(set.blueCubes > minCubes.blueCubes)
				minCubes.blueCubes = set.blueCubes;
		}

		size_t gameSum = (minCubes.redCubes * minCubes.greenCubes * minCubes.blueCubes);
		sum += gameSum;
	}
	return sum;
}

int main(int argc, char* argv[]){
	std::cout << "test_1: " << aoc_23_2_1(test_input) << "\n";
	std::cout << "test_2: " << aoc_23_2_2(test_input) << "\n";
	std::cout << "solution_1: " << aoc_23_2_1(input) << "\n";
	std::cout << "solution_2: " << aoc_23_2_2(input) << "\n";

	return 0;
}
