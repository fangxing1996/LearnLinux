#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
using namespace std;
int main(){
    cout << "exec program! ";
    cout << "pid: " << getpid() << endl;
    return 0;
}
