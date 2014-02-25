#ifndef __Debug_h_
#define __Debug_h_

#define LOG(message) \
  std::cout << "Log: " << message << std::endl

#define ASSERT(expression, message) \
  if(!expression) \
  	{ \
  		std::cout << "Assertion Fail: " << message << std::endl; \
  		exit(1); \
  	} \


#endif // #ifndef __Debug_h_