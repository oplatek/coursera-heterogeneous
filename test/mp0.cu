// Copyright by the authors of course https://www.coursera.org/course/hetero
// Ask them for licence, I am using the code for testing purposes only
// Since the goal of this repo is to make local version of wb.h
#include	"wb.h"

//@@ The purpose of this code is to become familiar with the submission 
//@@ process. Do not worry if you do not understand all the details of 
//@@ the code.

int main(int argc, char ** argv) {
    int deviceCount;

    wbArg_read(argc, argv);

    cudaGetDeviceCount(&deviceCount);

    for (int dev = 0; dev < deviceCount; dev++) {
        cudaDeviceProp deviceProp;

        cudaGetDeviceProperties(&deviceProp, dev);

        if (dev == 0) {
            if (deviceProp.major == 9999 && deviceProp.minor == 9999) {
                wbLog(TRACE, "No CUDA GPU has been detected");
                return -1;
            } else if (deviceCount == 1) {
                //@@ WbLog is a provided logging API (similar to Log4J).
                //@@ The logging function wbLog takes a level which is either
                //@@ OFF, FATAL, ERROR, WARN, INFO, DEBUG, or TRACE and a
                //@@ message to be printed.
                wbLog(TRACE, "Hello Ondra");
                wbLog(TRACE, "There is 1 device supporting CUDA");
            } else {
                wbLog(TRACE, "There are is n devices supporting CUDA", deviceCount);
            }
        }

        // wbLog(TRACE, "Device ", dev, " name: ", deviceProp.name);
         wbLog(TRACE, "Device name: ", deviceProp.name);
        wbLog(TRACE, " Computational Capabilities: ", deviceProp.major);
        wbLog(TRACE, " Computational Capabilities: ...", deviceProp.minor);
        wbLog(TRACE, " Maximum global memory size: ", deviceProp.totalGlobalMem);
        wbLog(TRACE, " Maximum constant memory size: ", deviceProp.totalConstMem);
        wbLog(TRACE, " Maximum shared memory size per block: ", deviceProp.sharedMemPerBlock);
        wbLog(TRACE, " Maximum block dimensions [0]: ", deviceProp.maxThreadsDim[0]); 
        wbLog(TRACE, " Maximum block dimensions [1]: ", deviceProp.maxThreadsDim[1]);
        wbLog(TRACE, " Maximum block dimensions [2]: ", deviceProp.maxThreadsDim[2]);
        wbLog(TRACE, " Maximum grid dimensions 0: ", deviceProp.maxGridSize[0]);
        wbLog(TRACE, " Maximum grid dimensions 1: ", deviceProp.maxGridSize[1]);
        wbLog(TRACE, " Maximum grid dimensions 2: ", deviceProp.maxGridSize[2]);
        wbLog(TRACE, " Warp size: ", deviceProp.warpSize);
    }

    return 0;
}
