#include "gameboard.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <typeinfo>

/**
* Requires: nothing
* Modifies: numBlocksWide, numBlocksHigh, blockWidth, and blockHeight
* Effects: Creates a new gameboard
*/
GameBoard::GameBoard() : GameBoard(0, 0, 0, 0) {
}
/**
* Requires: number of blocks wide and high, and block width and height to be
* positive
* Modifies: numBlocksWide, numBlocksHigh, blockWidth, and blockHeight
* Effects: Creates a new gameboard
*/
GameBoard::GameBoard(int nBlocksWide, int nBlocksHigh, int blockW, int blockH) {
  // Set the number of blocks wide
  if (nBlocksWide <= 0) {
    numBlocksWide = 30; // Default number of blocks wide
  } else {
    numBlocksWide = nBlocksWide;
  }

  // Set the number of blocks high
  if (nBlocksHigh <= 0) {
    numBlocksHigh = 20; // Default number of blocks high
  } else {
    numBlocksHigh = nBlocksHigh;
  }

  // Set the block width
  if (blockW <= 0) {
    blockWidth = 30; // Default block width
  } else {
    blockWidth = blockW;
  }

  // Set the block height
  if (blockH <= 0) {
    blockHeight = 30; // Default block height
  } else {
    blockHeight = blockH;
  }

  // Set the left edge of the display window to 0
  leftDisplayEdge = 0;

  // Initialize the seed to the current timestamp
  seed = time(nullptr);
  rand.seed(seed);

  // Initialize the percent wall
  percentWall = 0.3;

  // Default the player to (0,0)
  player.setVectorX(0);
  player.setVectorY(0);

  // Set the player's color to blue and alternate color to gold
  player.setColor({30 / 255.0, 144 / 255.0, 1});
  player.setAlternateColor({1, 215 / 255.0, 0});

  // Set the (0,0) position to a floor
  changes[0][0] = make_shared<Floor>();

  // Generate the board
  generateBoard();
}
/**
* Requires: same as previous constructor, plus seed, percentWall, and changes
* Modifies: numBlocksWide, numBlocksHigh, blockWidth, blockHeight, seed,
* percentWall, and changes map
* Effects: Creates a new gameboard (used primarly for testing purposes)
*/
GameBoard::GameBoard(int nBlocksWide, int nBlocksHigh, int blockW, int blockH, int s, double pWall, map<int, map<int, shared_ptr<Block>>> c) : GameBoard(nBlocksWide, nBlocksHigh, blockW, blockH) {
  // If the seed is set, use it
  if (s != 0) {
    seed = s;
    rand.seed(seed);
  }

  // If the wall is set, use it
  if (pWall != -1) {
    percentWall = max(0.0, min(pWall, 1.0));
  }

  // Set the changes if it is not empty
  if (!c.empty()) {
    changes = c;
  }

  // Regenerate the board (since this will only be used in testing, it's fine)
  generateBoard();
}

/**
* Requires: nothing
* Modifies: nothing
* Effects: Returns the respective field
*/
int GameBoard::getNumBlocksWide() const {
  return numBlocksWide;
}
int GameBoard::getNumBlocksHigh() const {
  return numBlocksHigh;
}
int GameBoard::getBlockWidth() const {
  return blockWidth;
}
int GameBoard::getBlockHeight() const {
  return blockHeight;
}
int GameBoard::getSeed() const {
  return seed;
}
double GameBoard::getPercentWall() const {
  return percentWall;
}
Player GameBoard::getPlayer() const {
  return player;
}
vector<vector<shared_ptr<Block>>> GameBoard::getBoard() const {
  return board;
}
map<int, map<int, shared_ptr<Block>>> GameBoard::getChanges() const {
  return changes;
}

/**
* Requires: nothing
* Modifies: nothing
* Effects: Computes and returns the total pixel width or hight
*/
int GameBoard::getGamePixelWidth() const {
  return blockWidth * numBlocksWide;
}
int GameBoard::getGamePixelHeight() const {
  return blockHeight * numBlocksHigh;
}

/**
* Requires: positive vectorX, vectorY, pixelX, or pixelY
* Modifies: nothing
* Effects: Converts the vector coordinate to pixel coordinates or vice versa
*/
int GameBoard::convertVectorXToPixelX(int vectorX) const {
  // Allow only positive coordinates
  if (vectorX < 0) {
    return 0;
  }

  // Calculate the pixelX position
  return vectorX * blockWidth;
}
int GameBoard::convertVectorYToPixelY(int vectorY) const {
  // Allow only positive coordinates
  if (vectorY < 0) {
    return 0;
  }

  // Calculate the pixelY position
  return vectorY * blockHeight;
}
int GameBoard::convertPixelXToVectorX(int pixelX) const {
  // Allow only positive coordinates
  if (pixelX < 0) {
    return 0;
  }

  // Calculate the pixelX position
  return pixelX / blockWidth;
}
int GameBoard::convertPixelYToVectorY(int pixelY) const {
  // Allow only positive coordinates
  if (pixelY < 0) {
    return 0;
  }

  // Calculate the pixelY position
  return pixelY / blockHeight;
}

