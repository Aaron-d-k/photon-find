#include <iostream>
#include <tuple>
#include "searcher.h"
#include "photon-find.h"
#include <functional>


bool spam_console = true;

auto parseArgs(std::vector<std::string> args)
{
    std::tuple<size_t, symmetry, bool, search_direction, std::string, search_type, size_t, bool> outp{};
    std::array<bool, std::tuple_size_v<decltype(outp)>> has_given{ {false, false, true, true, false, true, true, true} };

    std::get<2>(outp) = false;//floating or not
    std::get<3>(outp) = search_direction::top;
    std::get<5>(outp) = search_type::breadth_first_search;
    std::get<6>(outp) = 1; //number of threads to use
    std::get<7>(outp) = false; //quiet

    size_t index = 0;

    while (index < args.size())
    {
        if (args[index][0] == '-')
        {
            if (args[index].length() != 2) throw std::invalid_argument("not a proper argument");

            switch (args[index][1])
            {
            case 'r': std::get<4>(outp) = args[index + 1]; has_given[4] = true; index++; break;
            case 'f': std::get<2>(outp) = true; break;
            case 'q': std::get<7>(outp) = true; break;
            case 's':
                if (args[index + 1] == "even") { std::get<1>(outp) = symmetry::even; }
                else if (args[index + 1] == "asym") { std::get<1>(outp) = symmetry::asym; }
                else throw std::invalid_argument("not a symmetry");
                has_given[1] = true;
                index++;
                break;
            case 'd':
                switch (args[index + 1][0])
                {
                case 't': std::get<3>(outp) = search_direction::top; break;
                case 's': std::get<3>(outp) = search_direction::side; break;
                case 'b': std::get<3>(outp) = search_direction::bottom; break;
                default: throw std::invalid_argument("not a side");
                }
                index++;
                break;
            case 'w': std::get<0>(outp) = std::stoull(args[index + 1]); has_given[0] = true; index++; break;
            case 't': std::get<6>(outp) = std::stoull(args[index + 1]); index++; break;
            case 'm':
                if (args[index + 1] == "dfs") { std::get<5>(outp) = search_type::depth_first_search; }
                else if (args[index + 1] == "bfs") { std::get<5>(outp) = search_type::breadth_first_search; }
                else throw std::invalid_argument("not a search type");
                index++;
                break;
            default: throw std::invalid_argument("not a proper argument");
            }
        }
        else throw std::invalid_argument("not a proper argument");
        index++;
    }

    if (!std::all_of(has_given.cbegin(), has_given.cend(), std::identity{}))
    {
        throw std::invalid_argument("missing a required argument");
    }

    return outp;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << R"(
This is Photon-Find, a program to find photon in 2-state isotropic cellular automata.

Usage: photonfind -r <rule> -w <width> -s (even|asym) [ -f (-d (t|s|b)) (-m (dfs|bfs)) (-t <num-threads>) -q]
Options:
    -r <rule>           the rule in hensel notation.
    -w <width>          the search width (real width depends on symmetry). it should be an even number.
    -s (even|asym)      the symmetry of the search.
    -f                  perform a floating row search.
    -d (t|s|b)          direction of search (t for top, s for side, b for bottom). [default: t]
    -m (dfs|bfs)        whether to use dfs or bfs. [default: bfs]
    -t <num-threads>    number of threads to use. [default: 1]
    -q                  do not print partials and other info. only ship and depth reached.
)";
        return EXIT_SUCCESS;
    }

    std::vector<std::string> cmdline_arguments;
    std::copy(argv + 1, argv + argc, std::back_inserter(cmdline_arguments));

    try
    {
        const auto [width, sym, isfloating, sdirec, rule, srchtype, num_threads, be_quiet] = parseArgs(cmdline_arguments);
        spam_console = !be_quiet;
        search_photon(width, sym, isfloating, sdirec, rule, srchtype, num_threads);

    }
    catch (const std::invalid_argument& error)
    {
        std::cerr << "There was an error in your arguments: " << error.what() << "\nFor help, run without any arguments.";
        return EXIT_FAILURE;
    }
}

