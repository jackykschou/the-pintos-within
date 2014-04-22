#ifndef __ChatManager_h_
#define __ChatManager_h_

#include "common.h"
#include "Singleton.h"

struct Message {
	char name[24];
	char message[256];
};

class ChatManager: public Singleton<ChatManager> {
  public:
	std::string getTextForConsole();
	void addMessage(const char* name, const char* msg);
	void clear();
	int size();
  private:
	std::deque<Message> _log;
};

#endif
