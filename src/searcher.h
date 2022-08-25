#pragma once
#include "common.h"
#include <thread>

enum class search_type {
	breadth_first_search,
	depth_first_search
};

template<class TypeOfSearch>
class Searcher {
private:
	TypeOfSearch inner_searcher;
	const size_t num_thread;

	void remove_dead_ends(int n, std::vector<typename TypeOfSearch::LastRows>* partials, typename TypeOfSearch::TransposionTable* ttable, bool table_erase = false)
	{
		auto do_from_till = [partials, n, table_erase, this]
		(size_t from, size_t till, std::vector<typename TypeOfSearch::LastRows>* to_keep, std::vector<typename TypeOfSearch::LastRows>* to_erase)
		{
			for (size_t i = from; i < till; i++)
			{
				if (inner_searcher.dfs_n_levels(n, (*partials)[i]))
				{
					to_keep->push_back((*partials)[i]);
				}
				else if (table_erase)
				{
					to_erase->push_back((*partials)[i]);
				}
			}
		};
 
		std::vector<std::vector<typename TypeOfSearch::LastRows>> to_keep_arr(num_thread);
		std::vector<std::vector<typename TypeOfSearch::LastRows>> to_erase_arr(num_thread);
		std::vector<std::thread> tasks;

		for (size_t i = 0; i < num_thread; i++)
		{
			tasks.push_back(std::thread(do_from_till, (partials->size() * i) / num_thread, (partials->size() * (i + 1)) / num_thread, &to_keep_arr[i], &to_erase_arr[i]));
		}


		for (size_t i = 0; i < num_thread; i++)
		{
			tasks[i].join();
		}


		partials->clear();
		for (size_t i = 0; i < num_thread; i++)
		{
			partials->insert(partials->cend(), to_keep_arr[i].cbegin(), to_keep_arr[i].cend());
			for (typename TypeOfSearch::LastRows e : to_erase_arr[i]) { ttable->erase(e); }
		}
	}

public:
	Searcher(const TypeOfSearch& s, size_t how_many_thread = 1) : inner_searcher(s), num_thread(how_many_thread)
	{ }

	void search_dfs(typename TypeOfSearch::LastRows to_start = {}) {
		inner_searcher.initialise_for_search(to_start);

		typename TypeOfSearch::TransposionTable transposition_table = inner_searcher.defaultttable;
		int num_ships_left = max_ships;
		inner_searcher.extend_dfs(to_start, &transposition_table, &num_ships_left);
	}

	void search_bfs(typename TypeOfSearch::LastRows to_start = {}, size_t max_partial_list_size = (1 << 23), size_t max_transposition_table_size = (1 << 24)) {
		inner_searcher.initialise_for_search(to_start);

		typename TypeOfSearch::TransposionTable transposition_table = inner_searcher.defaultttable;

		std::vector<typename TypeOfSearch::LastRows> next_height_partials;
		next_height_partials.push_back(to_start);

		std::vector<typename TypeOfSearch::LastRows> current_partials;
		size_t height = 0;
		int num_ships_left = max_ships;


		while (!next_height_partials.empty())
		{
			current_partials = next_height_partials;
			next_height_partials.clear();
			int dead_end_removal_count = 0;
			while (!current_partials.empty())
			{
				typename TypeOfSearch::LastRows to_extend = current_partials.back();
				current_partials.pop_back();

				inner_searcher.extend_bfs(to_extend, &transposition_table, &next_height_partials, &num_ships_left);

				if (num_ships_left <= 0) return;

				if (next_height_partials.size() > max_partial_list_size)
				{
					print_info("Partials list reached size ", next_height_partials.size(), ", compacting...");
					if (dead_end_removal_count % 5 == 0) 
					{
						remove_dead_ends(dead_end_removal_count / 5, &current_partials, &transposition_table, (transposition_table.size() > max_transposition_table_size));
					}
					remove_dead_ends(dead_end_removal_count / 6 + 1, &next_height_partials, &transposition_table, (transposition_table.size() > max_transposition_table_size));
					dead_end_removal_count += 1;

					print_info("compaction completed with ", current_partials.size(), " in current height, ", next_height_partials.size(), " in next width.\n");
				}
			}

			if (height > 0) 
			{
				if (height > 1) 
				{
					print_info("Current Height done. removing dead ends...\n");
				}
				remove_dead_ends(dead_end_removal_count / 6, &next_height_partials, &transposition_table);

				if (transposition_table.size() > max_transposition_table_size)
				{
					print_info("transposition table full, performing garbage collection...\n");
					inner_searcher.perform_garbage_collection(&transposition_table, next_height_partials);
				}

				print_info("Height ", height - 1, " done, no of partials left:", next_height_partials.size(), "\nPartial:\n");
				if (!next_height_partials.empty())
				{
					print_info(inner_searcher.print_ship(next_height_partials[0], &transposition_table));
				}
			}
			height++;
		}
		print_info("Search ended, reached till height ");
		std::cout << height;
	}

	void search(search_type s = search_type::breadth_first_search, typename TypeOfSearch::LastRows to_start = {}, size_t max_partial_list_size = (1 << 23), size_t max_transposition_table_size = (1 << 24))
	{
		switch (s)
		{
		case search_type::depth_first_search: 
			search_dfs(to_start); 
			break;
		case search_type::breadth_first_search: 
			search_bfs(to_start, max_partial_list_size, max_transposition_table_size); 
			break;
		default: 
			throw std::invalid_argument("not a valid search mode");
		}
	}
};
