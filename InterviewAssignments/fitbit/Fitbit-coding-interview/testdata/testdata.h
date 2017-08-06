/*
Example datasets:
Original from interview:    1000, 1005, 100, 200, 150, 300, 305, 0, 200, 1
    Result:	min = 100, max = 305. Reason is 100 is local min, and the largest ascend from that minimum is 205.
Additional:                 500, 1000, 1005, 100, 200, 150, 300, 0, 305, 900, 1
    Result:	min = 0, max = 900. Reason is 0 is local min, and the largest ascend from that minimum is 900.
Additional:                 1000, 1005, 250, 100, 300, 305, 150, 500, 75, 100, 300, 600, 900, 700, 500
    Result:	min = 75, max = 900. Reason is 75 is local min, and the largest ascend from that minimum is 825.
Additional:                 80, 100, 250, 100, 300, 305, 150, 500, 75, 100, 300, 200, 100, 150, 200
    Result:	min = 80, max = 500. Reason is 80 is local min, and the largest ascend from that minimum is 420.
*/
int dataset[4][15] = {
    { 1000, 1005,  100,  200,  150,  300,  305,    0,  200,    1,  200,    1,    1,    1,    1 },   // min=100, max=305, max ascend=205
    {  500, 1000, 1005,  100,  200,  150,  300,    0,  305,  900,    1,    1,    1,    1,    1 },   // min=0, max=900, max ascend=900
    { 1000, 1005,  250,  100,  300,  305,  150,  500,   75,  100,  300,  600,  900,  700,  500 },   // min=75, max=900, max ascend=825
    {   80,  100,  250,  100,  300,  305,  150,  500,   75,  100,  300,  200,  100,  150,  200 }    // min=0, max=500, max ascend=500
    };
#define DATASETS        (int) 4

