#ifndef _LSFR_H_
#define _LSFR_H_

#include <iostream>
#include <bitset>
using namespace std;

#include "ap_int.h"
#define N 20
#define LSFR_LENGTH 16
#define LSFR_INIT 0xACE1u

#define N_DY_GEN 1024

namespace randFW
{

    static const ap_uint<16> randomNumberSeeds[128]=
    {
        0x5104u,0xf150u,0x1017u,0xbe8au,0x008bu,0xb78bu,0x4d82u,0xc486u,0x3f44u,
        0x1594u,0x81e1u,0x64b0u,0xabb5u,0x0815u,0x86f0u,0x736eu,0x99dau,0x14abu,
        0x6e45u,0x6608u,0x2780u,0xf7a5u,0x53fau,0xfd5du,0x15fcu,0xc49cu,0x7f68u,
        0xb0b3u,0x481cu,0x4908u,0x97c1u,0x088eu,0x81f5u,0x715eu,0x887cu,0x49c3u,
        0xa535u,0x1d37u,0xddf4u,0xe60bu,0x0692u,0x0329u,0x826eu,0xab2cu,0x3138u,
        0x09b8u,0x2741u,0x2446u,0x37c6u,0x235fu,0x3865u,0x26a9u,0x1073u,0x587bu,
        0xb7d9u,0xa096u,0xe0bfu,0xccb3u,0x8643u,0x8d64u,0x8adbu,0xa4f2u,0x6c7au,
        0x6ecdu,0x577du,0x3cf6u,0x74fdu,0xdf3fu,0x52cau,0xbe85u,0x4b73u,0x1bf0u,
        0xaef6u,0x45f7u,0xfafeu,0x8da7u,0x04c0u,0x0558u,0xc8d5u,0x7debu,0xdab5u,
        0xa073u,0xe3c7u,0xdf1bu,0xbb47u,0xa4bau,0xcdfdu,0xeda0u,0x780fu,0x4a58u,
        0x7fdau,0xe0edu,0xca39u,0x8f7cu,0xf014u,0x3f03u,0x7caeu,0x544eu,0xb44fu,
        0x36d8u,0x2129u,0xb2a7u,0x8cf1u,0x2349u,0x2a7fu,0x83dbu,0x8a15u,0x29cdu,
        0xc146u,0xbbb5u,0xe4b2u,0x33c2u,0xa400u,0xc952u,0xbddeu,0xeffdu,0xbae0u,
        0x5839u,0xd3cfu,0x4bf1u,0x82bau,0x66d1u,0xd9a9u,0x5889u,0x22a3u,0x2037u,
        0xcb8eu,0xe93cu
    };

