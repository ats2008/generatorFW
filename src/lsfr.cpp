#include "lsfr.h"
#include "math.h"
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

bool randFW::randomWord_16BitStandalone::isInitialized=0;
ap_uint<8> randFW::randomWord_16BitStandalone::init_state_id[16];
ap_uint<16> randFW::randomWord_16BitStandalone::current_state_b3[4] ;
ap_uint<15> randFW::randomWord_16BitStandalone::current_state_b2[4] ;
ap_uint<14> randFW::randomWord_16BitStandalone::current_state_b1[4] ;
ap_uint<13> randFW::randomWord_16BitStandalone::current_state_b0[4] ;


bool            randFW::GeneratorBase::isInitialized(0);
ap_fixed<16,2>  randFW::GeneratorBase::_lut_sintheta[1024];
ap_fixed<10,4>  randFW::GeneratorBase::_lut_eta[1024];
ap_ufixed<10,3> randFW::GeneratorBase::_lut_phi[256];

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

void _lutFill_sintheta(ap_fixed<16,2> _lut_sintheta[1024])
{
    #pragma HLS array_partition variable=_lut_sintheta
    std::cout<<" steta init : ";
    for(int i=0; i<1024; i++)
    {
        #pragma HLS unroll
        _lut_sintheta[i]= sqrt(i*(1024.0-i))/512.0;
        // std::cout<< "("<<i<<",";
        // auto x=_lut_sintheta[i];
        // std::cout<<float(x);
        // std::cout<<" ) ,";
        //_lut_sintheta[i]= fabs(sin( acos(1.0-2.0*i/1024.0) ));
        //_lut_sintheta[i]= sqrt(2.0*i/1024.0*(2-2.0*i/1024.0));
    }
    std::cout<<"\n";
}
void _lutFill_eta(ap_fixed<10,4> _lut_eta[1024])
{
    #pragma HLS array_partition variable=_lut_eta
    _lut_eta[0] = 10 ;
    _lut_eta[1023] = -10 ;
    std::cout<<" eta init : ";
    for(int i=1; i<1023; i++)
    {
        #pragma HLS unroll
        _lut_eta[i]= 0.5*log( 1024.0/i -1 );
        // auto x=_lut_eta[i];       
        // std::cout<< "("<<i<<",";
        // std::cout<<float(x);
        // std::cout<<" ) ,";
        //_lut_eta[i]= -log(abs(1.0-2.0*i/1024.0 + 1e-9)/2.0 );
        //double y = 1.0-2.0*i/1024.0 ;
        //y= std::acos(y);
        //y= std::tan(y/2.0);
        //y=-1.0*std::log(y);
        //
        ////const float x(-1.0*log(tan( acos(1.0-2.0*i/1024.0 + 1e-9)/2.0 )));
        ////const ap_fixed<10,4> y(x);
        //_lut_eta[i]= y;
    }
    std::cout<<"\n";
}
void _lutFill_phi(ap_ufixed<10,3> _lut_phi[256])
{
    #pragma HLS array_partition variable=_lut_phi
    for(int i=0; i<256; i++)
    {
        #pragma HLS unroll
        _lut_phi[i]= 2*M_PI*i/256.0;
    }
}

void GeneratorBase::init()
{
    randomWord_16Bit wordgen;
    wordgen.init();
    if( not isInitialized)
    {
        _lutFill_sintheta(_lut_sintheta);
        _lutFill_eta(_lut_eta);
        _lutFill_phi(_lut_phi);
    }
}

