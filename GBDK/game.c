// Copyright (c) 2020 Jacob Bonner All rights reserved.
//
// Created by: Jacob Bonner
// Created on: January 2020
// This is the main game file for Asteroid Dodger for the GameBoy

#include <stdio.h>
#include <gb/gb.h>
#include <time.h>
#include "menuSceneMap.c"
#include "menuSceneData.c"
#include "snakobSceneMap.c"
#include "snakobSceneData.c"
#include "sprites.c"
#include "gamemap.c"
#include "gameCharacter.c"

struct GameCharacter ship;
struct GameCharacter leftAsteroid;
struct GameCharacter upAsteroid;
struct GameCharacter rightAsteroid;
struct GameCharacter downAsteroid;
UBYTE spritesize = 8;

// The death determination variable
int deathCounter = 0;

void performantdelay(UINT8 numloops) {
    // This function is a performance delayer
    UINT8 ii;
    for (ii = 0; ii < numloops; ii++) {
        wait_vbl_done();
    }
}

void fadeout() {
    // This function fades a scene out
    int i;
    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                BGP_REG = 0xE4;
                break;
            case 1:
                BGP_REG = 0xF9;
                break;
            case 2:
                BGP_REG = 0xFE;
                break;
            case 3:
                BGP_REG = 0xFF;
                break;
        }
    performantdelay(50);
    }
}

void fadein() {
    // This function fades a scene in
    int i;
    for (i = 0; i < 3; i++) {
        switch (i) {
            case 0:
                BGP_REG = 0xFE;
                break;
            case 1:
                BGP_REG = 0xF9;
                break;
            case 2:
                BGP_REG = 0xE4;
                break;
        }
    performantdelay(50);
    }
}

// This function assembles games sprites
void moveGameCharacter(struct GameCharacter* character, UINT8 x, UINT8 y) {
    move_sprite(character->spritids[0], x, y);
}

// This function detects if two sprites have collided
UBYTE checkCollisions(struct GameCharacter* one, struct GameCharacter* two){
    return (one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height)
           || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}

// This function setups up ship
void setupShip() {
    ship.x = 80;
    ship.y = 72;
    ship.width = 8;
    ship.height = 8;

    set_sprite_tile(1, 7);
    ship.spritids[0] = 1;

    moveGameCharacter(&ship, ship.x, ship.y);
}

// This function setups up left asteroid
void setupLeftAsteroids() {
    leftAsteroid.x = 0;
    leftAsteroid.y = 50;
    leftAsteroid.width = 8;
    leftAsteroid.height = 8;

    set_sprite_tile(2, 2);
    leftAsteroid.spritids[0] = 2;

    moveGameCharacter(&leftAsteroid, leftAsteroid.x, leftAsteroid.y);
}

// This function setups up upwards asteroid
void setupUpAsteroids() {
    upAsteroid.x = 100;
    upAsteroid.y = 8;
    upAsteroid.width = 8;
    upAsteroid.height = 8;

    set_sprite_tile(3, 3);
    upAsteroid.spritids[0] = 3;

    moveGameCharacter(&upAsteroid, upAsteroid.x, upAsteroid.y);
}

// This function setups up right asteroid
void setupRightAsteroids() {
    rightAsteroid.x = 160;
    rightAsteroid.y = 100;
    rightAsteroid.width = 8;
    rightAsteroid.height = 8;

    set_sprite_tile(4, 4);
    rightAsteroid.spritids[0] = 4;

    moveGameCharacter(&rightAsteroid, rightAsteroid.x, rightAsteroid.y);
}

// This function setups up downwards asteroid
void setupDownAsteroids() {
    downAsteroid.x = 35;
    downAsteroid.y = 144;
    downAsteroid.width = 8;
    downAsteroid.height = 8;

    set_sprite_tile(5, 5);
    downAsteroid.spritids[0] = 5;

    moveGameCharacter(&downAsteroid, downAsteroid.x, downAsteroid.y);
}

// This is a specialised function for the game over scene
void gameOver() {
    // Moving sprites off screen
    ship.x = 180;
    leftAsteroid.x = 180;
    rightAsteroid.x = 180;
    upAsteroid.x = 180;
    downAsteroid.x = 180;
    moveGameCharacter(&ship, ship.x, ship.y);
    moveGameCharacter(&leftAsteroid, leftAsteroid.x, leftAsteroid.y);
    moveGameCharacter(&rightAsteroid, rightAsteroid.x, rightAsteroid.y);
    moveGameCharacter(&downAsteroid, downAsteroid.x, downAsteroid.y);
    moveGameCharacter(&upAsteroid, upAsteroid.x, upAsteroid.y);

    // Game Over and restart prompt text
    printf("\n \n \n \n \n \n \n ====Game  Over====");
}

