#ifndef __BUFFERMANAGER_H__
#define __BUFFERMANAGER_H__


#include <string>
#include <stack>
#include "Command.hpp"

using namespace std;

class BufferManager
{
private:
    string text;
    stack<Command *> stk[2]; //stk[0] for undos, stk[1] for redos
    enum StackType 
    {
        kUndo = 0, kRedo = 1
    };
    void handleState(StackType mode);

public:
    //constructor & destructor
    BufferManager();
    ~BufferManager();
    //methods
    void insert(const string &substring, unsigned int position);
    void append(const string &substring);
    void eraseArbitrary(unsigned int size, unsigned position);
    void eraseTrail(unsigned int size);
    void replaceAllWtih(const string &oldSubstr, const string &newSubstr);
    void undo();
    void redo();
    void load(const string &fileName);
    void save(const string &fileName) const;
};

#endif
