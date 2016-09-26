
typedef void *MyThread;
void MyThreadInit(void(*start_funct)(void *), void *args);
MyThread MyThreadCreate(void(*start_funct)(void *), void *args);
void MyThreadYield(void);
void MyThreadExit(void);
int MyThreadJoin(MyThread thread);