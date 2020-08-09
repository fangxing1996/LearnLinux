#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <wait.h>
using namespace std;
int main(){
    pid_t pid;
    pid = fork();
    if( pid > 0){
        cout << "father process! ";
        cout << "pid: " << getpid() << endl;
        int s;
        cout << "wait pid: " <<  wait(&s) << endl;
    }
    else if(pid == 0){
        cout << "child process! ";
        cout << "pid: " << getpid() << endl;
        execl("./n", NULL);
    }
    else{
        cout << "fork error!\n";
    }
    return 0;
}
