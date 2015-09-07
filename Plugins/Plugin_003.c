//#######################################################################################################
//##                    This Plugin is only for use with the RFLink software package                   ##
//##                                        Plugin-03: Kaku (ARC)                                      ##
//#######################################################################################################
/*********************************************************************************************\
 * This plugin takes care of sending and receiving the ARC protocol known from Klik-Aan-Klik-Uit (KAKU) 
 * transmitter, switch, PIR, door sensor etc.
 * Devices following the ARC protocol can often be recognized by their manual switch settings for 
 * the unit and house code number. Like for example by code wheel, dip switch or mini switch.
 *
 * This plugin also works with the following devices:
 * Princeton PT2262 / MOSDESIGN M3EB / Domia Lite / Klik-Aan-Klik-Uit / Intertechno / Sartano 2606. 
 * 
 * Author             : StuntTeam & Jonas Jespersen
 * Support            : http://sourceforge.net/projects/rflink/
 * License            : This code is free for use in any open source project when this header is included.
 * License            : This code is free for use in any open source project when this header is included.
 *                      Usage of any parts of this code in a commercial application is prohibited!
 ***********************************************************************************************
 * Address = A0..P16 according to KAKU adressing/notation
 ***********************************************************************************************
 * Het signaal bestaat drie soorten reeksen van vier pulsen, te weten: 
 * 0 = T,3T,T,3T, 1 = T,3T,3T,T, short 0 = T,3T,T,T Hierbij is iedere pulse (T) 350us PWDM
 *
 * KAKU Supports:
 *        on/off, waarbij de pulsreeks er als volgt uit ziet: 000x en x staat voor Off / On
 *    all on/off, waarbij de pulsreeks er als volgt uit ziet: 001x en x staat voor All Off / All On 
 ***********************************************************************************************
 * Brand               Model                   Chipset      Timing L/H
 * Intertechno         ITK200                  MDT10P61S    8/43
 * Profile             PN-47N                  SC5262       5/26-27
 * Elro Home Comfort   AB-600MA                PT2262       7/29
 * Eurodomest          972080                  HS2303
 * Elro Home Control   AB-440 (Home Control)   HX2262
 * Elro Home Control   AB-4xx (Home Control)   HX2272       
 * Profile             PN-44N                  SC2262       9/34
 * ProMax              RSL366T                 SC5262       11/41            
 * Phenix              YC-4000S                HX2262
 * Flamingo            FA500R                               8/30
 ***********************************************************************************************
 * POSSIBLE BIT VARIATIONS: (Opposite order compared to used 'bitstream' format) 
 * ------------------------
 *                    1111 1111 011 1
                      010111101111 rev:
                      1111 0111 101 0    41+f=50 7 fixed 101 ?! 0=on/off
 *                    AAAA BBBB CCC D
 * KAKU:              A bit 0-3  = address 2^4 = 16 addresses
 * Intertechno ITK200 B bit 4-7  = device  2^4 = 16 devices
 *                    C bit 8-10 = fixed (011) 
 *                    D bit 11   = on/off command
 * -------------------
 *                    0 111 0000 1 000
 *                    A BBB CCCC D EEE
 *                    A/C/E      = 0
 * Blokker:           B bit 1-3  = device number 
 *                    D bit 8    = on/off command
 * -------------------
 *                    1111 111111 11
 *                    AAAA BBBBBB CC  
 * Action:            A bit 0-3  = address 2^4 = 16 addresses 
 * Impuls             B bit 4-9  = device           
 *                    C bit 10-11= on/off command   10=on 01=off
 * -------------------
 * Elro Home Easy     10001 10000 10
 *                    AAAAA BBBBB CC
 * Elro Home Control  A bit 0-4  = address 2^5 = 32 addresses
 *                    B bit 5-9  = device  
 * Brennenstuhl       C bit 10-11= on/off command (only 10 or 01)
 *    Comfort
 * -------------------
 *                    10100 00010 0 0 
 *                    AAAAA BBBBB C D
 * InterTechno        A bit 0-4  = address 2^5 = 32 addresses
 * Düwi Terminal      B bit 5-9  = device  
 * Cogex              C bit 10   = always 0 
 *                    D bit 11   = on/off command 
 * -------------------
 *                    11111 11111 1 1
 *                    AAAAA BBBBB C D
 * Sartano:           A bit 0-4  = address 2^5 = 32 addresses / Housecode
 *                    B bit 5-9  = device / Unitcode   11111 each bit corresponds to a button number 
 *                    C bit 10   = off command
 *                    D bit 11   = on command
 * Address = A0..`32 according to KAKU adressing/notation (Sartano uses 5 bit for both house and unit code. Therefore the ascii notation exceeds the alphabet)
 * ------------
 *
 * 20;DB;DEBUG;Pulses=50;Pulses(uSec)=425,1050,250,1025,250,1025,250,1025,250,1025,250,1025,250,1025,250,1025,250,1050,250,1025,250,1025,250,1025,250,1025,250,1025,250,1025,250,1025,250,1050,250,1025,250,1025,950,300,250,1050,950,300,250,1025,950,300,250; 
 *
 * KAKU Doorbell          010111101111
 * 20;07;DEBUG;Pulses=50;Pulses(uSec)=300,950,250,950,250,950,950,275,250,950,250,950,250,950,950,275,250,950,950,275,250,950,950,250,250,950,950,275,250,950,250,950,250,950,950,275,250,950,950,250,250,950,950,275,250,950,950,250,250;
 * 20;09;DEBUG;Pulses=50;Pulses(uSec)=3675,950,250,950,250,950,950,250,250,950,250,950,250,950,950,275,250,950,950,250,250,950,950,275,250,950,950,250,250,950,250,950,250,950,950,250,250,950,950,275,250,950,950,275,250,950,950,275,250;
 * HE842
 * 20;03;DEBUG;Pulses=50;Pulses(uSec)=270,870,840,240,210,870,840,240,210,870,210,870,210,870,840,240,210,870,210,870,210,870,210,870,210,870,210,870,210,870,840,240,210,870,210,870,210,870,840,240,210,870,840,240,210,870,210,870,210,6990;
 * 360/1380  12/46
 * 20;54;DEBUG;Pulses=50;Pulses(uSec)=1410,390,1350,360,1350,360,1380,360,1350,360,1380,360,1380,360,1380,360,1350,360,1350,360,1350,360,1380,360,1380,360,1380,360,1350,360,1380,360,1350,360,1350,360,390,1350,390,1350,390,1320,390,1320,420,1320,420,1320,390,6990;
 \*********************************************************************************************/
