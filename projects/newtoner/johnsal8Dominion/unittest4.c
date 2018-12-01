/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdbool.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

// Discard card test
// int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)

int assertTrue(int condition) {
  if (condition > 0)
    return 1;
  else
    return 0;
}

int main() {
    int seed = 1000;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, testG;
    bool allTestsPassed = true;

    printf ("TESTING discardCard():\n");
     
    for (int player = 0; player < numPlayer; player++) {
      for (int trashFlag = 0; trashFlag < 2; trashFlag++) {
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	int r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	// assert init?
	assertTrue(r == 0);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	
	//while (G.handCount[player] != 0)
	for (int handPos = 0; handPos < G.handCount[player]; handPos++)
	  {
	    //printf("HANDCOUNT: %d", G.handCount[player]);
	    // original counts
	    int origHandCount = G.handCount[player];
	    //int origSupplyCount = G.supplyCount[supplyPos];
	    //int origDiscardCount = G.discardCount[player];
	    //    int origDeckCount = G.deckCount[player];
	    int origPlayedCount = G.playedCardCount;
	      
	    bool lastCardDiscarded; // Bool to track if the last card in hand was discarded
	    int lastCard;
	    
	    if ( (handPos == (G.handCount[player] - 1))
		 || G.handCount[player] == 1 ) {
	      lastCardDiscarded = true;
	      lastCard = G.hand[player][ (G.handCount[player] - 1)];
	    }
	    else {
	      lastCardDiscarded = false;
	      lastCard = G.hand[player][handPos];
	    }
	    // call discardCard
#if (NOISY_TEST == 1)
	    printf("Calling discardCard with handPos %d, player %d, trashFlag %d\n", handPos, player, trashFlag);
#endif
	    
	    // int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)
	    int discardCardReturn = discardCard(handPos, player, &G, trashFlag);

	    // Tests for not trashed (add to Played pile)
	    if (trashFlag < 1) {
	      // assert playedCards pile has increased in #, pre/post state
	      if(assertTrue(G.playedCardCount == origPlayedCount + 1)) {
		printf("discardCard() test: PASS, playedCards pile has increased by one.\n");
	      }
	      else {
		allTestsPassed = false;
		printf("discardCard() test: FAIL, playedCards pile has not increased by one.\n");
	      }
	      
	      // assert discarded card is on top of playedCards pile
	      if(assertTrue(G.playedCards[origPlayedCount] == lastCard))
		printf("discardCard() test: PASS, discarded card is on top of played cards pile.\n");
	      else {
		allTestsPassed = false;
		printf("discardCard() test: FAIL, discarded card is not on top of played cards pile.\n");
	      }
	    }
	  
	    // assert handCount[currentPlayer] has decreased by 1
	    if (assertTrue(G.handCount[player] == origHandCount - 1))
		printf("discardCard() test: PASS, player's handcount has been reduced by one.\n");
	      else {
		allTestsPassed = false;
		printf("discardCard() test: FAIL, player's handcount was not reduced by one.\n");
	      }
	    
	    // if card discarded is not last card in hand array or last card left in hand
	    if (!lastCardDiscarded) {
	      // assert discarded card has been replaced with previous last card in hand
	      assertTrue(G.hand[player][handPos] == lastCard);
	      
	      // assert last card has been set to -1
	      assertTrue(G.hand[player][G.handCount[player]] == -1);
	    }
	    else
	      // assert played handPos is set to -1
	      assertTrue(G.hand[player][handPos] == -1);

	    // assert successful function return
	    assertTrue(discardCardReturn == 0);
	  }
      }
    }

    // If all tests passed or otherwise print a statement accordingly
    if(allTestsPassed)
      printf("discardCard(): All tests passed!\n");
    else
      printf("discardCard(): All tests complete with failure(s)!\n");
    
    return 0;
}
