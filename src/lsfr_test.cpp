#include "lsfr.h"

void tets_lsfr()
{
	randFW::LSFR randGenerator[10];

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

int main()
{

	 tets_lsfr();

	return 0;
}
