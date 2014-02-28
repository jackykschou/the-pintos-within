#ifndef __Utility_h_
#define __Utility_h_

#define CHANCE(chance) \
  ((chance * 10000) >= (rand() % 10000 + 1))

#define RAND (((float) rand()) / (float) RAND_MAX)

#endif // #ifndef __Utility_h_