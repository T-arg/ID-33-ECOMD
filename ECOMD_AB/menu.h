#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "globals.h"

void stateMenuIntro()
{
  if (!arduboyIntro()) return;
  globalCounter++;
  sprites.drawSelfMasked(49, 20, T_arg, 0);
  drawTwinkles(49, 20, globalCounter);
  if (globalCounter > 180)
  {
    globalCounter = 0;
    gameState = STATE_MENU_MAIN;
  }
};

void stateMenuMain()
{
  arduboy.drawCompressed(0, 0, title_bitmap, WHITE);
  sprites.drawSelfMasked(20, 56, mainmenu_is_mask, menuSelection - 3);        // PLAY INFO CONF JBOX, in the order of the menu states
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < STATE_MENU_JBOX)) { menuSelection++; sfx(SFX_MOVE); }
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > STATE_MENU_PLAY)) { menuSelection--; sfx(SFX_MOVE); }
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) { gameState = menuSelection; sfx(SFX_SELECT); }
}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuInfo()
{
  arduboy.drawCompressed(20, 0, info_bitmap, WHITE);
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  arduboy.drawCompressed(0, 0, title_bitmap, WHITE);
  sprites.drawSelfMasked(22, 56, soundYesNo_is_mask, arduboy.audio.enabled());
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}


void stateMenuPlay()
{
  musicPickGameTune();                         // a different tune every game
  gameState = STATE_GAME_INIT_LEVEL;
}

// the jukebox: MUSIC (UP) and SNDFX (DOWN), LEFT = next number, RIGHT = previous number, B = play, A = stop (and leave when nothing plays)
byte jbRow = 0;
byte jbNr[2];                                  // MUSIC 0..6, SNDFX 0..12

// little arrows around the number at x, y (w x h pixels): v = above and below (UP / DOWN changes it), else left and right (LEFT / RIGHT changes it)
void jbArrows(byte x, byte y, byte w, byte h, bool v)
{
  byte cx = x + w / 2, cy = y + h / 2;
  if (v)
  {
    arduboy.fillRect(cx - 2, y - 5, 5, 4, BLACK);          // black mask behind the top arrow: 5 wide, starts 2 pixels above it
    arduboy.drawPixel(cx, y - 3, WHITE);
    arduboy.drawFastHLine(cx - 1, y - 2, 3, WHITE);
    arduboy.drawFastHLine(cx - 1, y + h + 1, 3, WHITE);
    arduboy.drawPixel(cx, y + h + 2, WHITE);
  }
  else
  {
    arduboy.drawPixel(x - 3, cy, WHITE);
    arduboy.drawFastVLine(x - 2, cy - 1, 3, WHITE);
    arduboy.drawFastVLine(x + w + 1, cy - 1, 3, WHITE);
    arduboy.drawPixel(x + w + 2, cy, WHITE);
  }
}

void stateMenuJbox()
{
  arduboy.drawCompressed(0, 0, title_bitmap, WHITE);
  for (byte i = 0; i < 2; i++)
  {
    byte x = 22 + 48 * i;
    jbArrows(x + 29, 56, 7, 5, true);
    sprites.drawSelfMasked(x, 55, jboxWords, 2 * i + (jbRow != i));            // the selected word is the lit one
    if (jbRow != i || (arduboy.frameCount & 16))                               // the selected number blinks
    {
      sprites.drawSelfMasked(x + 29, 56, numbersSmall, (jbNr[i] + 1) / 10);
      sprites.drawSelfMasked(x + 33, 56, numbersSmall, (jbNr[i] + 1) % 10);
    }
  }
  byte n = jbRow ? JBOX_SFX : JBOX_MUSIC;
  if (arduboy.justPressed(LEFT_BUTTON)) jbRow = 0;
  if (arduboy.justPressed(RIGHT_BUTTON)) jbRow = 1;
  if (arduboy.justPressed(UP_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + 1) % n;
  if (arduboy.justPressed(DOWN_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + n - 1) % n;
  if (arduboy.justPressed(B_BUTTON) && arduboy.audio.enabled())
  {
    if (jbRow) sfx(jbNr[1]);
    else musicPlay(jbNr[0]);
  }
  if (arduboy.justPressed(A_BUTTON))
  {
    if (musicId == MUSIC_OFF) gameState = STATE_MENU_MAIN;    // nothing playing: leave, the title tune restarts
    else musicPlay(MUSIC_OFF);
  }
}




#endif
