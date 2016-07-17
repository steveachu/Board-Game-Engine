#include "TicTacToeView.h"
#include <sstream>
using namespace std;

void TicTacToeView::PrintBoard(ostream &s) const {
   // print the top row (column names), is the same each print
   s << "- ";
   for (int i = 0; i < TTT_BOARD; i++) {
      s << i << " ";
   }
   
   s << endl;
   
   // print the row names and board state, row by row
   for (int row = 0; row < TTT_BOARD; row++) {
      s << row << " ";
      
      for (int col = 0; col < TTT_BOARD; col++) {
         if (mTicTacToeBoard->mBoard[row][col] == EMPTY) {
            s << ". ";
         }
         else if (mTicTacToeBoard->mBoard[row][col] == PLAYER_O) {
            s << "O ";
         }
         else if (mTicTacToeBoard->mBoard[row][col] == PLAYER_X) {
            s << "X ";
         }
      }
      s << endl;
   }
}
