#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

using namespace std;

class Command
{
public:
    Command() {}
    virtual ~Command() {}
    virtual void execute(string &text) const = 0;
    virtual Command * createReverseCommand() = 0;
};


class Insert : public Command
{
private:
    string substring;
    unsigned int position;

public:
    //constructor & destructor
    Insert(const string &s, unsigned int p);
    virtual ~Insert();
    //methods
    virtual void execute(string &text) const;
    virtual Command * createReverseCommand();
};

class Erase : public Command
{
private:
    string substring;
    unsigned int position;

public:
    //constructor & destructor
    Erase(const string & s, unsigned p);
    virtual ~Erase();
    //methods
    virtual void execute(string &text) const;
    virtual Command * createReverseCommand();
};


class Replace : public Command
{
private:
    string oldSubstr, newSubstr;

public:
    //constructor & destructor
    Replace(const string &oldS, const string &newS);
    virtual ~Replace();
    //methods
    virtual void execute(string &text) const;
    virtual Command * createReverseCommand();
};

#endif
