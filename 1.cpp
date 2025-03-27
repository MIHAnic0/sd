#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std; 

void exitProgram(bool a) {
    if (!a) cout<<"NO"<<endl;
    cout<<"Приходько Михаил Анатольевич"<<endl;
    cout<<"090301-ПОВа-о24"<<endl;
    getch();
}

//========= main ================
main() {
    int N, a=0;
    clock_t timer;
    double time;

    while (true) {
        cout<<"Enter N (1<=N<=10^6): "; cin>>N;
        if (1<=N && N<=1000000) break;
        else cout<<"Invalid input"<<endl;
    }

    timer = clock();

    switch (N%10) {
        case 1:
        case 3:
        case 7:
        case 9:
            for (int i=0; i<=N; i++) {
                a = (a*10+1)%N;
                if (a==0) {
                    for (int j=0; j<=i; j++) cout<<1;
                    cout<<" ("<<i+1<<")"<<endl;
                    time = (clock()-timer); time/=CLOCKS_PER_SEC;
                    cout<<"Time result: "<<time<<" sec"<<endl;
                    exitProgram(true);
                    return 0;
                }
            }
    }
    time = (clock()-timer); time/=CLOCKS_PER_SEC;
    cout<<"Time result: "<<time<<" sec"<<endl;
    exitProgram(false);
}