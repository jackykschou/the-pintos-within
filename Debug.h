#ifndef __Debug_h_
#define __Debug_h_

#define LOG(message) \
  ;

#define ASSERT(expression, message) \
  if(!expression) \
  	{ \
  		std::cout << "Assertion Fail: " << message << std::endl; \
  		exit(1); \
  	} \

#define DEBUG true

#endif // #ifndef __Debug_h_