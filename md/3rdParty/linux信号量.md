```c++
#ifndef UZSEM_H
#define UZSEM_H

#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sys/sem.h> // 需要的头文件

#define UZSEM_KEY 0x1234

class UZSem
{
public:
    static UZSem *getSingleton();

    int destroy();
    int wait();
    int post();

private:
    UZSem();
    UZSem(const UZSem &) = delete;
    UZSem& operator=(const UZSem &) = delete;
    ~UZSem();

    union semun  // 用于信号灯操作的共同体。
    {
        int val;
        struct semid_ds *buf;
        unsigned short *arry;
    };

    int init();

private:
    static UZSem *g_singleton;
    int m_semId;
};



#endif // UZSEM_H

```

```c++
#include "uzsem.h"

UZSem* UZSem::g_singleton = nullptr;

UZSem *UZSem::getSingleton()
{
    if (g_singleton == nullptr)
    {
        g_singleton = new UZSem();
    }
    return g_singleton;
}

UZSem::UZSem()
{
    init();
}

UZSem::~UZSem()
{
    destroy();
}

int UZSem::init()
{
    // 获取信号灯。 int semget(key_t key,int nsems,int semflg);
    // key是自定义的唯一字典值，nsems需要多少个信号量，semflg是权限
    if ((m_semId = semget(UZSEM_KEY, 1, 0640)) == -1) 
    {
        // 如果信号灯不存在，创建它。
        if(errno == 2)
        {
            // IPC_CREAT，没有则创建
            if ((m_semId = semget(UZSEM_KEY, 1, 0640 | IPC_CREAT)) == -1)
            {
                perror("init 1 semget()");
                return false;
            }

            // 信号灯创建成功后，还需要把它初始化成可用的状态。
            union semun sem_union;
            sem_union.val = 1;
            // int semctl(int semid,int semnum,int cmd,...);
            // semid 信号id，semnum信号的序号，cmd（SETVAL设置值，GETVAL获取值，IPC_RMID删除值）,sem_union自己的用户数据
            if(semctl(m_semId, 0, SETVAL, sem_union) < 0)
            {
                perror("init semctl()");
                return false;
            }
        }
        else
        {
            perror("init 2 semget()");
            return false;
        }
    }

    return 0;
}

int UZSem::destroy()
{
    if(semctl(m_semId, 0, IPC_RMID) == -1)
    {
        perror("destroy semctl()");
        return -1;
    }

    return 0;
}

int UZSem::wait()
{
    /*int semop(int semid，struct sembuf *sops，size_t nsops)
    参数：semid --》信号量的ID
         sops --》
         struct sembuf
         {
         	unsigned short sem_num;  //信号量的序号
            short sem_op;            //决定对信号量进行何种操作  P操作(减法)  -1  V操作(加法)  +1
            short sem_flg;           //一般设置SEM_UNDO 对信号量做了PV操作改变了值以后，重新运行程序，信号量的值又会自动恢复成原来的值
          }
         nsops --》结构体变量的个数struct sembuf，一般写1
    */
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;
    if(semop(m_semId, &sem_b, 1) == -1)
    {
        perror("wait semop()");
        return -1;
    }

    return 0;
}

int UZSem::post()
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;
    if(semop(m_semId, &sem_b, 1) == -1)
    {
        perror("post semop()");
        return false;
    }

    return 0;
}
```

