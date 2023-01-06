#pragma comment(lib, "dim")

#include <iostream>
#include <Windows.h>
#include <dic.hxx>
#include <chrono>
#include <ctime>   
#include <stdio.h>
#include <memory.h>
#include <string>

using namespace std;

int main()
{
    string serviceName;

    serviceName = "DIM_Meranie2";

    char nolink[] = "No RPC link";
    DimRpcInfo service(serviceName.c_str(), nolink);

    int times = 1000000; // repeat times
    int size; // size of buffer
    string text = ""; // input text and send buffer
    string response; // receive buffer
    string restart = "666"; // restart command

    float min = 1000;  // min - minimal delay
    float max = -1000; // max - maximum delay
    float avg = 0;     // avg - average delay
    double ping = 0;   // spent time for transaction 


    while (1) // always true
    {
        cout << "Enter size of buffer: ";
        cin >> text; // new buffer size

        service.setData((char*)text.c_str()); // send it to server
        size = stoi(text); // write it to "size"
        text = ""; // clear it
        for (int i = 0; i < size; i++)
        {
            text += "0"; // making filled buffer
        }

        for(int i = 0; i < times; i++)
        {
            auto start = chrono::system_clock::now(); // start the timer

            service.setData((char*)text.c_str()); // send buffer to server
            response = service.getString(); // receive the response

            auto end = chrono::system_clock::now(); // after answer received, end the timer
            chrono::duration<double> elapsed_seconds = end - start; // calculate how much time was spent
            double ping = elapsed_seconds.count(); // write it to "ping"

            if (max == -1000 && min == 1000) // if it's first attempt, then "avg = delay"
                avg = ping;

            else // if not, then just a general formula 
                avg = (avg + ping) / 2;

            if (ping > max) // check if it is new maximum 
                max = ping;

            if (ping < min) // or minimum
                min = ping;
        }

        service.setData((char*)restart.c_str()); // send reset command

        cout << "Minimum: " << min << " s" << endl; // print results
        cout << "Maximum: " << max << " s" << endl;
        cout << "Average: " << avg << " s" << endl;

        avg = 0; // reset variables
        ping = 0;
        min = 1000;
        max = -1000;
        text = "";
    }

    return 0;
}
