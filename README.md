# Arcanoid-test-task

## Instruction to use the game:

** left and right arrow is used to navigate the player.

** left mouse button and cursor used to navigate the ball. It will shoot the ball depending how far u dragged the cursor from the ball (NOTE: for comfortable gameplay drag cursor not further than half of game screen).

** right mouse button can buy you a boost if the cursor is located over buy boost button.

** press the down button arrow to restart the game.
 
## Requirements from pdf: 

** It is required to use our framework. We do not accept solutions created using other libraries (like SFML, SDL2 etc). Functionality provided by Framework should be enough to do all required drawing. You can load TGA, JPG and PNG images as sprites with it. **
All work was completed on Framework.lib.

** Window size should be possible to set from the command-line, e.g.: game -window 800x600 **
> All sprite size is calculated depending on window size that is indicated in the constructor. Work also was tested on 1920x1080 and with fullscreen mode (NOTE if window size changed please change also speed for player).

** The player can move a platform using left and right arrow keyboard buttons. **
> Player movement is left and right arrow.

** Initially the player can choose the angle of the ball by pressing the left mouse button (the ball should be launched in the direction of the mouse cursor). **
> Player can choose angles depending on the current cursor position. Farther the cursor is dragged the more speed the ball will obtain.

** The ball moves linearly with a constant speed. It reflects from edges, blocks and the player's platform. **
> Bounce() and OnHitBall() are in charge for ball being reflected from edges (invisible game borders) and blocks. Also in the Bounce() method the ball can be reflected from the player platform.

** The level consists of blocks, which could be destroyed by a single hit. The ball should reflect from blocks after their destruction. **
> Also in task 2 we made sure that different blocks have different hit points.

** The player loses when the ball is not caught by the platform. Victory can be achieved by destroying all blocks and catching the ball with the platform after. The game should be restarted after victory/loss. **
> Every time the player loses his ball, he is taken away from one of his hearts. When there is no heart left, the player is being told that he lost and he can't move the ball. If there are no blocks left (not including indestructible one) the player will be alerted that he won. He can restart the game with the down button arrow. 

<sub> 7/7 </sub>

## Tasks:

** The game should calculate a score (number of destroyed blocks). Destroying 3 blocks in a row should give an “x3” combo for every next destroyed block in a row. “In a row” means that the ball was not touching the player’s platform. **
> Combo is variable to count player combo score. Every time the player destroys a block he will obtain +1 combo point if he will obtain 3 of them every time he will obtain another score point it will triple its amount.

** Different types of blocks have different number of hit points. White blocks are destroyed from the first hit, Green ones - from the second, Gold blocks are indestructible. You can use any colors which you want. **
> As mentioned in requirement 6 we have 4 different blocks 2 of them with one hit point (cyan and blue) 1 with 2 hit points (lime) and 1 is indestructible (gold).

** The first ability (positive) increases and the second one (negative) decreases the speed of the platform by 40%. Ability stays active for 20 seconds. Abilities are “stackable”, it means that it is possible to have multiple active instances of the same ability. Minimum speed is 50%, maximum is 200% of a default speed. **
> Player can stack his speed to 200% and can catch a debuff that will stack till 50% of the default speed.

** Abilities cost 20 points. The player can buy an ability on the right mouse button. Points can be obtained by destroying blocks. A random ability is chosen and has an immediate impact. **
> Player can buy an ability in the buy section; they need to drag a mouse to buy button and press the right button as mentioned. A random buff/debuff will be implemented immediately.

<sub> 4/4 </sub>

## About some of mine decisions:

** Created ball and player as a structure to save all of the needed data in one place .

** Booster time is created as a clock vector to store multiple data (4 stacks) . We also check the front element of the vector in order to get the earliest info about boost .

** In the DrawBall() method memory leaks in order to slow down the program, because at normal speed it is impossible to play.

** In DrawScoreboard() we separate digits of score in order to draw it on a screen.

** In the Bounce() method we calculate if our ball hits an edge or platform (hitBoxes) if it is, the ball will go the opposite direction.

** OnHitBall() method is not the best or most beautiful solution, because we go through a loop in order to find if there is collision with the ball and a brick.
