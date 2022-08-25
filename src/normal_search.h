#pragma once
#include "neighbourfind.h"
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <sstream>

template<symmetry current_symmetry, size_t search_width>
class NormalSearch {

public:
	using LastRows = std::pair<row, row>;
	struct Hasher2row
	{
		inline size_t operator()(LastRows in) const
		{
			return hash_combine(in.first, in.second);
		}
	};
	using TransposionTable = std::unordered_map<LastRows, LastRows, Hasher2row>;
	const TransposionTable defaultttable = { {{0,0},{0,0}} };

private:
	const std::string rule;
	const search_direction direction;
	LastRows starter;

public:

	NormalSearch(std::string rule, search_direction direction) : rule(rule), direction(direction), starter({0,0})
	{ 
		if constexpr (current_symmetry != symmetry::asym)
		{
			if (direction == search_direction::side) 
			{
				throw std::invalid_argument("side first search cannot have symmetry.");
			}
		}
	}


	std::string print_ship(LastRows l, TransposionTable* t) const
	{
		std::ostringstream outp;
		while (l != starter && t->contains(l))
		{
			std::string s = std::bitset<search_width>(l.first).to_string();
			std::replace(s.begin(), s.end(), '0', '.');
			std::replace(s.begin(), s.end(), '1', 'O');
			outp << s;
			if constexpr (current_symmetry == symmetry::even)
			{
				std::reverse(s.begin(), s.end());
				outp << s;
			}
			else if constexpr (current_symmetry == symmetry::odd)
			{
				s.pop_back();
				std::reverse(s.begin(), s.end());
				outp << s;
			}
			outp << '\n';
			l = (*t)[l];
		}
		outp << '\n';

		return outp.str();
	}

	void initialise_for_search(LastRows to_start) {
		starter = to_start;
		fill_lookup(rule, direction);
	}

	void extend_dfs(LastRows to_extend, TransposionTable* transposition_table, int* num_ships_left) const
	{
		doforallphotonnextrow<search_width, current_symmetry>(to_extend.first, to_extend.second,
			[=, this](row r)
			{

				LastRows next = std::make_pair(to_extend.second, r);
				if (next.first == 0 && next.second == 0)
				{
					print_info("done!\n");
					std::cout << print_ship(to_extend, transposition_table);
					(*num_ships_left)--;
				} 
				else if (transposition_table->insert({ next, to_extend }).second && (*num_ships_left >= 0))
				{
					extend_dfs(next, transposition_table, num_ships_left);
				}
			});
	}

	//returns true if dfs crosses n levels.
	static bool dfs_n_levels(int n, LastRows to_extend)
	{
		bool crossed = false;
		dfs_do<search_width, current_symmetry>(&crossed, 0, 0, to_extend.first, to_extend.second,
			[=, &crossed](row r)
			{
				if (n == 0)
				{
					crossed = true;
				}
				else
				{
					crossed = dfs_n_levels(n - 1, { to_extend.second, r });
				}
			});
		return crossed;
	}

	void perform_garbage_collection(TransposionTable* transposition_table, const std::vector<LastRows>& usefull_nodes) const
	{
		std::unordered_map<LastRows, LastRows, Hasher2row> is_usefull_or_dependency = defaultttable;
		for (const LastRows& usefullrow : usefull_nodes)
		{
			LastRows prev_usefullrow = usefullrow;
			LastRows curr_usefullrow = (*transposition_table)[prev_usefullrow];
			while (prev_usefullrow != curr_usefullrow)
			{
				is_usefull_or_dependency.insert({ prev_usefullrow , curr_usefullrow });
				prev_usefullrow = curr_usefullrow;
				curr_usefullrow = (*transposition_table)[curr_usefullrow];
			}
		}
		*transposition_table = is_usefull_or_dependency;
	}

	void extend_bfs(LastRows to_extend, TransposionTable* transposition_table, std::vector<LastRows>* next_height_partials, int* num_ships_left) const
	{
		doforallphotonnextrow<search_width, current_symmetry>(to_extend.first, to_extend.second,
			[=, this](row r)
			{
				LastRows next = std::make_pair(to_extend.second, r);
				if (next.first == 0 && next.second == 0)
				{
					if ((to_extend.second != 0) || (to_extend.first != 0))
					{
						print_info("done!\n");
						std::cout << print_ship(to_extend, transposition_table);
						(*num_ships_left)--;
					}
				}
				else if (transposition_table->insert({ next, to_extend }).second)
				{
					next_height_partials->push_back(next);
				}
			}
		);
	}
};