#include "TicTacToeMove.h"
#include "TicTacToeBoard.h"
#include "GameExceptions.h"
#include <sstream>

using namespace std;

TicTacToeMove::TicTacToeMove() : mRow(0), mCol(0) {
   
}

TicTacToeMove::TicTacToeMove(int row, int col) : mRow(row), mCol(col) {
   
}

TicTacToeMove::TicTacToeMove(const TicTacToeMove &other) : mRow(other.mRow), mCol(other.mCol) {

}

TicTacToeMove& TicTacToeMove::operator=(const TicTacToeMove &rhs) {
   mRow = rhs.mRow;
   mCol = rhs.mCol;
   return *this;
}

GameMove& TicTacToeMove::operator=(const std::string &rhs) {
   istringstream is(rhs);
   char temp;
   is >> temp >> mRow >> temp >> mCol >> temp;
   
   if (!TicTacToeBoard::InBounds(mRow, mCol)) {
      throw TicTacToeException("Invalid move!");
   }
   
   return *this;
}

bool TicTacToeMove::Equals(const GameMove &other) const {
   return (mRow == ((TicTacToeMove&)other).mRow) && (mCol == ((TicTacToeMove&)other).mCol);
}

TicTacToeMove::operator std::string() const {
   ostringstream s;
   s << "(" << mRow << ", " << mCol << ")";
   return s.str();
}