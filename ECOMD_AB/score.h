#ifndef SCORE_H
#define SCORE_H

//define score font size
#define SCORE_SMALL_FONT          0
#define SCORE_BIG_FONT            1

// the score with 5 digits, zero padded
void drawScore(byte scoreX, byte scoreY, byte fontType)
{
  unsigned int s = scorePlayer;
  for (byte i = 5; i--; s /= 10)
  {
    if (fontType == SCORE_BIG_FONT) sprites.drawSelfMasked(scoreX + 20 * i, scoreY, numbersBig, s % 10);
    else sprites.drawSelfMasked(scoreX + 4 * i, scoreY, numbersSmall, s % 10);
  }
}

#endif
