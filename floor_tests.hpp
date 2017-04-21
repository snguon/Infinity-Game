#ifndef floor_tests_hpp
#define floor_tests_hpp

#include "floor.hpp"
#include "testing.hpp"

/**
* Requires: nothing
* Modifies: nothing
* Effects: Runs all tests
*/
void floorTests_run();

/**
* Requires: nothing
* Modifies: nothing
* Effects: Tests canMoveOnTop() and getBlockType()
*/
bool floorTests_constants();

/**
 * Requires: nothing
 * Modifies: nothing
 * Effects: Tests getTypeOfFloor() and changeFloorType()
 */
bool floorTests_types();
#endif
