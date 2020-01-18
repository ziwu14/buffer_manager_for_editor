
#include <iostream>
#include <fstream>
#include <iterator>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum CommandType
{
    type_insert,
    type_erase,
    type_replace
};

class Command
{
public:
    Command() {}
    virtual ~Command() {}
    virtual void execute(string &text) const = 0;
    virtual CommandType getCommandType() const = 0;
};

class Insert : public Command
{
private:
    string substring;
    unsigned int position;

public:
    //constructor & destructor
    Insert(const string &s, unsigned int p) : substring(s), position(p) {}
    virtual ~Insert() {}
    //methods
    virtual void execute(string &text) const { text.insert(position, substring); }
    virtual CommandType getCommandType() const { return type_insert; }
    //getters
    string getSubstr() const {return substring;}
    unsigned int getPosition() {return position;}
};

class Erase : public Command
{
private:
    unsigned int size, position;

public:
    //constructor & destructor
    Erase(unsigned int n, unsigned p) : size(n), position(p) {}
    virtual ~Erase() {}
    //methods
    virtual void execute(string &text) const { text.erase(position, size); }
    virtual CommandType getCommandType() const { return type_erase; }
    //getters
    unsigned int getSize() const {return size;}
    unsigned int getPosition() const {return position;}
};

class Replace : public Command
{
private:
    string oldSubstr, newSubstr;

public:
    //constructor & destructor
    Replace(const string &oldS, const string &newS) : oldSubstr(oldS), newSubstr(newS) {}
    virtual ~Replace() {}
    //methods
    virtual void execute(string &text) const
    {
        size_t it, oldSize = oldSubstr.size();
        while ((it = text.find(oldSubstr)) != std::string::npos)
            text.replace(it, oldSize, newSubstr);
    }
    virtual CommandType getCommandType() const { return type_replace; }
    //getters
    string getOldSubstr() const { return oldSubstr; }
    string getNewSubstr() const { return newSubstr; }
};

class BufferManager
{
private:
    string text;
    stack<Command *> stk[2]; //stk[0] for undos, stk[1] for redos
    void handleState(bool isUndo);

public:
    //constructor & destructor
    BufferManager() {}
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

BufferManager::~BufferManager()
{
    while(stk[0].size()) delete stk[0].top(), stk[0].pop();
    while(stk[1].size()) delete stk[1].top(), stk[1].pop();
}

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
    insert.execute(text);
    stk[0].push(new Erase(substring.size(), position));
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

    Erase erase(size, position);
    erase.execute(text);
    stk[0].push(new Insert(text.substr(position, size), position));
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
    replace.execute(text);
    stk[0].push(new Replace(newSubstr, oldSubstr));
}

void BufferManager::undo()
{
    handleState(true);
}

void BufferManager::redo()
{
    handleState(false);
}

void BufferManager::handleState(bool isUndo)
{
    //choose either undo or redo stack;
    int i, j;
    if (isUndo) i = 0, j = 1;
    else i = 1, j = 0;

    //base case
    if (stk[i].empty()) return;

    //get last command, execute it and push to redo stack
    auto cmd = stk[i].top(); stk[i].pop();
    cmd->execute(text);
    switch (cmd->getCommandType())
    {
        case type_insert: 
        {
            Insert * insert = dynamic_cast<Insert *>(cmd);
            auto size = insert->getSubstr().size();
            auto position = insert->getPosition();
            stk[j].push(new Erase(size, position)); 
            break;
        } 
        case type_erase:
        {
            Erase * erase = dynamic_cast<Erase *>(cmd);
            auto sz = erase->getSize();
            auto position = erase->getPosition();
            stk[j].push(new Insert(text.substr(position, sz), position)); 
            break;
        }
        case type_replace:
        {
            Replace * replace = dynamic_cast<Replace *>(cmd);
            stk[j].push(new Replace(replace->getNewSubstr(), replace->getOldSubstr())); 
            break;
        }
    }

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
    else cout << "open on success" << endl;
    if (infile.rdbuf()->in_avail() != 0)
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
    else cout << "open on success" << endl;
    outfile << text;
    outfile.close();
}

int main()
{
    BufferManager * bm = new BufferManager();
    bm->load("abc.txt");
    bm->append("abcde");//abcde
    bm->insert("abc", 1);//aabcbcde
    bm->eraseArbitrary(3, 1);//abcde
    bm->replaceAllWtih("abcde", "12345");//12345
    bm->eraseTrail(5);//
    bm->append("a");//a
    for (int i=0; i<5; ++i) bm->undo();
    for (int i=0; i<5; ++i) bm->redo();
    bm->save("abc.txt");
}
