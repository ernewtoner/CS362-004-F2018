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
#include "stdbool.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

// Discard card test
// int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)

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

    printf ("TESTING discardCard():\n");
    
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
     
    for (int player = 0; player < numPlayer; player++) {
      for (int trashFlag = 0; trashFlag < 2; trashFlag++) { 
	for (int handPos = 0; handPos < G.handCount[player]; handPos++)
	  {
	    // original counts
	    //int origSupplyCount = G.supplyCount[supplyPos];
	    int origDiscardCount = G.discardCount[player];
	    int origDeckCount = G.deckCount[player];
	    int origHandCount = G.handCount[player];
	    int origPlayedCount = G.playedCardCount;

	    bool lastCardDiscarded; // Bool to track if the last card in hand was discarded
	    int lastCard;
	    
	    if ( handPos == (G.handCount[player] - 1)
		 || G.handCount[player] == 1 ) {
	      lastCardDiscarded = false;
	      lastCard = G.hand[player][ (G.handCount[player] - 1)];
	    }
	    else
	      lastCardDiscarded = true;
      
	    // call discardCard
#if (NOISY_TEST == 1)
	    printf("Calling discardCard with supplyPos %d, toFlag %d, player %d\n", supplyPos, toFlag, player);
#endif
	    
	    // int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)
	    int discardCardReturn = discardCard(handPos, player, &G, trashFlag);

	    // Tests for not trashed (add to Played pile)
	    if (trashFlag < 1) {
	      // assert playedCards pile has increased in #, pre/post state
	     
	      // assert discarded card is on top of playedCards pile
	      assert(G.playedCards[origPlayedCount] == G.hand[player][handPos]);
	      
	      // assert # of playedCards has increased in #
	      assert(G.playedCardCount == origPlayedCount + 1);
	    }
	    
	    // assert handCount[currentPlayer] has decreased by 1
	    assert(G.handCount[player] == origHandCount - 1);
	    
	    // if card played is not last card in hand array or last card left in hand
	    if (!lastCardDiscarded) {
	      // assert discarded card has been replaced with previous last card in hand
	      assert(G.hand[player][handPos] == lastCard);
	      
	      // assert last card has been set to -1
	      assert(G.hand[player][G.handCount[player] - 1] == -1);
	    }
	    else
	      // assert played handPos is set to -1
	      assert(G.hand[player][handPos] == -1);

	    // ASSERT RETURNSSSSSSSSSSSSSSSS
	    assert(discardCardReturn == 0);

	    /*    else {
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
	    
	    }*/
	  }
      }
    }
        
    printf("All tests passed!\n");
    
    return 0;
}
