#include "lsfr.h"
#include <fstream>
#include <iostream>
void randGen(bool,ap_uint<16> *x);

void unitTests_lsfr()
{
    randFW::lsfr_16Bit randGenerator[10];

    for(int i=0; i < 10; i++)
    {
        std::cout<<i<<" init with : "<<randGenerator[i].start_state<<" | "<<std::bitset<16> (randGenerator[i].start_state)<<"\n";
    }
    std::cout<<"Printing taps ! \n";
    for(int i=0; i < 10; i++)
    {
        std::cout<<" i = "<<i<<"  ";
        for(int j=0 ; j < 10 ; j++)
        {
            std::cout<<" : "<<randGenerator[j].getRandomBit();
        }
        std::cout<<"\n";
    }


    randFW::lsfr_8Bit randGeneratorZ;
    randFW::lsfr_16Bit randGeneratorA;
    randFW::lsfr_15Bit randGeneratorB;
    randFW::lsfr_14Bit randGeneratorC;
    randFW::lsfr_13Bit randGeneratorD;
    ap_uint<16> init_state;

    //  Validating 8-Bit LSFR
    std::cout<<"\n\n> Validating the 8-bit lsfr \n";
    init_state = randGeneratorZ.current_state;
    uint64_t count=0,sucess=0;
    while(true)
    {
        count++;
        randGeneratorZ.getRandomBit();
        if(init_state==randGeneratorZ.current_state)
        {
            sucess=1;
            break;
        }
        if(count > (pow(2,9)+1))
        {
            std::cout<<" counter broke the loop \n";
            break;
        }

    }
    std::cout<<"   Found a match to the initial pattern : "<<sucess<<"\n"
//			 <<"   Init Pattern : "<<init_state<<" final state : "<<randGeneratorD.current_state<<"\n"
             <<"        count = "<<count<<" | expected count = "<<pow(2,8)-1<<"\n";

    //  Validating 13-Bit LSFR
    std::cout<<"\n\n> Validating the 13-bit lsfr \n";
    init_state = randGeneratorD.current_state;
    count=0;
    sucess=0;
    while(true)
    {
        count++;
        randGeneratorD.getRandomBit();
        if(init_state==randGeneratorD.current_state)
        {
            sucess=1;
            break;
        }
        if(count > (pow(2,13)+1))
        {
            std::cout<<" counter broke the loop \n";
            break;
        }

    }
    std::cout<<"   Found a match to the initial pattern : "<<sucess<<"\n"
//			 <<"   Init Pattern : "<<init_state<<" final state : "<<randGeneratorD.current_state<<"\n"
             <<"        count = "<<count<<" | expected count = "<<pow(2,13)-1<<"\n";


    //  Validating 14-Bit LSFR
    std::cout<<"\n\n> Validating the 14-bit lsfr \n";
    init_state = randGeneratorC.current_state;
    count=0;
    sucess=0;
    while(true)
    {
        count++;
        randGeneratorC.getRandomBit();
        if(init_state==randGeneratorC.current_state)
        {
            sucess=1;
            break;
        }
        if(count > (pow(2,14)+1))
        {
            std::cout<<" counter broke the loop \n";
            break;
        }

    }
    std::cout<<"   Found a match to the initial pattern : "<<sucess<<"\n"
//			 <<"   Init Pattern : "<<init_state<<" final state : "<<randGeneratorD.current_state<<"\n"
             <<"        count = "<<count<<" | expected count = "<<pow(2,14)-1<<"\n";

    //  Validating 15-Bit LSFR
    std::cout<<"\n\n> Validating the 15-bit lsfr \n";
    init_state = randGeneratorB.current_state;
    count=0;
    sucess=0;
    while(true)
    {
        count++;
        randGeneratorB.getRandomBit();
        if(init_state==randGeneratorB.current_state)
        {
            sucess=1;
            break;
        }
        if(count > (pow(2,15)+1))
        {
            std::cout<<" counter broke the loop \n";
            break;
        }

    }
    std::cout<<"   Found a match to the initial pattern : "<<sucess<<"\n"
//			 <<"   Init Pattern : "<<init_state<<" final state : "<<randGeneratorD.current_state<<"\n"
             <<"        count = "<<count<<" | expected count = "<<pow(2,15)-1<<"\n";


    //  Validating 16-Bit LSFR
    std::cout<<"\n\n> Validating the 16-bit lsfr \n";
    init_state = randGeneratorA.current_state;
    count=0;
    sucess=0;
    while(true)
    {
        count++;
        randGeneratorA.getRandomBit();
        if(init_state==randGeneratorA.current_state)
        {
            sucess=1;
            break;
        }
        if(count > (pow(2,16)+1))
        {
            std::cout<<" counter broke the loop \n";
            break;
        }

    }
    std::cout<<"   Found a match to the initial pattern : "<<sucess<<"\n"
//			 <<"   Init Pattern : "<<init_state<<" final state : "<<randGeneratorA.current_state<<"\n"
             <<"        count = "<<count<<" | expected count = "<<pow(2,16)-1<<"\n";


}

void unitTest_randGen()
{
    ap_uint<16> randNum;
    for(int j=0 ; j < 10 ; j++)
    {
        randGen(true,&randNum);
        std::cout<<j<<"  : "<<randNum<<"\n";
    }
}

