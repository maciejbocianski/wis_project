#include "mbed.h"

Mutex mtx;

Thread thread1(osPriorityNormal, 512, nullptr, "thread1");
void thread1_fun()
{
    while (true) {
        mtx.lock();
        printf("1111111111\n");
        mtx.unlock();
    }
}

Thread thread2(osPriorityNormal, 512, nullptr, "thread2");
void thread2_fun()
{
    while (true) {
        mtx.lock();
        printf("2222222222\n");
        mtx.unlock();
    }
}

Thread thread3(osPriorityNormal, 512, nullptr, "thread3");
void thread3_fun()
{
    while (true) {
        mtx.lock();
        printf("33333333\n");
        mtx.unlock();
    }
}

Thread thread4(osPriorityNormal, 512, nullptr, "thread4");
void thread4_fun()
{
    int abc = 8;
    while (true) {
        mtx.lock();
        printf("44444444444\n");
        mtx.unlock();
    }
}

Thread thread5(osPriorityNormal, 512, nullptr, "thread5");
void thread5_fun()
{
    while (true) {
        mtx.lock();
        printf("555555555\n");
        int *ptr = nullptr;
        *ptr = 8; // should crash here
        mtx.unlock();
    }
}

int main()
{
    thread1.start(thread1_fun);
    thread2.start(thread2_fun);
    thread3.start(thread3_fun);
    thread4.start(thread4_fun);
    thread5.start(thread5_fun);

    while (true) {
        mtx.lock();
        printf("mainmain\n");
        mtx.unlock();
    }

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    return 0;
}