    static const ap_ufixed<16,8> drellYanMassQuantiles[256]=
    {
        6.094,6.150,6.199,6.246,6.287,6.326,6.359,6.399,6.438,
        6.470,6.504,6.539,6.574,6.615,6.649,6.678,6.710,6.742,6.773,
        6.797,6.822,6.849,6.876,6.904,6.938,6.966,6.986,7.016,7.042,
        7.074,7.098,7.127,7.149,7.181,7.209,7.234,7.261,7.277,7.299,
        7.337,7.363,7.390,7.425,7.453,7.486,7.519,7.551,7.583,7.624,
        7.654,7.687,7.712,7.744,7.783,7.810,7.837,7.865,7.907,7.940,
        7.985,8.021,8.057,8.085,8.112,8.149,8.184,8.213,8.249,8.295,
        8.336,8.368,8.417,8.455,8.487,8.537,8.574,8.603,8.634,8.660,
        8.699,8.748,8.789,8.825,8.862,8.890,8.926,8.959,8.987,9.029,
        9.058,9.107,9.149,9.199,9.247,9.278,9.335,9.382,9.430,9.465,
        9.522,9.563,9.608,9.650,9.704,9.760,9.810,9.853,9.913,9.967,
        10.025,10.073,10.123,10.162,10.221,10.273,10.332,10.408,10.470,10.540,
        10.601,10.680,10.738,10.805,10.888,10.951,11.009,11.104,11.167,11.233,
        11.301,11.363,11.451,11.497,11.580,11.644,11.715,11.811,11.882,11.944,
        12.026,12.094,12.154,12.235,12.319,12.423,12.515,12.609,12.738,12.842,
        12.930,13.058,13.175,13.300,13.397,13.500,13.632,13.773,13.873,13.990,
        14.147,14.272,14.364,14.490,14.675,14.812,14.996,15.144,15.309,15.469,
        15.666,15.865,16.050,16.216,16.365,16.579,16.844,17.105,17.319,17.605,
        17.907,18.136,18.469,18.755,18.981,19.276,19.702,20.163,20.499,20.972,
        21.375,21.833,22.340,22.803,23.212,23.786,24.570,25.325,26.057,26.869,
        28.161,29.524,31.006,32.989,35.570,38.509,42.675,48.534,58.551,73.563,
        78.714,82.968,85.865,87.027,87.804,88.382,88.953,89.239,89.477,89.709,
        89.890,90.041,90.195,90.306,90.441,90.544,90.637,90.739,90.853,90.924,
        91.005,91.106,91.158,91.261,91.347,91.446,91.514,91.595,91.706,91.805,
        91.905,92.007,92.116,92.234,92.353,92.526,92.677,92.895,93.201,93.523,
        93.993,94.565,95.708,97.420,102.239,109.776,174.041
    };
    


    struct randBase
    {
        static ap_uint<8> lsfr_init_count;
    };

    template< size_t NBIT > struct LSFR
    {

        ap_uint<NBIT> start_state;
        ap_uint<8> init_state_id;
        ap_uint<NBIT> current_state;

        LSFR() //start_state(randomNumberSeeds[lsfr_init_count])
        {
//	      		        std::cout<<"Initializing with "<<randBase::lsfr_init_count<<" -> "<<randomNumberSeeds[randBase::lsfr_init_count]<<"\n";
            init_state_id=randBase::lsfr_init_count;
            start_state=randomNumberSeeds[randBase::lsfr_init_count];
            current_state=start_state;
            randBase::lsfr_init_count++;
        }


        LSFR(ap_uint<NBIT> start_val):
            start_state(start_val)
        {
            current_state=start_state;
        }
        void init()
        {
            current_state=randFW::randomNumberSeeds[init_state_id];
        }

    };

    struct lsfr_16Bit :  LSFR<16>
    {
        bool getRandomBit()
        {
            #pragma HLS INLINE
            bool x = current_state & 1u;
            current_state = current_state >> 1;
            if(x) current_state = current_state ^ 0xD008u;
            return x;
        }
    };
    struct lsfr_15Bit :  LSFR<15>
    {
        bool getRandomBit()
        {
            #pragma HLS INLINE
            bool x = current_state & 1u;
            current_state = current_state >> 1;
            if(x) current_state = current_state ^ 0x6000u;  // If is not needed
            return x;
        }
    };


    struct lsfr_14Bit :  LSFR<14>
    {
        bool getRandomBit()
        {
            #pragma HLS INLINE
            bool x = current_state & 1u;
            current_state = current_state >> 1;
            if(x) current_state = current_state ^ 0x3802u;  // If is not needed
            return x;
        }
    };


    struct lsfr_13Bit :  LSFR<13>
    {
        bool getRandomBit()
        {
            #pragma HLS INLINE
            bool x = current_state & 1u;
            current_state = current_state >> 1;
            if(x) current_state = current_state ^ 0x1C80u;  // If is not needed
            return x;
        }
    };


    struct lsfr_8Bit :  LSFR<8>
    {
        bool getRandomBit()
        {
            #pragma HLS INLINE
            bool x = current_state & 1u;
            current_state = current_state >> 1;
            if(x) current_state = current_state ^ 0xB8u;  // If is not needed
            return x;
        }
    };

