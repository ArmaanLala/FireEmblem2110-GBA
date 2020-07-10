#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
#include "images/garbage.h"
#include "images/Title.h"
#include "images/Map.h"
#include "images/Battle-Grass.h"
#include "images/Battle-Castle.h"
#include "images/Rules-Background.h"
#include "images/Character-Select.h"
#include "images/Lose.h"
#include "images/Win.h"
#include "images/Arrow.h"
#include "images/FemaleSprite.h"
#include "images/FemaleSprite2.h"
#include "images/FemaleSprite3.h"
#include "images/MaleSprite1.h"
#include "images/MaleSprite2.h"
#include "images/MaleSprite3.h"
#include "images/Boss1.h"
#include "images/Boss2.h"
#include "images/Boss3.h"
#include "images/Enemy1.h"
#include "images/Enemy2.h"
#include "images/Enemy3.h"
#include "images/Enemy4.h"
#include "images/Enemy5.h"
#include "images/Enemy6.h"
#include "images/FemaleFight.h"
#include "images/MaleFight.h"
#include "images/FemaleCastle.h"
#include "images/MaleCastle.h"
#include "images/General.h"
#include "images/Pirate.h"
#include "images/Sniper.h"
#include "images/FullHearts.h"
#include "images/TwoHearts.h"
#include "images/OneHeart.h"
/* TODO: */
// Add any additional states you need for your app.
typedef enum
{
  START,
  DRAWRULES,
  RULES,
  DRAWSELECT,
  SELECT,
  DRAWMAP,
  MAP,
  GRASSANIM,
  FIGHT,
  WAIT,
  CASTLEANIM,
  BOSSFIGHT,
  WIN,
  LOSE,
} GBAState;

void changeSprite(Character *a);
void changeSpriteBoss(Boss *a);
int checkBounds(int row, int col);
void startBattle(Character *a, Character *b, Character *c, Boss *d, GBAState *statePtr);

