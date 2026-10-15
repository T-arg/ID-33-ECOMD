/*
 EPIC CRATES OF MASS DESTRUCTION : https://t-arg.github.io/ID-33-ECOMD/

 Arduboy version 1.1:  https://github.com/T-arg/ID-33-ECOMD

 MADE by TEAM a.r.g. : https://t-arg.github.io/

 2015 - DRAGULA96 - JO3RI  (Firepit animation by @JUSTIN_CYR)

 Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 33

#include "globals.h"
#include "Rects.h"
#include "Player.h"
#include "Weapons.h"
#include "Crate.h"
#include "Enemy.h"
#include "score.h"
#include "inputs.h"
#include "stage.h"
#include "menu.h"
#include "game.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateMenuJbox,
  stateGameInitLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};


void setup()
{
  arduboy.boot();
  arduboy.audio.begin();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  musicUpdate();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}
