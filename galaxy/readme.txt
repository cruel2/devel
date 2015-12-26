
Contents
--------
1. Galaxy simulation
2. Paralellism
3. Frequent mistakes
4. Files


1. Galaxy simulation
-----------------

 * Lot of stars/particles (with equal mass)
 * Newtonian gravity/electrostatic interaction
 * multipole expansion of every cell


2. Paralellism
-----------
The propagation of particles is made by known state (x, y, vx, vy) and
computed acceleration values (ax, ay). The computation times:
 * coefficients O(N)
 * acceleration O(N^2)
 * propagation  O(N)
So only the acceleration computation is needed to be paralellized.


3. Frequent mistakes
-----------------
CELLS2 is the total number of cells, not CELLS.


4. Files
-----
Source
analize.c    analitic functions (energy sum, free particles, ...)
galaxy.c     functions
galaxy.h     structures
graph.c      OpenGL rendering
main.c       calls initialization and time-development functions
test.c      "unit" tests
test.h       header for tests
write_bmp.c  writes a memory bitmap to BMP file
Makefile

Compiled
main      main executable for simulation
test      main executable for tests


