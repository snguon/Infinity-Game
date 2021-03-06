#include "gameboard_tests.hpp"

bool gameboardTests_run() {
  cout << "Running Gameboard Tests:" << endl;
  cout << "------------------------" << endl;

  // Create a new testing object to keep track of the results of each test
  Testing t;

  // Run all tests
  t.check(gameboardTests_constructors());
  t.check(gameboardTests_getGamePixelWidthAndHeight());
  t.check(gameboardTests_convertCoordinates());
  t.check(gameboardTests_saveAndLoad());
  t.check(gameboardTests_generateBoard());
  t.check(gameboardTests_movePlayer());
  t.check(gameboardTests_changeFloorTypeUnderPlayer());
  t.check(gameboardTests_moveWall());
  t.check(gameboardTests_addWall());
  t.check(gameboardTests_removeWall());

  // Display pass or fail result
  if (t.getResult()) {
    cout << "Done testing. All " << t.getNumTested() << " tests passed!" << endl;
  } else {
    cout << "Failed " << t.getNumFailed() << "/" << t.getNumTested() << " test(s). See message(s) above." << endl;
  }

  return t.getResult();
}

// *** Tests ***

// Test constructors
bool gameboardTests_constructors() {
  // Start new testing object
  Testing t("constructors");

  // Create an object and check its default constructor worked
  GameBoard g1;
  t.check(g1.getNumBlocksWide() == 30 && g1.getNumBlocksHigh() == 20 && g1.getBlockWidth() == 30 && g1.getBlockHeight() == 30, "Default constructor didn't work correctly");

  // Create an object and check its values were set correctly
  GameBoard g2(21, -1, 40, 2);
  t.check(g2.getNumBlocksWide() == 21 && g2.getNumBlocksHigh() == 20 && g2.getBlockWidth() == 40 && g2.getBlockHeight() == 2, "Constructor with a negative value didn't work correctly");

  // Create an object and check its values were set correctly
  GameBoard g3(21, 23, 49, 24);
  t.check(g3.getNumBlocksWide() == 21 && g3.getNumBlocksHigh() == 23 && g3.getBlockWidth() == 49 && g3.getBlockHeight() == 24, "Constructor with accepted values didn't work correctly");

  return t.getResult(); // Return pass or fail result
}

// Test getGamePixelWidth() and getGamePixelHeight()
bool gameboardTests_getGamePixelWidthAndHeight() {
  // Start new testing object
  Testing t("getGamePixelWidth() and getGamePixelHeight()");

  // Create an object and check the default game pixel width and height
  GameBoard g1;
  t.check(g1.getGamePixelWidth() == 900 && g1.getGamePixelHeight() == 600, "Default constructor with getGamePixelWidth() and getGamePixelHeight() didn't work correctly");

  // Create an object and check its values were set correctly
  GameBoard g2(21, 23, 49, 24);
  t.check(g2.getGamePixelWidth() == 1029 && g2.getGamePixelHeight() == 552, "Constructor with accepted values and getGamePixelWidth() and getGamePixelHeight() didn't work correctly");

  return t.getResult(); // Return pass or fail result
}

