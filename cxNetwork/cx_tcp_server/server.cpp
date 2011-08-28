#include "server.h"

bool serverThread(void *, Socket_Base_Stream * v, const char * remotePair)
{
    printf("New connection from %s:%d\n", remotePair, v->getRemotePort());
    fflush(stdout);

    //////////////////////////////////////////
    bool readOK = false;
    std::string x;
    while (1)
    {
        x = v->readString(&readOK);
        if (readOK)
        {
            printf("CMD: %s\n", x.c_str());
            fflush(stdout);

            std::string ans;

            if (x == "quit")
            {
                printf("QUIT command executed, bye...\n");
                return true;
            }
            else if (x == "date")
            {
                char cxtime[128];
                time_t t = time(nullptr);
                ctime_r(&t, cxtime);
                ans = cxtime;
            }
            else if (x == "pid")
            {
                char cpid[128];
                sprintf(cpid, "%d", getpid());
                ans = cpid;
            }
            v->writeString8(ans);

        }
        else
        {
            printf("Connection Finished... ;)\n");
            fflush(stdout);
            return true;
        }
    }
    return true;
}
