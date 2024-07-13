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

	static ap_uint<16> randomNumberSeeds[64]={0x9a1cu,0x87e9u,0xf5f5u,0x7f87u,0xff3eu,0xe931u,0x5712u,0x74c3u,
			0xe4f1u,0x6f8bu,0x82f6u,0x3f65u,0xf288u,0x5effu,0x318du,0x8927u,
			0x0c3du,0xee21u,0x7486u,0x4b62u,0xfe41u,0xcebfu,0xf49au,0x825du,
			0x9081u,0x9751u,0xd950u,0xbb33u,0x1ab7u,0x7c23u,0x3378u,0x5c6fu,
			0xd0c3u,0x8a19u,0xea5fu,0x7e79u,0xb9b5u,0x48f7u,0xb7adu,0xccf7u,
			0x92e4u,0x90ccu,0xc3e2u,0xdbc6u,0xe433u,0x60c5u,0x351au,0xfc39u,
			0xe999u,0xeaddu,0xe75du,0x6a40u,0x3e5cu,0x9510u,0x3556u,0x1cd7u,
			0x91eeu,0x520cu,0xce64u,0x1071u,0x1bf0u,0xd58eu,0xe896u,0xdb71u};

	struct LSFR {
		static ap_uint<8> lsfr_init_count;
		const ap_uint<LSFR_LENGTH> start_state;
		ap_uint<LSFR_LENGTH> current_state;

		LSFR(): start_state(randomNumberSeeds[lsfr_init_count])
			{
				current_state=start_state;
				lsfr_init_count++;
			}


		LSFR(ap_uint<LSFR_LENGTH> start_val):
		  start_state(start_val)
			{
				current_state=start_state;
			}
		bool getRandomBit()
		{
			bool x = current_state & 1u;
			current_state = current_state >> 1;
			if(x) current_state = current_state ^ 0xB400u;
			return x;
		}
	};


}
#endif
