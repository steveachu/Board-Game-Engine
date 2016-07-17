#include "TicTacToeBoard.h"
#include "TicTacToeMove.h"
#include <iostream>
#include <vector>

using namespace std;

TicTacToeBoard::TicTacToeBoard() : GameBoard(), mBoard{0} {
   
}

void TicTacToeBoard::GetPossibleMoves(std::vector<GameMove *> *list) const {
   for (int row = 0; row < TTT_BOARD; row++) {
      for (int col = 0; col < TTT_BOARD; col++) {
         if (mBoard[row][col] == EMPTY) {
            GameMove *move = CreateMove();
            list->push_back(move);
            
            TicTacToeMove* m = (TicTacToeMove*)move;
            
            m->mRow = row;
            m->mCol = col;
            
            // Check the pushed move to see if it is a repeat
            // Use iterator to cycle through list
            // If repeat move is found, pop the pushed move
            // and then exit the loop.
            for (vector<GameMove *>::iterator itr = list->begin();
                 itr != list->end()-1; itr++) {
               if (**itr == *(list->back())) {
                  delete list->back();
                  list->pop_back();
                  break;
               }
            }
         }
      }
   }
}

void TicTacToeBoard::ApplyMove(GameMove *move) {
   TicTacToeMove *m = (TicTacToeMove*)move;
   mBoard[m->mRow][m->mCol] = mNextPlayer;
   mIsFinished = CheckFinished(move);
   mNextPlayer *= SWITCH_PLAYER;
   mHistory.push_back(m);
}

void TicTacToeBoard::UndoLastMove() {
   TicTacToeMove *m = (TicTacToeMove*)(GetMoveHistory()->back());
   mBoard[m->mRow][m->mCol] = EMPTY;
   
   mHistory.pop_back();
   delete m;
   
   mNextPlayer *= SWITCH_PLAYER;
}

GameMove* TicTacToeBoard::CreateMove() const {
   return new TicTacToeMove;
}

bool TicTacToeBoard::CheckFinished(GameMove *move) {
   ++mMoveCount;
   TicTacToeMove *m = (TicTacToeMove*)move;
   
   // Horizontal
   for (int i = 0; i < TTT_BOARD; i++) {
      if (mBoard[m->mRow][i] != mNextPlayer) {
         break;
      }
      if (i == TTT_BOARD - 1) {
         mValue = mNextPlayer;
         return true;
      }
   }

   // Vertical
   for (int i = 0; i < TTT_BOARD; i++) {
      if (mBoard[i][m->mCol] != mNextPlayer) {
         break;
      }
      if (i == TTT_BOARD - 1) {
         mValue = mNextPlayer;
         return true;
      }
   }
   
   if (m->mRow == m->mCol) {
      // Diagnol
      for (int i = 0; i < TTT_BOARD; i++) {
         if (mBoard[i][i] != mNextPlayer) {
            break;
         }
         if (i == TTT_BOARD - 1) {
            mValue = mNextPlayer;
            return true;
         }
      }
      
      // Anti Diagnol
      for (int i = 0; i < TTT_BOARD; i++) {
         if (mBoard[i][(TTT_BOARD - 1) - i] != mNextPlayer) {
            break;
         }
         if (i == TTT_BOARD - 1) {
            mValue = mNextPlayer;
            return true;
         }
      }
   }
   
   // Draw if all moves taken and a winner was not found.
   if (mMoveCount == (TTT_BOARD * TTT_BOARD)) {
      return true;
   }
   
   return false;
}


bool TicTacToeBoard::IsFinished() const {
   return mIsFinished;
}