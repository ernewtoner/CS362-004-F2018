#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

// buyCard test suite
// int buyCard(int supplyPos, struct gameState *state)

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
    struct gameState G;

    printf ("TESTING buyCard():\n");
         
    for (int supplyPos = adventurer; supplyPos <= treasure_map; supplyPos++) {
	for (int coinCount = 0; coinCount <= 12; coinCount++) {
	  memset(&G, 23, sizeof(struct gameState));   // clear the game state
	  int r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	  assertTrue(r == 0);

	  //struct gameState pre;
	  //memcpy (&pre, G, sizeof(struct gameState));
	  
	  int player = G.whoseTurn;
	  int cardCost = getCost(supplyPos);

	  G.coins = coinCount;
	  
	  // original counts
	  int origSupplyCount = G.supplyCount[supplyPos];
	  int origDiscardCount = G.discardCount[player];
	  //printf("cardCost: %d, origCoins: %d\n", cardCost, coinCount);
	  
	  int origNumBuys = G.numBuys;
	  
	  //int origDeckCount = G.deckCount[player];
	  //int origHandCount = G.handCount[player];
	  
	  // call buyCard
#if (NOISY_TEST == 1)
	  printf("Calling buyCard with supplyPos %d, toFlag %d, player %d\n", supplyPos, toFlag, player);
#endif	  
	  int buyCardReturn = buyCard(supplyPos, &G);
	  
	  // assert -1 return for fail conditions
	  if (origNumBuys < 1 || origSupplyCount < 1 || coinCount < cardCost) {
	    assertTrue(buyCardReturn == -1);
	    assertTrue(G.discardCount[player] == origDiscardCount);
	    assertTrue(G.supplyCount[supplyPos] == origSupplyCount);
	    printf("buyCard() test: PASS, function returned -1 as expected and no state change occurred.\n");
	  }
	  else {
	    // assert phase set to 1
	    assertTrue(G.phase == 1);
	    
	    // assert card goes in discard?
	    //printf("%d %d\n", G.discard[player][origDiscardCount], supplyPos);
	    assertTrue(G.discard[player][origDiscardCount] == supplyPos);
	    assertTrue(G.discardCount[player] == origDiscardCount + 1);
	    printf("buyCard() test: PASS, card added to discard.\n");
	    
	    //printf("coins: %d, cardCost: %d, origCoins: %d\n", G.coins, cardCost, origCoins);
	    // assert coins subtracted by cost
	    assertTrue(G.coins == (coinCount - cardCost));
	    printf("buyCard() test: PASS, card cost subtracted from coins sucessfully.\n");
	    
	    // assert numBuys decreases
	    assertTrue(G.numBuys == (origNumBuys - 1));
	    printf("buyCard() test: PASS, numBuys decreased by 1.\n");
	  }	
	}
      }
    
    printf("All tests passed!\n");
    
    return 0;
}
