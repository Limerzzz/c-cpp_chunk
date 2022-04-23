#include <string>
#include <stdio.h>
#include <cstring>

int main(){
    char buf[128] ;
	std::string str;
	while(::scanf("%s", buf) != EOF){
		str.append(buf);
	}
	::printf("%s", str.c_str());
	return 0;
}
