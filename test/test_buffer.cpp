#include "buffer.hpp"
#include <iostream>

using std::cout;
using std::endl;

void test_construct () {
    small_buffer sb;
    large_buffer lb;
    small_buffer* psb = new small_buffer;
    large_buffer* plg = new large_buffer;
    delete psb; 
    delete plg;
}

void test_consecutive_write() {
    small_buffer sb;
    large_buffer lb;


    const char* str = "test consecutive buffer";
    int str_len = strlen(str); 

    while(sb.write(str, str_len)) {}
    const char* data  = sb.get_data();
    cout << data << endl;
    cout << "length of data : " << strlen(data) << endl;
    cout << "current position of data : " << sb.get_current_position() << endl;

    while(lb.write(str, str_len)) {}
    data  = lb.get_data();
    cout << "length of data : " << strlen(data) << endl;
    cout << "current position of data : " << lb.get_current_position() << endl;


}


int main(){
    test_construct();
    test_consecutive_write();
}