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

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

// Gain card test
// int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)

int main() {
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, r, handCount;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;

    printf ("TESTING gainCard():\n");
    
    // Call with full supply piles
    
    // int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)
    // call all supplypos - each card
    // test each toFlag (0,1,2) - discard, deck, hand
    // test each player (0,1)
    
    // assert discard, deck, hand have increased in #
    // assert card is where it needs to be
    
    // Call with empty supply piles
    // Assert no change in discard, deck, hand
    // Assert return value of -1
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
     
    for (int player = 0; player < numPlayer; player++) {
      for (int toFlag = 0; toFlag <= 2; toFlag++) { 
	for (int supplyPos = adventurer; supplyPos <= treasure_map; supplyPos++)
	  {
	    // original counts
	    int origSupplyCount = G.supplyCount[supplyPos];
	    int origDiscardCount = G.discardCount[player];
	    int origDeckCount = G.deckCount[player];
	    int origHandCount = G.handCount[player];

	    // call gainCard
#if (NOISY_TEST == 1)
    printf("Calling gainCard with supplyPos %d, toFlag %d, player %d\n", supplyPos, toFlag, player);
#endif	  
	    int gainCardReturn = gainCard(supplyPos, &G, toFlag, player);

	    // If there was no card in that supply pile to gain, assert -1 return and no changes in discard, deck, hand, or supply
	    if (origSupplyCount < 1) {
	      assert(gainCardReturn == -1);
	      assert(G.discardCount[player] == origDiscardCount);
	      assert(G.deckCount[player] == origDeckCount);
	      assert(G.handCount[player] == origHandCount);
	      assert(G.supplyCount[supplyPos] == origSupplyCount);
	      printf("gainCard() test: PASS, function called on empty supply and no state change occurred for any card piles.\n");
	    }
	    else {
	      // assert the card gained is where it should be
	      // assert discard, deck, or hand have increased in #
	      if (toFlag == 0) { // Discard {
		//printf("%d %d\n", G.discard[player][origDiscardCount], supplyPos);
		assert(G.discard[player][origDiscardCount] == supplyPos);
		assert(G.discardCount[player] == origDiscardCount + 1);
		printf("gainCard() test: PASS, card added to discard.\n");
	      }
	      else if (toFlag == 1) { // Deck
		assert(G.deck[player][origDeckCount] == supplyPos);
		assert(G.deckCount[player] == origDeckCount + 1);
		printf("gainCard() test: PASS, card added to deck.\n");
	      }
	      else if (toFlag == 2) { // Hand
		assert(G.hand[player][origHandCount] == supplyPos);
		assert(G.handCount[player] == origHandCount + 1);
		printf("gainCard() test: PASS, card added to hand.\n");
	      }

	      // assert the supply of gained card has decreased and gainCard returned 0
	      assert(G.supplyCount[supplyPos] == (origSupplyCount - 1));
	      assert(gainCardReturn == 0);
	      printf("gainCard() test: PASS, supply decreased and gainCard returned 0.\n");
	    }
	    
	  }
      }
    }
        
    printf("All tests passed!\n");
    
    return 0;
}
