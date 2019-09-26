#include "cursor.cc"
#include "dBMeter.cc"
#include "bigPeaks.cc"
#include "midiClient.cc"

 
#define OLDPEAKS 128

class peaks 
	{
	public:
		peaks(sign &_ledsign,midiClient &_midiClient);
		~peaks();
		
		midiClient *MIDIPOINTER;
		
	  void check(uint i,uint s);		 
		void drawPeaks();
		void calcThreshold();
 		int xScale;
 		 

	 //peaks& operator=(const peaks&);
	  
		cursor CURSOR;
		dBMeter DBMETER;
		bigPeaks BIGPEAKS;
	
	  sign *signPointer;
	  //sign &signPointer;
	
		int aWeighting[SIGN_WIDTH];
		int cWeighting[SIGN_WIDTH];
		int itu[SIGN_WIDTH];
	  float postScale;
	
		uint PEAKDELAY;
		uint OLDPEAKDELAY;
		uint oldPeakDisplay;
		uint oldPeaks[OLDPEAKS];  
		
		uint peak[SIGN_WIDTH];
		uint peakThreshold[SIGN_WIDTH];
		
	  float parabolaWidth;
	  int parabolaY;
	  int parabolaX;
	  int parabolaView;
		int spectrogramType;	
	private:
		uint peakDelay;
		uint oldPeakDelay;
		uint spectrum[SIGN_WIDTH];
	};

//////////////////////////////////////
//
// peaks constructor
//
/////////////////////////////////////
peaks::peaks(sign &_ledsign,midiClient &_MIDIPOINTER) : 
															 signPointer(&_ledsign) ,
															 CURSOR(_ledsign) ,
															 DBMETER(_ledsign) ,
															 BIGPEAKS(_ledsign) ,
															 MIDIPOINTER(&_MIDIPOINTER)
	{
	uint i;
	int f;
	float r;
		
	
	PEAKDELAY		 = 500;
	peakDelay 	 = 0;
	OLDPEAKDELAY = 10;
	oldPeakDelay = 0;
 	postScale		 = 1.00;
	oldPeakDisplay = 0;
	

	// create A weighting function array...once
	f = 0;	
	
 	for (i = 0; i < SIGN_WIDTH; i++)
 		{
		f+= (uint)(signPointer->calcIndex(i,xScale) * signPointer->nIndexFreq);
		
		// the bracket contest
		r = (pow(12200,2) * pow(f, 4)) / 
			  ((pow(f , 2) + pow(20.6 , 2)) * 
				sqrt( (pow(f , 2) + pow(107.7 , 2)) * ((pow(f , 2) + pow(737.9 , 2)))) * 
				(pow(f , 2) + pow(12200 , 2)));

		aWeighting[i] = (int)(2 + 10 * log10(r));
		
	//	printf("aWeighting[%d] = %d    r = %f freq is %d\n\r",i,aWeighting[i],r,f);
		
		r = (pow(12200,2) * pow(f,2)) /
				((pow(f,2) + pow(20.6,2)) * (pow(f,2)+pow(12200,2)));
		
		cWeighting[i] = (int)(0.06 + 10 * log10(r));
		
		  //	printf("cWeighting[%d] = %d    r = %f freq is %d\n\r",i,cWeighting[i],r,f);
  	}
	
	// itu-468 weighting curve
	int ituTableHz[] = {32, 63, 100,200, 400, 800,1000,2000,3150,4000,
										5000, 6300, 7100, 8000, 9000, 10000, 12500,
										14000, 16000, 20000, 31500};

	int ituTabledB[]	= {-30, -24, -20, -14, -8, -2, 0,
											6 , 9, 11, 12, 12, 12, 11,
										10, 8, 0, -5, -12, -22, -43 };
										
	int index = 0;
	f = 0;
	
	for (i = 0; i < SIGN_WIDTH; i++)
 		{
		f+=(uint)(signPointer->calcIndex(i,xScale) * signPointer->nIndexFreq);
 		
 		itu[i]=(ituTabledB[index]);			
 		 		
 		if (f > ituTableHz[index])
 			index++;
 			
 	//	printf("itu[%d] = %d calcFreq[%d] = %d\n",i,itu[i],i,ledsign.calcFreq(f));
		}

	for (i = 0;i < SIGN_WIDTH;i++)
		peak[i] = 0;
 
 	// clear oldPeak array
	for (i = 0 ; i < OLDPEAKS;i++)
		oldPeaks[i]=OLDPEAKS-i;
		
	}

//////////////////////////////////////
//
// peaks destructor
//
/////////////////////////////////////
peaks::~peaks()
	{
		
	}

