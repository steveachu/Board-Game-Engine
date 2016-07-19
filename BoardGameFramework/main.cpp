/*
Written by Steve Hong

TODO LIST:
-Error checking for main menu (COMPLETED 7/19/2016)
-Add a way to exit a game back to the main menu
-Modularize the main menu a bit?
-Add AI using min-max algorithm
*/

#include "GameExceptions.h"
#include "OthelloBoard.h"
#include "OthelloView.h"
#include "OthelloMove.h"
#include "TicTacToeBoard.h"
#include "TicTacToeMove.h"
#include "TicTacToeView.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const int COMMAND_LENGTH = 5;
const int PLAYER_1 = 1;
const int PLAYER_2 = -1;
const int OTHELLO = 1;
const int TICTACTOE = 2;

int main(int argc, char* argv[]) {
   // Initialization
   GameBoard *board = NULL; // The state of the game board.
   GameView *v = NULL; // A View for outputting the board via operator<<
   string userInput; // a string to hold the user's command choice
   vector<GameMove *> possMoves; // a holder for possible moves
   
   int gameChoice;

   do {
	   // Main menu options
	   cout << "What game do you want to play?" << endl;
	   cout << "(1) Othello" << endl;
	   cout << "(2) Tic-Tac-Toe" << endl;

	   // This while loop block of code is used to ignore characters entered into the selection menu
	   // It also clears the cin stream
	   while (!(cin >> gameChoice)) {
		   cout << "Invalid choice. Please select a game from the list. \n";
		   cin.clear();

		   // This line ignores input up until '\n' or EOF (whichever comes first)
		   // '\n' is the delimiter -- the character after which cin stops ignoring
		   // numeric_limits<streamsize>::max() sets the max number of characters to ignore.
		   // This is the upper limit on the size of a stream, so this line there is no limit of chars to ignore
		   cin.ignore(numeric_limits<streamsize>::max(), '\n');

		   // Main menu options
		   cout << "What game do you want to play?" << endl;
		   cout << "(1) Othello" << endl;
		   cout << "(2) Tic-Tac-Toe" << endl;
	   }

	   // Decide which game to play
	   if (gameChoice < 0) {
		   cout << "Invalid selection. Please select a game from the list." << endl;
		   gameChoice = -1; // Reset game choice to loop again
	   }
	   else if (gameChoice == OTHELLO) {
		   board = new OthelloBoard();
		   v = new OthelloView(board);
	   }
	   else if (gameChoice == TICTACTOE) {
		   board = new TicTacToeBoard();
		   v = new TicTacToeView(board);
	   }
	   else {
		   cout << "Invalid choice. Please select a game from the list." << endl;
		   gameChoice = -1; // Reset game choice to loop again
	   }
   } while (gameChoice < 0); // Game will keep asking for input until a valid choice is selected.


	// Print the game board.
	cout << *v;
   // Main game loop
   do {
      // Print all possible moves
      cout << "Possible moves: " << endl;
      board->GetPossibleMoves(&possMoves);
      for (GameMove *move : possMoves) {
         cout << (string)*move << " ";
      }
      
      cout << endl;
      
      for (GameMove *move : possMoves) {
         delete move;
      }
      possMoves.clear();
      
      // Print the player's turn
      cout << endl << (board->GetNextPlayer() == 1 ?
       board->GetPlayerString(PLAYER_1) + "'s turn. " :
       board->GetPlayerString(PLAYER_2) + "'s turn. ");
      
      // Get command from user
      cout << "Enter a command: ";
      
      getline(cin, userInput);
      GameMove *move = nullptr;
      try {
         // Command: move (r,c)
         if (userInput.find("move") != string::npos) {
            
            // Parse the user's input as a command.
            // Gets input after COMMAND_LENGTH indices. Since "undo" and "move"
            // are both 4 characters, we make COMMAND_LENGTH length of 5
            // in order to account for the whitespace after. userInput will then
            // be equal to an OthelloMove in string form.
            userInput = userInput.substr(COMMAND_LENGTH);
            
            // Create the move with the string we parsed.
            move = board->CreateMove();
            
            *move = userInput; // May throw an exception
            
            // See if the move is in GetPossibleMoves
            board->GetPossibleMoves(&possMoves);
            for (GameMove *m : possMoves) {
               // Going through the list of moves
               // If user move is equal to a move in GetPossibleMoves
               // Then apply the move because it is a valid move.
               // Or if my move is a pass, then apply the move.
               // Then break out of the loop, stop checking moves.
               if (*move == *m) {
                  board->ApplyMove(move);
                  break;
               }
               // If we're at the last element of the vector then
               // the user move doesn't exist in GetPossibleMoves.
               // At this point, we already checked if the last element
               // is valid or not.
               else if (m == possMoves.back()){
                  throw OthelloException("Invalid move. \n");
               }
            }
            
            // Delete and clear the vector of possible moves.
            for (GameMove *m : possMoves) {
               delete m;
            }
            possMoves.clear();
         }
         // Command: undo n
         else if (userInput.find("undo") != string::npos) {
            // Parses "undo n" to grab only "n". Splits it at first whitespace.
            // Then convert the string into an integer.
            userInput = userInput.substr(userInput.find(" ", 0));
            int undoAmount = stoi(userInput);
            
            // If user tries to undo more moves than exists
            // Then just reset the board to initial state
            // Do this by limiting the undoAmount by move count.
            if (undoAmount > board->GetMoveCount()) {
               undoAmount = board->GetMoveCount();
            }
            
            // Undo the moves undoAmount times
            for (int i = 0; i < undoAmount; i++) {
               board->UndoLastMove();
            }
         }
         // Command: showValue
         else if (userInput == "showValue") {
            cout << "Board value: " << board->GetValue() << endl;
         }
         // Command: showHistory
         else if (userInput == "showHistory") {
            // Show move history in reverse order (most recent first).
			 // Moves are extracted out of the vector of GameMoves using an iterator
            char prevTurn = board->GetNextPlayer() * SWITCH_PLAYER;
            for (vector<GameMove *>::const_reverse_iterator rit =
                 board->GetMoveHistory()->rbegin();
                 rit != board->GetMoveHistory()->rend(); rit++) {
               
               cout << (prevTurn == PLAYER_1 ?
                board->GetPlayerString(PLAYER_1) + ": " :
                board->GetPlayerString(PLAYER_2) + ": ");
               cout << (string)**rit << endl;
               
               prevTurn *= SWITCH_PLAYER;
            }
         }
		 else if (userInput == "help") {
			 cout << "Valid commands:" << endl;
			 cout << "move (x, y) -- Place a piece at coordinate (x, y)" << endl;
			 cout << "undo x -- Undoes the game board state x amount of turns." << endl;
			 cout << "showHistory -- Shows the history of moves made, with the most recent move on top." << endl;
			 cout << "showValue -- Shows the value of pieces on the game board. (Possibly used to implement AI in the future)" << endl;
			 cout << "help -- Displays the list of valid commands." << endl;
		 }
         // Command: quit
         else if (userInput == "quit") {
            cout << "Good-bye." << endl;
            exit(1);
         }
         // Command: Invalid command.
         else {
            throw GameException("Invalid move! Please enter a valid move, or type \"help\" for a list of commands. \n");
         }
         
         // Print the game board.
         cout << *v;
      }
      catch (GameException &exc) {
         // Delete the move if it was an invalid move.
         delete move;
         cout << exc.what();
      }
   } while (!board->IsFinished());
   
   if (board->GetValue() > 0) {
      cout << board->GetPlayerString(PLAYER_1) << " wins!" << endl;
   }
   else if (board->GetValue() < 0) {
      cout << board->GetPlayerString(PLAYER_2) << " wins!" << endl;
   }
   else {
      cout << "Tie game!" << endl;
   }
}