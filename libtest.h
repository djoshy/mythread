typedef void *huh;
typedef void *MyThread;
void hmm(huh t);
void MyThreadInit(void(*start_funct)(void *), void *args);
MyThread MyThreadCreate(void(*start_funct)(void *), void *args);
void MyThreadYield(void);
void MyThreadExit(void);