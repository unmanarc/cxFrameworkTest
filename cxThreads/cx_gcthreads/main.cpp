#include <cx_srv_service/service.h>

void _initvars(int argc, char *argv[], GlobalArguments * globalArguments)
{
    // init program vars.
    globalArguments->setInifiniteWaitAtEnd(true);

    globalArguments->setAuthor("Aarón Mizrachi");
    globalArguments->setEmail("aaron@unmanarc.com");
    globalArguments->setVersion("1.0.2");
    globalArguments->setDescription("Program TEST");

    // cmd options:
    globalArguments->addCommandLineOption("Program", 't', "test", "test option", "127.0.0.1", ABSTRACT_IPV4);
}

bool _config(int argc, char *argv[], GlobalArguments * globalArguments)
{
    // process config here.
    printf("config test: %s\n", globalArguments->getCommandLineOptionValue("test")->toString().c_str());
    return true;
}

int _start(int argc, char *argv[], GlobalArguments * globalArguments)
{
    return 0;
}
