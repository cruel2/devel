
/*
    test.c
    
    Tests for the functions:
     * cell_neighbour
     * which_cell
    Test functions return the number of faults, 0 if all tests were OK.
*/

#include <stdio.h>
#include "galaxy.h"
#include "test.h"


// verbose mode switch
bool verbose = false;


int test_which_cell()
{
  int exp;         // expected value
  int faults = 0;  // fault counter
  int res;         // result
  
  // x = +0, y = +0
  res = which_cell(EPS, EPS);
  exp = CELLS / 2 * CELLS + CELLS / 2;
  if (res != exp || verbose)
  {
    printf("test_which_cell 1 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x = +0, y = -0
  res = which_cell(EPS, -EPS);
  exp = (CELLS / 2 - 1) * CELLS + CELLS / 2;
  if (res != exp || verbose)
  {
    printf("test_which_cell 2 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x = -0, y = -0
  res = which_cell(-EPS, -EPS);
  exp = (CELLS / 2 - 1) * CELLS + CELLS / 2 - 1;
  if (res != exp || verbose)
  {
    printf("test_which_cell 3 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x = -0, y = +0
  res = which_cell(-EPS, EPS);
  exp = (CELLS / 2) * CELLS + CELLS / 2 - 1;
  if (res != exp || verbose)
  {
    printf("test_which_cell 4 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x = CELLX+EPS, y = +0
  res = which_cell(CELLX + EPS, EPS);
  exp = CELLS / 2 * CELLS + CELLS / 2 + 1;
  if (res != exp || verbose)
  {
    printf("test_which_cell 5 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x = CELLX+EPS, y = -0
  res = which_cell(CELLX + EPS, -EPS);
  exp = (CELLS / 2 - 1) * CELLS + CELLS / 2 + 1;
  if (res != exp || verbose)
  {
    printf("test_which_cell 6 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x = -CELLX+EPS, y = +0
  res = which_cell(-CELLX + EPS, EPS);
  exp = CELLS / 2 * CELLS + CELLS / 2 - 1;
  if (res != exp || verbose)
  {
    printf("test_which_cell 7 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  return faults;
}


int test_cell_neighbour()
{
  int exp;         // expected value
  int faults = 0;  // fault counter
  int res;         // result
  
  // x+
  res = cell_neighbour(CELLS + CELLS / 2, CELLS + CELLS / 2 + 1);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 1 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x-
  res = cell_neighbour(CELLS + CELLS / 2, CELLS + CELLS / 2 - 1);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 2 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // y+
  res = cell_neighbour(CELLS + CELLS / 2, 2 * CELLS + CELLS / 2);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 3 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // y-
  res = cell_neighbour(CELLS + CELLS / 2, CELLS / 2 - 1);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 4 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x+ y-
  res = cell_neighbour(CELLS + CELLS / 2, CELLS / 2 + 1);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 5 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x+ y+
  res = cell_neighbour(CELLS + CELLS / 2, 2 * CELLS + CELLS / 2 + 1);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 6 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x- y-
  res = cell_neighbour(CELLS + CELLS / 2, CELLS / 2 - 1);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 7 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // x- y+
  res = cell_neighbour(CELLS + CELLS / 2, 2 * CELLS + CELLS / 2 - 1);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 8 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // same cell
  res = cell_neighbour(CELLS + CELLS / 2, CELLS + CELLS / 2);
  exp = true;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 9 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // far cell
  res = cell_neighbour(CELLS + CELLS / 2, CELLS + CELLS / 2 + 2);
  exp = false;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 10 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  // far cell
  res = cell_neighbour(3 * CELLS + CELLS / 2, CELLS + CELLS / 2);
  exp = false;
  if (res != exp || verbose)
  {
    printf("test_cell_neighbour 11 - result: %d  expected: %d\n", res, exp);
    faults++;
  }
  
  return faults;
}


int main(int argc, char** argv)
{
  int c; // for command line arguments
  
  while ((c = getopt(argc, argv, "v")) != -1)
  {
    switch (c)
    {
      case 'v':
        verbose = true;
        break;
      default:
        return -1;
    }
  }
  

  if (test_which_cell() == 0)
    printf("which_cell is OK\n");
    
  if (test_cell_neighbour() == 0)
    printf("cell_neighbour is OK\n");
  
  // all OK
  return 1;
}

