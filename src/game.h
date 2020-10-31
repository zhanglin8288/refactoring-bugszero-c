#ifndef REFACTOR_LENGTH_C_GAME_H
#define REFACTOR_LENGTH_C_GAME_H

#include "linked_list.h"
#include "array_list.h"
#include "question_category.h"
#include "player.h"
#include "players.h"

static const int MAX_QUESTIONS_COUNT = 50;
static const char *CATEGORIES[] = {"Pop", "Science", "Sports", "Rock", "Blues", "History"};
static const int CATEGORY_COUNT = sizeof(CATEGORIES) / sizeof(CATEGORIES[0]);

typedef struct _game {
    Players *players;
    QuestionCategory *questionCategory[CATEGORY_COUNT];
} Game;

Game *newGame();

int add(Game *game, const char *playerName);

int canAnswerQuestion(Game *game, int roll);

int wrongAnswer(Game *game);

int wasCorrectlyAnswered(Game *game);

#endif //REFACTOR_LENGTH_C_GAME_H
