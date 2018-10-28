#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
  // We'll stick to ASCII 32 - 125
  char randomChar = rand() % (126 - 32) + 32;
  
  return randomChar;
}

char *inputString()
{
  static char output[5];
  
  for (int i = 0; i < 5; i++) {
    // lowercase characters are ASCII 97 - 122
    // we'll stick to just 113 - 116 and count 113 as e
    // e = 101, r = 114, s = 115, t = 116
    int randomChar = rand() % (117 - 113) + 113;

    if (randomChar == 113)
      randomChar = 101;
  
    output[i] = randomChar;
  }
    
  return output;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
