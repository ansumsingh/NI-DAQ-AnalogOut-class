#ifndef SIGNALOUT_H
#define SIGNALOUT_H


#include <NIDAQmx.h>
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <vector>


class DACexception :public std::exception{

public:
	DACexception(std::string errorMsg) :errorMsg(errorMsg){}
	virtual const char* what()const throw(){
		return errorMsg.c_str();
	}

private:
	
	std::string errorMsg;
	
};

class operationResult{
public:
	operationResult(std::string errMssg = std::string(), bool result = true) :
		errorMssg(errMssg),
		Result(result){}
	bool isOK(){ return Result; }
	std::string getErrorMssg() { return errorMssg; }
private:
	bool Result;
	std::string errorMssg;
};




class analogOut;




class analogOut{
  friend class PortFactory;
public:
	explicit analogOut(int portNum, TaskHandle , float64 , float64, float64 );
		
	float64 getminVolt() const  { return minVolt; }//get the current minVolt for the AO port
	float64 getmaxVolt() const  { return maxVolt; }//get the current minVolt for the AO port
	std::string getPortName() const { return port; }//get the port name for the AO port.

	//both of the setter functions throw DACexception if minVolt is more than maxVolt.
	operationResult setminVolt(float64);//will throw the out_of_range exception is minimum voltage is less than -10
	operationResult setmaxVolt(float64);//will throw the out_of_range exception is maximum voltage is more than 10
	operationResult setAnalogOut(float64);//will set the desired Analog voltage to the port.

	analogOut(analogOut&&) = delete;
	analogOut(const analogOut&) = delete;
	~analogOut(){ 
		deinitialize();
		//std::cout << "\n Destructor is called"; 
	}
	static int32 callbackFromSetAnalogOut(TaskHandle, int32, void*);

private:
	TaskHandle handle;
	float64 data;
	float64 minVolt;
	float64 maxVolt;
	float64 samplingTime;
	std::string port;

	//std::string port1;
	
	void initialize();
	void deinitialize()
	{
		if (handle != 0) {

			DAQmxStopTask(handle);
			DAQmxClearTask(handle);
		}
	}


};

operationResult analogOut::setminVolt(float64 minIP)
{
	if (minIP < -10)
		return operationResult("Minimum voltage should not be less than 10 V.",true);
	
	if (DAQmxFailed(DAQmxSetAOMin(handle, port.c_str(), minIP)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		DAQmxGetExtendedErrorInfo(perrorMsg, 2048);
		deinitialize();
		//throw the DACexception
		
		return  operationResult(errorMsg, false);
		
	}
	
	if(DAQmxFailed(DAQmxGetAOMin(handle, port.c_str(), &minVolt)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		DAQmxGetExtendedErrorInfo(perrorMsg, 2048);
		deinitialize();
		//throw the DACexception
		return  operationResult(errorMsg, false);

	}
	minVolt = minIP;
	return  operationResult();
}

operationResult analogOut::setmaxVolt(float64 maxIP)
{
	if (maxIP > 10)
		throw std::out_of_range("Maximum voltage should not be more than 10 V.");
	
	if (DAQmxFailed(DAQmxSetAOMax(handle, port.c_str(), maxIP)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		DAQmxGetExtendedErrorInfo(perrorMsg, 2048);
		deinitialize();
		//throw the DACexception
		return  operationResult(errorMsg, false);

	}
	
	if (DAQmxFailed(DAQmxGetAOMax(handle, port.c_str(), &maxVolt)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		DAQmxGetExtendedErrorInfo(perrorMsg, 2048);
		deinitialize();
		//throw the DACexception
		return  operationResult(errorMsg, false);

	}
	maxVolt = maxIP;
	return  operationResult();
}

analogOut::analogOut(int portNum, TaskHandle handleIP = NULL, float64 minVolt = -10.0, float64 maxVolt = 10.0,float64 samplingTime = 1000.0) :
//portNo(portNo),
handle(handleIP),
minVolt(minVolt),
maxVolt(maxVolt),
samplingTime(samplingTime)

{
	

	if ((portNum != 0) && (portNum != 1))
		throw std::out_of_range("port no should be 0 or 1");
	port = "Dev1/ao" + std::to_string(portNum);
	initialize();


}

void analogOut::initialize()
{

	
	if (DAQmxFailed(DAQmxCreateTask("", &handle)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		DAQmxGetExtendedErrorInfo(perrorMsg, 2048);
		deinitialize();
		throw DACexception(errorMsg);
		//throw the DACexception
	}


	if (DAQmxFailed(DAQmxCreateAOVoltageChan(handle, port.c_str(), "", minVolt, maxVolt, DAQmx_Val_Volts, NULL)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		DAQmxGetExtendedErrorInfo(perrorMsg, 2048);
		deinitialize();
		throw DACexception(errorMsg);
	}
	

}

//This function sets given voltage to the output port. If the setting voltage is greater or less than the limit of the port this will give 
//the error message.
operationResult analogOut::setAnalogOut(float64 val)
{
	int32 samplePerChannel=1;
	/*********************************************/
	// DAQmx Write Code
	/*********************************************/
	if (val>maxVolt||val<minVolt)
		return  operationResult("Input voltage is out of range.", false);
	
	//std::cout << "error no" << errorNo;
	
	if (DAQmxFailed(DAQmxWriteAnalogF64(handle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, &val, &samplePerChannel, NULL)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		char  temp[2048] = "";
		DAQmxGetExtendedErrorInfo(temp, 2048);
		//deinitialize();
		std::cout << temp;
		errorMsg = "Error in DAQmxWriteAnalogF64"+errorMsg;
		return  operationResult(errorMsg, false);
	}
	
	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
//Stop the task.	
	if (DAQmxFailed(DAQmxStopTask(handle)))
	{
		//get the error string
		std::string errorMsg(2048, '\0');
		char* perrorMsg = const_cast<char*>(errorMsg.c_str());
		DAQmxGetExtendedErrorInfo(perrorMsg, 2048);
		deinitialize();
		errorMsg += "DAQmxStopTask";
		throw DACexception(errorMsg);
		//errorMsg = "Error in DAQmxStartTask: " + errorMsg;
		//return  operationResult(errorMsg, false);
	}
	return operationResult();
}

int32 analogOut::callbackFromSetAnalogOut(TaskHandle taskHandle, int32 status, void *callbackData)
{
	if (DAQmxFailed(status))
		std::cout << "Error";
	else
		std::cout << "Sucessfully written to port"<<std::endl;

	return 0;
}

//Factory class: 
class PortFactory
{
public:
	//Make a static member function Instance
	static PortFactory& Instance() { 
		static PortFactory portFactory;
		return portFactory;
	}

	analogOut& GetAnalogOut0() { return port0; }
	analogOut& GetAnalogOut1() { return port1; }

private:
	//static std::vector<analogOut&> ports;
	analogOut port0;
	analogOut port1;

private:
	PortFactory() : port0(0), port1(1) {}//make this private so that other cannot initialize
	PortFactory(const PortFactory&) = delete; //disable the copy ctor
	PortFactory& operator=(const PortFactory&) = delete; //disable the disable assignment
};
#endif