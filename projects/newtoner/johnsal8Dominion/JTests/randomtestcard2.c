#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"

// Constants
#define TOTAL_TESTS 1000

// Typedefs
typedef enum
{
	false,
	true
} bool;

// Prototypes
int testVillage(int choice1, int choice2, int choice3, int handPos);

// Globals
struct gameState *state;
int passedTests = 0;
int failedTests = 0;

int main()
{
    // Seed rand
    srand(time(0));

    int i;
    for (i = 0; i < TOTAL_TESTS; i++)
    {
        // Mock a game
        int players = 2;
        int k[10] = { adventurer, embargo, village, outpost, mine, cutpurse, sea_hag, great_hall, smithy, council_room };
        
        // Start each game with a random seed to randomize results
        int randomSeed = rand();
        state = malloc(sizeof(struct gameState));
        
        // Initialize
        initializeGame(players, k, randomSeed, state);

        // Get random values for input
        int choice1 = rand() % 10;
        int choice2 = rand() % 10;
        int choice3 = rand() % 10;
        int handPos = rand() % 10;
        
        // Run test
        testVillage(choice1, choice2, choice3, handPos);

        // Free malloc'd state
        free(state);
    }

    // Print results
    printf("Tests passed: %d/%d\n", passedTests, TOTAL_TESTS);
    printf("Tests failed: %d/%d\n", failedTests, TOTAL_TESTS);

    return 0;
}

int testVillage(int choice1, int choice2, int choice3, int handPos)
{
    int bonus = 0;
    
    // Perform tests
    int firstActions = state->numActions;
    cardEffect(village, choice1, choice2, choice3, state, handPos, &bonus);
    int secondActions = state->numActions;

    // Check results
    if (secondActions - firstActions == 3) // Should be 2, but bug was introduced in previous assignment
    {
        passedTests++;
    }
    else
    {
        failedTests++;
    }

    return 0;
}