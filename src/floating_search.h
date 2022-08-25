#pragma once
#include "neighbourfind.h"
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <sstream>
/*
* The transposition table contains partials of the form :
*<second_to_last, last_row> : <offset, third_to_last>
*
* The the last_rowand second_to_last_row must be offset by given offset
* to join to the third row while printing the partial.to get more rows, index the table
* using <third_to_last, second_to_last>.
*
* When extending a partial, if the new row and the last row have zeroes in the
* same direction, then extend a shifted version of both rows too. Remember to add
* appropriate entry to the transposition table.
*/

template<class T>
constexpr T offset_partial(T r, int offset)
{
	return (offset > 0) ? (r >> offset) : (r << (-offset));
}

constexpr std::pair<row, row> offset_partial(std::pair<row, row> r, int offset)
{
	return { offset_partial(r.first, offset), offset_partial(r.second, offset) };
}


template<size_t search_width>
class FloatingSearch {

public:
	using LastRows = std::pair<row, row>;
	struct Hasher2row
	{
		inline size_t operator()(LastRows in) const
		{
			return hash_combine(in.first, in.second);
		}
	};
	using TransposionTable = std::unordered_map<LastRows, std::pair<int, row>, Hasher2row>;
	const TransposionTable defaultttable = { {{0,0},{0,0}} };


private:
	const std::string rule;
	const search_direction direction;
	LastRows starter;

public:

	FloatingSearch(std::string rule, search_direction direction) : rule(rule), direction(direction), starter({ 0,0 })
	{ }

	std::string print_ship(LastRows l, TransposionTable* t) const
	{
		std::ostringstream outp;
		int offset = -60;
		while (l != starter && t->contains(l))
		{
			std::string s = offset_partial(std::bitset<160>(l.first), offset).to_string();
			std::replace(s.begin(), s.end(), '0', '.');
			std::replace(s.begin(), s.end(), '1', 'O');
			outp << s << "\n";
			const auto& dict_entry = (*t)[l];
			offset += dict_entry.first;
			l.second = offset_partial(l.first, -dict_entry.first);
			l.first = dict_entry.second;
		}
		outp << "\n";
		return outp.str();
	}

	void initialise_for_search(LastRows to_start) {
		starter = to_start;
		fill_lookup(rule, direction);
	}

	void extend_dfs(LastRows to_extend, TransposionTable* transposition_table, int* num_ships_left) const
	{
		doforallphotonnextrow<search_width, symmetry::asym>(to_extend.first, to_extend.second,
			[=, this](row r)
			{
				LastRows next = std::make_pair(to_extend.second, r);
				if (next.first == 0 && next.second == 0)
				{
					print_info("done!\n");
					std::cout << print_ship(to_extend, transposition_table);
					(*num_ships_left)--;
				}
				for (int offset = -std::min(std::countl_zero(next.first) - (int)(64 - search_width), std::countl_zero(next.second) - (int)(64 - search_width)); offset <= std::min(std::countr_zero(next.first), std::countr_zero(next.second)); offset++)
				{
					LastRows partial_to_extend_next = offset_partial(next, offset);
					if (transposition_table->insert({ partial_to_extend_next , { offset, to_extend.first } }).second && (*num_ships_left >= 0))
					{
						extend_dfs(partial_to_extend_next, transposition_table, num_ships_left);
					}
				}
			}
		);
	}

	static bool dfs_n_levels(int n, LastRows to_extend)
	{
		bool crossed = false;
		dfs_do<search_width, symmetry::asym>(&crossed, 0, 0, to_extend.first, to_extend.second,
			[=, &crossed](row r)
			{
				if (n == 0)
				{
					crossed = true;
				}
				else
				{
					for (int offset = -std::min(std::countl_zero(to_extend.second) - (int)(64 - search_width), std::countl_zero(r) - (int)(64 - search_width)); offset <= std::min(std::countr_zero(to_extend.second), std::countr_zero(r)) && !crossed; offset++)
					{
						crossed = dfs_n_levels(n - 1, offset_partial({ to_extend.second, r }, offset));
					}
				}
			});
		return crossed;
	}

	void perform_garbage_collection(TransposionTable* t, const std::vector<LastRows>& usefull_partials)
	{
		std::unordered_map<LastRows, std::pair<int, row>, Hasher2row> new_t = defaultttable;
 		for (LastRows r : usefull_partials)
		{
			LastRows l = r;
			while (!(l.first == 0 && l.second == 0))
			{
				assert(t->contains(l));
				const auto& dict_entry = (*t)[l];
				new_t[l] = dict_entry;
				l.second = offset_partial(l.first, -dict_entry.first);
				l.first = dict_entry.second;
			}
		}
		*t = new_t;
	}

	void extend_bfs(LastRows to_extend, TransposionTable* transposition_table, std::vector<LastRows>* next_height_partials, int* num_ships_left) const
	{
		doforallphotonnextrow<search_width, symmetry::asym>(to_extend.first, to_extend.second,
			[=, this](row r)
			{
				LastRows next = std::make_pair(to_extend.second, r);
				if (next.first == 0 && next.second == 0)
				{
					if (to_extend.first != 0 || to_extend.second != 0)
					{
						print_info("done!\n");
						std::cout << print_ship(to_extend, transposition_table);
						(*num_ships_left)--;
					}
				}
				for (int offset = -std::min(std::countl_zero(next.first) - (int)(64 - search_width), std::countl_zero(next.second) - (int)(64 - search_width)); offset <= std::min(std::countr_zero(next.first), std::countr_zero(next.second)); offset++)
				{
					LastRows partial_to_extend_next = offset_partial(next, offset);
					if (transposition_table->insert({ partial_to_extend_next , { offset, to_extend.first } }).second)
					{
						next_height_partials->push_back(partial_to_extend_next);
					}
				}
			}
		);
	}

	
};