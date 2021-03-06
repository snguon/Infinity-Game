#ifndef floor_hpp
#define floor_hpp

#include "block.hpp"

// Types of floor
enum FloorType { GrassFloor = 1, SandFloor = 2, DirtFloor = 3 };

class Floor : public Block {
public:
  /**
  * Requires: nothing
  * Modifies: floorType and color
  * Effects: Create a new floor
  */
  Floor();
  /**
  * Requires: a floor type
  * Modifies: floorType and color
  * Effects: Create a new floor of a certain type
  */
  Floor(FloorType f);

  /**
   * Requires: nothing
   * Modifies: nothing
   * Effects: Returns if the player can move on top of the block
   */
  virtual bool canMoveOnTop() const override;

  /**
   * Requires: nothing
   * Modifies: nothing
   * Effects: Returns the type of block
   */
  virtual BlockType getBlockType() const override;

  /**
   * Requires: nothing
   * Modifies: nothing
   * Effects: gets type of floor
   */
  FloorType getFloorType() const;
  /**
   * Requires: nothing
   * Modifies: nothing
   * Effects: changes type of floor and sets the respective color
   */
  void setFloorType(FloorType f);

  /**
  * Requires: nothing
  * Modifies: nothing
  * Effects: Convert the floor to json
  */
  virtual json toJson() const override;

  /**
  * Requires: json object
  * Modifies: floorType
  * Effects: Convert json to the floor
  */
  virtual void fromJson(json j) override;

private:
  FloorType floorType;
};

#endif
