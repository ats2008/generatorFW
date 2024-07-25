#ifndef _LSFR_H_
#define _LSFR_H_

#include <iostream>
#include <bitset>
using namespace std;

#include "ap_int.h"
#define N 20
#define LSFR_LENGTH 16
#define LSFR_INIT 0xACE1u

namespace randFW {

	static ap_uint<16> randomNumberSeeds[128]={
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

	struct randBase {
		static ap_uint<8> lsfr_init_count;
	};

	template< size_t NBIT > struct LSFR : public randBase {

	public :
		const ap_uint<NBIT> start_state;
		ap_uint<NBIT> current_state;

		LSFR(): start_state(randomNumberSeeds[lsfr_init_count])
			{
//	      		        std::cout<<"Initializing with "<<lsfr_init_count<<" -> "<<randomNumberSeeds[lsfr_init_count]<<"\n";
				current_state=start_state;
				lsfr_init_count++;
			}


		LSFR(ap_uint<NBIT> start_val):
		  start_state(start_val)
			{
				current_state=start_state;
			}
	};

	class lsfr_16Bit : public LSFR<16>
	  {
	  public:
		bool getRandomBit()
		{
		  #pragma HLS INLINE
			bool x = current_state & 1u;
			current_state = current_state >> 1;
			if(x) current_state = current_state ^ 0xD008u;
			return x;
		}
	};
	class lsfr_15Bit : public LSFR<15>
	  {
	  public:
		bool getRandomBit()
		{
		  #pragma HLS INLINE
			bool x = current_state & 1u;
			current_state = current_state >> 1;
			if(x) current_state = current_state ^ 0x6000u;  // If is not needed
			return x;
		}
	};


	class lsfr_14Bit : public LSFR<14>
	  {
	  public:
		bool getRandomBit()
		{
		  #pragma HLS INLINE
			bool x = current_state & 1u;
			current_state = current_state >> 1;
			if(x) current_state = current_state ^ 0x3802u;  // If is not needed
			return x;
		}
	};


	class lsfr_13Bit : public LSFR<13>
	  {
	  public:
		bool getRandomBit()
		{
          #pragma HLS INLINE
			bool x = current_state & 1u;
			current_state = current_state >> 1;
			if(x) current_state = current_state ^ 0x1C80u;  // If is not needed
			return x;
		}
	};


	class lsfr_8Bit : public LSFR<8>
	  {
	  public:
		bool getRandomBit()
		{
          #pragma HLS INLINE
			bool x = current_state & 1u;
			current_state = current_state >> 1;
			if(x) current_state = current_state ^ 0xB8u;  // If is not needed
			return x;
		}
	};

	class randomWord_4Bit {

		static ap_uint<4> current_state;

		static lsfr_16Bit b3;
		static lsfr_15Bit b2;
		static lsfr_14Bit b1;
		static lsfr_13Bit b0;

	public :
		ap_uint<4> getRandom()
		{
          #pragma HLS PIPELINE
			current_state= b3.getRandomBit()  <<3 | b2.getRandomBit() <<2 | b1.getRandomBit() <<1 | b0.getRandomBit()  ;
			return current_state;
		}

	};

	class randomWord_16Bit {

		static lsfr_16Bit b3[4];
		static lsfr_15Bit b2[4];
		static lsfr_14Bit b1[4];
		static lsfr_13Bit b0[4];

		static ap_uint<16> current_state;


	public :
		ap_uint<16> getRandom()
		{
		    #pragma HLS PIPELINE
			ap_uint<4> val;
			current_state=b3[0].getRandomBit()  <<3 | b2[0].getRandomBit() <<2 | b1[0].getRandomBit() <<1 | b0[0].getRandomBit() ;
			for(uint8_t i=1;i<4;i++ )
			{
				#pragma HLS UNROLL
				current_state= current_state <<4 | b3[i].getRandomBit()  <<3 | b2[i].getRandomBit() <<2 | b1[i].getRandomBit() <<1 | b0[i].getRandomBit(); ;
			}
			return current_state;
		}
	};

}

void randWordGen16Bit(bool status,ap_uint<16> *randNum);

#endif




