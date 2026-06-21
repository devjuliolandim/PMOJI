#pragma once


int getScore(const char *gameId);

void saveScore(const char *gameId, int score);

const char *handleGameId (const char* gameId);