#include "BufferManager.hpp"
#include <string>
#include <stack>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

//constructors & destructors
BufferManager::BufferManager() {}

BufferManager::~BufferManager()
{
    while(stk[0].size()) delete stk[0].top(), stk[0].pop();
    while(stk[1].size()) delete stk[1].top(), stk[1].pop();
}

//methods
void BufferManager::insert(const string &substring, unsigned int position)
{
    //base case
    if (position < 0 || position > text.size())
    {
        cerr << "insert into invalid position" << endl;
        return;
    }
    if (substring.empty()) return;

    Insert insert(substring, position);
    stk[0].push(insert.createReverseCommand());
    insert.execute(text);
}

void BufferManager::append(const string &substring)
{
    insert(substring, text.size());
}

void BufferManager::eraseArbitrary(unsigned int size, unsigned int position)
{
    //base case
    if (position < 0 || position >= text.size())
    {
        cerr << "erase at invalid position" << endl;
        return;
    }
    if (size == 0) return;
    if (size < 0)
    {
        cerr << "erase with wrong size" << endl;
        return;
    }

    Erase erase(text.substr(position, size), position);
    stk[0].push(erase.createReverseCommand());
    erase.execute(text);
    
}

void BufferManager::eraseTrail(unsigned int size)
{
    eraseArbitrary(size, ((unsigned int)text.size()) - size);
}

void BufferManager::replaceAllWtih(const string &oldSubstr, const string &newSubstr)
{
    //base case
    if (oldSubstr.empty()) return;

    Replace replace(oldSubstr, newSubstr);
    stk[0].push(replace.createReverseCommand());
    replace.execute(text);  
}

void BufferManager::undo()
{
    handleState(kUndo);
}

void BufferManager::redo()
{
    handleState(kRedo);
}

void BufferManager::handleState(StackType mode)
{
    //base case
    if (stk[mode].empty()) return;
    //get last command, create reverse command
    auto cmd = stk[mode].top(); stk[mode].pop();
    stk[mode ^ 1].push(cmd->createReverseCommand());
    //execute command and free memory
    cmd->execute(text);
    delete cmd;
}

void BufferManager::load(const string &fileName)
{
    ifstream infile(fileName);
    infile.open(fileName);
    if (!infile.is_open()) 
    {
        cerr << "load(): Fail to open file: " + fileName << endl; 
        return;
    }
    else cout << "open for loading on success" << endl;
    text = string(istreambuf_iterator<char>(infile), istreambuf_iterator<char>());
    infile.close();
}

void BufferManager::save(const string &fileName) const
{
    ofstream outfile(fileName, std::ofstream::trunc);
    if (!outfile.is_open())
    {
        cerr << "save(): Fail to open file: " + fileName << endl; 
        return;
    }
    else cout << "open for saving on success" << endl;
    outfile << text;
    outfile.close();
}
