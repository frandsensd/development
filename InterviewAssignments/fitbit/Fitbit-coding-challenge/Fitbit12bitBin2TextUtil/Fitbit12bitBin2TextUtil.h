#ifndef _FITBIT12BITBIN2TEXTUTIL_H_
#define _FITBIT12BITBIN2TEXTUTIL_H_

/*
Fitbit12bitBin2TextUtil
-----------------------
ANSI-C based command line utility supporting two arguments:
<inputfilename>		binary file containing 12-bit unsigned values.
<outputfilename>	filename for a textfile to contain a formatted 
					output based on content of <inputfilename>.

Format specification for output file:
NOTE: 32 is default amount of samples stored. However if input contains less 
than 32 samples this number will reflect the amount of samples read.
If less than 32 values are read, simply output what is read.
"--Sorted Max 32 Values--\n"
"<32 largest values from inputfile, one per line, ascending order>"
"--Last 32 Values--\n"
"<32 last values from inputfile, one per line, same order as read>"

Considerations for implementation:
----------------------------------
Key part here is to keep track of two sets of values: Largest 32 values from 
input set and last 32 values read from input set. 
Memory and MIPS limited system, best option is to choose compromise where we 
use as small data memory footprint as possible as well as keep code as simple as 
possible (code size and execution speed).
Absolute minimum data memory required is for the two 32 unsigned integer data sets.

As the largest 32 values requires some insertion based on value orders,
choosing an array for storage will take a hit on MIPS as we have to move elements
to be able to insert new value. A linked list will be a better approach here as we
simply insert new node by modifying a couple of references. A single linked list
will suffice as we can sort samples while inserting new node. This will also save 
mode memory as we don't need to have two references per node, we don't have to store 
the full input data set to be able to run sort after all data has been read.
Hence for this purpose a single linked list is considered better approach than 
moving every single sample in array for worst case every new sample.

On the last 32 values read from input set, I consider a FIFO the simplest and most 
efficient way to store this. I choose a circular buffer as structure.
Saving last 32 values must be done such that order follows input data. Should we 
have less than 32 samples in last reading we need to place the read values 
overwriting oldest samples in last 32 values buffer. E.g. assume SIZE = 4 samples:
Read values		Stored values
1 2 3 4			1 2 3 4
5 6 - -			5 6 3 4
Values written to output:
3
4
5
6
Hence we need to keep track on where we wrote last to buffer. A circular buffer
structure typically contains read/write pointers keeping track of this info.

Input interface. Input data is represented in 12bits/sample. At most 32 
samples are to be stored in input buffer. For fast file access I choose to read
as large a block as possible matching the buffer requirements, i.e. block size
should be 32 12-bit samples.
As fread parses data in bytes, we need a size which aligns with bytes. Indeed
32 12-bit samples fits here.
As the data are 12bits/sample, we will need to map the samples to internal data
path width, typically 16 bit (not specified, but assumption is made here).
Hence there's a need to read three nibbles from input and write them to a 16-bit
unsigned integer as storage. From analyzing the input test files, data seem to be 
ordered as big-endian, hence reading is straight forward.
*/

#define TWO_ARGUMENTS			3		/* nargs pass value */

enum {
	EXECUTABLENAMEID,
	INPUTFILENAMEID,
	OUTPUTFILENAMEID
};

#endif /* _FITBIT12BITBIN2TEXTUTIL_H_ */
