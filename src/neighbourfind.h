#pragma once
#include "common.h"

extern std::array<std::array<bool, 1 << 6>, 1 << 6> can_connect_lookup;

void fill_lookup(const std::string& rule, search_direction direction);
void showlookupentry(grid2x3 i);

//with cancellation
template<size_t width, symmetry sym, class Funtor>
inline void dfs_do(const bool* cancel, row current, grid2x3 node, row row1 /*This can be longer than given width, but extra will be unused*/, row row2 /*This can be longer than given width, but extra will be unused*/, Funtor fun)
{
	static_assert(width % 2 == 0, "Only even widths supported.");
	static_assert(width > 0, "What!!");
	if constexpr (width > 2)
	{
		grid2x3 to_test = (((row1 >> (width - 2)) & 0b11) << 4) | (((row2 >> (width - 2)) & 0b11) << 2);
		for (unsigned i = 0; i < 4; i++)
		{
			if (can_connect_lookup[node][to_test | i])
			{
				dfs_do<width - 2, sym, Funtor>(cancel, ((current << 2) + i), to_test + i, row1, row2, fun);
				if (*cancel)
				{
					return;
				}
			}
		}
	}
	else
	{
		grid2x3 to_test = (((row1 >> (width - 2)) & 0b11) << 4) | (((row2 >> (width - 2)) & 0b11) << 2);
		for (unsigned i = 0; i < 4; i++)
		{
			if (can_connect_lookup[node][to_test | i])
			{
				grid2x3 ending_part = to_test | i;
				if constexpr (sym == symmetry::asym)
				{
					if (can_connect_lookup[ending_part][0])
					{
						fun((current << 2) | i);
						if (*cancel)
						{
							return;
						}
					}
				}
				else if constexpr (sym == symmetry::even)
				{
					grid2x3 ending_part_flipped = ((ending_part & 0x00000015) << 1)
						| ((ending_part & 0x0000002a) >> 1);
					if (can_connect_lookup[ending_part][ending_part_flipped])
					{
						fun((current << 2) | i);
						if (*cancel)
						{
							return;
						}
					}
				}
				else
				{
					static_assert(sym != symmetry::even && sym == symmetry::asym, "symmetry not supported. will be added later");
				}
			}
		}
	}
}

template<size_t width, symmetry sym, class Funtor>
inline void doforallphotonnextrow(row row1, row row2, Funtor fun = {})
{
	static_assert(width % 2 == 0, "Only even widths supported.");
	static_assert(width < sizeof(row)*CHAR_BIT, "width is too large.");
	static_assert(width > 0, "Width should be more than zero.");
	bool cancel = false;
	dfs_do<width, sym, Funtor>(&cancel, 0, 0, row1, row2, fun);
}
