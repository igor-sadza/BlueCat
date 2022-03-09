
// Arm Wrestling Flash game is designed to work with two inputs from two players.
// The game needs pecentage values of both signals sent to this Flash from Biograph.
// In order to set up this game you need a screen which is based on channel set 
// which has two Force Track sensors. Then create a screen that has two bargraphs that take
// outputs from these two sensors. Then place connection instrument on the same screen, 
// select TTFlashPlayer as target application and
// take percentage output from bargraphs(one for each connection). Recommended values for scale are 0(min) and 40(max).
// Then you can open screen for Recording , start TTFlashPlayer(make sure connection is established)
// , load ArmWrestling.swf and then start session. In order to start game in Flash, the main
// control bar must be within start sector. Whoever puts opponnents down first is the winner.
//