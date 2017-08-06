/*
Fitbit12bitBin2TextUtil
-----------------------
ANSI-C based commandline utility
supporting two arguments:
<inputfilename>
<outputfilename>

<inputfilename> must be a binary file containing 12-bit unsigned values.
<outputfilename> is desired filename for a textfile to contain a formatted 
output based on content of <inputfilename>.
Format specification for output file (ignore quotes):
"--Sorted Max 32 Values--\n"
"<32 largest values from inputfile, one per line, ascending order>"
"--Last 32 Values--\n"
"<32 last values from inputfile, one per line, same order as read>"

NOTE: If less than 32 values are read, simply output what is read.

Considerations for implementation:
----------------------------------
1) Key part here is to keep track of two sets of values: Largest 32 values and 
last 32 values. As we have memory limited system, best option is to choose 
option requiring smallest data memory footprint. 2x32 unsigned 12-bit 
integers are required for storing the required output, before saving to file.
There's no need to save the input data unless they're in the two relevant 
buckets, hence sort can run sample-by-sample.

2) Saving last 32 values must be done such that order follows input data, i.e. 
FIFO structure is suitable. Should we have less than 32 samples in last reading
we need to place the read values overwriting oldest samples in last 32 values 
buffer. E.g. assume SIZE = 4 samples:
Read values		Stored values
1 2 3 4			1 2 3 4
5 6 - -			5 6 3 4
Values written to output:
3
4
5
6
Hence we need to keep track on where we wrote last to buffer. A circular buffer
structure contains read/write pointers keeping track of this info.

3) Sort algorithm required: We do not need to sort ALL numbers, simply quick 
compare-save/discard. Simplified insertion sort principle can be applied.

4) Input interface. Input data is represented in 12bits/sample. At most 32 
samples are to be stored in input buffer.
Assuming we have a 32-bit CPU data path, 12 32-bit unsigned numbers are 
required as buffer.
Assuming we have a 16-bit CPU data path, 24 16-bit unsigned numbers are 
required as buffer.
PROS: Minimum data memory footprint.
CONS: Requires bit extraction during sort as 12-bit requires 1.5 bytes which 
leads to extra processing consuming more power/sample.
To overcome this the read part should read a multiple amount of bytes 
containing nx12-bit numbers - and map each number to a 16-bit unsigned integer 
which will be saved in the buffer for last 32 values.

Defining a bitblock as a 32x12 bits, which will be stored in 32x16 bits FIFO 
circular buffer. The samples from this buffer will be used as basis for sorting 
samples to be stored in a linear buffer containing the 32 highest numbers.
*/

#ifndef _FITBIT12BITBIN2TEXTUTIL_H_
#define _FITBIT12BITBIN2TEXTUTIL_H_

#define TWO_ARGUMENTS			3		/* nargs pass value */

enum {
	EXECUTABLENAMEID,
	INPUTFILENAMEID,
	OUTPUTFILENAMEID
};

#endif /* _FITBIT12BITBIN2TEXTUTIL_H_ */