#define KAKU_CodeLength             12              // number of data bits
#define KAKU_T                     390 //420 // 370              // 370? 350 us
#define KAKU_R                     300 //360 // 300              // 370? 350 us
#define Sartano_T                  300 //360 // 300              // 300 uS

#ifdef PLUGIN_003
boolean Plugin_003(byte function, char *string) {
      if (RawSignal.Number!=(KAKU_CodeLength*4)+2) return false; // conventionele KAKU bestaat altijd uit 12 data bits plus stop. Ongelijk, dan geen KAKU!
      if (RawSignal.Pulses[0]==15) return true;     // Home Easy, skip KAKU
      if (RawSignal.Pulses[0]==63) return false;    // No need to test, packet for plugin 63
      int i,j;
      boolean error=false; 
      unsigned long bitstream=0L;
      byte command=0;
      byte housecode=0;
      byte unitcode=0;
      int PTLow=22;                                 // Pulse Time - lowest found value (22 = a pulse duration of 550)
      int PTHigh=22;                                // Pulse Time - highest found value
      byte signaltype=0;                            // 0 = Original Kaku

      for (i=0; i<KAKU_CodeLength; i++) {
        j=(KAKU_R*2)/RawSignal.Multiply;   
        j--;
        if (RawSignal.Pulses[0]==33) {              // impuls
           RawSignal.Pulses[0]=0;                   // undo impulse conversion
           j=(KAKU_R)/RawSignal.Multiply;   
           signaltype=5;
        }
        if (RawSignal.Pulses[4*i+1]<j && RawSignal.Pulses[4*i+2]>j && RawSignal.Pulses[4*i+3]<j && RawSignal.Pulses[4*i+4]>j) { // 0101
            bitstream=(bitstream >> 1);  // bit 0
        } else 
        if (RawSignal.Pulses[4*i+1]<j && RawSignal.Pulses[4*i+2]>j && RawSignal.Pulses[4*i+3]>j && RawSignal.Pulses[4*i+4]<j) { // 0110
           bitstream=(bitstream >> 1 | (1 << (KAKU_CodeLength-1)));  // bit f (1)
        } else 
        if (RawSignal.Pulses[4*i+1]<j && RawSignal.Pulses[4*i+2]>j && RawSignal.Pulses[4*i+3]<j && RawSignal.Pulses[4*i+4]<j) { // 0100
           bitstream=(bitstream >> 1);   // Short 0, Group command on 2nd bit.  (NOT USED?!)
           command=2;
        } else 
        if (RawSignal.Pulses[4*i+1]>j && RawSignal.Pulses[4*i+2]<j && RawSignal.Pulses[4*i+3]>j && RawSignal.Pulses[4*i+4]<j) { // 1010
           if (signaltype==5) { 
              bitstream=(bitstream >> 1);  // bit 1 (stored as 0) (IMPULS REMOTE) 
           } else {
              error=true;
           }
        } else {
          if (i==0) {                     // are we dealing with a RTK/AB600 device? then the first bit is sometimes mistakenly seen as 1101  
             if (RawSignal.Pulses[4*i+1]>j && RawSignal.Pulses[4*i+2]>j && RawSignal.Pulses[4*i+3]<j && RawSignal.Pulses[4*i+4]>j) { // 1101
                bitstream=(bitstream >> 1);  // 0
             } else { 
                error=true;
             } 
          } else {
             error=true;
          }
        }                                 // bad signal
      }
      //==================================================================================
      // Prevent repeating signals from showing up
      //==================================================================================
      if(SignalHash!=SignalHashPrevious || (RepeatingTimer<millis()) || SignalCRC != bitstream ) { 
         // not seen the RF packet recently
         SignalCRC=bitstream;
      } else {
         // already seen the RF packet recently
         return true;
      }       
      // ==========================================================================
      // TIMING DEBUG, this will find the shortest and longest pulse within the RF packet
      for (i=2;i<RawSignal.Number;i++) {            // skip first pulse as it is often affected by the start bit pulse duration
          if(RawSignal.Pulses[i] < PTLow) {         // shortes pulse?
             PTLow=RawSignal.Pulses[i];             // new value
          } else 
          if(RawSignal.Pulses[i] > PTHigh) {        // longest pulse?
             PTHigh=RawSignal.Pulses[i];            // new value
          }
      }
      
      //PTLow=PTLow*RawSignal.Multiply;               // get actual value
      //PTHigh=PTHigh*RawSignal.Multiply;             // get actual value

      if( (PTLow == 9 ||  PTLow == 10) && (PTHigh == 36 ||  PTHigh == 37  ||  (PTHigh >= 40  && PTHigh <= 42)) ) signaltype=1; // ELRO 
      else
      if( (PTLow == 10 ||  PTLow == 11)    && ((PTHigh >=40) && (PTHigh <=42)) ) signaltype=2; // Profile PR44N
      else
      if( (PTLow == 13) && ((PTHigh >= 32) && (PTHigh <= 34)) ) signaltype=3; // Profile PR47N
      else
      if( ((PTLow >= 125) && (PTLow <= 12)) && ((PTHigh >= 31) && (PTHigh <= 37)) ) signaltype=4; // Sartano
      else
      if( (PTLow == 12 ||  PTLow == 13) && (PTHigh == 45 ||  PTHigh == 46) ) signaltype=4; // Philips SBC
      //sprintf(pbuffer, "T=%d %d/%d",signaltype,PTLow,PTHigh);     
      //Serial.println( pbuffer );
      //Serial.print(" ");
      //Serial.println(bitstream,BIN);
      // END OF TIMING DEBUG
      // ==========================================================================
      if (error==true) {                            // Error means that a pattern other than 0101/0110 was found
         //Serial.print("Kaku bitstream error : "); 
         //Serial.println(bitstream, BIN);
         return false;                              // This usually means we are dealing with a semi-compatible device 
      }                                             // that might have more states than used by ARC    
      //==================================================================================
      // Determine signal type
      //==================================================================================
      if (signaltype == 4) {                        // Sartano 
         // ----------------------------------      // Sartano 
         housecode = ((bitstream) & 0x0000001FL);       // .......11111b
         unitcode = (((bitstream) & 0x000003E0L) >> 5); // ..1111100000b
         housecode = ~housecode;                    // Sartano housecode is 5 bit ('A' - '`')
         housecode &= 0x0000001FL;                  // Translate housecode so that all jumpers off = 'A' and all jumpers on = '`'
         housecode += 0x41;
         switch(unitcode) {                         // Translate unit code into button number 1 - 5
            case 0x1E:                     // E=1110
                      unitcode = 1;
                      break;
            case 0x1D:                     // D=1101
                      unitcode = 2;
                      break;
            case 0x1B:                     // B=1011
                      unitcode = 3;
                      break;
            case 0x17:                     // 7=0111
                      unitcode = 4;
                      break;
            case 0x0F:                     // f=1111 
                      unitcode = 5;
                      break;
            default:
                     //Serial.print("Sartano:");
                     signaltype=3;
                     break;
                     
         }	  
         if (signaltype == 4) {                        // Sartano 
            if ( ((bitstream >> 10) & 0x03) == 2) {
                command = 1; // On
            } else if ( ((bitstream >> 10) & 0x03) == 1){
                command = 0;// Off
            }		  
         }
      } else     
      if (signaltype == 5) {                        // IMPULS
         housecode = ((bitstream) & 0x0000000FL);       // ........1111b
         unitcode = (((bitstream) & 0x000003F0L) >> 4); // ..1111110000b
         housecode = ~housecode;                    // Impuls housecode is 4 bit ('A' - 'P')
         housecode &= 0x0000000FL;                  // Translate housecode so that all jumpers off = 'A' and all jumpers on = 'P'
         housecode += 0x41;
         unitcode = ~unitcode;                      // Impuls unitcode is 5 bit 
         unitcode &= 0x0000001FL;                   // Translate unitcode so that all jumpers off = '1' and all jumpers on = '64'
         if ( ((bitstream >> 10) & 0x03) == 2) {
            command = 0; // Off
         } else 
         if ( ((bitstream >> 10) & 0x03) == 1){
            command = 1;// On
         }		  
      } else 
      if (signaltype != 4) {                        // KAKU (and some compatibles for now)
         // ----------------------------------
         housecode=(((bitstream) &0x0f) +0x41);
         unitcode=((((bitstream) &0xf0) >> 4)+1);
         // ----------------------------------
         if ((bitstream&0x600)!=0x600) {
            //Serial.println("Kaku 0/1 error");
            return false;                           // use two static bits as checksum
         }
         if ( ((bitstream >> 11) & 0x01) == 1) {
            command=1;   // ON
         } else {
            command=0;   // OFF
         }
      }
      // ==========================================================================
      // Output
      // ----------------------------------
      sprintf(pbuffer, "20;%02X;", PKSequenceNumber++); // Node and packet number 
      Serial.print( pbuffer );
      // ----------------------------------
      if (signaltype < 4) {                         // KAKU (and some compatibles for now)
        Serial.print(F("Kaku;"));                   // Label
      } else {
        if (signaltype == 4) {                      // KAKU (and some compatibles for now)
           Serial.print(F("AB400D;"));              // Label
        } else {
           Serial.print(F("Impuls;"));              // Label
        }
      }
      sprintf(pbuffer, "ID=%02x;", housecode);      // ID    
      Serial.print( pbuffer );
      sprintf(pbuffer, "SWITCH=%d;", unitcode);     
      Serial.print( pbuffer );
      Serial.print(F("CMD="));                    
      if ( command == 1 ) {
         Serial.print(F("ON;"));
      } else {
         Serial.print(F("OFF;"));
      }
      Serial.println();
      // ----------------------------------
      RawSignal.Repeats=true;                    // suppress repeats of the same RF packet 
      RawSignal.Number=0;
      return true;
}
#endif //PLUGIN_003

