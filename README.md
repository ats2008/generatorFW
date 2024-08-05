# Event Generator 

## LSFR based random number generator

Implements a LSFR based 16 bit random word generator. 

The LSFR is templated to the number of bits used and implementation of `state-update` functionality is implemented for 16/15/14/13/8/4 bit LSFRs.

The basic bulding block for the random word maker is quruplet of LSFRs of interal states 16,15,14 and 13 bit long managing 4 bits of the word.Each of these LSFRs manage one bit.The `state-update` funtion uses the maximal period configuration and provied a cycle length of 2^N - 1. This ensures that the qudruplet we have has a cycle length of ~10^17. For the 16 bit word maker we use 4 such quadruplet each controling 4 bits of the word.

The base implementation can be found in  `include/lsfr.h` and `src/lsfr.cpp` files. A testbench is avalable as `src/lsfr_test.cpp`.

The codebase also has a PS-PL validation workflow setup based on XRT.

For making the testbench
```bash
make test
```

For making the `sw_emu`
```bash
make all TARGET=sw_emu
```

for full set of options `make help`

## Drell-Yan Events at 13TeV

For making the .xclbin and the host code for intefacing the kernel
```
make all TARGET=hw KERNEL=DY
```
The host code outputs `data.dat` file which containes the information of the created di-muon pairs. See `python/plot_drellYan.py` script understand the ouput description.

For making distribution plots, use :
```bash
python python/plot_drellYan.py -i data.dat -t HW
```

TODO :
    * Incorporate boosts to Lab frame


_AStyle configuration_
```
astyle -s4 --indent=spaces=4 --style=bsd -N
```	

_codes are tesed on the ZCU102 Ultrascale+ MPSoc development board from Xilinx_

