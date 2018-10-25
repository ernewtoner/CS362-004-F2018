// void smithyCardEffect(int currentPlayer, struct gameState *state, int handPos)
// int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <stdbool.h>

int assertTrue(int condition) {
  if (condition > 0)
    return 1;
  else
    return 0;
}

int main() {
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  struct gameState G, testG;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
  bool allTestsPassed = true;
  
  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Card: Village ----------------\n");

  // +1 Card
  // +2 Actions

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  // set first card to Smithy
  testG.hand[thisPlayer][0] = village;

  printf("Current player: %d\n", whoseTurn(&testG));

  //printf("handCount before call: %d\n", testG.handCount[thisPlayer]);
  // Call the Village card effect on test state
  int cardEffectReturn = cardEffect(village, choice1, choice2, choice3, &testG, handPos, &bonus);
  // printf("handCount after call: %d\n", testG.handCount[thisPlayer]);

  // Player should now have 1 more card in their hand and have discarded the Village card
  int newCards = 1;
  int discarded = 1;
  printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
  if (assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded))
    printf("village cardEffect() test: PASS, player has gained one card and discarded one card.\n");
  else {
    allTestsPassed = false;
    printf("village cardEffect() test: FAIL, hand count does not match expected value.\n");
  }

  // Confirm discarded card is Village

  // Player should have 2 more actions than before Village was played
  int newActions = 2;
  printf("action count = %d, expected = %d\n", testG.numActions, G.numActions + newActions);
  if (assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded))
    printf("village cardEffect() test: PASS, player has gained two actions.\n");
  else {
    allTestsPassed = false;
    printf("village cardEffect() test: FAIL, action count does not match expected value.\n");
  }

  // Check if the cardEffect() function returned 0
  if (assertTrue(cardEffectReturn == 0))
    printf("village cardEffect() test: PASS, cardEffect() returned 0.\n");
  else {
    allTestsPassed = false;
    printf("village cardEffect() test: FAIL, cardEffect() did not return 0..\n");
  }

  // If all tests passed or otherwise print a statement accordingly
  if(allTestsPassed)
    printf("All tests passed!\n");
  else
    printf("All tests complete with failure(s)!\n");
}
