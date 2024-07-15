#include "lsfr.h"

using namespace randFW ;
ap_uint<8> randFW::randBase::lsfr_init_count=0;

ap_uint<4> randFW::randomWord_4Bit::current_state=0;
lsfr_16Bit randFW::randomWord_4Bit::b3=lsfr_16Bit();
lsfr_15Bit randFW::randomWord_4Bit::b2=lsfr_15Bit();
lsfr_14Bit randFW::randomWord_4Bit::b1=lsfr_14Bit();
lsfr_13Bit randFW::randomWord_4Bit::b0=lsfr_13Bit();

ap_uint<16> randFW::randomWord_16Bit::current_state=0;
lsfr_16Bit  randFW::randomWord_16Bit::b3[4] ;
lsfr_15Bit  randFW::randomWord_16Bit::b2[4] ;
lsfr_14Bit  randFW::randomWord_16Bit::b1[4] ;
lsfr_13Bit  randFW::randomWord_16Bit::b0[4] ;

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

   if(status){
	   randFW::lsfr_16Bit randGenerator;
	   *randNum=randGenerator.getRandomBit();
    }
}


void randWordGen16Bit(bool status,ap_uint<16> *randNum)
{
  #pragma HLS PIPELINE  II=4

   if(status){
	   randFW::randomWord_16Bit word;
	   *randNum=word.getRandom();
    }
}


