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

void testCardEffectAdventurer(int thisPlayer, struct gameState* G)
{
  printf("----------------- Testing Card: Adventurer ----------------\n");
  
  int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  struct gameState testG;
  bool allTestsPassed = false;
    
  // void adventurerCardEffect(int drawntreasure, struct gameState *state, int currentPlayer, int temphand[])
  
  // copy the game state to a test case
  memcpy(&testG, G, sizeof(struct gameState));
  
  //int origDiscardCount = G.discardCount[thisPlayer];
  int origPlayedCount = G->playedCardCount;
  
  // set first card to Adventurer
  testG.hand[thisPlayer][0] = adventurer;

  // If our hand is empty, increment handCount by 1 effectively adding Adventurer to the hand
  if (testG.handCount[thisPlayer] == 0) {
    G->handCount[thisPlayer]++;
    testG.handCount[thisPlayer]++;
  }
  
  //printf("handCount before call: %d\n", testG.handCount[thisPlayer]);
  // Call the Adventurer card effect on test state
  int cardEffectReturn = cardEffect(adventurer, choice1, choice2, choice3, &testG, handPos, &bonus);
  //printf("handCount after call: %d\n", testG.handCount[thisPlayer]);
  
  // Confirm two cards were drawn
  int newCards = 2;
  int discarded = 1;
  printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G->handCount[thisPlayer] + newCards - discarded);
  if (assertTrue(testG.handCount[thisPlayer] == G->handCount[thisPlayer] + newCards - discarded))
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

int main() {
  int numPlayers = 2;
  struct gameState G;
  
  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (int n = 0; n < 2000; n++) {   
    // Randomize player #
    int p = floor(Random() * numPlayers);

    // Randomize player's deck/hand/discard
    initializeRandomGame(p, &G);

    // Test card on given player
    testCardEffectAdventurer(p, &G);
  }
}
