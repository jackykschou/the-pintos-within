#include "ChatManager.h"

std::string ChatManager::getTextForConsole() {
	std::stringstream stream;
	for (int i = 0; i < _log.size(); i++) {
		Message m = _log[i];
		stream << m.name << "> " << m.message << "\n";
	}
	return stream.str();
}

void ChatManager::addMessage(const char* name, const char* msg) {
	Message m;
	strncpy(m.name, name, sizeof(m.name));
	strncpy(m.message, msg, sizeof(m.message));
	_log.push_back(m);
}

void ChatManager::clear() {
	_log.clear();
}

int ChatManager::size() {
	return _log.size();
}
