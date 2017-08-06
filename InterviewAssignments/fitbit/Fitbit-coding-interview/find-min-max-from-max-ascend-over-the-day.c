/*
Coding interview with Dustin Torres using Collabedit.
From dataset containing altitude samples from a day hike, find the max ascend over the course of a day.
My solution:
Look at ascending slopes, only.
Find min base level for the current ascend.
-Given this min base level, find the highest point reached (not going below min base level).
-If new min base level is found, save previous data to find the highest climb once this path is analyzed.
Once all samples are analyzed, compare current ascend to previous ascend and make current the highest ascend (swap).
current ascend data will now contain the result.
*/
#include <stdio.h>
#include "testdata/testdata.h"

typedef struct
{
    int hMin;       // local base / minimum value of current ascend
    int accAscend;  // accumulated ascend from hMin
} TAscendCandidate;

void Swap(TAscendCandidate* const a, TAscendCandidate* const b)
{
    TAscendCandidate tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(void)
{
    // Loop over the data sets available in testdata.h:
    int dataSetNo = 0;
    do
    {
        int i;
        int samples = sizeof(dataset[dataSetNo])/sizeof(int);
        TAscendCandidate    prevCandidate = {   0xffff /*hMin*/, 0 /*accAscend*/ }; // initialize to values we for sure will exceed
        TAscendCandidate    currCandidate = {   dataset[dataSetNo][0],
                                                dataset[dataSetNo][1] - dataset[dataSetNo][0] };    // current best estimate is first sample set. Slope is ascend.
        // Dump dataset
        printf("\ndataset[%d]:", dataSetNo);
        for (i=0; i<samples-1;i++)
            printf(" %d,", dataset[dataSetNo][i]);
        printf(" %d\n", dataset[dataSetNo][samples-1]);


        // Loop over the samples in the selected dataset:
        for (i = 0; i < samples-1; i++)
        {
            // two sample window slope, new-previous
            int slope = dataset[dataSetNo][i+1] - dataset[dataSetNo][i];
            if (slope > 0)
            {
                // ascending
                printf("ascending samplewindow analysis: %d -> %d\n", dataset[dataSetNo][i], dataset[dataSetNo][i+1]);

                // Check if new start point in window could be new hMin
                if (currCandidate.hMin > dataset[dataSetNo][i])
                {
                    // new start point hMin candidate. Save current to prev for later comparison.
                    printf("New base level\n");
                    prevCandidate = currCandidate;
                    currCandidate.hMin = dataset[dataSetNo][i];
                    currCandidate.accAscend = slope;
                    printf("hMin:%d, slope:%d; previous: hMin:%d, accAscend:%d\n",
                        currCandidate.hMin, slope, prevCandidate.hMin, prevCandidate.accAscend );
                }
                else
                {
                    // Check if distance from hMin to new end point in window exceeds current max ascend
                    int ascendFromhMin = dataset[dataSetNo][i+1] - currCandidate.hMin;
                    if ( ascendFromhMin > currCandidate.accAscend )
                    {
                        printf("Ascending more\n");
                        currCandidate.accAscend = ascendFromhMin;
                    }
                }
            }
        }
        if (currCandidate.accAscend < prevCandidate.accAscend)
        {   // swap if prev is better than curr.
            printf("curr: hMin=%d, accAscend=%d; prev: hMin=%d, accAscend=%d\n",
                currCandidate.hMin, currCandidate.accAscend, prevCandidate.hMin, prevCandidate.accAscend);
            Swap(&currCandidate, &prevCandidate);
            printf("swapped curr: hMin=%d, accAscend=%d; prev: hMin=%d, accAscend=%d\n",
                currCandidate.hMin, currCandidate.accAscend, prevCandidate.hMin, prevCandidate.accAscend);
        }
        // Dump result
        printf("Greatest ascend for dataset[%d] starts from %d climbing up %d\n", dataSetNo, currCandidate.hMin, currCandidate.accAscend);
    } while (++dataSetNo < DATASETS);

	system("pause");
	return 0;
}
