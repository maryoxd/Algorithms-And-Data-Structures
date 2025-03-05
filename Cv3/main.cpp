#include <libds/mm/compact_memory_manager.h>
#include <iostream>


using namespace std;

struct Person
{
    int id_;
    string name_;

    virtual ~Person() = default;

    virtual void say_hello() { cout << "Som clovekom"; }
    
};

struct Student : Person
{
    int group_number_;

    void say_hello() override { cout << "Som studentom"; }

};

 int main()
 {
     /*::mm::CompactMemoryManager<int> cmm(10);
     *cmm.allocateMemory() = 1;
     *cmm.allocateMemory() = 2;
     *cmm.allocateMemory() = 3;

     cout << (((unsigned)cmm.getBlockAt(9)) & 0x0F) << "\n";

     cmm.print(cout);*/

     /*ds::mm::CompactMemoryManager<string> cmm(10);
     *cmm.allocateMemory() = "abcdefgh";
     *cmm.allocateMemory() = "milan";
     *cmm.allocateMemory() = "jozef";

     cmm.print(cout);*/

     ds::mm::CompactMemoryManager<Student> cmm;
	 Student* first = cmm.allocateMemory();
     Student* second = cmm.allocateMemory();
     Person* alsoFirst = first;

     cout << first << "\n";
     cout << alsoFirst << "\n";

	 alsoFirst->id_ = 255;
	 alsoFirst->name_ = "Milan";
	 first->group_number_ = 100;


     //cout << (int)*reinterpret_cast<unsigned char*>(first) << "\n";

     cmm.print(cout);


 }