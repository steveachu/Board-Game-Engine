#ifndef __TICTACTOEBOARD_H
#define __TICTACTOEBOARD_H

#include "GameBoard.h"

const int TTT_BOARD = 3;
const int PLAYER_X = 1;
const int PLAYER_O = -1;

class TicTacToeBoard : public GameBoard {
   
public:
   enum Player {EMPTY = 0, PLAYER_X = 1, PLAYER_O = -1};
   
   TicTacToeBoard();
   
   virtual void GetPossibleMoves(std::vector<GameMove *> *list) const;
   virtual void ApplyMove(GameMove *move);
   virtual void UndoLastMove();
   virtual GameMove *CreateMove() const;
   virtual bool IsFinished() const;
   
   virtual std::string GetPlayerString(char player) {
      return (player == 1 ? "X" : "O");
   }
   
   inline static bool InBounds(int row, int col) {
      return row >= 0 && row < TTT_BOARD && col >= 0 && col <TTT_BOARD;
   }

   
private:
   friend class TicTacToeView;
   
   int mMoveCount = 0;
   char mBoard[TTT_BOARD][TTT_BOARD];
   bool CheckFinished(GameMove *move);
   bool mIsFinished = false;
};
#endif
