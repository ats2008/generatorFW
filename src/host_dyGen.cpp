#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

//#include "cmdlineparser.h"
#include <iostream>
#include <cstring>
#include <fstream>

// XRT includes
#include "ap_int.h"
#include "xrt/xrt_bo.h"
#include <experimental/xrt_xclbin.h>
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"


#include <lsfr.h>

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

    size_t vector_size_bytes = N_DY_GEN*sizeof(ap_uint<32>);

    //auto krnl = xrt::kernel(device, uuid, "vadd");
    auto krnl = xrt::kernel(device, uuid, "drellYanPairGenerator");//, xrt::kernel::cu_access_mode::exclusive);

    std::cout << "Allocate Buffer in Global Memory\n";
    std::cout<<" krnl.group_id(0) : "<<krnl.group_id(0)<<"\n";
    std::cout<<" krnl.group_id(0) : "<<krnl.group_id(1)<<"\n";
    auto mu1 = xrt::bo(device, vector_size_bytes, krnl.group_id(0)); //Match kernel arguments to RTL kernel
    auto mu2 = xrt::bo(device, vector_size_bytes, krnl.group_id(1)); //Match kernel arguments to RTL kernel

    std::cout<<" got bo s \n";
    // Map the contents of the buffer object into host memory
    auto mu1_map = mu1.map<ap_uint<32> *>();
    //std::fill(mu1_map, mu1_map + vector_size_bytes, 0);
    auto mu2_map = mu2.map<ap_uint<32> *>();
    //std::fill(mu2_map, mu2_map + vector_size_bytes, 0);
    std::cout<<" done fillingup  \n";

    // Synchronize buffer content with device side
    //std::cout << "synchronize input buffer data to device global memory\n";
    //boIn1.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "  > Execution of the kernel : drellYanPairGenerator "<<"\n";
        auto run = krnl(mu1,mu2); //DATA_SIZE=size
        run.wait();
        std::cout << "  > Get the output data from the device" << std::endl;
        mu1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        mu2.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    
    randFW::muon mu;
    ofstream ofile("data.dat");
    for(size_t j=0;j<1024;j++) {
      
        if((j%8)==0)
            std::cout<<"  Making j="<<j<<" th loop ! \n";
        
        for(int i=0;i<N_DY_GEN;i++)
        {
            mu.unpack(mu1_map[i]);
            ofile<<i+j*1024<<",";
            ofile<<mu.pt<<","<<mu.eta<<","<<mu.phi<<",";
            mu.unpack(mu2_map[i]);
            ofile<<mu.pt<<","<<mu.eta<<","<<mu.phi<<"";
            ofile<<"\n";
            if((j%16)==0)
            if((i%256)==0){
                std::cout<<"  > "<<mu1_map[i]<<" | "<<mu2_map[i]<<"\n";
                mu.unpack(mu1_map[i]);
                std::cout<<"      mu1 > pt : "<<mu.pt<<" , eta : "<<mu.eta<<" phi : "<<mu.phi<<"   | ["<<mu.pt<<","<<mu.eta<<","<<mu.phi<<"] "<<"\n";
                mu.unpack(mu2_map[i]);
                std::cout<<"      mu2 > pt : "<<mu.pt<<" , eta : "<<mu.eta<<" phi : "<<mu.phi<<"   | ["<<mu.pt<<","<<mu.eta<<","<<mu.phi<<"] "<<"\n";
        }
    }
    }
    ofile.close();

    std::cout << "Existing the Host application ! \n";
    return 0;
}
