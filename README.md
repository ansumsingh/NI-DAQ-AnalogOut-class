# NI-DAQ-AnalogOut-class
This is APIs to access the analog output port of NI-DAQ. Model Number NI USB-6001/6002/6003. This is a wrapper class for the NI DAQ C API. 
To download the NIDAQ C API visit: http://www.ni.com/download/ni-daqmx-16.0/6120/en/



# Setup:
IN CMakeList

1. ADD following part in CMakeLists:

```CMake
  set(NI_folder "Your system path to/National Instruments/NI-DAQ/DAQmx ANSI C Dev")
  set(NI_LIB_DIRS "${NI_folder}/lib/msvc")

  include_directories("${NI_folder}/include")
```
# Usage:
```C++
  PortFactory& factory = PortFactory::Instance();
  analogOut& port1 = factory.GetAnalogOut1();
  analogOut& port0 = factory.GetAnalogOut0();
  
  //To set the output port with value
  operationResult op1=port1.setAnalogOut(inputPort1[count++]);
```C++
