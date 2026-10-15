#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"

// x, y, width of the platforms
const byte PROGMEM wallSpec[4][3] = { { 0, 31, 55 }, { 72, 31, 55 }, { 40, 15, 47 }, { 40, 47, 47 } };

void stateGameInitLevel()
{
  scorePlayer = 0;
  //set starting  weapon type
  CURRENT_WEAPON = DEFAULT_GUN;
  weapon.weaponSet(0); //set to default gun specs
  crate.randomizeCrate();
  player.xSpeed = 1;
  player.ySpeed = 3;
  player.x = 24;
  player.y = 38;

  enemy[0].enemyType = 0; //set to big enemy
  enemy[1].enemyType = 1;
  enemy[2].enemyType = 1; //set to midium size enemy;

  for (byte i = 0; i < 8; i++)
  {
    enemy[i].reset();
    enemyWalkingFrame = 0;
  }

  topHole.width = 16;
  topHole.height = 4;
  topHole.x = 56;
  topHole.y = 0;

  bottomHole.width = 16;
  bottomHole.height = 4;
  bottomHole.x = 56;
  bottomHole.y = 63;

  // the four platforms: left, right, middle top, middle bottom
  for (byte i = 0; i < 4; i++)
  {
    wall[i].x = pgm_read_byte(&wallSpec[i][0]);
    wall[i].y = pgm_read_byte(&wallSpec[i][1]);
    wall[i].width = pgm_read_byte(&wallSpec[i][2]);
    wall[i].height = 2;
  }

  walkingFrame = 0;
  waitingFrame = 0;
  waitingTimer = 0;

  gameState = STATE_GAME_PLAYING;
}

void stateGamePlaying()
{
  checkInputs();
  gravityEffect();
  checkCrateCollisions();
  checkEnemyCollisions();
  updateEnemies();
  updateWeapon();

  drawBackDrop();
  drawPlayer();
  drawBullets();
  drawFirePit();
  drawEnemies();
  drawCrates();

  drawSurrounding();
  drawScore(98, 6, SCORE_SMALL_FONT);
  sprites.drawSelfMasked(11, 6, guns, CURRENT_WEAPON);
  if (gameState == STATE_GAME_OVER) delay(500);
};


void stateGamePause()
{
  arduboy.drawCompressed(0, 0, pause_bitmap, WHITE);
  if (arduboy.justPressed(UP_BUTTON)) { gameState = STATE_GAME_PLAYING; sfx(SFX_PAUSE); }
};


void stateGameOver()
{
  arduboy.drawCompressed(0, 0, gameover_bitmap, WHITE);
  drawScore(14, 32, SCORE_BIG_FONT);
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_MAIN;
};

#endif
