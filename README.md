photon-find
-----------

A program to find [photons](https://www.conwaylife.com/wiki/Photon) in isotropic 2-state [cellular automaton](https://en.wikipedia.org/wiki/Cellular_automaton). It supports:

* Searching photons with D2_+2 symmetry
* Sideways, back-first and front-first searching
* Floating rows for asymmetric searches
* Both BFS and DFS searching
* Supports INT rulestrings using [hensel notation](http://golly.sourceforge.net/Help/Algorithms/QuickLife.html#nontotal).

Compiling
--------

To compile the program, you must have a c++ compiler. You can use an IDE like Visual Studio to run it after downloading.    
Follow the relevant steps in [this tutorial](https://docs.microsoft.com/en-us/cpp/build/get-started-linux-cmake?view=msvc-170) to run it using Visual Studio.  


If you have cygwin, mingw or linux you can run 
```
g++ src/photon-find.cpp src/neighbourfind.cpp -O3 -march=native -std=c++20 -o photon-find
```
 to compile it.  


 Usage
 -----
You can get help by running the program without any command line arguments. The program only supports widths less than 60 which are a **multiple of 2** for now.

As an example, here is the commands to search for a photon with width 4 in [seeds](https://en.wikipedia.org/wiki/Seeds_(cellular_automaton)) (which is `B2/S` in hensel notation) with no symmetry in side first mode:  
```
photon-find -r "B2/S" -w 4 -s asym -d s
```

It can output 
```
.O..
O...
O...
..O.
```

which is a valid photon in seeds.

Here is another search you can try:  
```
photon-find -r "B2578/S13568" -w 16 -s even 
```

To see the photon in action, you can use a CA simulator like [golly](http://golly.sourceforge.net/webapp/golly.html). 
The algorithm used here was inspired by the one in [gfind](https://www.conwaylife.com/wiki/gfind).

Here is a photon in `B25/S0246` found using this program

```
............................................................
.............O..O..O....................O..O..O.............
......O.....OOO.O.OOO..................OOO.O.OOO.....O......
.....OOO...OOOOOOOOOOO..O..........O..OOOOOOOOOOO...OOO.....
...O.O..OOO...OOOOO...OOOO........OOOO...OOOOO...OOO..O.O...
....O.O.O..OO...O...OO...O.O....O.O...OO...O...OO..O.O.O....
.....OO...OOO...O...OOO.O.O......O.O.OOO...O...OOO...OO.....
........O.O.O.O.O..OOO..OO........OO..OOO..O.O.O.O.O........
....OOOO......O..O.O.OO..............OO.O.O..O......OOOO....
......O...O..O........O..............O........O..O...O......
......O.OO........O..OO..O..O..O..O..OO..O........OO.O......
.....O..OOOOO....OOOOOO.OOOO....OOOO.OOOOOO....OOOOO..O.....
.........OO....O.O.....O..OOOOOOOO..O.....O.O....OO.........
......OO.OO.....O.O.OO.O.O.OO..OO.O.O.OO.O.O.....OO.OO......
..........O......OO....O...OO..OO...O....OO......O..........
.........OO............O..O..OO..O..O............OO.........
.......O.O.O..O........O.OO.OOOO.OO.O........O..O.O.O.......
........O.O.OOOO......O..O.O....O.O..O......OOOO.O.O........
.........OOO...O.O.........O.OO.O.........O.O...OOO.........
...........OO.O.O......OO..O....O..OO......O.O.OO...........
...........O..OO...........O....O...........OO..O...........
...........OO..............O....O..............OO...........
..........O.O..............O....O..............O.O..........
......O.....OO.............O....O.............OO.....O......
.....OOO...O.O.O...........O....O...........O.O.O...OOO.....
...O.O..OOO.O.O............O....O............O.O.OOO..O.O...
....O.O.O..OOO.............O....O.............OOO..O.O.O....
.....OO...OO...............O....O...............OO...OO.....
........O..O...............O....O...............O..O........
........O.OO...............O....O...............OO.O........
.......O..O.O..............O....O..............O.O..O.......
............OO........O....O....O....O........OO............
........OO..OOO......OOO..OOO..OOO..OOO......OOO..OO........
............O.O.O.....O....O....O....O.....O.O.O............
.......OOOO......OOOO.O.OO.O.OO.O.OO.O.OOOO......OOOO.......
.........O.O..O...O...O....O....O....O...O...O..O.O.........
.........O.....OO.O...O....O....O....O...O.OO.....O.........
.........O..OOOOO..O..O....O....O....O..O..OOOOO..O.........
.........O....OO......O....O....O....O......OO....O.........
.........O....OO.OO...O....O....O....O...OO.OO....O.........
.........O....O.......O....O....O....O.......O....O.........
.........O....OO......O....O....O....O......OO....O.........
.........O....OOO.....O....O....O....O.....OOO....O.........
........OOO..OOO......O....O....O....O......OOO..OOO........
......O.O..OOOO.OO....O....O....O....O....OO.OOOO..O.O......
.......O.O.O.OO..OO..OOO..OOO..OOO..OOO..OO..OO.O.O.O.......
........OO.....O.O....O....O....O....O....O.O.....OO........
............OOO.O.OOO.O.OO.O.OO.O.OO.O.OOO.O.OOO............
..............O.O.O...O....O....O....O...O.O.O..............
........O.....OOOOO...O....O....O....O...OOOOO.....O........
.......OOO...O.O..OO..O....O....O....O..OO..O.O...OOO.......
.....O.O..OOOO..O..O.OOO..OOO..OOO..OOO.O..O..OOOO..O.O.....
......O.O.O.......OO..O....O....O....O..OO.......O.O.O......
.......OO...OOO.O.O.O.O.OO.O.OO.O.OO.O.O.O.O.OOO...OO.......
...........O.O.OOOO...O....O....O....O...OOOO.O.O...........
.............O...O.O..O....O....O....O..O.O...O.............
............O....O....O....O....O....O....O....O............
..........O.OOO..O..OO..O..O....O..O..OO..O..OOO.O..........
...........O.OO.OOO.OOOO...O....O...OOOO.OOO.OO.O...........
............O.O..O...OO....O....O....OO...O..O.O............
.............O.O.O.OO.OO.O.O....O.O.OO.OO.O.O.O.............
..............OO..O.O..O.O..O..O..O.O..O.O..OO..............
...................O.O.OO..........OO.O.O...................
....................OO.......OO.......OO....................
............................................................
```
 

