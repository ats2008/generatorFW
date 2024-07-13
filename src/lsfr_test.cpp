#include "lsfr.h"
#include <fstream>

void randGen(bool,ap_uint<16> *x);

void tets_lsfr()
{
	randFW::lsfr_16Bit randGenerator[10];

	for(int i=0;i < 10; i++)
	{
		std::cout<<i<<" init with : "<<randGenerator[i].start_state<<" | "<<std::bitset<16> (randGenerator[i].start_state)<<"\n";
	}
	std::cout<<"Printing taps ! \n";
	for(int i=0;i < 10; i++)
	{
		std::cout<<" i = "<<i<<"  ";
		for(int j=0 ; j < 10 ; j++){
			std::cout<<" : "<<randGenerator[j].getRandomBit();
		}
		std::cout<<"\n";
	}

}

void test_randGen()
{
	ap_uint<16> randNum;
	for(int j=0 ; j < 10 ; j++){
				randGen(true,&randNum);
				std::cout<<j<<"  : "<<randNum<<"\n";
			}
}

void test_randomWordMaker()
{
	randFW::randomWord_4Bit word;

		for(int j=0 ; j < 500 ; j++){
			if (j%20==0)
			std::cout<<"\n"<<j<<" -> ";
			std::cout<< word.getRandom()<<"  ";
		}
}

void test_randomWordMaker_16Bit()
{

	ofstream outFile("randOut_16Bit.txt");
	randFW::randomWord_16Bit word;
		ap_uint<16> randVal;

		for(int j=0 ; j < 1000*1000 ; j++){
			randVal=word.getRandom();
			if(j<200){
				if (j%10==0) std::cout<<"\n"<<j<<" -> ";
				std::cout<< randVal<<"  ";
			}
			outFile<<randVal<<"\n";
			if(j%5000==0)
				std::cout<<j<<" vals made !\n";
		}
		outFile.close();
}


void test_randomWordMaker_4Bit()
{

	ofstream outFile("randOut_4Bit.txt");
	randFW::randomWord_16Bit word;
		ap_uint<4> randVal;

		for(int j=0 ; j < 1000*1000 ; j++){
			randVal=word.getRandom();
			if(j<200){
				if (j%10==0) std::cout<<"\n"<<j<<" -> ";
				std::cout<< randVal<<"  ";
			}
			outFile<<randVal<<"\n";
			if(j%5000==0)
				std::cout<<j<<" vals made !\n";
		}
		outFile.close();
}
void test_randomWordMaker_16BitCHK()
{

	randFW::randomWord_16Bit word;
		ap_uint<16> randVal;
		for(int j=0 ; j < 20 ; j++){
			std::cout<<"\n"<<j<<" --------------> \n";
			randVal=word.getRandom();
			std::cout<<"  Random val : "<< randVal<<"  ";
		}
}



int main()
{

//  tets_lsfr();
//  test_randGen();
//	test_randomWordMaker();
  test_randomWordMaker_16Bit();
//  test_randomWordMaker_4Bit();
//	test_randomWordMaker_16BitCHK();
	return 0;
}
