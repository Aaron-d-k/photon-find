#include <iostream>
#include "neighbourfind.h"
#include "parse_rule.h"

/// <summary>
/// It is a lookup table that has all the grid2x3 that can connect
/// to the side of the grid2x3 given by the index.
/// Two of them can connect if when placed side be side they can be part of a photon (so some valid connection will not be true as they can never be part of a photon of given width)
/// </summary>
std::array<std::array<bool, 1 << 6>, 1 << 6> can_connect_lookup;


void fill_lookup(const std::string& rule, search_direction direction)
{
	can_connect_lookup.fill({});
	std::array<std::uint8_t, (1 << 10)> rule_lookup = parse_rule(rule);

	//fill table
	for (grid2x3 i = 0; i < (1 << 6); i++)
	{
		for (grid2x3 c = 0; c < (1 << 6); c++)
		{

			uint32_t neighbourhood1 = (((i >> 4) & 0b11) << 7) | (((i >> 2) & 0b11) << 4) | ((i & 0b11) << 1) |
				                      ((c >> 1) & 0b1) | (((c >> 3) & 0b1) << 3) | (((c >> 5) & 0b1) << 6);

			uint32_t neighbourhood2 = ((i&0b1)<<2) | (((i>>2)& 0b1) << 5) | (((i >> 4) & 0b1) << 8) |
									  (((c>>4)&0b11)<<6) | (((c >> 2) & 0b11) << 3) | (c& 0b11);

			bool can_there_be_connection;
			if (direction == search_direction::top)//front first
				can_there_be_connection = ((rule_lookup[neighbourhood1] == ((i) & 0b1)) && (rule_lookup[neighbourhood2] == ((c >> 1) & 0b1)));
			else if (direction == search_direction::bottom)//back first
				can_there_be_connection = ((rule_lookup[neighbourhood1] == ((i >> 4) & 0b1)) && (rule_lookup[neighbourhood2] == ((c >> 5) & 0b1)));
			else if (direction == search_direction::side)//side first
				can_there_be_connection = ((rule_lookup[neighbourhood1] == ((c >> 3) & 0b1)) && (rule_lookup[neighbourhood2] == ((c >> 2) & 0b1)));
			else if (direction == search_direction::still_life)//still life search
				can_there_be_connection = ((rule_lookup[neighbourhood1] == ((i >> 2) & 0b1)) && (rule_lookup[neighbourhood2] == ((c >> 3) & 0b1)));
			else
			{
				std::cerr << "invalid search mode.";
				exit(EXIT_FAILURE);
			}
			if (can_there_be_connection)
			{
				can_connect_lookup[i][c] = true;
			}
		}
	}
	//destroy dead ends.
	bool has_changed = true;
	while (has_changed)
	{
		has_changed = false;
		std::array<bool, 1 << 6> is_deadend{};

		//mark ends which don't reach anywhere
		for (grid2x3 i = 0; i < (1 << 6); i++)
		{
			bool has_one_connect = false;
			for (grid2x3 c = 0; c < (1 << 6); c++)
			{
				has_one_connect |= can_connect_lookup[i][c];
			}
			is_deadend[i] |= !has_one_connect;
		}

		//mark unreachable ends
		for (grid2x3 i = 0; i < (1 << 6); i++)
		{
			bool has_one_connect = false;
			for (grid2x3 c = 0; c < (1 << 6); c++)
			{
				has_one_connect |= can_connect_lookup[c][i];
			}
			is_deadend[i] |= !has_one_connect;
		}
		
		//destroy parts which can't extend downward.
		for (grid2x3 i = 0; i < (1 << 4); i++)
		{
			grid2x3 lasttwoontop = (i << 2) & 0b111111;
			bool cannotextend = true;
			for (size_t c = 0; c < 4; c++)
			{
				cannotextend = cannotextend && is_deadend[lasttwoontop | c];
			}
			if (cannotextend)
			{
				is_deadend[i] = true;
				is_deadend[i | 0b010000ULL] = true;
				is_deadend[i | 0b100000ULL] = true;
				is_deadend[i | 0b110000ULL] = true;
			}
		}

		//destroy dead ends from table. 
		for (grid2x3 i = 0; i < (1 << 6); i++)
		{
			for (grid2x3 c = 0; c < (1 << 6); c++)
			{
				if (can_connect_lookup[i][c]) 
				{
					if (is_deadend[c] || is_deadend[i])
					{
						has_changed = true;
						can_connect_lookup[i][c] = false;
					}
				}
			}
		}
	}
}




#define P(a) ((a&0b1) ? 'O' : '.')

void printgrid(grid2x3 i)
{
	for (size_t j = 0; j < 3; j++)
	{
		print_info(
			P(i >> (5 - j * 2)),
			P(i >> (4 - j * 2)),
		'\n');
	}
	print_info('\n');
}

void showlookupentry(grid2x3 i)
{
	for (grid2x3 c = 0; c < (1 << 6); c++)
	{
		if (can_connect_lookup[i][c])
		{
			for (size_t j = 0; j < 3; j++)
			{
				print_info(
					P(i >> (5 - j * 2)),
					P(i >> (4 - j * 2)),
					P(c >> (5 - j * 2)),
					P(c >> (4 - j * 2)),
				'\n');
			}
			print_info("\n\n\n");
		}
	}
}

