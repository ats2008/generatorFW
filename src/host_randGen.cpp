#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

//#include "cmdlineparser.h"
#include <iostream>
#include <cstring>

// XRT includes
#include "ap_int.h"
#include "xrt/xrt_bo.h"
#include <experimental/xrt_xclbin.h>
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"


int main(int argc, char** argv)
{

    std::string binaryFile;
    std::cout << "argc = " << argc << std::endl;
    for(int i=0; i < argc; i++)
    {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    if (argc > 1)
    {
        binaryFile = argv[1];
    }
    else
    {
        binaryFile = "./krnl_vadd.sw_emu.xclbin";
    }

    // Read settings
    int device_index = 0;
    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    size_t vector_size_bytes = 2;

    //auto krnl = xrt::kernel(device, uuid, "vadd");
    auto krnl = xrt::kernel(device, uuid, "randWordGen16Bit");//, xrt::kernel::cu_access_mode::exclusive);

    std::cout << "Allocate Buffer in Global Memory\n";
    auto boIn1 = xrt::bo(device, vector_size_bytes, krnl.group_id(1)); //Match kernel arguments to RTL kernel

    // Map the contents of the buffer object into host memory
    auto bo0_map = boIn1.map<ap_uint<16> *>();
    std::fill(bo0_map, bo0_map + 1, 0);

    // Create the test data
    //int bufReference[DATA_SIZE];
    //for (int i = 0; i < DATA_SIZE; ++i) {
    //    bo0_map[i] = i;
    //    bo1_map[i] = i;
    //    bufReference[i] = bo0_map[i] + bo1_map[i]; //Generate check data for validation
    //}

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";
    boIn1.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    for(int i=0; i< 10000; i++)
    {
        std::cout << "  > Execution of the kernel : "<<i<<"\n";
        bool upd = (i%3)==0 ;
        auto run = krnl(upd,boIn1); //DATA_SIZE=size
        run.wait();
        std::cout << "  > Get the output data from the device" << std::endl;
        boIn1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        std::cout << "    > ["<<i<<"] "<<" Value read as : ";
        for(int j=0; j<5; j++)
            std::cout<<bo0_map[j]<<"  |  ";
        std::cout<<"\n";

    }

    // Get the output;
    //for (int i = 0; i < DATA_SIZE; ++i) {
    //		std::cout<<"  > Comparing "<<i<<" -> referance : "<<bufReference[i]<<",  bo2_map : "<<bo2_map[i]<<"\n";
    //}

    std::cout << "Existing the Host application ! \n";
    return 0;
}
