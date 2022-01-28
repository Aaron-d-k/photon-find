#pragma once
#include "searcher.h"
#include "floating_search.h"
#include "normal_search.h"


template<template<size_t> class ToCall, size_t w, class... Args>
decltype(auto) call([[maybe_unused]] size_t i, [[maybe_unused]] const Args&... args) {
	if constexpr (w == 2)
	{
		throw std::invalid_argument("something is not right width must be even, greater than 2 and less than 60");
	}
	else
	{
		if (i == w)
		{
			return ToCall<w>{}(args...);
		}
		else
		{
			return call<ToCall, w - 2>(i, args...);
		}
	}
}

template<size_t w>
struct sf
{
	void operator ()(search_direction d, std::string r, search_type stype, size_t num_threads)
	{
		Searcher<FloatingSearch<w>> searcher{ { r, d }, num_threads };
		searcher.search(stype);
	}
};

template<size_t w, symmetry s>
struct sn
{
	void operator ()(search_direction d, std::string r, search_type stype, size_t num_threads)
	{
		Searcher<NormalSearch<s, w>> searcher{ { r, d }, num_threads };
		searcher.search(stype);
	}
};

template<size_t w>
using sa = sn<w, symmetry::asym>;

template<size_t w>
using se = sn<w, symmetry::even>;

void search_photon(size_t width, symmetry sym, bool floating_rows, search_direction direction, std::string rule, search_type stype, size_t num_threads)
{
	if (width > 60 || width < 2)
	{
		throw std::invalid_argument("width is out of range");
	}
	if (floating_rows)
	{
		if (sym != symmetry::asym)
		{
			throw std::invalid_argument("floating rows does not support symmetry");
		}
		call<sf, 62>(width, direction, rule, stype, num_threads);
	}
	else
	{
		if (direction == search_direction::side && sym != symmetry::asym)
		{
			throw std::invalid_argument("side first does not support symmetry");
		}
		switch (sym)
		{
		case symmetry::asym: call<sa, 62>(width, direction, rule, stype, num_threads); break;
		case symmetry::even: call<se, 62>(width, direction, rule, stype, num_threads); break;
		default: throw std::invalid_argument("unsupported symmetry");
		}
	}
}