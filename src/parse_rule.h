#pragma once
#include "common.h"
#include <set>
#include <cctype>


inline std::array<std::uint8_t, (1 << 10)> parse_rule(const std::string& rule)
{
	static const std::string nlut[(1 << 10)] = { "b0x","b1c","b1e","b2a","b1c","b2c","b2a","b3i","b1e","b2a","b2e","b3a","b2k","b3n","b3j","b4a","s0x","s1c","s1e","s2a","s1c","s2c","s2a","s3i","s1e","s2a","s2e","s3a","s2k","s3n","s3j","s4a","b1e","b2k","b2e","b3j","b2a","b3n","b3a","b4a","b2i","b3r","b3e","b4r","b3r","b4i","b4r","b5i","s1e","s2k","s2e","s3j","s2a","s3n","s3a","s4a","s2i","s3r","s3e","s4r","s3r","s4i","s4r","s5i","b1c","b2c","b2k","b3n","b2n","b3c","b3q","b4n","b2a","b3i","b3j","b4a","b3q","b4n","b4w","b5a","s1c","s2c","s2k","s3n","s2n","s3c","s3q","s4n","s2a","s3i","s3j","s4a","s3q","s4n","s4w","s5a","b2k","b3y","b3k","b4k","b3q","b4y","b4q","b5j","b3r","b4t","b4j","b5n","b4z","b5r","b5q","b6a","s2k","s3y","s3k","s4k","s3q","s4y","s4q","s5j","s3r","s4t","s4j","s5n","s4z","s5r","s5q","s6a","b1e","b2k","b2i","b3r","b2k","b3y","b3r","b4t","b2e","b3j","b3e","b4r","b3k","b4k","b4j","b5n","s1e","s2k","s2i","s3r","s2k","s3y","s3r","s4t","s2e","s3j","s3e","s4r","s3k","s4k","s4j","s5n","b2e","b3k","b3e","b4j","b3j","b4k","b4r","b5n","b3e","b4j","b4e","b5c","b4j","b5y","b5c","b6c","s2e","s3k","s3e","s4j","s3j","s4k","s4r","s5n","s3e","s4j","s4e","s5c","s4j","s5y","s5c","s6c","b2a","b3n","b3r","b4i","b3q","b4y","b4z","b5r","b3a","b4a","b4r","b5i","b4q","b5j","b5q","b6a","s2a","s3n","s3r","s4i","s3q","s4y","s4z","s5r","s3a","s4a","s4r","s5i","s4q","s5j","s5q","s6a","b3j","b4k","b4j","b5y","b4w","b5k","b5q","b6k","b4r","b5n","b5c","b6c","b5q","b6k","b6n","b7c","s3j","s4k","s4j","s5y","s4w","s5k","s5q","s6k","s4r","s5n","s5c","s6c","s5q","s6k","s6n","s7c","b1c","b2n","b2k","b3q","b2c","b3c","b3n","b4n","b2k","b3q","b3k","b4q","b3y","b4y","b4k","b5j","s1c","s2n","s2k","s3q","s2c","s3c","s3n","s4n","s2k","s3q","s3k","s4q","s3y","s4y","s4k","s5j","b2a","b3q","b3j","b4w","b3i","b4n","b4a","b5a","b3r","b4z","b4j","b5q","b4t","b5r","b5n","b6a","s2a","s3q","s3j","s4w","s3i","s4n","s4a","s5a","s3r","s4z","s4j","s5q","s4t","s5r","s5n","s6a","b2c","b3c","b3y","b4y","b3c","b4c","b4y","b5e","b3n","b4n","b4k","b5j","b4y","b5e","b5k","b6e","s2c","s3c","s3y","s4y","s3c","s4c","s4y","s5e","s3n","s4n","s4k","s5j","s4y","s5e","s5k","s6e","b3n","b4y","b4k","b5k","b4n","b5e","b5j","b6e","b4i","b5r","b5y","b6k","b5r","b6i","b6k","b7e","s3n","s4y","s4k","s5k","s4n","s5e","s5j","s6e","s4i","s5r","s5y","s6k","s5r","s6i","s6k","s7e","b2a","b3q","b3r","b4z","b3n","b4y","b4i","b5r","b3j","b4w","b4j","b5q","b4k","b5k","b5y","b6k","s2a","s3q","s3r","s4z","s3n","s4y","s4i","s5r","s3j","s4w","s4j","s5q","s4k","s5k","s5y","s6k","b3a","b4q","b4r","b5q","b4a","b5j","b5i","b6a","b4r","b5q","b5c","b6n","b5n","b6k","b6c","b7c","s3a","s4q","s4r","s5q","s4a","s5j","s5i","s6a","s4r","s5q","s5c","s6n","s5n","s6k","s6c","s7c","b3i","b4n","b4t","b5r","b4n","b5e","b5r","b6i","b4a","b5a","b5n","b6a","b5j","b6e","b6k","b7e","s3i","s4n","s4t","s5r","s4n","s5e","s5r","s6i","s4a","s5a","s5n","s6a","s5j","s6e","s6k","s7e","b4a","b5j","b5n","b6k","b5a","b6e","b6a","b7e","b5i","b6a","b6c","b7c","b6a","b7e","b7c","b8x","s4a","s5j","s5n","s6k","s5a","s6e","s6a","s7e","s5i","s6a","s6c","s7c","s6a","s7e","s7c","s8x" };
	static const std::set<std::string> numtohensel[9] = {
		{"0x",},
		{"1c", "1e",},
		{"2a", "2c", "2e", "2i", "2k", "2n",},
		{"3a", "3c", "3e", "3i", "3j", "3k", "3n", "3q", "3r", "3y",},
		{"4a", "4c", "4e", "4i", "4j", "4k", "4n", "4q", "4r", "4t", "4w", "4y", "4z",},
		{"5a", "5c", "5e", "5i", "5j", "5k", "5n", "5q", "5r", "5y",},
		{"6a", "6c", "6e", "6i", "6k", "6n",},
		{"7c", "7e",},
		{"8x",},
	};

	auto bad_rule_if = [](bool x) { if (x) { throw std::invalid_argument("rulestring is invalid"); } };

	std::set<std::string> transitionsforlife;
	bool isminus = false;
	bool is_survival = false;
	int num = -1;


	bad_rule_if(rule.length() < 3);
	bad_rule_if(rule[0] != 'B');

	size_t i = 1;
	while (i < rule.length())
	{
		if (rule[i] == '/') {
			bad_rule_if(is_survival);
			is_survival = true;
			i++;
			bad_rule_if(i == rule.length());
			bad_rule_if(rule[i] != 'S');
			num = -1;
		}
		else if (isdigit(rule[i]))
		{
			num = rule[i] - '0';
			bad_rule_if(num == 9);
			if (((i + 1) == rule.length()) || isdigit(rule[i + 1]) || rule[i + 1] == '/')
			{
				for (const std::string& t : numtohensel[num])
				{
					transitionsforlife.insert((is_survival ? "s" : "b") + t);
				}
			}
			else if (rule[i + 1] == '-')
			{
				i++;
				isminus = true;
				for (const std::string& t : numtohensel[num])
				{
					transitionsforlife.insert((is_survival ? "s" : "b") + t);
				}
			}
			else
			{
				isminus = false;
			}
		}
		else
		{
			bad_rule_if((num == -1) || !numtohensel[num].contains(std::to_string(num) + rule[i]));
			if (isminus)
			{
				transitionsforlife.erase((is_survival ? "s" : "b") + std::to_string(num) + rule[i]);
			}
			else
			{
				transitionsforlife.insert((is_survival ? "s" : "b") + std::to_string(num) + rule[i]);
			}
		}
		i++;
	}

	std::array<std::uint8_t, (1 << 10)> outp{};

	for (size_t j = 0; j < (1 << 10); j++)
	{
		if (transitionsforlife.contains(nlut[j]))
		{
			outp[j] = 1;
		}
		else
		{
			outp[j] = 0;
		}
	}
	return outp;
}