int main(void)
{

  Character player;
  Character enemy1;
  Character enemy2;
  ArrowSelect gender;
  ArrowSelect rps;
  Boss boss;

  gender.row = 126;
  gender.col = 16;

  rps.row = 126;
  rps.col = 16;

  for (int i = 0; i < 1054; i++)
  {
    boss.sprite1[i] = Boss1[i];
    boss.sprite2[i] = Boss2[i];
    boss.sprite3[i] = Boss3[i];
  }

  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;

  while (1)
  {

    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    waitForVBlank();
    switch (state)
    {

    case START:
      drawFullScreenImageDMA(Title);

      if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons))
      {
        state = DRAWRULES;
        // fillScreenDMA(BLACK);
      }

      player.health = 3;
      player.row = 9;
      player.col = 1;
      player.male = 0;
      player.female = 0;
      player.counter = 0;
      player.sprite = player.sprite1;

      enemy1.health = 1;
      enemy1.row = 2;
      enemy1.col = 2;
      enemy1.sprite = enemy1.sprite1;

      enemy2.health = 1;
      enemy2.row = 8;
      enemy2.col = 11;
      enemy2.sprite = enemy2.sprite1;

      boss.row = 16;
      boss.col = 160;
      boss.health = 1;
      boss.sprite = boss.sprite1;

      // state = ?
      break;

    case DRAWRULES:
      drawFullScreenImageDMA(RulesBackground);
      drawString(40, 5, "Hello My name is Armaan Lala", WHITE);
      drawString(50, 5, "Welcome to my GBA game", WHITE);
      drawString(60, 5, "This game is based off of Fire Embelem", WHITE);
      drawString(70, 5, "Use the D-pad to move on the screen", WHITE);
      drawString(80, 5, "Run into an enemy to fight them", WHITE);
      drawString(90, 5, "During a fight, pick Rock, Paper,", WHITE);
      drawString(100, 5, "or Scissors", WHITE);
      drawString(110, 5, "You must defeat all enemies to win", WHITE);
      drawString(120, 5, "You have three lives, Good Luck!", WHITE);
      drawCenteredString(70, 0, 240, 160, "Press Start to continue", BLACK);

      state = RULES;
      break;

    case RULES:
      if (KEY_DOWN(BUTTON_START, currentButtons) && !KEY_DOWN(BUTTON_START, previousButtons))
      {
        state = DRAWSELECT;
        // fillScreenDMA(BLACK);
      }
      if (KEY_DOWN(BUTTON_SELECT, currentButtons))
      {
        state = START;
        // fillScreenDMA(BLACK);
      }
      break;

    case DRAWSELECT:

      drawFullScreenImageDMA(CharacterSelect);
      // drawCenteredString(50,0,240,160,"Would you like to play as a Boy or a Girl",WHITE);
      drawCenteredString(30, 0, 240, 160, "Which gender would you like to play as?", WHITE);
      drawCenteredString(50, 80, 240, 160, "Boy", WHITE);
      drawCenteredString(50, -80, 240, 160, "Girl", WHITE);
      state = SELECT;

      break;

    case SELECT:
      if (KEY_DOWN(BUTTON_SELECT, currentButtons))
      {
        state = START;
        // fillScreenDMA(BLACK);
      }
      drawImageDMA(gender.row, gender.col, ARROW_WIDTH, ARROW_HEIGHT, Arrow);
      if (KEY_DOWN(BUTTON_RIGHT, currentButtons) && !KEY_DOWN(BUTTON_RIGHT, previousButtons))
      {
        gender.col = 180;
        drawRectDMA(gender.row, 16, 7, 7, BLACK);
      }
      if (KEY_DOWN(BUTTON_LEFT, currentButtons) && !KEY_DOWN(BUTTON_LEFT, previousButtons))
      {
        gender.col = 16;
        drawRectDMA(gender.row, 180, 7, 7, BLACK);
      }

      if (KEY_DOWN(BUTTON_A, currentButtons) && !KEY_DOWN(BUTTON_A, previousButtons))
      {
        if (gender.col == 16)
        {
          player.female = 1;
          for (int i = 0; i < 256; i++)
          {
            player.sprite1[i] = FemaleSprite[i];
            player.sprite2[i] = FemaleSprite2[i];
            player.sprite3[i] = FemaleSprite3[i];

            enemy1.sprite1[i] = Enemy1[i];
            enemy1.sprite2[i] = Enemy2[i];
            enemy1.sprite3[i] = Enemy3[i];

            enemy2.sprite1[i] = Enemy4[i];
            enemy2.sprite2[i] = Enemy5[i];
            enemy2.sprite3[i] = Enemy6[i];
          }
        }
        else if (gender.col == 180)
        {
          player.male = 1;

          for (int i = 0; i < 256; i++)
          {
            player.sprite1[i] = MaleSprite1[i];
            player.sprite2[i] = MaleSprite2[i];
            player.sprite3[i] = MaleSprite3[i];

            enemy1.sprite1[i] = Enemy1[i];
            enemy1.sprite2[i] = Enemy2[i];
            enemy1.sprite3[i] = Enemy3[i];

            enemy2.sprite1[i] = Enemy4[i];
            enemy2.sprite2[i] = Enemy5[i];
            enemy2.sprite3[i] = Enemy6[i];
          }
        }
        state = DRAWMAP;
        // fillScreenDMA(BLACK);
      }

      break;

    case DRAWMAP:

      if (enemy1.health == 0 && enemy2.health == 0 && boss.health == 0)
      {
        state = WIN;
      }

      else if (player.health == 3)
      {
        drawFullScreenImageDMA(Map);
        drawImageDMA(0, 228, FULLHEARTS_WIDTH, FULLHEARTS_HEIGHT, FullHearts);
        state = MAP;
      }
      else if (player.health == 2)
      {
        drawFullScreenImageDMA(Map);
        drawImageDMA(0, 228, FULLHEARTS_WIDTH, FULLHEARTS_HEIGHT, TwoHearts);
        state = MAP;
      }
      else if (player.health == 1)
      {
        drawFullScreenImageDMA(Map);
        drawImageDMA(0, 228, FULLHEARTS_WIDTH, FULLHEARTS_HEIGHT, OneHeart);
        state = MAP;
      }
      else if (player.health == 0)
      {
        state = LOSE;
      }
      if (enemy1.health == 0 && enemy2.health == 0 && boss.health == 0)
      {
        state = WIN;
      }
      // fillScreenDMA(BLACK);

      break;

    case MAP:

      if (KEY_DOWN(BUTTON_SELECT, currentButtons))
      {
        state = START;
        // fillScreenDMA(BLACK);
      }

      startBattle(&player, &enemy1, &enemy2, &boss, &state);

      // Have to draw boss first to avoid weird tearing.
      if (enemy1.health == 1)
      {

        drawImageDMA(enemy1.row * 16, enemy1.col * 16, 16, 16, enemy1.sprite);
      }
      if (enemy2.health == 1)
      {

        drawImageDMA(enemy2.row * 16, enemy2.col * 16, 16, 16, enemy2.sprite);
      }
      if (boss.health == 1)
      {
        drawImageDMA(boss.row, boss.col, BOSS1_WIDTH, BOSS1_HEIGHT, boss.sprite);
      }
      drawImageDMA(player.row * 16, player.col * 16, 16, 16, player.sprite);

      if (KEY_DOWN(BUTTON_DOWN, currentButtons) && !KEY_DOWN(BUTTON_DOWN, previousButtons) && player.row < 9 && checkBounds(player.row + 1, player.col) == 1)
      {
        drawBackgroundImageDMA(player.row * 16, player.col * 16, 16, 16, Map);
        player.row += 1;
      }
      if (KEY_DOWN(BUTTON_UP, currentButtons) && !KEY_DOWN(BUTTON_UP, previousButtons) && player.row >= 1 && checkBounds(player.row - 1, player.col) == 1)
      {
        drawBackgroundImageDMA(player.row * 16, player.col * 16, 16, 16, Map);
        player.row -= 1;
      }
      if (KEY_DOWN(BUTTON_RIGHT, currentButtons) && !KEY_DOWN(BUTTON_RIGHT, previousButtons) && player.col < 14 && checkBounds(player.row, player.col + 1) == 1)
      {
        drawBackgroundImageDMA(player.row * 16, player.col * 16, 16, 16, Map);
        player.col += 1;
      }
      if (KEY_DOWN(BUTTON_LEFT, currentButtons) && !KEY_DOWN(BUTTON_LEFT, previousButtons) && player.col >= 1 && checkBounds(player.row, player.col - 1) == 1)
      {
        drawBackgroundImageDMA(player.row * 16, player.col * 16, 16, 16, Map);
        player.col -= 1;
      }

      if (vBlankCounter % 30 == 0)
      {
        changeSprite(&player);
        changeSprite(&enemy1);
        changeSprite(&enemy2);
        changeSpriteBoss(&boss);
      }

      break;

    case GRASSANIM:

      fillScreenDMA(BLACK);

      for (int i = 0; i < 120; i += 2)
      {
        waitForVBlank();
        for (int j = 0; j < 240; j += 2)
        {
          drawBackgroundImageDMA(i, j, 2, 1, BattleGrass);
        }
      }
      for (int i = 1; i < 120; i += 2)
      {
        waitForVBlank();
        for (int j = 1; j < 240; j += 2)
        {
          drawBackgroundImageDMA(i, j, 2, 1, BattleGrass);
        }
      }

      state = FIGHT;

      break;

    case FIGHT:

      // Have to draw boss first to avoid weird tearing.
      // drawImageDMA(boss.row, boss.col, BOSS1_WIDTH, BOSS1_HEIGHT, boss.sprite);
      if (player.row == 2 && player.col == 2)
      {
        drawImageDMA(32, 176, 64, 64, Pirate);
      }
      else if (player.row == 8 && player.col == 11)
      {
        drawImageDMA(20, 160, 64, 64, Sniper);
      }
      if (player.female == 1)
      {
        drawImageDMA(48, 16, 64, 64, FemaleFight);
      }
      else if (player.male == 1)
      {
        drawImageDMA(48, 16, 64, 64, MaleFight);
      }

      drawCenteredString(50, 0, 240, 160, "Lance", WHITE);
      drawCenteredString(50, 80, 240, 160, "Sword", WHITE);
      drawCenteredString(50, -80, 240, 160, "Axe", WHITE);
      drawImageDMA(rps.row, rps.col, ARROW_WIDTH, ARROW_HEIGHT, Arrow);

      if (KEY_DOWN(BUTTON_RIGHT, currentButtons) && !KEY_DOWN(BUTTON_RIGHT, previousButtons) && rps.col == 90)
      {
        rps.col = 170;
        drawRectDMA(rps.row, 90, 7, 7, BLACK);
      }

      else if (KEY_DOWN(BUTTON_RIGHT, currentButtons) && !KEY_DOWN(BUTTON_RIGHT, previousButtons) && rps.col == 16)
      {
        rps.col = 90;
        drawRectDMA(rps.row, 16, 7, 7, BLACK);
      }

      else if (KEY_DOWN(BUTTON_LEFT, currentButtons) && !KEY_DOWN(BUTTON_LEFT, previousButtons) && rps.col == 90)
      {
        rps.col = 16;
        drawRectDMA(rps.row, 90, 7, 7, BLACK);
      }
      else if (KEY_DOWN(BUTTON_LEFT, currentButtons) && !KEY_DOWN(BUTTON_LEFT, previousButtons) && rps.col == 170)
      {
        rps.col = 90;
        drawRectDMA(rps.row, 170, 7, 7, BLACK);
      }

      if (KEY_DOWN(BUTTON_A, currentButtons) && !KEY_DOWN(BUTTON_A, previousButtons))
      {

        int enemy = randint(1, 3);

        if (rps.col == 90)
        {

          if (enemy == 2)
          {
            //tie
            drawCenteredString(60, 0, 240, 160, "The enemy picked lance", WHITE);
            drawCenteredString(70, 0, 240, 160, "Its a tie", WHITE);
            // player.health --;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 3)
          {
            drawCenteredString(60, 0, 240, 160, "The enemy picked sword", WHITE);
            drawCenteredString(70, 0, 240, 160, "You won this fight", WHITE);
            if (player.row == 2 && player.col == 2)
            {
              enemy1.health--;
              // player.health --;
              // player.row++;
              state = WAIT;
            }
            else if (player.row == 8 && player.col == 11)
            {
              enemy2.health--;
              // player.health --;
              // player.row++;
              state = WAIT;
            }
          }
          else if (enemy == 1)
          {
            drawCenteredString(60, 0, 240, 160, "The enemy picked axe", WHITE);
            drawCenteredString(70, 0, 240, 160, "You lost this fight", WHITE);

            player.health--;
            player.row++;
            state = WAIT;
          }
        }
        else if (rps.col == 170)
        {

          if (enemy == 2)
          {
            //tie
            drawCenteredString(60, 0, 240, 160, "The enemy picked lance", WHITE);
            drawCenteredString(70, 0, 240, 160, "You lost this fight", WHITE);
            player.health--;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 3)
          {
            drawCenteredString(60, 0, 240, 160, "The enemy picked sword", WHITE);
            drawCenteredString(70, 0, 240, 160, "Its a tie", WHITE);
            // player.health --;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 1)
          {
            drawCenteredString(60, 0, 240, 160, "The enemy picked axe", WHITE);
            drawCenteredString(70, 0, 240, 160, "You won this fight", WHITE);
            if (player.row == 2 && player.col == 2)
            {
              enemy1.health--;
              // player.health --;
              // player.row++;
              state = WAIT;
            }
            else if (player.row == 8 && player.col == 11)
            {
              enemy2.health--;
              // player.health --;
              // player.row++;
              state = WAIT;
            }
          }
        }
        else if (rps.col == 16)
        {

          if (enemy == 2)
          {
            //tie
            drawCenteredString(60, 0, 240, 160, "The enemy picked lance", WHITE);
            drawCenteredString(70, 0, 240, 160, "You won this fight", WHITE);
            if (player.row == 2 && player.col == 2)
            {
              enemy1.health--;
              // player.health --;
              // player.row++;
              state = WAIT;
            }
            else if (player.row == 8 && player.col == 11)
            {
              enemy2.health--;
              // player.health --;
              // player.row++;
              state = WAIT;
            }
          }
          else if (enemy == 3)
          {
            drawCenteredString(60, 0, 240, 160, "The enemy picked sword", WHITE);
            drawCenteredString(70, 0, 240, 160, "You lost this fight", WHITE);
            player.health--;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 1)
          {
            drawCenteredString(60, 0, 240, 160, "The enemy picked axe", WHITE);
            drawCenteredString(70, 0, 240, 160, "Its a tie", WHITE);
            // player.health --;
            player.row++;
            state = WAIT;
          }
        }
      }

      // state = ?
      break;

    case CASTLEANIM:
      fillScreenDMA(BLACK);

      for (int i = 0; i < 120; i += 2)
      {
        waitForVBlank();
        for (int j = 0; j < 240; j += 2)
        {
          drawBackgroundImageDMA(i, j, 2, 1, BattleCastle);
        }
      }
      for (int i = 1; i < 120; i += 2)
      {
        waitForVBlank();
        for (int j = 1; j < 240; j += 2)
        {
          drawBackgroundImageDMA(i, j, 2, 1, BattleCastle);
        }
      }
      state = BOSSFIGHT;
      break;

    case BOSSFIGHT:

      // Have to draw boss first to avoid weird tearing.
      // drawImageDMA(boss.row, boss.col, BOSS1_WIDTH, BOSS1_HEIGHT, boss.sprite);

      drawImageDMA(32, 130, GENERAL_WIDTH, GENERAL_HEIGHT, General);

      if (player.female == 1)
      {
        drawImageDMA(48, 16, 64, 64, FemaleCastle);
      }
      else if (player.male == 1)
      {
        drawImageDMA(48, 16, 64, 64, MaleCastle);
      }

      drawCenteredString(50, 0, 240, 160, "Lance", WHITE);
      drawCenteredString(50, 80, 240, 160, "Sword", WHITE);
      drawCenteredString(50, -80, 240, 160, "Axe", WHITE);
      drawImageDMA(rps.row, rps.col, ARROW_WIDTH, ARROW_HEIGHT, Arrow);

      if (KEY_DOWN(BUTTON_RIGHT, currentButtons) && !KEY_DOWN(BUTTON_RIGHT, previousButtons) && rps.col == 90)
      {
        rps.col = 170;
        drawRectDMA(rps.row, 90, 7, 7, BLACK);
      }

      else if (KEY_DOWN(BUTTON_RIGHT, currentButtons) && !KEY_DOWN(BUTTON_RIGHT, previousButtons) && rps.col == 16)
      {
        rps.col = 90;
        drawRectDMA(rps.row, 16, 7, 7, BLACK);
      }

      else if (KEY_DOWN(BUTTON_LEFT, currentButtons) && !KEY_DOWN(BUTTON_LEFT, previousButtons) && rps.col == 90)
      {
        rps.col = 16;
        drawRectDMA(rps.row, 90, 7, 7, BLACK);
      }
      else if (KEY_DOWN(BUTTON_LEFT, currentButtons) && !KEY_DOWN(BUTTON_LEFT, previousButtons) && rps.col == 170)
      {
        rps.col = 90;
        drawRectDMA(rps.row, 170, 7, 7, BLACK);
      }

      if (KEY_DOWN(BUTTON_A, currentButtons) && !KEY_DOWN(BUTTON_A, previousButtons))
      {

        int enemy = randint(1, 3);

        if (rps.col == 90)
        {

          if (enemy == 2)
          {
            //tie
            drawCenteredString(60, 0, 240, 160, "The boss picked lance", WHITE);
            drawCenteredString(70, 0, 240, 160, "Its a tie", WHITE);
            // player.health --;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 3)
          {
            drawCenteredString(60, 0, 240, 160, "The boss picked sword", WHITE);
            drawCenteredString(70, 0, 240, 160, "You won this fight", WHITE);
            boss.health--;
            state = WAIT;
          }
          else if (enemy == 1)
          {
            drawCenteredString(60, 0, 240, 160, "The boss picked axe", WHITE);
            drawCenteredString(70, 0, 240, 160, "You lost this fight", WHITE);

            player.health--;
            player.row++;
            state = WAIT;
          }
        }
        else if (rps.col == 170)
        {

          if (enemy == 2)
          {
            //tie
            drawCenteredString(60, 0, 240, 160, "The boss picked lance", WHITE);
            drawCenteredString(70, 0, 240, 160, "You lost this fight", WHITE);
            player.health--;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 3)
          {
            drawCenteredString(60, 0, 240, 160, "The boss picked sword", WHITE);
            drawCenteredString(70, 0, 240, 160, "Its a tie", WHITE);
            // player.health --;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 1)
          {
            drawCenteredString(60, 0, 240, 160, "The boss picked axe", WHITE);
            drawCenteredString(70, 0, 240, 160, "You won this fight", WHITE);
            boss.health--;
            state = WAIT;
          }
        }
        else if (rps.col == 16)
        {

          if (enemy == 2)
          {
            //tie
            drawCenteredString(60, 0, 240, 160, "The boss picked lance", WHITE);
            drawCenteredString(70, 0, 240, 160, "You won this fight", WHITE);
            boss.health--;
            state = WAIT;
          }
          else if (enemy == 3)
          {
            drawCenteredString(60, 0, 240, 160, "The boss picked sword", WHITE);
            drawCenteredString(70, 0, 240, 160, "You lost this fight", WHITE);
            player.health--;
            player.row++;
            state = WAIT;
          }
          else if (enemy == 1)
          {
            drawCenteredString(60, 0, 240, 160, "The boss picked axe", WHITE);
            drawCenteredString(70, 0, 240, 160, "Its a tie", WHITE);
            // player.health --;
            player.row++;
            state = WAIT;
          }
        }
      }

      break;

    case WAIT:
      if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons))
      {
        state = DRAWMAP;
      }
      break;

    case WIN:
      drawFullScreenImageDMA(Win);
      drawCenteredString(50, 0, 240, 160, "Press START to play again", WHITE);
      if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons) || KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
      {
        state = START;
      }
      // state = ?
      break;

    case LOSE:
      drawFullScreenImageDMA(Lose);
      drawCenteredString(50, 0, 240, 160, "Press START to try again", RED);
      if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons) || KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
      {
        state = START;
      }
      // state = ?
      break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }
  UNUSED(player);

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}

