#include "OthelloView.h"

using namespace std;

void OthelloView::PrintBoard(ostream &s) const{
   s << "- ";
   for (int i = 0; i < BOARD_SIZE; i++) {
      s << i << " ";
   }
   
   s << endl;
   
   for (int row = 0; row < BOARD_SIZE; row++) {
      s << row << " ";
      
      for (int col = 0; col < BOARD_SIZE; col++) {
         if (mOthelloBoard->mBoard[row][col] ==
             OthelloBoard::Player::EMPTY) {
            s << ". ";
         }
         else if (mOthelloBoard->mBoard[row][col] ==
                  OthelloBoard::Player::BLACK) {
            s << "B ";
         }
         else if (mOthelloBoard->mBoard[row][col] ==
                  OthelloBoard::Player::WHITE) {
            s << "W ";
         }
      }
      s << endl;
   }
}