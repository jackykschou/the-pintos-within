#ifndef __BaseApplication_h_
#define __BaseApplication_h_

template <typename N>
class Singleton
{
public:
	static Singleton& instance();
};

#endif // #ifndef __BaseApplication_h_
