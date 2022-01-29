#pragma once
#include <cstdint>
#include <bit>
#include <string>
#include <climits>
#include <vector>
#include <array>
#include <stdexcept>
#include <cassert>


enum class symmetry
{
	asym,
	even,
	odd,
};

enum class search_direction
{
	top,
	side,
	bottom,
	still_life
};

/// <summary>
/// It is a 2x3 grid.
///   a1 a2
///	  b1 b2
///   c1 c2
/// it is represented in bits as:
///  a1 a2 b1 b2 c1 c2 
/// </summary>
using grid2x3 = std::uint32_t;

/// <summary>
/// It is a row of a partial,
///    a1 a2 a3 a4 .... a(n-1) a(n)
/// it is represented in bits as:
///    a1 a2 a3 a4 .... a(n-1) a(n)
/// </summary>
using row = std::uint64_t;

extern bool spam_console;
template<class... T>
inline void print_info(T&&... t)
{
	if (spam_console) 
	{
		(std::cerr << ... << t);
	}
}

inline size_t hash_combine(size_t lhs, size_t rhs) {
	lhs ^= rhs + 0x9e3779b9ull + std::rotl(lhs, 6) + std::rotr(lhs, 2);
	return lhs;
}


