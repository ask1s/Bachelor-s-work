#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <dis.hxx>
using namespace std;

class Server : public DimRpc
{
public:
    Server(string name) : DimRpc(name.c_str(), "C", "C")
    {
    }

private:
    string text = ""; // receive buffer
    string response = ""; // send buffer
    int size; // size of packages

    bool rerun = true;

    void rpcHandler()
    {
        text = getString(); // get data
        if (text == "666") // check if it's restart command
        {
            rerun = true;
        }
        if (rerun == true) // restart command
        {
            text = getString(); // get new buffer size
            rerun = false;
            size = stoi(text); // write it to int
            text = ""; // clear buffers
            response = "";
            for (int i = 0; i < size; i++)
            {
                response += "0"; // create new response
            }
        }
        else // send response
        {
            setData((char*)response.c_str());
        }
    }
};

int main()
{
    string serverName, serviceName;
    serverName = "DIM_Meranie";
    serviceName = "DIM_Meranie2";

    Server service(serviceName);
    DimServer::start(serverName.c_str());

    do
    {

    } while (getchar() != 0);

    return 0;
}