    struct randomWord_4Bit
    {

        static ap_uint<4> current_state;

        static lsfr_16Bit b3;
        static lsfr_15Bit b2;
        static lsfr_14Bit b1;
        static lsfr_13Bit b0;

        ap_uint<4> getRandom()
        {
            #pragma HLS PIPELINE
            current_state= b3.getRandomBit()  <<3 | b2.getRandomBit() <<2 | b1.getRandomBit() <<1 | b0.getRandomBit()  ;
            return current_state;
        }

    };

    struct randomWord_16Bit
    {

        static lsfr_16Bit b3[4];
        static lsfr_15Bit b2[4];
        static lsfr_14Bit b1[4];
        static lsfr_13Bit b0[4];
        static bool isInitialized;
        static ap_uint<16> current_state;

        void init()
        {
            if(not isInitialized)
            {
                for(uint8_t i=0; i<4; i++ )
                {
                    b3[i].init();
                    b2[i].init();
                    b1[i].init();
                    b0[i].init();
                }
                isInitialized=true;
            }

        }
        ap_uint<16> getRandom()
        {
            #pragma HLS PIPELINE
            current_state=b3[0].getRandomBit()  <<3 | b2[0].getRandomBit() <<2 | b1[0].getRandomBit() <<1 | b0[0].getRandomBit() ;
            for(uint8_t i=1; i<4; i++ )
            {
                #pragma HLS UNROLL
                current_state= current_state <<4 | b3[i].getRandomBit()  <<3 | b2[i].getRandomBit() <<2 | b1[i].getRandomBit() <<1 | b0[i].getRandomBit(); ;
            }
            return current_state;
        }
    };
    struct randomWord_16BitStandalone
    {

        //static lsfr_16Bit b3[4];
        //static lsfr_15Bit b2[4];
        //static lsfr_14Bit b1[4];
        //static lsfr_13Bit b0[4];

        static ap_uint<8>  init_state_id[16];
        static ap_uint<16> current_state_b3[4];
        static ap_uint<15> current_state_b2[4];
        static ap_uint<14> current_state_b1[4];
        static ap_uint<13> current_state_b0[4];
        ap_uint<16> current_state_w;

        static bool isInitialized;

        randomWord_16BitStandalone()
        {
            ap_uint<5> idx(0);
            ap_uint<8> lsfr_init_count = randFW::randBase::lsfr_init_count;
            for(int i=0; i<4; i++)
            {
                current_state_b0[i] = randomNumberSeeds[lsfr_init_count];
                init_state_id[idx]=lsfr_init_count;
                lsfr_init_count++;
                idx++;

                current_state_b1[i] = randomNumberSeeds[lsfr_init_count];
                init_state_id[idx]=lsfr_init_count;
                lsfr_init_count++;
                idx++;

                current_state_b2[i] = randomNumberSeeds[lsfr_init_count];
                init_state_id[idx]=lsfr_init_count;
                lsfr_init_count++;
                idx++;

                current_state_b3[i] = randomNumberSeeds[lsfr_init_count];
                init_state_id[idx]=lsfr_init_count;
                lsfr_init_count++;
                idx++;

            }
            randFW::randBase::lsfr_init_count = lsfr_init_count ;
            std::cout<<" Constructor for randomWord_16BitStandalone  done ! the init bits for the lsfrs are : \n";
            for(int i=0; i<4; i++)
            {
                std::cout<<"    --> "<<current_state_b0[i]<<" | "<<current_state_b1[i]<<" | "<<current_state_b2[i]<<" | "<<current_state_b3[i]<<"\n";
            }

        }

