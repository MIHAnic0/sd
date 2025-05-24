#include <iostream>
#include <stdlib.h>
using namespace std; 


class Stack {
    int *arr, height=0, maxHeight;
public:
    Stack(int a) {
        maxHeight = a;
        arr = new int[a];
    }
    void push(int a) {
        if (height<maxHeight) {
            height++;
            arr[height] = a;
        }
    }
    int pop() {
        if (height > 0) {
            height--;
            return arr[height+1];
        }
        return 0;
    }
    void clear() {height=0;}
    int getHeight() const {return height;}
    ~Stack() {
        delete[] arr;
    }
};


class Queue {
    Stack *fir, *sec;
    bool pushRepeat, popRepeat;
    int height = 0, maxHeight;

    void transferFS() {
        int size = fir->getHeight();
        sec->clear();
        for (int i=0; i<size; i++) sec->push(fir->pop());
    }
    void transferSF() {
        int size = sec->getHeight();
        fir->clear();
        for (int i=0; i<size; i++) fir->push(sec->pop());
    }
public:
    Queue(int a) {
        maxHeight = a;
        fir = new Stack(a);
        sec = new Stack(a);
    }
    void push(int a) {
        popRepeat = false;
        if (pushRepeat == false) {
            pushRepeat = true;
            transferSF();
        }
        if (height<maxHeight) {
            height++;
            fir->push(a);
        }
    }
    int pop() {
        pushRepeat = false;
        if (popRepeat == false) {
            popRepeat = true;
            transferFS();
        }
        if (height > 0) {
            height--;
            return sec->pop();
        }
        return 0;
    }
    int getHeight() const {return height;}
    ~Queue() {
        delete fir;
        delete sec;
    }
};


main() {
    int size = 10;
    Queue conteiner(size);

    cout << "\nFilling the queue:" << endl;
    for (int i=0; i<size; i++) {
        conteiner.push(i);
        cout << i << " ";
    }
    cout << "\nPulling elements from the queue: " << endl;
    for (int i=0; i<size; i++) {
        cout << conteiner.pop() << " ";
    }
}