// Test convertVectorXToPixelX(), convertVectorYToPixelY(),
// convertPixelXToVectorX(), and convertPixelYToVectorY()
bool gameboardTests_convertCoordinates() {
  // Start new testing object
  Testing t("convertVectorXToPixelX(), convertVectorYToPixelY(), convertPixelXToVectorX(), and convertPixelYToVectorY()");

  // Create an object and check if the vector coordinates are converted
  // to pixel coordinates correctly for negative, 0, and positive cases
  GameBoard g1(21, 23, 49, 24);
  t.check(g1.convertVectorXToPixelX(-4) == 0, "Converting vectorX coordinates for negative numbers is not working");
  t.check(g1.convertVectorYToPixelY(-1) == 0, "Converting vectorY coordinates for negative numbers is not working");
  t.check(g1.convertPixelXToVectorX(-2) == 0, "Converting pixelX coordinates for negative numbers is not working");
  t.check(g1.convertPixelYToVectorY(-3) == 0, "Converting pixelY coordinates for negative numbers is not working");
  t.check(g1.convertVectorXToPixelX(0) == 0, "Converting vectorX coordinates for 0 is not working");
  t.check(g1.convertVectorYToPixelY(0) == 0, "Converting vectorY coordinates for 0 is not working");
  t.check(g1.convertPixelXToVectorX(0) == 0, "Converting pixelX coordinates for 0 is not working");
  t.check(g1.convertPixelYToVectorY(0) == 0, "Converting pixelY coordinates for 0 is not working");
  t.check(g1.convertVectorXToPixelX(2) == 98, "Converting vectorX coordinates for positive numbers is not working");
  t.check(g1.convertVectorYToPixelY(4) == 96, "Converting vectorY coordinates for positive numbers is not working");
  t.check(g1.convertPixelXToVectorX(147) == 3, "Converting pixelX coordinates for positive numbers is not working");
  t.check(g1.convertPixelYToVectorY(120) == 5, "Converting pixelY coordinates for positive numbers is not working");

  return t.getResult(); // Return pass or fail result
}

// Test saveGame() and loadGame()
bool gameboardTests_saveAndLoad() {
  // Start new testing object
  Testing t("saveGame() and loadGame()");

  // Create an object and save it
  GameBoard g1(1, 2, 3, 4);
  g1.saveGame("testing.infinity.json");

  // Create a new object and load it
  GameBoard g2;
  g2.loadGame("testing.infinity.json");

  // Make sure the values were loaded correctly
  t.check(g1.getBlockHeight() == g2.getBlockHeight() && g1.getBlockWidth() == g2.getBlockWidth() && g1.getNumBlocksHigh() == g2.getNumBlocksHigh() && g1.getNumBlocksWide() == g2.getNumBlocksWide() && g1.getSeed() == g2.getSeed(), "saveGame() and loadGame() doesn't work with only blockHeight/Width, and numBlocksHigh/Wide");

  // Create a map of changes
  map<int, map<int, shared_ptr<Block>>> testChanges;
  testChanges[1][1] = make_shared<Wall>();
  testChanges[1][2] = make_shared<Floor>();
  testChanges[3][6] = make_shared<Floor>();

  // Create an game with custom percentWall, seed, and changes, and save it
  GameBoard g3(2, 3, 34, 35, 42, 0.3, testChanges);
  g3.movePlayer(DIR_RIGHT);
  g3.saveGame("testing.infinity.json");
  // Game map:
  // F F
  // W W
  // F F

  // Create a new object and load it
  GameBoard g4;
  g4.loadGame("testing.infinity.json");

  // Make sure the values were loaded correctly
  t.check(g3.getBlockHeight() == g4.getBlockHeight() && g3.getBlockWidth() == g4.getBlockWidth() && g3.getNumBlocksHigh() == g4.getNumBlocksHigh() && g3.getNumBlocksWide() == g4.getNumBlocksWide() && g3.getSeed() == g4.getSeed() && g3.getPercentWall() == g4.getPercentWall() && g4.getChanges()[1][1]->getBlockType() == WallBlock && g4.getChanges()[1][2]->getBlockType() == FloorBlock && g4.getChanges()[3][6]->getBlockType() == FloorBlock && g4.getPlayer().getVectorX() == 1 && g4.getPlayer().getVectorY() == 0, "saveGame() and loadGame() doesn't work for custom seed, percentWall, and changes");

  return t.getResult(); // Return pass or fail result
}

