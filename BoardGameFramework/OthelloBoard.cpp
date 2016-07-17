#include "OthelloBoard.h"
#include "GameExceptions.h"
#include <vector>
#include <iostream>

using namespace std;

OthelloBoard::OthelloBoard() : GameBoard(), mBoard() {
   mBoard[INITIAL_PIECE][INITIAL_PIECE] = WHITE;
   mBoard[INITIAL_PIECE2][INITIAL_PIECE2] = WHITE;
   mBoard[INITIAL_PIECE][INITIAL_PIECE2] = BLACK;
   mBoard[INITIAL_PIECE2][INITIAL_PIECE] = BLACK;
}

void OthelloBoard::ApplyMove(GameMove *move) {
   OthelloMove* m = (OthelloMove*)move;
   
   if (m->IsPass()) {
      mPassCount++;
      mNextPlayer *= SWITCH_PLAYER;
   }
   else {
      // Place piece on the board, update score.
      // Reset the pass count.
      mBoard[m->mRow][m->mCol] = mNextPlayer;
      mNextPlayer == BLACK ? ++mValue : --mValue;
      mPassCount = 0;
      
      for (int cDelta = CHECK_DIRECTION; cDelta <= 1; cDelta++) {
         for (int rDelta = CHECK_DIRECTION; rDelta <= 1; rDelta++) {
            int curRow = m->mRow + rDelta;
            int curCol = m->mCol + cDelta;
            int enemyCount = 0;
            
            while (true) {
               // If the space is not in bounds or empty, then change direction.
               if (!InBounds(curCol, curRow) ||
                   mBoard[curRow][curCol] == EMPTY) {
                  break;
               }
               
               // Enemy found, so continue in the same direction.
               // While loop allows us to step without changing direction.
               else if (mBoard[curRow][curCol] == mNextPlayer * SWITCH_PLAYER) {
                  enemyCount++;
                  curRow += rDelta;
                  curCol += cDelta;
               }
               
               else if (mBoard[curRow][curCol] == mNextPlayer) {
                  // The move will be applied in this case.
                  if (enemyCount > 0) {
                     // Create a FlipSet for the move.
                     // Records amount of enemies that will be flipped
                     // and direction while flipping.
                     // Push the FlipSet onto the FlipSet for the move.
                     m->AddFlipSet
                     (OthelloMove::FlipSet(enemyCount, rDelta, cDelta));
                  }
                  
                  // Friendly unit found with enemies in between.
                  // Move one step backwards, change piece to player's color.
                  // Repeat for number of enemies found.
                  while (enemyCount > 0) {
                     if (mNextPlayer == BLACK) {
                        mValue += FLIP_VALUE;
                     }
                     else if (mNextPlayer == WHITE) {
                        mValue -= FLIP_VALUE;
                     }
                     
                     // Convert the single enemy into friendly color.
                     mBoard[curRow -= rDelta][curCol -= cDelta] = mNextPlayer;
                     enemyCount--;
                  }
                  // Change directions after applying this move.
                  break;
               }
               // Any other case is invalid, switch directions.
               else {
                  break;
               }
            }
         }
      }
      // Push the move onto move history.
      mHistory.push_back(m); // should i push back a game move?
      // Switch the player (changing the turn).
      mNextPlayer *= SWITCH_PLAYER;
   }
}

void OthelloBoard::GetPossibleMoves(std::vector<GameMove *> *list) const {
   // Loop through every square on the board.
   // On every square, loop through all 8 directions.
   for (int row = 0; row < BOARD_SIZE; row++) {
      for (int col = 0; col < BOARD_SIZE; col++) {
         if (mBoard[row][col] == EMPTY && InBounds(row, col)) {
            for (int rDirection = CHECK_DIRECTION; rDirection <= 1;
                 rDirection++) {
               for (int cDirection = CHECK_DIRECTION; cDirection <= 1;
                    cDirection++) {
                  int nextRow = row + rDirection;
                  int nextCol = col + cDirection;
                  int enemyCount = 0;
                  
                  while (true) {
                     // If stepping in the direction will be out of bounds or
                     // is empty, then just break to check a new direction.
                     if (!InBounds(nextRow, nextCol) ||
                         mBoard[nextRow][nextCol] == EMPTY) {
                        break;
                     }
                     
                     else if (mBoard[nextRow][nextCol] ==
                              mNextPlayer * SWITCH_PLAYER) {
                        enemyCount++;
                        nextRow += rDirection;
                        nextCol += cDirection;
                     }
                     
                     else if (mBoard[nextRow][nextCol] == mNextPlayer) {
                        if (enemyCount > 0) {
                           // Create move and push onto move list.
                           GameMove *move = CreateMove();
                           list->push_back(move);
                           
                           OthelloMove* m = (OthelloMove*)move;
                           
                           m->mRow = row;
                           m->mCol = col;
                           
                           // Check the pushed move to see if it is a repeat
                           // Use iterator to cycle through list
                           // If repeat move is found, pop the pushed move
                           // and then exit the loop.
                           for (vector<GameMove *>::iterator itr
                                = list->begin();
                                itr != list->end()-1; itr++) {
                              if (**itr == *(list->back())) {
                                 delete list->back();
                                 list->pop_back();
                                 break;
                              }
                           }
                        }
                        break; // break for new direction
                     }
                     else { // empty space
                        break;
                     }
                  }
               }
            }
         }
      }
   }
   
   if (list->empty()) {
      // No possible moves, push in a pass.
      list -> push_back(CreateMove());
      ((OthelloMove*)(list -> back())) -> mRow = PASS;
      ((OthelloMove*)(list -> back())) -> mCol = PASS;
   }
}

void OthelloBoard::UndoLastMove() {
   // Get the most recent move.
   OthelloMove *lastMove = (OthelloMove*)(GetMoveHistory()->back());
   int lastMoveRow = lastMove->mRow;
   int lastMoveCol = lastMove->mCol;
   
   // If the move was a pass, then reset the pass counter.
   if (lastMove->IsPass()) {
      mPassCount = 0;
   }
   else {
      // Remove the last piece placed on the board.
      mBoard[lastMoveRow][lastMoveCol] = EMPTY;
      mNextPlayer == WHITE ? --mValue : ++mValue;
      // Go through each FlipSet of the lastMove
      // (since one move can have more than
      // one directional flips, a move can have more than one FlipSet).
      for (OthelloMove::FlipSet fs : lastMove->mFlips) {
         for (int i = 0; i < fs.switched; i++) {
            lastMoveRow = lastMove->mRow;
            lastMoveCol = lastMove->mCol;
            // Step one tile in a direction from last move's (r,c)
            // Convert the piece to the color of last player (aka nextPlayer).
            mBoard[lastMoveRow += fs.rowDelta][lastMoveCol += fs.colDelta]
            = mNextPlayer;
            
            if (mNextPlayer == BLACK) {
               mValue += (FLIP_VALUE);
            }
            else if (mNextPlayer == WHITE) {
               mValue -= (FLIP_VALUE);
            }
         }
      }
      
      mHistory.pop_back();
      delete lastMove;
   }
   mNextPlayer *= SWITCH_PLAYER;
}