/**
* Requires: a valid path to save the file to
* Modifies: nothing
* Effects: saves the game to a file
*/
bool GameBoard::saveGame(string filename) {
  // Keep track of save status
  bool isSaved = true;

  // If filename is empty, use default filename
  if (filename.empty()) {
    if (gameFilename.empty()) {
      filename = to_string(seed) + ".infinity.json"; // Default seed based filename
      gameFilename = filename;                       // Store the generated filename
    } else {
      filename = gameFilename; // Custom filename
    }
  }

  // Open session file
  ofstream gameFile(filename);
  if (gameFile) {
    // Create new json object
    json gameJson;

    // Add game version
    gameJson["gameVersion"] = GAME_VERSION;

    // Add time saved
    gameJson["saveTime"] = time(nullptr);

    // Add the game dimensions
    gameJson["numBlocksWide"] = numBlocksWide;
    gameJson["numBlocksHigh"] = numBlocksHigh;
    gameJson["blockWidth"] = blockWidth;
    gameJson["blockHeight"] = blockHeight;

    // Save the left display edge
    gameJson["leftDisplayEdge"] = leftDisplayEdge;

    // Add the seed
    gameJson["seed"] = seed;

    // Add the percentages
    gameJson["percentWall"] = percentWall;

    // Add the player
    gameJson["player"] = player.toJson();

    // Add the changes
    for (map<int, map<int, shared_ptr<Block>>>::iterator i = changes.begin(); i != changes.end(); i++) {
      for (map<int, shared_ptr<Block>>::iterator j = (i->second).begin(); j != (i->second).end(); j++) {
        json object = (j->second)->toJson();   // Convert the block to JSON
        object["column"] = i->first;           // Add the column position
        object["row"] = j->first;              // Add the row position
        gameJson["changes"].push_back(object); // Add to the changes json array
      }
    }

    // Save to the file in pretty print
    gameFile << setw(4) << gameJson << endl;
  } else {
    // Error opening file
    isSaved = false;
  }

  // Close file
  gameFile.close();

  // Return saved status
  return isSaved;
}

/**
* Requires: a valid path to an existing game file
* Modifies: all GameBoard fields
* Effects: loads the game from a file
*/
bool GameBoard::loadGame(string filename) {
  // Keep track of the game load status
  bool isLoaded = true;

  // If filename is empty, use default filename
  if (filename.empty()) {
    filename = "game.infinity.json";
  }

  // Store the filename for when we save
  gameFilename = filename;

  // Open game file
  ifstream gameFile(filename);
  if (gameFile) {
    // Create new json object
    json gameJson;

    // Try converting file to json object
    try {
      gameFile >> gameJson;
    } catch (exception e) {
      cout << "Error converting file to json..." << endl;
      isLoaded = false;
    }

    // Load game dimensions
    try {
      numBlocksWide = gameJson.at("numBlocksWide").get<int>();
      numBlocksHigh = gameJson.at("numBlocksHigh").get<int>();
      blockWidth = gameJson.at("blockWidth").get<int>();
      blockHeight = gameJson.at("blockHeight").get<int>();
    } catch (exception e) {
      cout << "Syntax invalid for save file... Error loading game dimensions..." << endl;
      isLoaded = false;
    }

    // Load left display edge - defaults to 0 if not in file
    try {
      if (gameJson.count("leftDisplayEdge") > 0) {
        leftDisplayEdge = gameJson.at("leftDisplayEdge").get<int>();
      }
    } catch (exception e) {
      cout << "Syntax invalid for save file... Error loading left display edge..." << endl;
      isLoaded = false;
    }

    // Load game seed
    try {
      seed = gameJson.at("seed").get<int>();
    } catch (exception e) {
      cout << "Syntax invalid for save file... Error loading seed..." << endl;
      isLoaded = false;
    }

    // Create random object using the seed
    rand.seed(seed);

    // Load board percentages
    try {
      percentWall = gameJson.at("percentWall").get<double>();
    } catch (exception e) {
      cout << "Syntax invalid for save file... Error loading percentages..." << endl;
      isLoaded = false;
    }

    // Load player
    try {
      player.fromJson(gameJson.at("player"));
    } catch (exception e) {
      cout << "Syntax invalid for save file... Error loading player..." << endl;
      isLoaded = false;
    }

    // Load changed blocks
    try {
      for (json::iterator change = gameJson["changes"].begin(); change != gameJson["changes"].end(); change++) {
        // Create the type of block
        switch (change->at("type").get<int>()) {
        case WallBlock:
          changes[change->at("column").get<int>()][change->at("row").get<int>()] = make_shared<Wall>();
          break;
        case FloorBlock:
          changes[change->at("column").get<int>()][change->at("row").get<int>()] = make_shared<Floor>();
          break;
        }
        // Load the data into the block
        changes[change->at("column").get<int>()][change->at("row").get<int>()]->fromJson(*change);
      }

      // Generate the board
      generateBoard();
    } catch (exception e) {
      cout << "Syntax invalid for save file... Error loading changed blocks..." << endl;
      isLoaded = false;
    }
  } else {
    // Couldn't find file
    isLoaded = false;
  }

  // Close file
  gameFile.close();

  // Return loaded result
  return isLoaded;
}