//////////////////////////////////
//
//		peaks::calcThreshold()
//
///////////////////////////////////
void peaks::calcThreshold()
	{
	int i;
	
	// setup threshold parabola
	for (i=0;i<SIGN_WIDTH;i++)
		peakThreshold[i]=(int)((parabolaWidth*(  (i - parabolaX) * (i - parabolaX) )) + parabolaY);
	}

/////////////////////////////////////
//
//		check for peaks
//
////////////////////////////////////
void peaks::check(uint x, uint y)
	{
	uint i;
	int m;	
/*	
	peakDelay++;	
	
	if (peakDelay > PEAKDELAY)
		{
		// drop the peak....reset peak counter
		for (i=0;i<SIGN_WIDTH;i++)
			if (peak[i] > 0)
				peak[i]--;
 		
		peakDelay = 0;
		}
*/
	oldPeakDelay++;

	peak[x] = y;

	// new peak?
	//if (y > peakThreshold[x])
		{
		switch (oldPeakDisplay)
			{
			case 0: // array..replace y value with new one
				if (oldPeakDelay > OLDPEAKDELAY)
					{
					oldPeakDelay = 0;
					
					// drop peaks
					for (i=0;i<OLDPEAKS;i++)
						if (oldPeaks[i] > 0)
							oldPeaks[i]--;
					}			

//				peak[x] = y;
				
		 		if ( y > peakThreshold[x])
					oldPeaks[x] = y;
			break;

			case 1: // stack .. shift it in
//				peak[x] = y;
			 	
			 	if (y > peakThreshold[x])
			 		{
				 	for (m = OLDPEAKS-2;m > -1 ;m--)
						oldPeaks[m+1] = oldPeaks[m];						

					oldPeaks[0] = x;
					}
				
				oldPeakDelay=0;
			break;
			}		
		}

 	// check for new big peaks
 	BIGPEAKS.bigPeaksCheck(x,y,peakThreshold); 
	}

///////////////////////////////////////////
//
//	draw peaks on sign
//
///////////////////////////////////////////
void peaks::drawPeaks()
	{
  uint x,y,k;
  int color;
   	
	if (CURSOR.CURSOR_MODE == 1)
	 	CURSOR.updateCursor(postScale);

	for (x = 0; x < 127; x++)
		signPointer->setxy(x,47,color);
 	
	for (x = 0;x < SIGN_WIDTH;x++)
		{
		color = 1;

 		if (peak[x] > peakThreshold[x])
			{
			color = 3;
   //	MIDIPOINTER->queueNote(signPointer->calcFreq(x,xScale) ,65.0);
	
	
			} else {
	  					MIDIPOINTER->noteOffCheck();
							}
	    
		if (signPointer->weightingView == 0)
			{
			for (y = 0; y < peak[x]; y++)
				signPointer->setxy(x,SIGN_HEIGHT - 1 - y,color);
		
 			signPointer->setxy(x, SIGN_HEIGHT - 1 - peak[x], 3);	  						
			} 
	 		
 		// show weighting curve
  	if (signPointer->weightingView == 1)
  		{
  		// a weighting
  		if (signPointer->weighting == 1)
  		 signPointer->setxy(x, 26-aWeighting[x] * postScale,3) ;

			// c weighting
  		if (signPointer->weighting == 2)
  			signPointer->setxy(x, 24-cWeighting[x] * postScale,3) ;
 		
 			// uti-468 weighting
  		if (signPointer->weighting == 3)
  			signPointer->setxy(x, 24-itu[x] * postScale,3) ;
			}
		 
		//	parabola view
		if (parabolaView ==1)
			signPointer->setxy(x,SIGN_HEIGHT - 1 - peakThreshold[x],x % 2);

		} // for (x = 0;x < SIGN_WIDTH;x++)
 
	for (k = 0 ; k < OLDPEAKS;k++) 
		{
		if (oldPeakDisplay == 0)  // array view
			{
			if (oldPeaks[k] < 10)
				color = 1;
					else 
						color = 3;
			
			for (y=0;y<15.0 - (((float)48.0 - oldPeaks[k] ) / (float)48.0) * 15.0;y++)
				signPointer->setxy( k,8 + y ,color );  
			}

			if (oldPeakDisplay == 1) // stack view
				{
				color =  (((float)OLDPEAKS - k ) / (float)OLDPEAKS) * 3.0; 	

				for (y=0;y < (((float)OLDPEAKS -k ) / (float)OLDPEAKS) * 8.0;y++)  				
					signPointer->setxy( oldPeaks[k],y+8 ,color );  
				}
			} // for (k = 0 ; k < OLDPEAKS;k++) 
	}
	