// Test generateBoard() (and generateColumn() - tested indirectly)
bool gameboardTests_generateBoard() {
  // Start new testing object
  Testing t("generateBoard() (and generateColumn())");

  // Create an object with a custom seed
  GameBoard g1(2, 3, 20, 20, 42, 0.3);
  // Game map:
  // F F
  // W F
  // F W

  // Assert the board is correctly created from that random seed
  t.check(g1.getBoard()[0][0]->getBlockType() == FloorBlock && g1.getBoard()[0][1]->getBlockType() == WallBlock && g1.getBoard()[0][2]->getBlockType() == FloorBlock && g1.getBoard()[1][0]->getBlockType() == FloorBlock && g1.getBoard()[1][1]->getBlockType() == FloorBlock && g1.getBoard()[1][2]->getBlockType() == WallBlock, "generateBoard() with no changes, random seed of 42, and percent wall of 0.3 doesn't work");

  // Create a map of changes
  map<int, map<int, shared_ptr<Block>>> testChanges;
  testChanges[1][1] = make_shared<Wall>();
  testChanges[1][2] = make_shared<Floor>();

  // Create an object with a custom seed and changes
  GameBoard g2(2, 3, 20, 20, 42, 0.3, testChanges);
  // Game map:
  // F F
  // W W
  // F F

  // Assert the board is correctly created from that random seed
  t.check(g2.getBoard()[0][0]->getBlockType() == FloorBlock && g2.getBoard()[0][1]->getBlockType() == WallBlock && g2.getBoard()[0][2]->getBlockType() == FloorBlock && g2.getBoard()[1][0]->getBlockType() == FloorBlock && g2.getBoard()[1][1]->getBlockType() == WallBlock && g2.getBoard()[1][2]->getBlockType() == FloorBlock, "generateBoard() with changes, random seed of 42, and percent wall of 0.3 doesn't work");

  return t.getResult(); // Return pass or fail result
}

