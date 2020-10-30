#include <stdio.h>
#include <string.h>
#include "game.h"
#include "question_category.h"

int howManyPlayers(Game *pGame);

void movePlayerAndAskQuestion(Game *pGame, int roll);

const char *currentCategory(Game *pGame);

void askQuestion(Game *pGame);

int didPlayerWin(Game *pGame);

Game *newGame() {
    Game *game = (Game *) malloc(sizeof(Game));

    game->currentPlayer = 0;
    memset(game->purses, 0, sizeof(int) * MAX_PLAYERS_COUNT);
    memset(game->places, 0, sizeof(int) * MAX_PLAYERS_COUNT);
    memset(game->inPenaltyBox, 0, sizeof(int) * MAX_PLAYERS_COUNT);

    game->players = newArrayList();

    for (int i = 0; i < 4; ++i) {
        game->questionCategory[i] = newQuestionCategory(CATEGORIES[i]);
        initQuestions(game->questionCategory[i], MAX_QUESTIONS_COUNT);
    }
    return game;
}

int add(Game *game, const char *playerName) {
    addElement(game->players, playerName);
    game->places[howManyPlayers(game)] = 0;
    game->purses[howManyPlayers(game)] = 0;
    game->inPenaltyBox[howManyPlayers(game)] = 0;

    printf("%s was added\r\n", playerName);
    printf("They are player number %d\r\n", size(game->players));
    return 1;
}

int howManyPlayers(Game *pGame) {
    return size(pGame->players);
}

void roll(Game *game, int roll) {
    printf("%s is the current player\r\n", get(game->players, game->currentPlayer));
    printf("They have rolled a %d\r\n", roll);

    if (game->inPenaltyBox[game->currentPlayer]) {
        if (roll % 2 != 0) {
            game->isGettingOutOfPenaltyBox = 1;

            printf("%s is getting out of the penalty box\r\n", get(game->players, game->currentPlayer));
            movePlayerAndAskQuestion(game, roll);
        } else {
            printf("%s is not getting out of the penalty box\r\n", get(game->players, game->currentPlayer));
            game->isGettingOutOfPenaltyBox = 0;
        }
    } else {
        movePlayerAndAskQuestion(game, roll);
    }
}

void movePlayerAndAskQuestion(Game *pGame, int roll) {
    pGame->places[pGame->currentPlayer] = pGame->places[pGame->currentPlayer] + roll;
    if (pGame->places[pGame->currentPlayer] > 11)
        pGame->places[pGame->currentPlayer] = pGame->places[pGame->currentPlayer] - 12;

    printf("%s's new location is %d\r\n", get(pGame->players, pGame->currentPlayer),
           pGame->places[pGame->currentPlayer]);
    printf("The category is %s\r\n", currentCategory(pGame));
    askQuestion(pGame);
}

void askQuestion(Game *pGame) {
    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        if (strcmp(currentCategory(pGame), CATEGORIES[i]) == 0)
            printf("%s\r\n", pickOneQuestion(pGame->questionCategory[i]));
    }
}

const char *currentCategory(Game *pGame) {
    if (pGame->places[pGame->currentPlayer] == 0) return "Pop";
    if (pGame->places[pGame->currentPlayer] == 4) return "Pop";
    if (pGame->places[pGame->currentPlayer] == 8) return "Pop";
    if (pGame->places[pGame->currentPlayer] == 1) return "Science";
    if (pGame->places[pGame->currentPlayer] == 5) return "Science";
    if (pGame->places[pGame->currentPlayer] == 9) return "Science";
    if (pGame->places[pGame->currentPlayer] == 2) return "Sports";
    if (pGame->places[pGame->currentPlayer] == MAX_PLAYERS_COUNT) return "Sports";
    if (pGame->places[pGame->currentPlayer] == 10) return "Sports";
    return "Rock";
}

int wrongAnswer(Game *game) {
    printf("Question was incorrectly answered\r\n");
    printf("%s was sent to the penalty box\r\n", get(game->players, game->currentPlayer));
    game->inPenaltyBox[game->currentPlayer] = 1;

    game->currentPlayer++;
    if (game->currentPlayer == game->players->size) game->currentPlayer = 0;
    return 1;
}

int wasCorrectlyAnswered(Game *game) {
    if (game->inPenaltyBox[game->currentPlayer]) {
        if (game->isGettingOutOfPenaltyBox) {
            printf("Answer was correct!!!!\r\n");
            game->currentPlayer++;
            if (game->currentPlayer == game->players->size) game->currentPlayer = 0;
            game->purses[game->currentPlayer]++;
            printf("%s now has %d Gold Coins.\r\n", get(game->players, game->currentPlayer), game->purses[game->currentPlayer]);
            int winner = didPlayerWin(game);

            return winner;
        } else {
            game->currentPlayer++;
            if (game->currentPlayer == game->players->size) game->currentPlayer = 0;
            return 1;
        }
    } else {
        printf("Answer was correct!!!!\r\n");
        game->purses[game->currentPlayer]++;
        printf("%s now has %d Gold Coins.\r\n", get(game->players, game->currentPlayer), game->purses[game->currentPlayer]);

        int winner = didPlayerWin(game);
        game->currentPlayer++;
        if (game->currentPlayer == game->players->size) game->currentPlayer = 0;

        return winner;
    }
}

int didPlayerWin(Game *pGame) {
    return pGame->purses[pGame->currentPlayer] != MAX_PLAYERS_COUNT;
}

