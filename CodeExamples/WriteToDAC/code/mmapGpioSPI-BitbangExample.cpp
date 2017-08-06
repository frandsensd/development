    #include "mmapGpio.cpp"
    #include "stdio.h"

    #define MOSI 10   // GPIO for MOSI
    #define MISO 9   // GPIO for MISO
    #define SCLK 11   // GPIO for Clock
    #define CE 8   // GPIO for chip enable
     
    using namespace std;
     
    int main (void)
    {
        int a,b,val,loop;
        int a2dChannel=0;
        char tx[3], rx[3];
       
        mmapGpio rpiGpio; // instantiate an instance of the mmapGpio class
       
       printf("GPIO pins exported\n");
       
       rpiGpio.setPinDir(MOSI,mmapGpio::OUTPUT);    // set MOSI to output
       rpiGpio.setPinDir(MISO,mmapGpio::INPUT);    // set MISO to input
       rpiGpio.setPinDir(SCLK,mmapGpio::OUTPUT);    // set SCLK to output
       rpiGpio.setPinDir(CE,mmapGpio::OUTPUT);    // set CE to output
       rpiGpio.writePinHigh(CE);                 // set chip enable high
            rpiGpio.writePinHigh(SCLK);              // set clock high
           
        printf("Set GPIO pin directions\n");

        rpiGpio.writePinLow(CE);  // set chip enable low so ADC will take notice
       
       
        for(loop=0; loop<1000; loop++)
        {
        // setup transmit and recieve bytes
       
        tx[0] = 6 |( ((a2dChannel & 7) >>2));
        tx[1] = 0 |( ((a2dChannel & 3) << 6));
        tx[2] = 0;
        rx[0]=0;
        rx[1]=0;
        rx[2]=0;
           

       for(a=0; a<3; a++)                  // there are 3 bytes to send
       {
          for(b=7; b>=0; b--)               // loop 8 times for each byte
          {
             if(tx[a]& 0x80)             // if bit 7 of tx[a] is a 1
                rpiGpio.writePinHigh(MOSI); // set mosi high
             else
                rpiGpio.writePinLow(MOSI);    // else set mosi low

             rpiGpio.writePinLow(SCLK);      // Set clock low to tell the ADC to read MOSI
             /* Optimization BEGIN: 
             ** By replacing below usleep(10) by this loop author found a significant speedup */
             //for(delay=0; delay<30; delay++);
             /* Optimization END */ 

             if(rpiGpio.readPin(MISO) == mmapGpio::HIGH)
             rx[a]=rx[a]|(0x1<<(b));       // if MISO is high shift left and or it with rx byte         
             
             /* Optimization BEGIN: 
             ** By replacing below usleep(10) by above for loop author found a significant speedup */
             usleep(10);                  // short delay
             /* Optimization END */ 
             
             rpiGpio.writePinHigh(SCLK);    // Set clock back high ready for next loop

       
             tx[a] = tx[a]<<1; // shift tx bits one place left
          }
       }
       
       
       
        val = 0;
        val = (rx[1] & 15 ) <<8;
        val |=  (rx[2] & 255);

       printf("ADC returned  %u\n",val);

       }
       
       rpiGpio.writePinHigh(CE);  // reset chip enable high
       
       
        return 0;
    }