// Test movePlayer()
bool gameboardTests_movePlayer() {
  // Start new testing object
  Testing t("movePlayer()");

  // Create a map of changes
  map<int, map<int, shared_ptr<Block>>> testChanges;
  testChanges[1][2] = make_shared<Floor>();

  // Create an object with a custom seed and one change
  GameBoard g1(3, 3, 20, 20, 42, 0.3, testChanges);
  // Game map:
  // F F F | F F F
  // W F F | W W F
  // F F W | F F W

  // Check moving the player, while not moving out of bounds or into walls
  g1.movePlayer(DIR_UP); // Tests not moving up
  t.check(g1.getPlayer().getVectorX() == 0 && g1.getPlayer().getVectorY() == 0, "movePlayer() can move up when at (0,0) out of bounds");
  g1.movePlayer(DIR_LEFT); // Tests not moving left
  t.check(g1.getPlayer().getVectorX() == 0 && g1.getPlayer().getVectorY() == 0, "movePlayer() can move left when at (0,0) out of bounds");
  g1.movePlayer(DIR_RIGHT); // Tests moving right
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 0, "movePlayer() can't move right when at (0,0)");
  g1.movePlayer(DIR_RIGHT); // Tests moving right
  t.check(g1.getPlayer().getVectorX() == 2 && g1.getPlayer().getVectorY() == 0, "movePlayer() can't move right when at (1,0)");
  g1.movePlayer(DIR_RIGHT); // Tests moving right into newly generated region
  t.check(g1.getPlayer().getVectorX() == 3 && g1.getPlayer().getVectorY() == 0, "movePlayer() can't move right when at (2,0) into newly generated region");
  g1.movePlayer(DIR_DOWN); // Tests not moving down into wall in newly generated region
  t.check(g1.getPlayer().getVectorX() == 3 && g1.getPlayer().getVectorY() == 0, "movePlayer() can move down when at (3,0) into wall in newly generated region");
  g1.movePlayer(DIR_UP); // Tests not moving upin newly generated region
  t.check(g1.getPlayer().getVectorX() == 3 && g1.getPlayer().getVectorY() == 0, "movePlayer() can move up when at (0,0) out of bounds in newly generated region");
  g1.movePlayer(DIR_LEFT); // Tests moving left out of newly generated region
  t.check(g1.getPlayer().getVectorX() == 2 && g1.getPlayer().getVectorY() == 0, "movePlayer() can't move right when at (3,0) out of newly generated region");
  g1.movePlayer(DIR_DOWN); // Tests moving down
  t.check(g1.getPlayer().getVectorX() == 2 && g1.getPlayer().getVectorY() == 1, "movePlayer() can't move down when at (2,0)");
  g1.movePlayer(DIR_DOWN); // Tests not moving down into wall
  t.check(g1.getPlayer().getVectorX() == 2 && g1.getPlayer().getVectorY() == 1, "movePlayer() can move down when at (2,1) into wall");
  g1.movePlayer(DIR_LEFT); // Tests moving left
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 1, "movePlayer() can't move left when at (2,1)");
  g1.movePlayer(DIR_LEFT); // Tests not moving left into wall
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 1, "movePlayer() can move left when at (1,1) into wall");
  g1.movePlayer(DIR_UP); // Tests moving up
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 0, "movePlayer() can't move up when at (1,1)");
  g1.movePlayer(DIR_DOWN); // Tests moving down
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 1, "movePlayer() can't move up when at (1,0)");
  g1.movePlayer(DIR_DOWN); // Tests moving down
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 2, "movePlayer() can't move down when at (1,1)");
  g1.movePlayer(DIR_DOWN); // Tests not moving down
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 2, "movePlayer() can move down when at (1,2) out of bounds");
  g1.movePlayer(DIR_RIGHT); // Tests not moving right into wall
  t.check(g1.getPlayer().getVectorX() == 1 && g1.getPlayer().getVectorY() == 2, "movePlayer() can move right when at (1,2) into wall");
  g1.movePlayer(DIR_LEFT); // Tests moving left
  t.check(g1.getPlayer().getVectorX() == 0 && g1.getPlayer().getVectorY() == 2, "movePlayer() can't move left when at (1,2)");
  g1.movePlayer(DIR_UP); // Tests not moving up into wall
  t.check(g1.getPlayer().getVectorX() == 0 && g1.getPlayer().getVectorY() == 2, "movePlayer() can move up when at (0,2) into wall");

  return t.getResult(); // Return pass or fail result
}

/**
 * Requires: nothing
 * Modifies: nothing
 * Effects: Test changeFloorTypeUnderPlayer()
 */
bool gameboardTests_changeFloorTypeUnderPlayer() {
  // Start new testing object
  Testing t("changeFloorTypeUnderPlayer()");

  // Create a map of changes
  map<int, map<int, shared_ptr<Block>>> testChanges;
  testChanges[1][2] = make_shared<Floor>();

  // Create an object with a custom seed and one change
  GameBoard g1(3, 3, 1, 1, 42, 0.3, testChanges);
  // Game map:
  // F F F | F F F
  // W F F | W W F
  // F F W | F F W

  // Store the initial values for the board and changes
  vector<vector<shared_ptr<Block>>> initialBoard = g1.getBoard();
  map<int, map<int, shared_ptr<Block>>> initialChanges = g1.getChanges();

  // Check to make sure each floor type changes
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[0][0])->getFloorType() == GrassFloor, "Default floor isn't grass");
  g1.changeFloorTypeUnderPlayer(DirtFloor);
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[0][0])->getFloorType() == DirtFloor, "Floor didn't change to dirt");
  g1.changeFloorTypeUnderPlayer(GrassFloor);
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[0][0])->getFloorType() == GrassFloor, "Floor didn't change to grass");
  g1.movePlayer(DIR_RIGHT);
  g1.changeFloorTypeUnderPlayer(SandFloor);
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[1][0])->getFloorType() == SandFloor, "Floor didn't change to sand");

  // Move player into newly generated region
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);

  // Make sure the floor type changes in the newly generated region
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[4][0])->getFloorType() == GrassFloor, "Default floor isn't grass in the newly generated region");
  g1.changeFloorTypeUnderPlayer(DirtFloor);
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[4][0])->getFloorType() == DirtFloor, "Floor didn't change to dirt in the newly generated region");
  g1.changeFloorTypeUnderPlayer(GrassFloor);
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[4][0])->getFloorType() == GrassFloor, "Floor didn't change to grass in the newly generated region");
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_DOWN);
  g1.changeFloorTypeUnderPlayer(SandFloor);
  t.check(dynamic_pointer_cast<Floor>(g1.getBoard()[5][1])->getFloorType() == SandFloor, "Floor didn't change to sand in the newly generated region");

  return t.getResult(); // Return pass or fail result
}

