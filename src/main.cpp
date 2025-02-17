#include "BufferManager.hpp"
#include <string>
#include <utility>

using namespace std;



int main()
{
    BufferManager * bm = new BufferManager();
    bm->load("test.txt");
    bm->append("abcde");//abcde
    bm->insert("abc", 1);//aabcbcde
    bm->eraseArbitrary(3, 1);//abcde
    bm->replaceAllWtih("abcde", "12345");//12345
    bm->eraseTrail(5);//
    bm->append("a");//a
    for (int i=0; i<5; ++i) bm->undo();
    for (int i=0; i<5; ++i) bm->redo();
    bm->save("test.txt");
   
}
