#include "OthelloMove.h"
#include "GameExceptions.h"
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

int OthelloMove::mOutstanding = 0;

// Since GameMove is an abstract class.
// Do not initialize it in constructor.
OthelloMove::OthelloMove() : mRow(DEFAULT_MOVE), mCol(DEFAULT_MOVE) {
   
}

OthelloMove::OthelloMove(int row, int col) : mRow(row), mCol(col) {
   
}

OthelloMove::OthelloMove(const OthelloMove& other) :
mRow(other.mRow), mCol(other.mCol) {
   
}

OthelloMove& OthelloMove::operator=(const OthelloMove& rhs) {
   mRow = rhs.mRow;
   mCol = rhs.mCol;
   
   // Copy the FlipSet as well.
   for (int i = 0; i < rhs.mFlips.size(); i++) {
      AddFlipSet(rhs.mFlips.at(i));
   }
   return *this;
}

OthelloMove::operator std::string() const {
   ostringstream s;
   s << "(" << mRow << ", " << mCol << ")";
   return IsPass() ? "pass" : s.str();
}

GameMove& OthelloMove::operator=(const string& rhs) {
   if (rhs == "pass") {
      mRow = PASS;
      mCol = PASS;
   }
   else if (mRow >= BOARD_SIZE || mRow < 0 || mCol >= BOARD_SIZE || mCol < 0) {
      throw OthelloException("Invalid move! \n");
   }
   else {
      // Open cin like input stream
      // Using temp as a dummy variable for input
      istringstream is(rhs);
      char temp;
      is >> temp >> mRow >> temp >> mCol >> temp;
   }
   return *this;
}

bool OthelloMove::Equals(const GameMove &other) const {
   return (mRow == ((OthelloMove&)other).mRow) && (mCol == ((OthelloMove&)other).mCol);
}