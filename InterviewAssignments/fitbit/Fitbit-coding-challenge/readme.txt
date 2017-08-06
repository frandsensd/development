Overview of solution

dataset contains testvectors and test results.
NOTE: I added two extra test vectors as I found an issue with alternating samples. So test4 and test5 are addressing this.

source contains source code and headers.

linux contains makefile and test script for Linux platform.

windows contains visual studio project and test script for Windows platform.

In sources
Fitbit12bitBin2TextUtil is the main module.
Header file contains the initial design considerations where the details are left near the implementation.
types.h and commonDefinitions.h are the common headers.
I chose to split all modules up in files to make it easy for maintenance and source control across multiple
engineers as I have done in the past and found very useful.
By splitting up functionality in smaller modules also allows for hiding some of the complexity from the module 
using the functionality. Hence list and fifoBuffer modules are only function calls.
There were a comprimise between ease of use and memory footprint, where I took out on-the-fly initialization
for list and fifoBuffer modules at the cost of having to call ListInit and FifoBufferInit prior to using modules.


Findings:
test1/test2/test3 are generated in Windows and hence produces
a difference when comparing to output from Linux executable.
Also there's a spelling mistake in the out files, which reveals 
itself when comparing to the solution output. I chose not to change
this as I don't like changing reference vectors without consent from 
producer.
Simply ignore the trailing carriage return and the spelling mistake in the reference out files and its good.
Comparing reference to output from visual studio again spelling mistake surfaces - the rest is bit-exact.


Best regards,

Torben Frandsen
