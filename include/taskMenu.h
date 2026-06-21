#pragma once
#include "states.h"

//How many options we have in the menu
#define MENU_OPTIONS_SIZE 4

extern bool isMenuActive;

extern MenuOption currentMenuOption;

void taskMenu(void * params);
void pmojiOpening();