ap_uint<64> DellYanGenerator::getDimuonPairs()
{
    randomWord_16Bit wordGen;
    ap_uint<16> word= wordGen.getRandom();
    ap_uint<8> rid1=word & 0xff;
    word=word >>8;
    ap_uint<8> rid3=word & 0xff;
    word=word >>8;
    word=wordGen.getRandom();
    ap_uint<10> rid2=word & 0x3ff;
    word=word >>10;

    //std::cout<<"     >  RIDs   : "<<rid1<<" | "<<rid2<<" | "<<rid3<<"\n";
    ap_ufixed<12,8> mass = randFW::drellYanMassQuantiles[rid1];
    muon mu1,mu2;

    mu1.eta  = _lut_eta[rid2];
    mu2.eta = -mu1.eta;
    mu1.pt   =  mass*_lut_sintheta[rid2];
    mu1.pt   *=ap_ufixed<3,2>(0.5);
    mu2.pt   =  mu1.pt;
    //std::cout<<"         mass : "<<randFW::drellYanMassQuantiles[rid1]<<"  | "<<mass<<" | "<<_lut_sintheta[rid2]<<"  , pt = "<<mu1.pt<<"\n";

    mu1.phi = _lut_phi[rid3];
    mu2.phi = rid3 < 128 ?  _lut_phi[rid3+128] : _lut_phi[rid3-128] ;

    ap_uint<64> MU1 = mu1.pack();
    ap_uint<32> mux = mu2.pack();
    std::cout<<"    > Mu1/2  : "<<mu1.pack()<<" /  "<<mu2.pack()<<"\n";
    MU1  = MU1 <<32 ;
    MU1  = MU1 | mu2.pack()  ;
#ifndef __SYNTHESIS__
    std::cout<<"     > mass  : "<<mass
             <<" | pt : "<<mu1.pt<<" , "  <<mu2.pt
             <<" | eta : "<<mu1.eta<<" , "<<mu2.eta
             <<" | theta : "<<acos(1.0-2.0*rid2/1024.0)
             <<" | phi : "<<mu1.phi<<" , "<<mu2.phi
             <<" | ptrn "<< MU1
             <<"\n";
#endif
    return MU1;
}


extern "C" {

    void randWordGen16Bit(bool status,ap_uint<16> randNum[5])
    {
        #pragma HLS INTERFACE mode=ap_vld port=status
        #pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=randNum
        #pragma HLS INTERFACE s_axilite port=randNum
        #pragma HLS INTERFACE mode=s_axilite port=return

        #pragma HLS PIPELINE style=flp

        randFW::randomWord_16Bit word;
        word.init();
        for(int k=0; k < 5; k++)
        {
            //randNum[k]=testWord.getRandom();
            randNum[k]=word.getRandom();
        }
    }

    void drellYanPairGenerator( ap_uint<32> MU1[N_DY_GEN], ap_uint<32> MU2[N_DY_GEN])
    {
        #pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=MU1
        #pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=MU2
        #pragma HLS INTERFACE s_axilite port=MU1
        #pragma HLS INTERFACE s_axilite port=MU2
        #pragma HLS INTERFACE mode=s_axilite port=return

        #pragma HLS PIPELINE style=flp


        randomWord_16Bit wordGen;
        randFW::DellYanGenerator dyGen;

        MU1[0] = wordGen.isInitialized ;
        dyGen.init();
        
        for(int i=0;i<1024;i++)
        {
            std::cout<<"LUT i = "<<i<<" | "<< float(dyGen._lut_eta[i])<<" , "<<float(dyGen._lut_sintheta[i])<<"\n";

        }

        ap_uint<64> diMuons[N_DY_GEN];
        
        #pragma HLS array_partition diMuons
        
  DiMuGenLoop:     for( int i =0 ; i < N_DY_GEN ; i++)
        {
            diMuons[i]=dyGen.getDimuonPairs() ;
#ifndef __SYNTHESIS__
            std::cout<<" DIMU P : "<<diMuons[i]<<"\n";
#endif
        }

  DiMuCopyLoop:for( int i =0 ; i < N_DY_GEN ; i++)
        {
            MU1[i] = diMuons[i] & 0xffffffff;
            MU2[i] = (diMuons[i]>>32) & 0xffffffff;
#ifndef __SYNTHESIS__
            std::cout<<" > OUT PATTERNS : "<< diMuons[i]<<"   -> "<<MU1[i]<<" + "<<MU2[i]<<"\n";
#endif
        }
    }

}