/**
* Requires: a GameDirection
* Modifies: player
* Effects: moves the player in the direction specified
*/
void GameBoard::movePlayer(GameDirection direction) {
  if (direction == DIR_LEFT && player.getVectorX() > leftDisplayEdge && board[player.getVectorX() - 1][player.getVectorY()]->canMoveOnTop()) {
    // We can move to the left (no edge or wall blocking)
    if (((player.getVectorX() - leftDisplayEdge) / (double)numBlocksWide < 0.25 || player.getVectorX() - leftDisplayEdge == 1) && leftDisplayEdge > 0) {
      // Scroll board
      leftDisplayEdge--;
    }
    // Move player to the left
    player.setVectorX(player.getVectorX() - 1);
  } else if (direction == DIR_RIGHT && player.getVectorX() < board.size() - 1 && board[player.getVectorX() + 1][player.getVectorY()]->canMoveOnTop()) {
    // We can move to the right (no edge or wall blocking)
    if ((leftDisplayEdge + numBlocksWide - 1 - player.getVectorX()) / (double)numBlocksWide < 0.25 || leftDisplayEdge + numBlocksWide - 1 - player.getVectorX() == 1) {
      // Scroll board
      leftDisplayEdge++;
      // Generate an extra column
      generateColumn();
    }
    // Move player to the right
    player.setVectorX(player.getVectorX() + 1);
  } else if (direction == DIR_UP && player.getVectorY() > 0 && board[player.getVectorX()][player.getVectorY() - 1]->canMoveOnTop()) {
    // We can move up (no edge or wall blocking)
    player.setVectorY(player.getVectorY() - 1);
  } else if (direction == DIR_DOWN && player.getVectorY() < numBlocksHigh - 1 && board[player.getVectorX()][player.getVectorY() + 1]->canMoveOnTop()) {
    // We can move down (no edge or wall blocking)
    player.setVectorY(player.getVectorY() + 1);
  }
}

/**
 * Requires: nothing
 * Modifies: player
 * Effects: swaps the player color
 */
void GameBoard::swapPlayerColor() {
  player.swapColor();
}

/**
 * Requires: nothing
 * Modifies: floor type
 * Effects: change the floor type
 */
void GameBoard::changeFloorTypeUnderPlayer(FloorType f) {
  // If the block below the player is a floor
  if (board[player.getVectorX()][player.getVectorY()]->getBlockType() == FloorBlock) {
    // Change the floor type (need to cast to a floor pointer first)
    dynamic_pointer_cast<Floor>(board[player.getVectorX()][player.getVectorY()])->setFloorType(f);
    // Update the changes map at that location with the changed floor
    changes[player.getVectorX()][player.getVectorY()] = board[player.getVectorX()][player.getVectorY()];
  }
}

/**
* Requires: positive lastX, lastY, currentX, and currentY
* Modifies: board and changes
* Effects: Moves the wall located at the last coordinates to the current
* coordinates, boolean return value signifies if we moved
*/
bool GameBoard::moveWall(int lastX, int lastY, int currentX, int currentY) {
  // Convert to vector coordinates (will make them positive)
  int lastVectorX = convertPixelXToVectorX(lastX) + leftDisplayEdge;
  int lastVectorY = convertPixelYToVectorY(lastY);
  int currentVectorX = convertPixelXToVectorX(currentX) + leftDisplayEdge;
  int currentVectorY = convertPixelYToVectorY(currentY);

  // Make sure all the coordinates are within the board
  if (lastVectorX >= board.size() || lastVectorY >= getNumBlocksHigh() || currentVectorX >= board.size() || currentVectorY >= getNumBlocksHigh()) {
    return false;
  }

  // Make sure we aren't moving to the same vector location
  if (lastVectorX == currentVectorX && lastVectorY == currentVectorY) {
    return false;
  }

  // Make sure the old location is a wall
  if (board[lastVectorX][lastVectorY]->getBlockType() != WallBlock) {
    return false;
  }

  // Make sure we can move on top of the new location
  if (!board[currentVectorX][currentVectorY]->canMoveOnTop()) {
    return false;
  }

  // Make sure the player isn't at the current location
  if (player.getVectorX() == currentVectorX && player.getVectorY() == currentVectorY) {
    return false;
  }

  // Create the floor to replace the wall
  shared_ptr<Block> floor = make_shared<Floor>();

  // Grab the pointer to the wall
  shared_ptr<Block> wall = board[lastVectorX][lastVectorY];

  // Save the changes
  changes[lastVectorX][lastVectorY] = floor;
  changes[currentVectorX][currentVectorY] = wall;

  // Update the board
  board[lastVectorX][lastVectorY] = floor;
  board[currentVectorX][currentVectorY] = wall;

  return true;
}

