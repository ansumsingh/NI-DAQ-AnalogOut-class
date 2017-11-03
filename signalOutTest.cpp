#include "signalOut.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
#include <conio.h>



#define PI 3.14159
int main()
{	
	using namespace std::this_thread; // sleep_for, sleep_until
	using namespace std::chrono; // nanoseconds, system_clock, seconds

	try{
		PortFactory& factory = PortFactory::Instance();
		analogOut& port1 = factory.GetAnalogOut1();
		analogOut& port0 = factory.GetAnalogOut0();
		float64 minVolt = -0.0;
		float64 maxVolt = 0.1;
		std::vector<float64> inputPort1;
		std::vector<float64> inputPort0;
		for (auto i = 0; i < 100; i++)
		{ 
			inputPort1.push_back(sin(2 * PI*i / 100));
			inputPort0.push_back(cos(2 * PI*i / 100));
		
		}
		

		operationResult op1Set1 = port1.setminVolt(-10.0);
		if (!op1Set1.isOK())
			std::cout << "Failed to set";
		operationResult op1Set2 = port1.setmaxVolt(10.0);
		if (!op1Set1.isOK())
			std::cout << "Failed to set";

		int count = 0;
		while(!kbhit())
		{
			
				operationResult op1=port1.setAnalogOut(inputPort1[count++]);
				//port0.setAnalogOut(1.1);
				//operationResult op0=port0.setAnalogOut(inputPort0[count++]);
				//std::cout <<"\nName of port:"<<port1.getPortName()<<std::endl<<"Min Volt: "<< port1.getminVolt()<<"Max Volt: "<<port1.getmaxVolt()<<std::endl;
				if (!op1.isOK())
					std::cout << "\nerror" << op1.getErrorMssg();
				else
					std::cout << "\n No error.";
				//if (!op0.isOK())
				//	std::cout << op0.getErrorMssg();
				
				if (count >= 100)
					count = 0;
				sleep_for(milliseconds(10));
				
		}
		//signalOut A0(DACportNo::port1());
		//signalOut A1(DACportNo::port1());
	}
	catch (std::exception& e)
	{
		std::cout <<"Exception"<< e.what();
	}
	



	return 1;
}