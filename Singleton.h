#ifndef __Singleton_h_
#define __Singleton_h_

template <typename N>
class Singleton {
  public:
	static N* instance() {
		if (Singleton<N>::_ptr == 0) {
			Singleton<N>::_ptr = new N();
		}
		return Singleton<N>::_ptr;
	}

  private:
	static N* _ptr;
};

template <typename N>
N* Singleton<N>::_ptr = 0;


#endif
