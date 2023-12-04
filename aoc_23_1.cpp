#include "string-literal.hpp"
#include <iostream>
#include <cstddef>

static constexpr aoc::utils::string_literal test_data_23_1_2{
"two1nine\n"
"eightwothree\n"
"abcone2threexyz\n"
"xtwone3four\n"
"4nineeightseven2\n"
"zoneight234\n"
"7pqrstsixteen\n"
};

static constexpr aoc::utils::string_literal test_data_23_1_1{
"1abc2\n"
"pqr3stu8vwx\n"
"a1b2c3d4e5f\n"
"treb7uchet\n"
};

static constexpr aoc::utils::string_literal input{
	"input..."
};

template<size_t N>
static constexpr size_t aoc_23_1_1(aoc::utils::string_literal<N> input){
	size_t sum = 0;
	char firstNumberChar = '\0';
	char lastNumberChar = '\0';
	for(size_t i = 0; i < N; ++i){
		char c = input[i];
		if(c == '\n' && firstNumberChar != '\0'){
			sum += (10 * (firstNumberChar - '0')) + (lastNumberChar - '0');
			firstNumberChar = '\0';
			lastNumberChar = '\0';
		}
		if(c < '0' || c > '9'){
			continue;
		}
		if(firstNumberChar == '\0'){
			firstNumberChar = c;
			lastNumberChar = c;
			continue;
		}
		lastNumberChar = c;
	}
	return sum;
}

template<size_t N>
static constexpr size_t aoc_23_1_2(aoc::utils::string_literal<N> input){
	size_t sum = 0;
	char firstNumberChar = '\0';
	char lastNumberChar = '\0';
	char buffer[5]{'\0', '\0', '\0', '\0', '\0'};
	for(size_t i = 0; i < N; ++i){
		char c = input[i];
		if(c == '\n'){
			sum += (10 * (firstNumberChar - '0')) + (lastNumberChar - '0');
			firstNumberChar = '\0';
			lastNumberChar = '\0';
			buffer[0] = '\0';
			buffer[1] = '\0';
			buffer[2] = '\0';
			buffer[3] = '\0';
			buffer[4] = '\0';
		}
		if(c < '0' || c > '9'){
			buffer[0] = buffer[1];
			buffer[1] = buffer[2];
			buffer[2] = buffer[3];
			buffer[3] = buffer[4];
			buffer[4] = c;
			if(buffer[2] == 'o' && buffer[3] == 'n' && buffer[4] == 'e')
				c = '1';
			if(buffer[2] == 't' && buffer[3] == 'w' && buffer[4] == 'o')
				c = '2';
			if(buffer[0] == 't' && buffer[1] == 'h' && buffer[2] == 'r' && buffer[3] == 'e' && buffer[4] == 'e')
				c = '3';
			if(buffer[1] == 'f' && buffer[2] == 'o' && buffer[3] == 'u' && buffer[4] == 'r')
				c = '4';
			if(buffer[1] == 'f' && buffer[2] == 'i' && buffer[3] == 'v' && buffer[4] == 'e')
				c = '5';
			if(buffer[2] == 's' && buffer[3] == 'i' && buffer[4] == 'x')
				c = '6';
			if(buffer[0] == 's' && buffer[1] == 'e' && buffer[2] == 'v' && buffer[3] == 'e' && buffer[4] == 'n')
				c = '7';
			if(buffer[0] == 'e' && buffer[1] == 'i' && buffer[2] == 'g' && buffer[3] == 'h' && buffer[4] == 't')
				c = '8';
			if(buffer[1] == 'n' && buffer[2] == 'i' && buffer[3] == 'n' && buffer[4] == 'e')
				c = '9';

			if(c < '0' || c > '9')
				continue;
		}
		if(firstNumberChar == '\0'){
			firstNumberChar = c;
			lastNumberChar = c;
			continue;
		}
		lastNumberChar = c;
	}
	return sum;
}

static constexpr size_t solution_aoc_23_1_1_test = aoc_23_1_1(test_data_23_1_1);
static constexpr size_t solution_aoc_23_1_2_test = aoc_23_1_2(test_data_23_1_2);
static constexpr size_t solution_aoc_23_1_1 = aoc_23_1_1(input);
static constexpr size_t solution_aoc_23_1_2 = aoc_23_1_2(input);

int main(int argc, char* argv[]){
	std::cout << "test_1: " << solution_aoc_23_1_1_test << "\n";
	std::cout << "test_2: " << solution_aoc_23_1_2_test << "\n";
	std::cout << "solution_1: " << solution_aoc_23_1_1 << "\n";
	std::cout << "solution_2: " << solution_aoc_23_1_2 << "\n";
}
