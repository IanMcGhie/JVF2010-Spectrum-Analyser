#define SCOPEBUFFERSIZE BUFFERFRAMES * 24

extern double sqr (double arg); 


class oscilloscope
	{
	public:
		oscilloscope(sign &_ledsign);
		~oscilloscope();
		
		sign &scopeLedsign;
		
		void acquire(jack_default_audio_sample_t _inL[BUFFERFRAMES]);
		void draw();

		int scopeGain;
		float timeBase;	
		float triggerLevel;
		float timeBaseX;
		float timeBaseY;
	
	private:	
		float buffer[SCOPEBUFFERSIZE];
		uint bufferPointer;
		uint triggered;
		uint triggerX;
		uint frames;
	};
	
//////////////////////////// 
// 	
//consturctor 
//
/////////////////////////////
//peaks::peaks(sign &_ledsign) : peaksledsign(_ledsign) ,
oscilloscope::oscilloscope(sign &_ledsign) : scopeLedsign(_ledsign) 
	{
	bufferPointer = 0;
	triggered = 0;
	frames = 0;
	}

////////////////////////////
//
//		destruction
//
/////////////////////////////	
oscilloscope::~oscilloscope()
	{
		
	}
	
////////////////////////////
//
//	void oscilloscope::acquire(jack_default_audio_sample_t _inL[BUFFERFRAMES])
//
/////////////////////////////////
void oscilloscope::acquire(jack_default_audio_sample_t _inL[BUFFERFRAMES])
	{


	uint i,x;
	frames++;
		
	for (i=0;i<BUFFERFRAMES;i++) 
	//for (i=0;i<4;i++) 
			{

//			if (triggerLevel > 0.0) 
				if ((_inL[i] * 24.0) > triggerLevel)
					triggered = 1;


	//		if (triggerLevel < 0.0) 
				if ((_inL[i] * 24.0) < triggerLevel)
						triggered = 1;
	
			if (triggered == 1) 
				{
				timeBaseY = 24 -((2.0 * 24.0) / M_PI) * 
								atan( (1 / tan((triggerX*M_PI) / (48-(1.0/timeBase)))) ); // 128 = 22050hz

				timeBaseX = 64 -((2.0 * 64.0) / M_PI) * 
								atan( (1 / tan((triggerX*M_PI) / (64-(1.0/timeBase)))) ) - 1; // 128 = 11hz
		/*		
				if (timeBaseY > (timeBase*64))
					{
					triggered = 0;	
					}
				
				if (timeBaseX > 127)
					{
		//			triggerX = 0;
					triggered = 0;

					}					
*/
//assert(timeBaseY < 48);
//assert(timeBaseX < 128);

				// show timebase
				scopeLedsign.setxy(triggerX,timeBaseY,1);	
				
				// show triggered signal
				//if (timeBaseX < 128)
	  			
	  	//		scopeLedsign.setxy(timeBaseX,(24.0 + (_inL[i / (int)128/timeBaseX] * scopeGain)),3);	
	  			 
	  			//scopeLedsign.setxy(triggerX,(24+(_inL[i / ((BUFFERFRAMES/4)/(128-timeBaseX+1))] * scopeGain)),1);	
	  				
	  				
	  				
	  			//scopeLedsign.setxy(timeBaseX,48.0 - (24.0 + 10*log10(_inL[i] * pow(10,scopeGain/10 ))),1);	
				 
		
		
				triggerX++;
							
				if (triggerX > 127)
					{
					triggerX = 0;
//					triggered = 0;
	//				 scopeLedsign.clear(); 
					}
				
				}	
			} //for (i=0;i<BUFFERFRAMES;i++) 
	}
/////////////////////////////////
//
// draw
//
//////////////////////////////////
void oscilloscope::draw()
	{
	int i;

scopeLedsign.setxy(1,24,3);
	// 0.000045351473 = 22.5KHz
			


	for (i=0;i<128;i++) // this takes 45 micro seconds (45uS) = 22050hZ
			{
//				timeBaseY = 24 -((2.0 * 24.0) / M_PI) * 
//								atan( (1 / tan((i*M_PI) / (128))) ); // 128 = 11hz

//			timeBaseY = 24 -((2.0 * 24.0) / M_PI) * 
//								atan( (1 / tan((i*M_PI) / (128))) ); // 128 = 11025hz

//				timeBaseY = 24 -((2.0 * 24.0) / M_PI) * 
//								atan( (1 / tan((i*M_PI) / (128))) ); // 128 = 22050hz


				timeBaseY = 24 -((2.0 * 24.0) / M_PI) * 
								atan( (1 / tan((i*M_PI) / (128))) ); // 128 = 22050hz
			
			// draw timebase	
			scopeLedsign.setxy(i,timeBaseY,1);	
		
			triggerX++;

			

		//	scopeLedsign.setxy(i,48-(24+buffer[i * 384 ] * scopeGain),1);	// 2hz
	//		scopeLedsign.setxy(i,48-(24+buffer[i * 64] * scopeGain),1);	//11025hz
//			scopeLedsign.setxy(i,48-(24+buffer[i * 32] * scopeGain),1);	//22050hz			
		
			}
	
	//scopeLedsign.refresh();
	
	triggered = 0;
	bufferPointer=0;
	
	}