#ifdef PLUGIN_TX_003
void Arc_Send(unsigned long address);
void TriState_Send(unsigned long bitstream);

boolean PluginTX_003(byte function, char *string) {
        boolean success=false;
        unsigned long bitstream=0L;
        byte command=0;
        uint32_t housecode = 0;
        uint32_t unitcode = 0;
        byte Home=0;                             // KAKU home A..P
        byte Address=0;                          // KAKU Address 1..16
        byte c=0;
        byte x=0;
        // ==========================================================================
        //10;Kaku;00004d;1;OFF;                     
        //012345678901234567890
        // ==========================================================================
        if (strncasecmp(InputBuffer_Serial+3,"KAKU;",5) == 0) { // KAKU Command eg. Kaku;A1;On
           if (InputBuffer_Serial[14] != ';') return false;
        
           x=15;                                    // character pointer
           InputBuffer_Serial[10]=0x30;
           InputBuffer_Serial[11]=0x78;             // Get home from hexadecimal value 
           InputBuffer_Serial[14]=0x00;             // Get home from hexadecimal value 
           Home=str2int(InputBuffer_Serial+10);     // KAKU home A is intern 0  
           if (Home < 0x51)                         // take care of upper/lower case
              Home=Home - 'A';
           else 
           if (Home < 0x71)                         // take care of upper/lower case
              Home=Home - 'a';
           else {
              return false;                       // invalid value
           }
           while((c=InputBuffer_Serial[x++])!=';'){ // Address: 1 to 16/32
              if(c>='0' && c<='9'){Address=Address*10;Address=Address+c-'0';}
           }

           //if (Address==0) {                        // group command is given: 0=all 
           //   command=2;                            // Set 2nd bit for group.
           //   bitstream=Home;
           //} else {
           //   bitstream= Home | ((Address-1)<<4);        
           //}

           bitstream= Home | ((Address-1)<<4);        
           command |= str2cmd(InputBuffer_Serial+x)==VALUE_ON; // ON/OFF command
           bitstream = bitstream | (0x600 | ((command & 1) << 11)); // Stel een bitstream samen
           Arc_Send(bitstream);
           success=true;
        // --------------- END KAKU SEND ------------
        } else
        // ==========================================================================
        //10;AB400D;00004d;1;OFF;                     
        //012345678901234567890
        // ==========================================================================
        if (strncasecmp(InputBuffer_Serial+3,"AB400D;",7) == 0) { // KAKU Command eg. Kaku;A1;On
           if (InputBuffer_Serial[16] != ';') return false;
           x=17;                                    // character pointer
           InputBuffer_Serial[12]=0x30;
           InputBuffer_Serial[13]=0x78;             // Get home from hexadecimal value 
           InputBuffer_Serial[16]=0x00;             // Get home from hexadecimal value 
           Home=str2int(InputBuffer_Serial+12);     // KAKU home A is intern 0  
           if (Home < 0x61)                         // take care of upper/lower case
              Home=Home - 'A';
           else 
           if (Home < 0x81)                         // take care of upper/lower case
              Home=Home - 'a';
           else {
              return false;                       // invalid value
           }
           while((c=InputBuffer_Serial[x++])!=';'){ // Address: 1 to 16/32
              if(c>='0' && c<='9'){Address=Address*10;Address=Address+c-'0';}
           }
           command = str2cmd(InputBuffer_Serial+x)==VALUE_ON; // ON/OFF command
           housecode = ~Home;
           housecode &= 0x0000001FL;
           unitcode=Address;
           if ((unitcode  >= 1) && (unitcode <= 5) ) {
              bitstream = housecode & 0x0000001FL;
              if (unitcode == 1) bitstream |= 0x000003C0L;
              else if (unitcode == 2) bitstream |= 0x000003A0L;
              else if (unitcode == 3) bitstream |= 0x00000360L;
              else if (unitcode == 4) bitstream |= 0x000002E0L;
              else if (unitcode == 5) bitstream |= 0x000001E0L;

              if (command) bitstream |= 0x00000800L;
              else bitstream |= 0x00000400L;					
           }
           Arc_Send(bitstream);
           success=true;
        }
        // --------------- END SARTANO SEND ------------
        // ==========================================================================
        //10;Impuls;00004d;1;OFF;                     
        //012345678901234567890
        // ==========================================================================
        if (strncasecmp(InputBuffer_Serial+3,"Impuls;",7) == 0) { // KAKU Command eg. Kaku;A1;On
           if (InputBuffer_Serial[16] != ';') return false;
           x=17;                                    // character pointer
           InputBuffer_Serial[12]=0x30;
           InputBuffer_Serial[13]=0x78;             // Get home from hexadecimal value 
           InputBuffer_Serial[16]=0x00;             // Get home from hexadecimal value 
           Home=str2int(InputBuffer_Serial+12);     // KAKU home A is intern 0  
           if (Home < 0x61)                         // take care of upper/lower case
              Home=Home - 'A';
           else 
           if (Home < 0x81)                         // take care of upper/lower case
              Home=Home - 'a';
           else {
              return false;                       // invalid value
           }
           while((c=InputBuffer_Serial[x++])!=';'){ // Address: 1 to 16/32
              if(c>='0' && c<='9'){Address=Address*10;Address=Address+c-'0';}
           }
           command = str2cmd(InputBuffer_Serial+x)==VALUE_ON; // ON/OFF command
           housecode = ~Home;
           housecode &= 0x0000001FL;
           unitcode=Address;
           if ((unitcode  >= 1) && (unitcode <= 5) ) {
              bitstream = housecode & 0x0000001FL;
              if (unitcode == 1) bitstream |= 0x000003C0L;
              else if (unitcode == 2) bitstream |= 0x000003A0L;
              else if (unitcode == 3) bitstream |= 0x00000360L;
              else if (unitcode == 4) bitstream |= 0x000002E0L;
              else if (unitcode == 5) bitstream |= 0x000001E0L;

              if (command) bitstream |= 0x00000800L;
              else bitstream |= 0x00000400L;					
           }
           TriState_Send(bitstream);
           success=true;
        }
        return success;
}

