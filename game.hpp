#ifndef H_CUBE
#define H_CUBE

#include <cstdint>
#include <cstddef>
#include <vector>

enum class cube_color{
	blue,
	red,
	green
};

class cube{
public:
	constexpr cube(cube_color color, size_t amount) : m_color(color), m_amount(amount){}
	constexpr cube_color color() const{
		return m_color;
	}
	constexpr size_t amount() const{
		return m_amount;
	}
private:
	cube_color m_color;
	size_t m_amount;
};

struct cube_set{
	size_t redCubes = 0;
	size_t greenCubes = 0;
	size_t blueCubes = 0;
};

class game{
public:
	constexpr game(int64_t id, std::vector<cube_set> cubeSets) : m_id(id), m_cubeSets(cubeSets){}
	constexpr int64_t id() const{
		return m_id;
	}
	constexpr std::vector<cube_set> cube_sets() const{
		return m_cubeSets;
	}

private:
	int64_t m_id;
	std::vector<cube_set> m_cubeSets;
};

#endif
