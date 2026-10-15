#ifndef WEAPONS_H
#define WEAPONS_H

#define POWERUP_MAXTIME     600
enum WEAPON_TYPE { DEFAULT_GUN = 0, SHOTGUN, MAGNUM, MACHINEGUN, FLAMETHROWER, DUAL_PISTOLS};
WEAPON_TYPE CURRENT_WEAPON;

class Bullet
{
  public:
    byte x;
    byte y;
    byte width = 4;
    byte height = 1;
    byte fuse = 0;
    byte xSpeed = 3;
    byte ySpeed = 0;
    byte damage = 1;
    boolean isActive = false;

    void Update() {
      if (isActive) {
        fuse--; if (fuse == 0) isActive = false;
        x += xSpeed;
        y += ySpeed;
      }
    }
};

// per weapon: frameDelay, maxFuse, maxCooldown, kickBack, width, height, xSpeed, xSpeedOffset, yOffset, ySpeedOffset, damage
const byte PROGMEM weaponSpecs[6][11] = {
  { 1, 30,  3, 0,  4, 2, 5, 0, 0, 0, 2 },   // DEFAULT_GUN: faster bullet, 2 damage
  { 1, 16, 24, 6, 20, 8, 1, 0, 0, 0, 6 },   // SHOTGUN: wider blast, shorter wait
  { 1, 35, 14, 3,  6, 3, 5, 0, 0, 0, 4 },   // MAGNUM: fast, hits hard
  { 1, 30,  6, 2,  5, 3, 6, 0, 1, 0, 2 },   // MACHINEGUN: fast, little kickback
  { 3,  8,  2, 0,  6, 6, 7, 2, 2, 2, 2 },   // FLAMETHROWER: three times as far, twice as many flames
  { 1, 30,  5, 0,  4, 2, 5, 0, 0, 0, 2 },   // DUAL_PISTOLS: two bullets per shot
};

class Weapons
{
  public:

    static const byte maxBullets = 12;
    int powerupTimer = POWERUP_MAXTIME;
    byte maxFuse = 23;
    byte fuse = 0;
    byte frameDelay = 1;
    byte maxCooldown =  3;
    byte kickBack = 0;
    byte cooldown = 0;
    byte currentBullet = 0;
    byte width = 4;
    byte height = 1;
    byte xSpeed = 3;
    byte xSpeedOffset = 0;
    byte yOffset = 0;
    byte ySpeed = 0;
    byte ySpeedOffset = 0;
    byte damage = 1;

    Bullet bullet[maxBullets];

    void shootDual()
    {
      currentBullet ++; if (currentBullet == maxBullets)currentBullet = 0;
      bullet[currentBullet].isActive = true;
      bullet[currentBullet].fuse =  maxFuse ;
      bullet[currentBullet].ySpeed = random(0, ySpeedOffset + 1);
      byte rand = random(0, 2); if (rand == 1) bullet[currentBullet].ySpeed *= -1;
    }
    boolean shoot()
    {
      if (cooldown == 0)
      {
        currentBullet ++; if (currentBullet == maxBullets)currentBullet = 0;
        bullet[currentBullet].isActive = true;
        bullet[currentBullet].fuse =  maxFuse ;
        bullet[currentBullet].ySpeed = random(0, ySpeedOffset + 1);
        byte rand = random(0, 2); if (rand == 1) bullet[currentBullet].ySpeed *= -1;

        cooldown = maxCooldown;
        return true;
      }
      else
      {
        return false;
      }
    }


    void Update()
    {

      //timer for weapons powerup (to avoid exploitation)
      powerupTimer --;
      
      if (cooldown > 0) cooldown --;

      for (byte i = 0; i < maxBullets; i++)
      {
        if (bullet[i].isActive)
        {
          bullet[i].Update();
        }
      }
    }

    void setBulletSpecs()
    {
      for (byte i = 0; i < maxBullets; i++)
      {
        bullet[i].width = width;
        bullet[i].height = height;
        bullet[i].xSpeed = xSpeed;
        bullet[i].ySpeed = ySpeed;
        bullet[i].damage = damage;
        bullet[i].isActive = false;
      }

    }

    void weaponSet(byte weapType)
    {
      const byte *p = weaponSpecs[weapType];
      powerupTimer = POWERUP_MAXTIME;
      frameDelay = pgm_read_byte(p++);
      maxFuse = pgm_read_byte(p++);
      maxCooldown = pgm_read_byte(p++);
      kickBack = pgm_read_byte(p++);
      width = pgm_read_byte(p++);
      height = pgm_read_byte(p++);
      xSpeed = pgm_read_byte(p++);
      xSpeedOffset = pgm_read_byte(p++);
      yOffset = pgm_read_byte(p++);
      ySpeedOffset = pgm_read_byte(p++);
      damage = pgm_read_byte(p);
      fuse = 0;
      cooldown = 0;
      ySpeed = 0;
      setBulletSpecs();
    }
};

Weapons weapon;

void makeShootingSounds() //pew pew sound: pistol, shotgun, magnum, machine gun, flamethrower; the dual pistols sound like the pistol
{
  sfx(SFX_GUN + CURRENT_WEAPON % 5);
}

void shoot()
{
  if ((arduboy.pressed(A_BUTTON) && CURRENT_WEAPON == MACHINEGUN)  || (arduboy.pressed(A_BUTTON) && CURRENT_WEAPON == FLAMETHROWER) || arduboy.justPressed(A_BUTTON))
  {
    //shoot normal bullets
    if (weapon.shoot()) {
      makeShootingSounds();
      byte yOffset = weapon.yOffset;
      byte randNum = random(0, 2); if (randNum == 1) yOffset *= -1;
      weapon.bullet[weapon.currentBullet].y = player.y + 2 + yOffset ;
      byte randxSpeedOffset = random(1, weapon.xSpeedOffset + 1);
      weapon.bullet[weapon.currentBullet].x = player.x + ((1 - (2 * playerFacing)) * 8);
      weapon.bullet[weapon.currentBullet].xSpeed =  ((1 - (2 * playerFacing)) * weapon.xSpeed) - ((1 - (2 * playerFacing)) * randxSpeedOffset);
      {
        for (byte i = 0; i <  weapon.kickBack; i++)
        {
          if (!checkCollisions()) player.x -= (1 - (2 * playerFacing));
        }
      }
      if (CURRENT_WEAPON == DUAL_PISTOLS)
      {
        weapon.shootDual();
        weapon.bullet[weapon.currentBullet].y = player.y + 3;
        weapon.bullet[weapon.currentBullet].x = player.x - ((-1 * playerFacing) * weapon.width);
        weapon.bullet[weapon.currentBullet].xSpeed = -  ((1 - (2 * playerFacing)) * weapon.xSpeed) + ((1 - (2 * playerFacing)));
      }
    }
  }
}

void updateWeapon()
{
  if (arduboy.everyXFrames(weapon.frameDelay))
  {
    weapon.Update();
    if (weapon.powerupTimer < 1)
    {
      CURRENT_WEAPON = DEFAULT_GUN;
      weapon.weaponSet(0);
    }
  }
}

void drawBullets()
{
  for (byte i = 0; i < weapon.maxBullets; i++)
  {
    if (weapon.bullet[i].isActive)  sprites.drawSelfMasked(weapon.bullet[i].x, weapon.bullet[i].y, bullets_is_mask, CURRENT_WEAPON + (playerFacing * 6));
  }
}

#endif