void Arc_Send(unsigned long bitstream) { 
    int fpulse = 360;                               // Pulse width in microseconds
    int fretrans = 7;                               // Number of code retransmissions
    uint32_t fdatabit;
    uint32_t fdatamask = 0x00000001;
    uint32_t fsendbuff;

    digitalWrite(PIN_RF_RX_VCC,LOW);                // Turn off power to the RF receiver 
    digitalWrite(PIN_RF_TX_VCC,HIGH);               // Enable the 433Mhz transmitter
    delayMicroseconds(TRANSMITTER_STABLE_DELAY);    // short delay to let the transmitter become stable (Note: Aurel RTX MID needs 500µS/0,5ms)

    for (int nRepeat = 0; nRepeat <= fretrans; nRepeat++) {
        fsendbuff = bitstream;
        // Send command
        
		for (int i = 0; i < 12; i++) {              // Arc packet is 12 bits 
            // read data bit
            fdatabit = fsendbuff & fdatamask;       // Get most right bit
            fsendbuff = (fsendbuff >> 1);           // Shift right

			// PT2262 data can be 0, 1 or float. Only 0 and float is used by regular ARC
            if (fdatabit != fdatamask) {            // Write 0
                digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 3);
				digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 3);
            } else {                                // Write float
                digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse * 1);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 3);
				digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse * 3);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 1);
            }
        }
		// Send sync bit
        digitalWrite(PIN_RF_TX_DATA, HIGH);         
        delayMicroseconds(fpulse * 1);
        digitalWrite(PIN_RF_TX_DATA, LOW);          // and lower the signal
        delayMicroseconds(fpulse * 31);
    }
    delayMicroseconds(TRANSMITTER_STABLE_DELAY);    // short delay to let the transmitter become stable (Note: Aurel RTX MID needs 500µS/0,5ms)
    digitalWrite(PIN_RF_TX_VCC,LOW);                // Turn thew 433Mhz transmitter off
    digitalWrite(PIN_RF_RX_VCC,HIGH);               // Turn the 433Mhz receiver on
    RFLinkHW();
}