void unitTest_randomWordMaker()
{
    randFW::randomWord_4Bit word;

    for(int j=0 ; j < 500 ; j++)
    {
        if (j%20==0)
            std::cout<<"\n"<<j<<" -> ";
        std::cout<< word.getRandom()<<"  ";
    }
}

void unitTest_randomWordMaker_16Bit()
{

    ofstream outFile("randOut_16Bit.txt");
    randFW::randomWord_16Bit word;
    ap_uint<16> randVal;

    for(int j=0 ; j < 1000*1000 ; j++)
    {
        randVal=word.getRandom();
        if(j<200)
        {
            if (j%10==0) std::cout<<"\n"<<j<<" -> ";
            std::cout<< randVal<<"  ";
        }
        outFile<<randVal<<"\n";
        if(j%5000==0)
            std::cout<<j<<" vals made !\n";
    }
    outFile.close();
}


void unitTest_randomWordMaker_4Bit()
{

    ofstream outFile("randOut_4Bit.txt");
    randFW::randomWord_4Bit word;
    ap_uint<4> randVal;

    for(int j=0 ; j < 1000*1000 ; j++)
    {
        randVal=word.getRandom();
        if(j<200)
        {
            if (j%10==0) std::cout<<"\n"<<j<<" -> ";
            std::cout<< randVal<<"  ";
        }
        outFile<<randVal<<"\n";
        if(j%5000==0)
            std::cout<<j<<" vals made !\n";
    }
    outFile.close();
}
void unitTest_randomWordMaker_16BitCHK()
{

    randFW::randomWord_16Bit word;
    ap_uint<16> randVal;
    for(int j=0 ; j < 20 ; j++)
    {
        std::cout<<"\n"<<j<<" --------------> \n";
        randVal=word.getRandom();
        std::cout<<"  Random val : "<< randVal<<"  ";
    }
}

void testBench_randWordGen16Bit()
{

    randFW::lsfr_8Bit statusGEN;
    ap_uint<16> randVal[5];
    bool status;
    for(int i=0; i<20; i++)
    {
        status=statusGEN.getRandomBit();
        randWordGen16Bit(status,randVal);
        std::cout << "   ["<<i<<"]   Value read as : ";
        for(int j=0; j<5; j++)
            std::cout<<randVal[j]<<"  |  ";
        std::cout<<"\n";
    }
}

void testBench_DYGen()
{
    ap_ufixed<8,8> a;
    ap_fixed<16,2> b;
    ap_fixed<10,4> c;
    ap_ufixed<10,3> d;

    b=-0.01  ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=0.51   ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=-0.999   ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=0.999   ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=1.01   ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=2.0001 ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=0.0001 ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=0.001  ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=0.01   ;
    std::cout<<__LINE__<<" | "<<b<<" \n";
    b=0.484123   ;
    std::cout<<__LINE__<<" | "<<b<<" \n";

    c=2.0    ;
    std::cout<<__LINE__<<" | "<<c<<" \n";
    c=7.0    ;
    std::cout<<__LINE__<<" | "<<c<<" \n";
    c=4.0    ;
    std::cout<<__LINE__<<" | "<<c<<" \n";
    c=3.0    ;
    std::cout<<__LINE__<<" | "<<c<<" \n";
    c=-3.0    ;
    std::cout<<__LINE__<<" | "<<c<<" \n";
    c=-4.0    ;
    std::cout<<__LINE__<<" | "<<c<<" \n";

    d=2.0    ;
    std::cout<<__LINE__<<" | "<<d<<" \n";
    d=7.0    ;
    std::cout<<__LINE__<<" | "<<d<<" \n";
    d=4.0    ;
    std::cout<<__LINE__<<" | "<<d<<" \n";
    d=3.0    ;
    std::cout<<__LINE__<<" | "<<d<<" \n";
    d=0.0    ;
    std::cout<<__LINE__<<" | "<<d<<" \n";
    d=0.0    ;
    std::cout<<__LINE__<<" | "<<(d<<5)<<" \n";

    ap_uint<64> MU1[N_DY_GEN] ;
    ap_uint<64> MU2[N_DY_GEN] ;
    drellYanPairGenerator( MU1,MU2 );
    std::cout<<" Obtained patterns : "<<"\n";
    randFW::muon mu;
    for(int i=0; i<N_DY_GEN; i++)
    {
        std::cout<<"  > "<<MU1[i]<<" | "<<MU2[i]<<"\n";
        mu.unpack(MU1[i]);
        std::cout<<"      mu1 > pt : "<<mu.pt<<" , eta : "<<mu.eta<<" phi : "<<mu.phi<<"   | ["<<mu.pt<<","<<mu.eta<<","<<mu.phi<<"] "<<"\n";
        mu.unpack(MU2[i]);
        std::cout<<"      mu2 > pt : "<<mu.pt<<" , eta : "<<mu.eta<<" phi : "<<mu.phi<<"   | ["<<mu.pt<<","<<mu.eta<<","<<mu.phi<<"] "<<"\n";
    }
}

int main()
{

//  unitTests_lsfr();
//  unitTest_randGen();
//  unitTest_randomWordMaker();
//  unitTest_randomWordMaker_16Bit();
//  unitTest_randomWordMaker_4Bit();
//  unitTest_randomWordMaker_16BitCHK();
//  testBench_randWordGen16Bit();
    testBench_DYGen();
    return 0;
}
