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

  // original counts
  int origPlayedCount = G.playedCardCount;
  int origDeckCount = G.deckCount[thisPlayer];
  int origHandCount = G.handCount[thisPlayer];

  // set first card to Smithy
  testG.hand[thisPlayer][0] = smithy;

  //printf("handCount before call: %d\n", testG.handCount[thisPla*yer]);
  // Call the Smithy card effect on test state
  int cardEffectReturn = cardEffect(smithy, choice1, choice2, choice3, &testG, handPos, &bonus);
  //printf("handCount after call: %d\n", testG.handCount[thisPlayer]);

  // Player should now have 3 more cards in their hand and have played the Smithy card
  // These tests performs as expected, bugs in Smithy make it not discard Smithy and causes drawing 4 cards
  int newCards = 3;
  int discarded = 1;
  printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], origHandCount + newCards - discarded);
  if (assertTrue(testG.handCount[thisPlayer] == origHandCount + newCards - discarded))
    printf("smithy cardEffect() test: PASS, player's hand has gained three cards minus the expected played Smithy.\n");
  else {
    allTestsPassed = false;
    printf("smithy cardEffect() test: FAIL, hand count does not match expected value.\n");
  }

  // Confirm the three cards came from the player's deck
  printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], origDeckCount - newCards);
  if (assertTrue(testG.deckCount[thisPlayer] == origDeckCount - newCards))
    printf("smithy cardEffect() test: PASS, player's deck count reduced by 3 cards.\n");
  else {
    allTestsPassed = false;
    printf("smithy cardEffect() test: FAIL, player's deck count does not match expected value.\n");
  }

  // Confirm a card was played and that played card was Smithy
  if(assertTrue(testG.playedCardCount == origPlayedCount + 1)) {
    printf("smithy cardEffect() test: PASS, a card was added to played cards.\n");

    if(assertTrue(testG.playedCards[testG.playedCardCount - 1] == smithy))
      printf("smithy cardEffect() test: PASS, card played was smithy.\n");
    else {
      allTestsPassed = false;
      printf("smithy cardEffect() test: FAIL, card played was not smithy.\n");
    }
  }
  else {
    allTestsPassed = false;
    printf("smithy cardEffect() test: FAIL, a card was not added to played cards.\n");    
  }

  // Confirm no state change for kingdom/victory card piles
  bool supplyChange = false;
  
  for (int supplyPos = adventurer; supplyPos <= treasure_map; supplyPos++) {
    if (testG.supplyCount[supplyPos] == G.supplyCount[supplyPos])
      continue;
    else {
      supplyChange = true;
    
      printf("supply count of %d = %d, expected = %d\n", supplyPos, testG.supplyCount[thisPlayer], G.supplyCount[thisPlayer]);
      break;
    }
  }

  // If there was a change in supply, test is a failure
  if (supplyChange) {
    allTestsPassed = false;
    printf("smithy cardEffect() test: FAIL, supply count has unexpectedly changed.\n");
  }
  else
    printf("smithy cardEffect() test: PASS, supply count of all cards has remained the same.\n");

  // Confirm no state change for other player's
  
  // Check if the cardEffect() function returned 0
  if (assertTrue(cardEffectReturn == 0))
    printf("smithy cardEffect() test: PASS, cardEffect() returned 0.\n");
  else {
    allTestsPassed = false;
    printf("smithy cardEffect() test: FAIL, cardEffect() did not return 0..\n");
  }

  // If all tests passed or otherwise print a statement accordingly
  if(allTestsPassed)
    printf("smithy cardEffect(): All tests passed!\n");
  else
    printf("smithy cardEffect(): All tests complete with failure(s)!\n");
}