void TriState_Send(unsigned long bitstream) { 
    int fpulse = 360;                               // Pulse width in microseconds
    int fretrans = 7;                               // Number of code retransmissions
    uint32_t fdatabit;
    uint32_t fdatamask = 0x00000003;
    uint32_t fsendbuff;

    digitalWrite(PIN_RF_RX_VCC,LOW);                // Turn off power to the RF receiver 
    digitalWrite(PIN_RF_TX_VCC,HIGH);               // Enable the 433Mhz transmitter
    delayMicroseconds(TRANSMITTER_STABLE_DELAY);    // short delay to let the transmitter become stable (Note: Aurel RTX MID needs 500µS/0,5ms)

    for (int nRepeat = 0; nRepeat <= fretrans; nRepeat++) {
        fsendbuff = bitstream;
        // Send command
        
		for (int i = 0; i < 12; i++) {              // Sartano packet is 12 bits 
            // read data bit
            fdatabit = fsendbuff & fdatamask;       // Get most right 2 bit
            fsendbuff = (fsendbuff >> 2);           // Shift right

			// data can be 0, 1 or float. 
            if (fdatabit == 0) {                    // Write 0
                digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 3);
				digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 3);
            } else 
            if (fdatabit == 1) {                    // Write 1
                digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse * 3);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 1);
				digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse * 3);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 1);
            } else {                                // Write float
                digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse * 1);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 3);
				digitalWrite(PIN_RF_TX_DATA, HIGH);
                delayMicroseconds(fpulse * 3);
                digitalWrite(PIN_RF_TX_DATA, LOW);
                delayMicroseconds(fpulse * 1);
            }
        }
		// Send sync bit
        digitalWrite(PIN_RF_TX_DATA, HIGH);         
        delayMicroseconds(fpulse * 1);
        digitalWrite(PIN_RF_TX_DATA, LOW);          // and lower the signal
        delayMicroseconds(fpulse * 31);
    }
    delayMicroseconds(TRANSMITTER_STABLE_DELAY);    // short delay to let the transmitter become stable (Note: Aurel RTX MID needs 500µS/0,5ms)
    digitalWrite(PIN_RF_TX_VCC,LOW);                // Turn thew 433Mhz transmitter off
    digitalWrite(PIN_RF_RX_VCC,HIGH);               // Turn the 433Mhz receiver on
    RFLinkHW();
}
#endif //PLUGIN_TX_003
