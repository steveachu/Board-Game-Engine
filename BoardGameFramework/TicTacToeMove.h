#ifndef __TICTACTOEMOVE_H
#define __TICTACTOEMOVE_H

#include "GameMove.h"

class TicTacToeMove : public GameMove {
private:
   friend class TicTacToeBoard;
   
   int mRow, mCol;
   
   TicTacToeMove(int row, int col);
   
public:
   TicTacToeMove();
   TicTacToeMove(const TicTacToeMove &);
   TicTacToeMove& operator=(const TicTacToeMove &rhs);
   virtual GameMove& operator=(const std::string &);
   
   virtual bool Equals(const GameMove &other) const;
   virtual operator std::string() const;
};

#endif