        void init()
        {
            if(not isInitialized)
                for(uint8_t i=0; i<4; i++ )
                {
                    current_state_b0[i] = randomNumberSeeds[init_state_id[i*4+0]];
                    current_state_b1[i] = randomNumberSeeds[init_state_id[i*4+1]];
                    current_state_b2[i] = randomNumberSeeds[init_state_id[i*4+2]];
                    current_state_b3[i] = randomNumberSeeds[init_state_id[i*4+3]];
                }
            isInitialized=true;
        }
        void update_state()
        {

            for(uint8_t i=0; i<4; i++ )
            {
                //current_state_b0[i] = ( current_state_b0[i] >> 1) ^ ( 0x1C80u >> 1 );
                //current_state_b1[i] = ( current_state_b1[i] >> 1) ^ ( 0x3802u >> 1 );
                //current_state_b2[i] = ( current_state_b2[i] >> 1) ^ ( 0x6000u >> 1 );
                //current_state_b3[i] = ( current_state_b3[i] >> 1) ^ ( 0xD008u >> 1 );

                bool x = current_state_b0[i] & 1u;
                current_state_b0[i] = ( current_state_b0[i] >> 1) ;
                if(x) current_state_b0[i] = current_state_b0[i] ^ 0x1C80u;

                x = current_state_b1[i] & 1u;
                current_state_b1[i] = ( current_state_b1[i] >> 1) ;
                if(x) current_state_b1[i] = current_state_b1[i] ^ 0x3802u;

                x = current_state_b2[i] & 1u;
                current_state_b2[i] = ( current_state_b2[i] >> 1) ;
                if(x) current_state_b2[i] = current_state_b2[i] ^ 0x6000u;

                x = current_state_b3[i] & 1u;
                current_state_b3[i] = ( current_state_b3[i] >> 1) ;
                if(x) current_state_b3[i] = current_state_b3[i] ^ 0xD008u;

            }
            std::cout<<" update for randomWord_16BitStandalone  done ! the bits for the lsfrs states are : \n";
            for(int i=0; i<4; i++)
            {
                std::cout<<"    --> "<<current_state_b0[i]<<" | "<<current_state_b1[i]<<" | "<<current_state_b2[i]<<" | "<<current_state_b3[i]<<"\n";
            }
            set_state();
        }
        void set_state()
        {
            current_state_w=0;
            for(uint8_t i=0; i<4; i++ )
            {
                //	#pragma HLS UNROLL
                current_state_w= current_state_w <<4 | (current_state_b3[i] & 1u  <<3) | (current_state_b2[i] & 1u  <<2) |
                                 (current_state_b1[i] & 1u  <<1)| (current_state_b0[i] & 1u) ;
            }
        }
        ap_uint<16> getRandom()
        {
            #pragma HLS PIPELINE
            std::cout<<"  [KERNEL] call : randomWord_16BitStandalone::getRandom() | current_state "<<current_state_w<<"\n";
            update_state();
            std::cout<<"  [KERNEL] call : randomWord_16BitStandalone::getRandom() | current_state [upd] "<<current_state_w<<"\n";
            return current_state_w;
        }
    };
    static randomWord_16BitStandalone testWord;
    
    struct muon 
{
        ap_ufixed<12,8> pt;
        ap_fixed<10,4>  eta;
        ap_ufixed<10,3> phi;
        ap_uint<32> pack ()
        {
            return  (pt(11,0) << 20 ) | (eta(9,0)  <<10 ) | phi(9,0);
        }
        void unpack(ap_uint<32> data)
        {
            pt(11,0) =data(31,20);
            eta(9,0) =data(19,10);
            phi(9,0) =data( 9, 0);
        }

};
    struct DellYanGenerator
    {
        static ap_fixed<16,2> _lut_sintheta[1024];
        static ap_fixed<10,4> _lut_eta[1024];
        static ap_ufixed<10,3> _lut_phi[256];
        ap_uint<64>  getDimuonPairs();   
        void init();
    };
    

}

extern "C" { 
    void randWordGen16Bit(bool status,ap_uint<16> randNum[5]);
    void drellYanPairGenerator( ap_uint<32> MU1[N_DY_GEN] , ap_uint<32> MU2[N_DY_GEN]);
}

#endif




