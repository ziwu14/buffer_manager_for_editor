#include "Command.hpp"
#include <string>

using namespace std;

//constructors & destructors
Insert::Insert(const string &s, unsigned int p) : substring(s), position(p) {}
Insert::~Insert() {}

Erase::Erase(const string & s, unsigned p) : substring(s), position(p) {}
Erase::~Erase() {}

Replace::Replace(const string &oldS, const string &newS) : oldSubstr(oldS), newSubstr(newS) {}
Replace::~Replace() {}

//methods
void Insert::execute(string &text) const { text.insert(position, substring); }
void Erase::execute(string &text) const { text.erase(position, substring.size()); }
void Replace::execute(string &text) const
{
    size_t it, oldSize = oldSubstr.size();
    while ((it = text.find(oldSubstr)) != std::string::npos)
        text.replace(it, oldSize, newSubstr);
}

Command * Insert::createReverseCommand() { return new Erase(substring, position); }
Command * Erase::createReverseCommand() { return new Insert(substring, position); }
Command * Replace::createReverseCommand() { return new Replace(newSubstr, oldSubstr); }
