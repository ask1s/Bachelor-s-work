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
    int packages = 0;

    bool rerun = true; // restart bool 

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
            cout << packages << endl;;
            packages = 0;
            for (int i = 0; i < size / 8; i++)
            {
                response += "0"; // create new response
            }
        }
        else // send response
        {
            setData((char*)response.c_str());
            packages++;
        }
    }
};

int main()
{
    string serverName, serviceName;
    serverName = "DIM_Meranie";
    serviceName = "DIM_Meranie1";

    Server service(serviceName);
    DimServer::start(serverName.c_str());

    do
    {

    } while (getchar() != 0);

    return 0;
}