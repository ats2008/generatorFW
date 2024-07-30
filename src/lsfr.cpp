#include "lsfr.h"

using namespace randFW ;
ap_uint<8> randFW::randBase::lsfr_init_count=0;

ap_uint<4> randFW::randomWord_4Bit::current_state=0;
lsfr_16Bit randFW::randomWord_4Bit::b3=lsfr_16Bit();
lsfr_15Bit randFW::randomWord_4Bit::b2=lsfr_15Bit();
lsfr_14Bit randFW::randomWord_4Bit::b1=lsfr_14Bit();
lsfr_13Bit randFW::randomWord_4Bit::b0=lsfr_13Bit();

bool randFW::randomWord_16Bit::isInitialized=0;
ap_uint<16> randFW::randomWord_16Bit::current_state=0;
lsfr_16Bit  randFW::randomWord_16Bit::b3[4] ;
lsfr_15Bit  randFW::randomWord_16Bit::b2[4] ;
lsfr_14Bit  randFW::randomWord_16Bit::b1[4] ;
lsfr_13Bit  randFW::randomWord_16Bit::b0[4] ;

bool randFW::randomWord_16BitV2::isInitialized=0;
ap_uint<8> randFW::randomWord_16BitV2::init_state_id[16];
ap_uint<16> randFW::randomWord_16BitV2::current_state_b3[4] ;
ap_uint<15> randFW::randomWord_16BitV2::current_state_b2[4] ;
ap_uint<14> randFW::randomWord_16BitV2::current_state_b1[4] ;
ap_uint<13> randFW::randomWord_16BitV2::current_state_b0[4] ;

/*
for(uint8_t i=0;i<4;i++ )
{

	lsfr_16Bit randFW::randomWord_16Bit::b3[i]=lsfr_16Bit();
	lsfr_15Bit randFW::randomWord_16Bit::b2[i]=lsfr_15Bit();
	lsfr_14Bit randFW::randomWord_16Bit::b1[i]=lsfr_14Bit();
	lsfr_13Bit randFW::randomWord_16Bit::b0[i]=lsfr_13Bit();

}
*/


void randGen(bool status,ap_uint<16> *randNum)
{

    if(status)
    {
        randFW::lsfr_16Bit randGenerator;
        *randNum=randGenerator.getRandomBit();
    }
}


extern "C" {

    void init_generator()
    {
        randFW::randomWord_16Bit word;
        word.init();
    }

    void randWordGen16Bit(bool status,ap_uint<16> randNum[5])
    {
        #pragma HLS INTERFACE mode=ap_vld port=status
        #pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=randNum
        #pragma HLS INTERFACE s_axilite port=randNum
        #pragma HLS INTERFACE mode=s_axilite port=return

        #pragma HLS PIPELINE style=flp


        randFW::randomWord_16Bit word;
        for(int k=0; k < 5; k++)
        {
            //randNum[k]=testWord.getRandom();
            randNum[k]=word.getRandom();
        }
    }

}