/**
* Requires: positive pixelX and pixelY
* Modifies: board and changes
* Effects: Adds a wall to the current position if we can move on top, boolean
* return value signifies success
*/
bool GameBoard::addWall(int pixelX, int pixelY) {
  // Convert to vector coordinates (will make them positive)
  int vectorX = convertPixelXToVectorX(pixelX) + leftDisplayEdge;
  int vectorY = convertPixelYToVectorY(pixelY);

  // Make sure all the coordinates are within the board
  if (vectorX >= board.size() || vectorY >= getNumBlocksHigh()) {
    return false;
  }

  // Make sure we can move on top of the new location
  if (!board[vectorX][vectorY]->canMoveOnTop()) {
    return false;
  }

  // Make sure the player isn't at the current location
  if (player.getVectorX() == vectorX && player.getVectorY() == vectorY) {
    return false;
  }

  // Create the wall
  shared_ptr<Block> wall = make_shared<Wall>();

  // Save the changes
  changes[vectorX][vectorY] = wall;

  // Update the board
  board[vectorX][vectorY] = wall;

  return true;
}

/**
* Requires: positive pixelX and pixelY
* Modifies: board and changes
* Effects: Removes a wall at the current position, boolean return value
* signifies success
*/
bool GameBoard::removeWall(int pixelX, int pixelY) {
  // Convert to vector coordinates (will make them positive)
  int vectorX = convertPixelXToVectorX(pixelX) + leftDisplayEdge;
  int vectorY = convertPixelYToVectorY(pixelY);

  // Make sure all the coordinates are within the board
  if (vectorX >= board.size() || vectorY >= getNumBlocksHigh()) {
    return false;
  }

  // Make sure the block is a wall
  if (board[vectorX][vectorY]->getBlockType() != WallBlock) {
    return false;
  }

  // Create the floor
  shared_ptr<Block> floor = make_shared<Floor>();

  // Save the changes
  changes[vectorX][vectorY] = floor;

  // Update the board
  board[vectorX][vectorY] = floor;

  return true;
}

/**
* Requires: nothing
* Modifies: nothing
* Effects: Displays the board to the screen
*/
void GameBoard::display() const {
  // Loop through the board to draw each block
  for (int row = 0; row < numBlocksHigh; row++) {
    for (int column = 0; column < numBlocksWide; column++) {
      // Draw the block
      board[column + leftDisplayEdge][row]->draw(convertVectorXToPixelX(column), convertVectorYToPixelY(row), getBlockWidth(), getBlockHeight());
    }
  }

  // Draw the player (draw last so it is on top)
  player.draw(convertVectorXToPixelX(player.getVectorX() - leftDisplayEdge), convertVectorYToPixelY(player.getVectorY()), getBlockWidth(), getBlockHeight());
}

/**
* Requires: nothing
* Modifies: board
* Effects: generates the board based on the seed
*/
void GameBoard::generateBoard() {
  // Clear the current board
  board.clear();

  // Generate enough columns to display past the player and the right edge
  for (int column = 0; column < max(player.getVectorX(), leftDisplayEdge) + numBlocksWide + 1; column++) {
    generateColumn();
  }
}

/**
* Requires: nothing
* Modifies: board
* Effects: generates a new column for the board based on the seed
*/
void GameBoard::generateColumn() {
  // Create a new column
  board.push_back(vector<shared_ptr<Block>>());

  // The column number is the size of the board
  int column = board.size() - 1;

  // Add rows to the column
  for (int row = 0; row < numBlocksHigh; row++) {
    // If there is an existing block for here in the changes map, use it
    if (changes.find(column) != changes.end() && changes[column].find(row) != changes[column].end()) {
      board[column].push_back(changes[column][row]); // Add to the board
      dist(rand);                                    // Eat the random value for this location
    } else {
      // Randomly create a new block for the board
      if (dist(rand) <= percentWall) {
        board[column].push_back(make_shared<Wall>());
      } else {
        board[column].push_back(make_shared<Floor>());
      }
    }
  }
}
