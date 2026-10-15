#ifndef CRATE_H
#define CRATE_H

extern int scorePlayer;

PROGMEM const unsigned char crate_plus_mask[] = {
  // width, height
  8, 8,
  0x77, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0x75, 0xFF, 0x75, 0xFF,
  0x77, 0xFF, 0xFF, 0xFF, 0x77, 0xFF,
};


// the 8 places a crate can appear (x, y)
const byte PROGMEM crateLoc[8][2] = { { 47, 7 }, { 74, 7 }, { 10, 23 }, { 39, 23 }, { 82, 23 }, { 110, 23 }, { 60, 39 }, { 10, 55 } };

class Crate
{
  public:
    byte weapon;
    byte x, y;

    void randomizeCrate()
    {
      byte oldWeapon = weapon;
      while (weapon == oldWeapon) weapon = random(0, 6);
      byte oldx = x;
      byte oldy = y;
      while (x == oldx && y == oldy)
      {
        byte pos = random(0, 8);
        x = pgm_read_byte(&crateLoc[pos][0]);
        y = pgm_read_byte(&crateLoc[pos][1]);
      }
    }
};

Crate crate;


void checkCrateCollisions()
{
  Rect crateRect;
  Rect playerRect;

  playerRect.x = player.x;
  playerRect.y = player.y;
  playerRect.width = 8;
  playerRect.height = 8;

  crateRect.x = crate.x;
  crateRect.y = crate.y;
  crateRect.width = 8;
  crateRect.height = 8;

  if (arduboy.collide(playerRect, crateRect)) {
    scorePlayer += 100;
    sfx(SFX_CRATE);
    CURRENT_WEAPON = (WEAPON_TYPE)crate.weapon;
    weapon.weaponSet(crate.weapon);
    crate.randomizeCrate();
  }
}


void drawCrates()
{
  sprites.drawPlusMask(crate.x, crate.y, crate_plus_mask, 0);
}

#endif
