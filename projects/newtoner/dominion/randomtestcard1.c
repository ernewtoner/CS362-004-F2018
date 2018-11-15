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
#include <math.h>

int assertTrue(int condition) {
  if (condition > 0)
    return 1;
  else
    return 0;
}

////////////////////////////////////////////////////////
/// Initialize Random Game
/// Deck/hand/discard are randomized for one player
////////////////////////////////////////////////////////
void initializeRandomGame(int p, struct gameState *G) {
    G->whoseTurn = p;
  
    // Randomize deck, discard, and hand counts
    int randomDiscardCount = floor(Random() * MAX_DECK);
    int randomDeckCount = floor(Random() * MAX_DECK);
    int randomHandCount = floor(Random() * randomDeckCount);

    G->handCount[p] = 0; // Initialize to 0 since we'll draw the cards from the deck
    
    G->deckCount[p] = randomDeckCount;
    G->discardCount[p] = randomDiscardCount;

    /*printf("handcount: %d, randomvar %d\n", G->handCount[p], randomHandCount);
    printf("deckcount: %d, randomvar %d\n", G->deckCount[p], randomDeckCount);
    printf("discardcount: %d, randomvar %d\n", G->discardCount[p], randomDiscardCount);*/
    
    // Randomize actual cards in player's deck
    for (int j = 0; j < randomDeckCount; j++)
      G->deck[p][j] = (Random() * 27); // 0 - 26 for adventurer to treasure_map
   
    // Randomize actual cards in player's discard pile
    for (int j = 0; j < randomDiscardCount; j++)
      G->discard[p][j] = (Random() * 27); // 0 - 26 for adventurer to treasure_map
    
    // Draw up to the randomized hand count
    for (int it = 0; it < randomHandCount; it++){
      drawCard(p, G);
    }

    /* printf("handcount: %d, randomvar %d\n", G->handCount[p], randomHandCount);
    printf("deckcount: %d, randomvar %d\n", G->deckCount[p], randomDeckCount);
    printf("discardcount: %d, randomvar %d\n", G->discardCount[p], randomDiscardCount);*/
   
    //initialize first player's turn
    G->outpostPlayed = 0;
    G->phase = 0;
    G->numActions = 1;
    G->numBuys = 1;
    G->playedCardCount = 0;
  
    // Update coins for the new hand
    updateCoins(p, G, 0);

    printf("\n---- Random game state initialized ----\n");
    printf("deckCount: %d\n", G->deckCount[p]);
    printf("discardCount: %d\n", G->discardCount[p]);
    printf("handCount: %d\n", G->handCount[p]);
}

void testCardEffectSmithy(int thisPlayer, struct gameState* G) {
 printf("----------------- Testing Card: Smithy ----------------\n");
 
 int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
 struct gameState testG;
 bool allTestsPassed = false;

  // copy the game state to a test case
  memcpy(&testG, G, sizeof(struct gameState));

  // original counts
  int origPlayedCount = G->playedCardCount;
  int origDeckCount = G->deckCount[thisPlayer];
  int origDiscardCount = G->discardCount[thisPlayer];
  int origHandCount = G->handCount[thisPlayer];

  // set first card to Smithy
  testG.hand[thisPlayer][0] = smithy;

   // If our hand is empty, increment handCount by 1 effectively adding Smityh to the hand
  if (testG.handCount[thisPlayer] == 0) {
    G->handCount[thisPlayer]++;
    origHandCount++;
    testG.handCount[thisPlayer]++;
  }
 
  //printf("handCount before call: %d\n", testG.handCount[thisPlayer]);
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

  // In case shuffling discard into deck has occurred
  int expectedDeckCount = origDeckCount - newCards;
  if (expectedDeckCount < 0) {
    //printf("TRIGGERED!\n");
    expectedDeckCount = origDiscardCount - abs(expectedDeckCount);
  }
  
  // Confirm the three cards came from the player's deck
  printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], expectedDeckCount);
  if (assertTrue(testG.deckCount[thisPlayer] == expectedDeckCount))
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
    if (testG.supplyCount[supplyPos] == G->supplyCount[supplyPos])
      continue;
    else {
      supplyChange = true;
    
      printf("supply count of %d = %d, expected = %d\n", supplyPos, testG.supplyCount[thisPlayer], G->supplyCount[thisPlayer]);
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

int main() {
  int numPlayers = 2;
  struct gameState G;

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (int n = 0; n < 2000; n++) {
    // Randomize player #
    int p = floor(Random() * numPlayers);

    //for (int i = 0; i < sizeof(struct gameState); i++) {
    //  ((char*)&G)[i] = floor(Random() * 256);
    //}
    /*G.numPlayers = 2;
    int p = floor(Random() * 2);
    G.whoseTurn = p;
    G.deckCount[p] = floor(Random() * MAX_DECK);
    printf("deckCount: %d\n", G.deckCount[p]);
    G.discardCount[p] = floor(Random() * MAX_DECK);
    printf("discardCount: %d\n", G.discardCount[p]);
    G.handCount[p] = floor(Random() * MAX_HAND);
    printf("handCount: %d\n", G.handCount[p]);*/

    // Randomize player's deck/hand/discard
    initializeRandomGame(p, &G);

    // Test card on given player
    testCardEffectSmithy(p, &G);
  }
}
