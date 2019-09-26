class bigPeaks
	{
	public:
		bigPeaks(sign &_ledsign); 
		~bigPeaks();
		void bigPeaksCheck(uint i,uint spectrum,uint peakThreshold[128]);
	  void drawBigPeaks(int xScale);
		
	//	bigPeaks& operator=(const bigPeaks&);
//		sign &SIGNPOINTER;
		sign *SIGNPOINTER;
	
		uint FEEDBACKCHECKDELAY;
		uint ZOOM_BAND_DELAY;
		uint BIGPEAKDELAY;
		uint bigFreq[4];
		uint bigPeak[4];
		
	  uint midiThreshold;
	  uint midiMode;

	private:
		uint oldBigFreq[4];
		uint oldBigFreqChange[4];	
		uint highLightBand[4];
	  uint fbCheckDelay[4];
	 	uint bigPeakDelay; 
		uint zoomBandDelay;
		uint zoomBandColor;
	};

//////////////////////////////////////
//
// bigPeaks constructor
//
/////////////////////////////////////
bigPeaks::bigPeaks(sign &_ledsign) : SIGNPOINTER(&_ledsign) 
	{
	uint i;

	FEEDBACKCHECKDELAY = 1000;
	ZOOM_BAND_DELAY = 50;
	zoomBandDelay		= 0;
	zoomBandColor		= 0; // zoom band color
	bigPeakDelay		= 0;

	// reset peak freqs (peak freqs @ top of sign)
	for (i = 0;i < 4;i++)			
		{
		bigFreq[i] = 2;
		bigPeak[i] = 0;
		highLightBand[i] = 1;
		fbCheckDelay[i] = 0;
		bigFreq[i] = SIGNPOINTER->start[i];
		}
	 
	midiMode = -1;	
	midiThreshold = 0;	
		
	}

//////////////////////////////////////
//
// bigpeaks destructor
//
/////////////////////////////////////
bigPeaks::~bigPeaks()
	{
		
	}

//////////////////////////////////////////
//
//	big peaks check
//
///////////////////////////////////////
void bigPeaks::bigPeaksCheck(uint x, uint spectrumPeak,uint peakThreshold[128])
	{
	uint p;
	int m;

	bigPeakDelay++;
	
	if (bigPeakDelay > BIGPEAKDELAY)
		{
		// drop all bigPeaks
 		for (p=0;p<4;p++)
 			if (bigPeak[p] > 0)
	 			bigPeak[p]--;

		// reset bigPeak counter
		bigPeakDelay = 0;		
		}

	// check for new big peaks
	for (p=0;p<4;p++)
		{
	 	if (spectrumPeak > (bigPeak[p] ) &&  		 // found a new peak...5 bigger than last peak
	  	 (spectrumPeak > peakThreshold[x])  && // found a new peak...5 bigger than last peak	 		
	 		 (x >= SIGNPOINTER->start[p]) &&    // its within the start range
	 		 (x < SIGNPOINTER->start[p+1]  ))   // and the stop range
	 				
	 				{
					fbCheckDelay[p]++;
			
					if (fbCheckDelay[p] > FEEDBACKCHECKDELAY)
			  		{
			  		fbCheckDelay[p] = 0;	
			  	
				 		if ( (bigFreq[p] == oldBigFreq[p]) && (oldBigFreqChange[p] == 0))
			 				{
			 				highLightBand[p] = x;
			 				} else {
			 								highLightBand[p] = 0;
			 								}
			  		}	
					 				
					// add new oldPeak at index 0
					if (bigFreq[p] != oldBigFreq[p] )
						oldBigFreqChange[p] = 1;
							else
								oldBigFreqChange[p] = 0;
								
					oldBigFreq[p] = bigFreq[p];	
			
					// set new big peak
					bigPeak[p] = spectrumPeak;
					bigFreq[p] = x;
					}	 
  	} // for (p=0;p<4;p++)
   
	}

////////////////////////////////////
//
//	void bigPeaks::drawBigPeaks()
//
///////////////////////////////////////
void bigPeaks::drawBigPeaks(int xScale)
	{
	uint p,x,y,flags,color;
	int nIndex;
	int k;
 	uint spacing[4] = {0, 26, 60, 91}; // x coords of big peak freq text		
 	int freq;
 	int fullOn = -8;
 	char freqStr[12];
 	char* note;
 	flags=0;

	if (xScale == 2)
		{
		spacing[1] ++;
		spacing[2] --;
		}

 		if (SIGNPOINTER->zoom == 1) 	
			{
	 		if (SIGNPOINTER->zoomBand > 0)
	 			{
	 			spacing[0] += 0;
	 			spacing[1] += 4;
	 			spacing[2] += 0;
	 			}
	 		}


	// show big peak freqs text at top of ledsign
	for (p=0;p < 4;p++)
		{
 	 	nIndex=2;

 		flags = 2;// turn on Hz text bit

		if (highLightBand[p] != 0)
			flags = flags | 16;

		zoomBandDelay++;
					
		if (zoomBandDelay > ZOOM_BAND_DELAY)
			{
			zoomBandDelay = 0;
			
			// make selected zoom band blink
			if (zoomBandColor == 0)
				zoomBandColor = 3;
					else
						zoomBandColor = 0;
			}
		
		// make selected zoom band brighter
		if (SIGNPOINTER->zoom == 1)
			{
			for	(x=SIGNPOINTER->start[SIGNPOINTER->zoomBand];x < SIGNPOINTER->start[SIGNPOINTER->zoomBand+1];x++)
				{
				SIGNPOINTER->setxy(x,6,3 & zoomBandColor);
				SIGNPOINTER->setxy(x,7,3 & zoomBandColor);
				}
			}

		for (k=0;k < 7;k++)
			SIGNPOINTER->setxy(bigFreq[p],8 + k,3);
			
		for (k=0;k < bigFreq[p];k++) 
			nIndex += SIGNPOINTER->calcIndex(k,xScale);	

		freq = (int)(nIndex) * SIGNPOINTER->nIndexFreq;

		if (SIGNPOINTER->tunerMode == 1)
			{
			// change spacing for notes instead of freqs
			spacing[0] = 2;
			spacing[1] = 30;
			spacing[2] = 81;
			spacing[3] = 115;
			
			note = SIGNPOINTER->freqToNote(freq);
			
			snprintf(freqStr, strlen(note) + 1, "%s",note);
			} else {
							snprintf(freqStr, 8, "%dHz", freq); // take the float freq...format it as 1.0f(turning it to an int)
						
							// left align text?
							if ((p == 0) && (freq < 99) )
								flags = flags | 32;	
					
							if ((p == 1) && (freq < 999)  )
								flags = flags | 32;	
					
							if ((p == 3) && (freq < 9999) )
								flags = flags | 32;	
							}
			
			if (p > 0)	
				{
				SIGNPOINTER->setxy(SIGNPOINTER->start[p],7,3);  // low,low mid,
				SIGNPOINTER->setxy(SIGNPOINTER->start[p],6,3);	// high mid, high markers
				}
				
			SIGNPOINTER->strToSign(freqStr,spacing[p] ,0,flags); 
		} // for (p=0;p < p;p++)
	}

