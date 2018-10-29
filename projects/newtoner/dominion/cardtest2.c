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
  int k[10] = {treasure_map, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
  bool allTestsPassed = true;
  
  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Card: Adventurer ----------------\n");

  // void adventurerCardEffect(int drawntreasure, struct gameState *state, int currentPlayer, int temphand[])

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  //int origDiscardCount = G.discardCount[thisPlayer];
  int origPlayedCount = G.playedCardCount;
  
  // set first card to Adventurer
  testG.hand[thisPlayer][0] = adventurer;

  printf("Current hand (5 cards): %d %d %d %d %d\n", testG.hand[thisPlayer][ testG.handCount[thisPlayer]-5],
	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-4],
  	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-3],
	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-2],
  	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-1]);
  
  //printf("handCount before call: %d\n", testG.handCount[thisPlayer]);
  // Call the Adventurer card effect on test state
  int cardEffectReturn = cardEffect(adventurer, choice1, choice2, choice3, &testG, handPos, &bonus);
  //printf("handCount after call: %d\n", testG.handCount[thisPlayer]);

  // Confirm two cards were drawn
  int newCards = 2;
  int discarded = 1;
  printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
  if (assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded))
    printf("adventurer cardEffect() test: PASS, player has gained two cards and discarded one card.\n");
  else {
    allTestsPassed = false;
    printf("adventurer cardEffect() test: FAIL, hand count does not match expected value.\n");
  }

  // Confirm a card was played and that played card was Adventurer
  if(assertTrue(testG.playedCardCount == origPlayedCount + 1)) {
    printf("adventurer cardEffect() test: PASS, a card was added to played cards.\n");

    if(assertTrue(testG.playedCards[testG.playedCardCount - 1] == adventurer))
      printf("adventurer cardEffect() test: PASS, card played was adventurer.\n");
    else {
      printf("adventurer cardEffect() test: FAIL, card played was not adventurer.\n");
      allTestsPassed = false;
    }
  }
  else {
    allTestsPassed = false;
    printf("adventurer cardEffect() test: FAIL, a card was not added to played cards.\n");    
  }
   
  //printf("Top 2 cards: %d %d", testG.hand[thisPlayer][ testG.handCount[thisPlayer]-1],
  //	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-2]);

  printf("Current hand (7 cards): %d %d %d %d %d %d %d\n", testG.hand[thisPlayer][ testG.handCount[thisPlayer]-7],
	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-6],
	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-5],
	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-4],
  	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-3],
	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-2],
  	 testG.hand[thisPlayer][ testG.handCount[thisPlayer]-1]);
  
  // Confirm the drawn cards were treasure cards
  int cardDrawn1 = testG.hand[thisPlayer][ testG.handCount[thisPlayer]-2];
  int cardDrawn2 = testG.hand[thisPlayer][ testG.handCount[thisPlayer]-1];
 
  if(assertTrue(cardDrawn1 == copper || cardDrawn1 == silver || cardDrawn1 == gold))
    printf("adventurer cardEffect() test: PASS, first drawn card is a treasure card.\n");
  else {
    allTestsPassed = false;
    printf("adventurer cardEffect() test: FAIL, first drawn card is not a treasure card.\n");
    }

  if(assertTrue(cardDrawn2 == copper || cardDrawn2 == silver || cardDrawn2 == gold))
    printf("adventurer cardEffect() test: PASS, second drawn card is a treasure card.\n");
  else {
    allTestsPassed = false;
    printf("adventurer cardEffect() test: FAIL, second drawn card is not a treasure card.\n");
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
    printf("adventurer cardEffect() test: FAIL, supply count has unexpectedly changed.\n");
  }
  else
    printf("adventurer cardEffect() test: PASS, supply count of all cards has remained the same.\n");
  
  // Check if the cardEffect() function returned 0
  if (assertTrue(cardEffectReturn == 0))
    printf("adventurer cardEffect() test: PASS, cardEffect() returned 0.\n");
  else {
    allTestsPassed = false;
    printf("adventurer cardEffect() test: FAIL, cardEffect() did not return 0..\n");
  }

  // If all tests passed or otherwise print a statement accordingly
  if(allTestsPassed)
    printf("adventurer cardEffect(): All tests passed!\n");
  else
    printf("adventurer cardEffect(): All tests complete with failure(s)!\n");
}
