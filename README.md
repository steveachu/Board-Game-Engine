# BoardGameFramework
A lightweight framework for 2-player board games written in C++.
<br><br>
Uses the Model-View-Control pattern to organize code.
<br><br>
Two games are already implemented using the pattern as examples: <b>Othello</b> (also known as Reversi), and <b>Tic-Tac-Toe</b>. 
<br>
The games are seletable through a text menu.
<br><br>
<center><img src = http://i.imgur.com/SLXHUro.png>
<br>
<i>Pictured Above: Running Othello from the framework</i></center>
<br><br>
The UI is all text-based through the console. These are the list of valid commands:

<b>NOTE:</b> "x" and "y" are variables and must be integers for the move to be accepted.

<ul>
  <li>move (x, y) -- Places a game piece at the specified coordinate.</li>
  <li>undo x -- Undo the game board x amount of turns.</li>
  <li>showValue -- Shows the total value of pieces on the board.</li>
  <li>showHistory -- Shows the history of moves made in the current game (most recent move first).</li>
  <li>quit -- Quit the game.</li>
  <li>help -- Display list of valid commands.</li>
</ul>
<br>
<b>Developers</b>
<br>
Any game that uses this framework must inherit from the base classes. The base classes start with "Game", such as "GameBoard.cpp" and "GameView.cpp".
<br>
It is important to note that this framework follows the Model-View-Controller pattern to separate code. The model is the GameBoard, the view is the GameView, and the controller is the main class.
