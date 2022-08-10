#include "Manager.h"

int main()
//int main(int argc, char* argv[])
{
    /*const char* commandFilepath = "command.txt";
    if (argc > 1)
        commandFilepath = argv[1];
		*/
    Manager m;			//create manager object
    m.Run("command.txt");	//run manager

    return 0;
}