void changeSprite(Character *a)
{
  a->counter++;
  if ((a->counter) % 3 == 0)
  {
    a->sprite = a->sprite1;
  }
  else if ((a->counter) % 3 == 1)
  {
    a->sprite = a->sprite2;
  }
  else if ((a->counter) % 3 == 2)
  {
    a->sprite = a->sprite3;
  }
}

void changeSpriteBoss(Boss *a)
{
  a->counter++;
  if ((a->counter) % 3 == 0)
  {
    a->sprite = a->sprite1;
  }
  else if ((a->counter) % 3 == 1)
  {
    a->sprite = a->sprite2;
  }
  else if ((a->counter) % 3 == 2)
  {
    a->sprite = a->sprite3;
  }
}

int checkBounds(int row, int col)
{
  if (row == 9 && col == 4)
  {
    return 0;
  }
  else if (row == 8 && col == 4)
  {
    return 0;
  }
  else if (row == 7 && col == 4)
  {
    return 0;
  }
  else if (row == 7 && col == 5)
  {
    return 0;
  }
  else if (row == 6 && col == 5)
  {
    return 0;
  }
  else if (row == 5 && col == 5)
  {
    return 0;
  }
  else if (row == 0 && col == 8)
  {
    return 0;
  }
  else if (row == 1 && col == 8)
  {
    return 0;
  }
  else if (row == 2 && col == 8)
  {
    return 0;
  }
  else if (row == 3 && col == 8)
  {
    return 0;
  }
  else if (row == 4 && col == 8)
  {
    return 0;
  }
  else if (row == 5 && col == 8)
  {
    return 0;
  }
  else if (row == 6 && col == 8)
  {
    return 0;
  }
  else if (row == 7 && col == 8)
  {
    return 0;
  }
  else if (row == 0 && col == 14)
  {
    return 0;
  }
  else if (row == 1 && col == 14)
  {
    return 0;
  }
  else if (row == 2 && col == 14)
  {
    return 0;
  }
  else if (row == 3 && col == 14)
  {
    return 0;
  }
  else if (row == 4 && col == 14)
  {
    return 0;
  }
  else if (row == 5 && col == 14)
  {
    return 0;
  }
  else if (row == 6 && col == 14)
  {
    return 0;
  }
  else if (row == 7 && col == 14)
  {
    return 0;
  }
  else if (row == 7 && col == 13)
  {
    return 0;
  }
  else if (row == 7 && col == 12)
  {
    return 0;
  }
  else if (row == 7 && col == 10)
  {
    return 0;
  }
  else if (row == 7 && col == 9)
  {
    return 0;
  }
  else if (row == 6 && col == 13)
  {
    return 0;
  }
  else if (row == 6 && col == 12)
  {
    return 0;
  }
  else if (row == 6 && col == 10)
  {
    return 0;
  }
  else if (row == 6 && col == 9)
  {
    return 0;
  }
  else if (row == 0 && col == 13)
  {
    return 0;
  }
  else if (row == 0 && col == 12)
  {
    return 0;
  }
  else if (row == 0 && col == 10)
  {
    return 0;
  }
  else if (row == 0 && col == 9)
  {
    return 0;
  }
  else if (row == 3 && col == 10)
  {
    return 0;
  }
  else if (row == 3 && col == 12)
  {
    return 0;
  }
  else if (row == 0 && col == 11)
  {
    return 0;
  }

  return 1;
}

void startBattle(Character *a, Character *b, Character *c, Boss *d, GBAState *state)
{

  if (a->row == 2 && a->col == 2 && b->health == 1)
  {
    *state = GRASSANIM;
  }
  else if (a->row == 8 && a->col == 11 && c->health == 1)
  {
    *state = GRASSANIM;
  }
  else if (a->row == 2 && a->col == 11 && d->health == 1)
  {
    *state = CASTLEANIM;
  }
}