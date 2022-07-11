#include <mdz_prg_service/application.h>
#include "config.h"
using namespace Mantids::Application;

class Main : public Application
{
public:
    void _shutdown()
    {
    }

    int _start(int , char *[], Arguments::GlobalArguments *globalArguments)
    {
        // Start
        return 0;
    }

    void _initvars(int , char *[], Arguments::GlobalArguments * globalArguments)
    {
        // init variables (pre-config):
        globalArguments->setInifiniteWaitAtEnd(true);

        globalArguments->setLicense("LGPLv3");
        globalArguments->setAuthor("Aarón Mizrachi");
        globalArguments->setEmail("aaron@unmanarc.com");
        globalArguments->setVersion(atoi(PROJECT_VER_MAJOR), atoi(PROJECT_VER_MINOR), atoi(PROJECT_VER_PATCH), "a");
        globalArguments->setDescription(PROJECT_DESCRIPTION);

        globalArguments->addCommandLineOption("Program", 't', "test" , "test option", "127.0.0.1" , Mantids::Memory::Abstract::Var::TYPE_IPV4 );
    }

    bool _config(int , char *argv[], Arguments::GlobalArguments * globalArguments)
    {
        printf("config test: %s\n", globalArguments->getCommandLineOptionValue("test")->toString().c_str());
        return true;
    }

};

int main(int argc, char *argv[])
{
    Main * main = new Main;
    return StartApplication(argc,argv,main);
}

