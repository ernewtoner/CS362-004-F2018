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
  bool allTestsPassed = false;
  
  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Card: Smithy ----------------\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  // set first card to Smithy
  testG.hand[thisPlayer][0] = smithy;

  printf("Current player: %d\n", whoseTurn(&testG));

  //printf("handCount before call: %d\n", testG.handCount[thisPlayer]);
  // Call the Smithy card effect on test state
  cardEffect(smithy, choice1, choice2, choice3, &testG, handPos, &bonus);
  // printf("handCount after call: %d\n", testG.handCount[thisPlayer]);

  // Player should now have 3 more cards in their hand and have discarded the Smithy card
  int newCards = 3;
  int discarded = 1;
  printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
  if (assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded))
    printf("smithy cardEffect() test: PASS, player's hand has gained three cards and discarded Smithy.\n");
  else {
    printf("smithy cardEffect() test: FAIL, hand count does not match expected value.\n");
    exit(1);
  }

  if(allTestsPassed)
    printf("All tests passed!\n");
}

// Performs as expected, bugs in Smithy make it not discard Smithy and causes drawing 4 cards