// Test moveWall()
bool gameboardTests_moveWall() {
  // Start new testing object
  Testing t("moveWall()");

  // Create a map of changes
  map<int, map<int, shared_ptr<Block>>> testChanges;
  testChanges[1][2] = make_shared<Floor>();

  // Create an object with a custom seed and one change
  GameBoard g1(3, 3, 1, 1, 42, 0.3, testChanges);
  // Game map:
  // F F F | F F F
  // W F F | W W F
  // F F W | F F W

  // Store the initial values for the board and changes
  vector<vector<shared_ptr<Block>>> initialBoard = g1.getBoard();
  map<int, map<int, shared_ptr<Block>>> initialChanges = g1.getChanges();

  // We should return false if any coordinate is out of bounds
  t.check(!g1.moveWall(6, 1, 1, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "lastX can be out of bounds");
  t.check(!g1.moveWall(0, 6, 1, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "lastY can be out of bounds");
  t.check(!g1.moveWall(0, 0, 6, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "currentX can be out of bounds");
  t.check(!g1.moveWall(0, 0, 0, 6) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "currentY can be out of bounds");

  // We should return false if we are in the same spot
  t.check(!g1.moveWall(1, 1, 1, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "We moved even though we shouldn't");

  // We should return false if the old location was not a wall
  t.check(!g1.moveWall(1, 1, 1, 0) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "We moved something other than a wall");

  // We should return false if we try to move on top of a wall
  t.check(!g1.moveWall(0, 1, 2, 2) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "We moved on top of a wall");

  // We should return false if we try to move on top of a player
  t.check(!g1.moveWall(0, 1, 0, 0) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "We moved on top of the player");

  // Check if we are prevented from a valid move
  t.check(g1.moveWall(0, 1, 1, 1), "Something prevented a valid move");
  t.check(g1.getBoard()[0][1]->getBlockType() == FloorBlock, "board moved from position is not a floor");
  t.check(g1.getBoard()[1][1]->getBlockType() == WallBlock, "board moved to position is not a wall");
  t.check(g1.getChanges()[0][1]->getBlockType() == FloorBlock, "changes moved from position is not a floor");
  t.check(g1.getChanges()[1][1]->getBlockType() == WallBlock, "changes moved to position is not a wall");

  // Move player into newly generated region
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);

  // Attempt moving a wall in the newly generated region
  t.check(g1.moveWall(1, 1, 1, 2), "Something prevented a valid move in the newly generated region");
  t.check(g1.getBoard()[4][1]->getBlockType() == FloorBlock, "board moved from position is not a floor in the newly generated region");
  t.check(g1.getBoard()[4][2]->getBlockType() == WallBlock, "board moved to position is not a wall in the newly generated region");
  t.check(g1.getChanges()[4][1]->getBlockType() == FloorBlock, "changes moved from position is not a floor in the newly generated region");
  t.check(g1.getChanges()[4][2]->getBlockType() == WallBlock, "changes moved to position is not a wall in the newly generated region");

  return t.getResult(); // Return pass or fail result
}

// Test addWall()
bool gameboardTests_addWall() {
  // Start new testing object
  Testing t("addWall()");

  // Create a map of changes
  map<int, map<int, shared_ptr<Block>>> testChanges;
  testChanges[1][2] = make_shared<Floor>();

  // Create an object with a custom seed and one change
  GameBoard g1(3, 3, 1, 1, 42, 0.3, testChanges);
  // Game map:
  // F F F | F F F
  // W F F | W W F
  // F F W | F F W

  // Store the initial values for the board and changes
  vector<vector<shared_ptr<Block>>> initialBoard = g1.getBoard();
  map<int, map<int, shared_ptr<Block>>> initialChanges = g1.getChanges();

  // We should return false if any coordinate is out of bounds
  t.check(!g1.addWall(3, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "pixelX can be out of bounds");
  t.check(!g1.addWall(0, 3) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "pixelY can be out of bounds");

  // We should return false if the location is not a floor
  t.check(!g1.addWall(0, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "We added a wall on a wall");

  // We should return false if we try to add on top of a player
  t.check(!g1.addWall(0, 0) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "We moved on top of the player");

  // Check if we are prevented from a valid add
  t.check(g1.addWall(1, 1), "Something prevented a valid add");
  t.check(g1.getBoard()[1][1]->getBlockType() == WallBlock, "board at added position is not a wall");
  t.check(g1.getChanges()[1][1]->getBlockType() == WallBlock, "changes at added position is not a wall");

  // Move player into newly generated region
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);

  // Attempt adding a wall in the newly generated regionre
  t.check(g1.addWall(1, 2), "Something prevented a valid add in the newly generated region");
  t.check(g1.getBoard()[4][2]->getBlockType() == WallBlock, "board at added position is not a wall in the newly generated region");
  t.check(g1.getChanges()[4][2]->getBlockType() == WallBlock, "changes at added position is not a wall in the newly generated region");

  return t.getResult(); // Return pass or fail result
}

// Test removeWall()
bool gameboardTests_removeWall() {
  // Start new testing object
  Testing t("removeWall()");

  // Create a map of changes
  map<int, map<int, shared_ptr<Block>>> testChanges;
  testChanges[1][2] = make_shared<Floor>();

  // Create an object with a custom seed and one change
  GameBoard g1(3, 3, 1, 1, 42, 0.3, testChanges);
  // Game map:
  // F F F | F F F
  // W F F | W W F
  // F F W | F F W

  // Store the initial values for the board and changes
  vector<vector<shared_ptr<Block>>> initialBoard = g1.getBoard();
  map<int, map<int, shared_ptr<Block>>> initialChanges = g1.getChanges();

  // We should return false if any coordinate is out of bounds
  t.check(!g1.removeWall(6, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "pixelX can be out of bounds");
  t.check(!g1.removeWall(0, 6) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "pixelY can be out of bounds");

  // We should return false if the location is not a wall
  t.check(!g1.removeWall(1, 1) && g1.getBoard() == initialBoard && g1.getChanges() == initialChanges, "We removed a floor instead of a wall");

  // Check if we are prevented from a valid remove
  t.check(g1.removeWall(0, 1), "Something prevented a valid remove");
  t.check(g1.getBoard()[0][1]->getBlockType() == FloorBlock, "board at remove position is not a floor");
  t.check(g1.getChanges()[0][1]->getBlockType() == FloorBlock, "changes at remove position is not a floor");

  // Move player into newly generated region
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);
  g1.movePlayer(DIR_RIGHT);

  // Attempt removing a wall in the newly generated region
  t.check(g1.removeWall(1, 1), "Something prevented a valid remove in the newly generated region");
  t.check(g1.getBoard()[4][1]->getBlockType() == FloorBlock, "board at remove position is not a floor in the newly generated region");
  t.check(g1.getChanges()[4][1]->getBlockType() == FloorBlock, "changes at remove position is not a floor in the newly generated region");

  return t.getResult(); // Return pass or fail result
}
