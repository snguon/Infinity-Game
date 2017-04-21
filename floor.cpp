#include "floor.hpp"

/**
 * Requires: nothing
 * Modifies: nothing
 * Effects: Returns if the player can move on top of the block
 */
bool Floor::canMoveOnTop() const { return true; }

/**
 * Requires: nothing
 * Modifies: nothing
 * Effects: Returns the type of block
 */
BlockType Floor::getBlockType() const { return FloorBlock; }

/**
 * Requires: nothing
 * Modifies: nothing
 * Effects: gets type of floor
 */
FloorType Floor::getFloorType() const { return floorType; }

/**
 * Requires: nothing
 * Modifies: nothing
 * Effects: changes type of floor
 */
void Floor::setFloorType(FloorType f) {
  floorType = f;
  switch (f) {
  case GrassFloor:
    setColor({0, 123 / 255.0, 12 / 255.0});
    break;
  case SandFloor:
    setColor({237 / 255.0, 201 / 255.0, 175 / 255.0});
    break;
  case DirtFloor:
    setColor({120 / 255.0, 72 / 255.0, 0});
    break;
  }
}

/**
* Requires: nothing
* Modifies: nothing
* Effects: Convert the floor to json
*/
json Floor::toJson() const {
  // Get the parent's json
  json obj = Block::toJson();

  // Add the floor type
  obj["floorType"] = floorType;

  return obj;
}

/**
* Requires: json object
* Modifies: alternateColor and vectorX/Y
* Effects: Convert json to the floor
*/
void Floor::fromJson(json j) {
  // Import into the parent
  Block::fromJson(j);

  // Import the floor type
  floorType = j.at("floorType").get<FloorType>();
}