void main() {
    // Main game functions

    // Sprite data
    set_sprite_data(0, 8, spriteTiles);

    // Setting sound channels and volume
    //NR52_REG = 0x80;
    //NR50_REG = 0x77;
    //NR51_REG = 0xFF;

    // Setting the Snakob Studios splash screen tiles and data
    set_bkg_data(0, 100, snakobSceneData);
    set_bkg_tiles(0, 0, 20, 18, snakobSceneMap);

    // Displaying the snakob splash scene background
    SHOW_BKG;
    DISPLAY_ON;

    // Waiting for the user to press start to swap and fade out the scenes
    performantdelay(100);
    fadeout();

    // Setting background tiles and data for menu scene
    set_bkg_data(0, 107, menuSceneData);
    set_bkg_tiles(0, 0, 20, 18, menuSceneMap);

    // Displaying the menu scene background
    SHOW_BKG;
    DISPLAY_ON;

    // Fading in the menu scene
    fadein();

    // Waiting for the player to start the game
    waitpad(J_START);

    // Game start sound
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    NR10_REG = 0x16;
    NR11_REG = 0x40;
    NR12_REG = 0x73;
    NR13_REG = 0x00;
    NR14_REG = 0xC3;

    // Setting the game background
    set_bkg_data(0, 100, spriteTiles);
    set_bkg_tiles(0, 0, 40, 18, gamemap);

    // Showing sprites and background
    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    // Setting up sprites
    setupShip();
    setupLeftAsteroids();
    setupUpAsteroids();
    setupRightAsteroids();
    setupDownAsteroids();

    // Game loop
    while(deathCounter == 0) {
        // Movement for the spaceship
        switch(joypad()) {
            case J_LEFT:
                if(ship.x < 24) {
                    ship.x = 24;
                    moveGameCharacter(&ship, ship.x, ship.y);
                } else {
                    ship.x -= 8;
                    moveGameCharacter(&ship, ship.x, ship.y);
                    break;
                }
            case J_RIGHT:
                if(ship.x > 158) {
                    ship.x = 158;
                    moveGameCharacter(&ship, ship.x, ship.y);
                } else {
                    ship.x += 8;
                    moveGameCharacter(&ship, ship.x, ship.y);
                    break;
                }
            case J_UP:
                if(ship.y < 8) {
                    ship.y = 8;
                    moveGameCharacter(&ship, ship.x, ship.y);
                } else {
                    ship.y -= 8;
                    moveGameCharacter(&ship, ship.x, ship.y);
                    break;
                }
            case J_DOWN:
                if(ship.y > 136) {
                    ship.y = 136;
                    moveGameCharacter(&ship, ship.x, ship.y);
                } else {
                    ship.y += 8;
                    moveGameCharacter(&ship, ship.x, ship.y);
                    break;
                }
        }

        // Scrolling and reseting for the left asteroid
        leftAsteroid.x += 6;
        if (leftAsteroid.x >= 160) {
            leftAsteroid.y = ship.y;
            leftAsteroid.x = 0;
        }
        moveGameCharacter(&leftAsteroid, leftAsteroid.x, leftAsteroid.y);

        // Scrolling and reseting for the right asteroid
        rightAsteroid.x -= 4;
        if (rightAsteroid.x <= 1) {
            rightAsteroid.y = ship.x;
            rightAsteroid.x = 160;
        }
        moveGameCharacter(&rightAsteroid, rightAsteroid.x, rightAsteroid.y);

        // Scrolling and reseting for the upward asteroid
        upAsteroid.y += 5;
        if (upAsteroid.y >= 144) {
            upAsteroid.y = 8;
            upAsteroid.x = ship.x;
        }
        moveGameCharacter(&upAsteroid, upAsteroid.x, upAsteroid.y);

        // Scrolling and reseting for the downward asteroid
        downAsteroid.y -= 3;
        if (downAsteroid.y <= 0) {
            downAsteroid.y = 144;
            downAsteroid.x = ship.y;
        }
        moveGameCharacter(&downAsteroid, downAsteroid.x, downAsteroid.y);

        // Checks for a collsion between the ship and the left asteroid
        if(checkCollisions(&ship, &leftAsteroid)) {
            // Variable updated to end game loop
            deathCounter = 1;
            
            // Death sound
            NR52_REG = 0x80;
            NR51_REG = 0x11;
            NR50_REG = 0x77;

            NR10_REG = 0x1E;
            NR11_REG = 0x10;
            NR12_REG = 0xF3;
            NR13_REG = 0x00;
            NR14_REG = 0x87;

            // Calling the game over scene
            gameOver();
        }

        // Checks for a collision between the ship and the right asteroid
        if(checkCollisions(&ship, &rightAsteroid)) {
            // Variable updated to end game loop
            deathCounter = 1;

            // Death sound
            NR52_REG = 0x80;
            NR51_REG = 0x11;
            NR50_REG = 0x77;

            NR10_REG = 0x1E;
            NR11_REG = 0x10;
            NR12_REG = 0xF3;
            NR13_REG = 0x00;
            NR14_REG = 0x87;
            
            // Game over scene
            gameOver();
        }

        // Checks for a collision between the ship and the upwards asteroid
        if(checkCollisions(&ship, &upAsteroid)) {
            // Variable updated to end game loop
            deathCounter = 1;
            
            // Death sound
            NR52_REG = 0x80;
            NR51_REG = 0x11;
            NR50_REG = 0x77;

            NR10_REG = 0x1E;
            NR11_REG = 0x10;
            NR12_REG = 0xF3;
            NR13_REG = 0x00;
            NR14_REG = 0x87;
            
            // Game over scene
            gameOver();
        }

        // Checks for a collision between the ship and the downwards asteroid
        if(checkCollisions(&ship, &downAsteroid)) {
            // Variable updated to end game loop
            deathCounter = 1;
            
            // Death sound
            NR52_REG = 0x80;
            NR51_REG = 0x11;
            NR50_REG = 0x77;

            NR10_REG = 0x1E;
            NR11_REG = 0x10;
            NR12_REG = 0xF3;
            NR13_REG = 0x00;
            NR14_REG = 0x87;
            
            // Game over scene
            gameOver();
        }

        performantdelay(5);
    }
}
