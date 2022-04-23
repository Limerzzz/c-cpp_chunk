#include <iostream>

class base{
	public:
			void test( ){
				std::cout << "test func of base!" << std::endl;	
			}
			virtual void func(){
				std::cout << "func of base" << std::endl;	
			}
};
class drived:public base{
	public:
			void test(){
				std::cout << "test func of drived!" << std::endl;	
			}
			virtual void func(){
				std::cout << "func of drived" << std::endl;	
			}
};

int main(void){
		base* b = new drived;
		b->test();
	   b->func();	
	   return 0;
}
