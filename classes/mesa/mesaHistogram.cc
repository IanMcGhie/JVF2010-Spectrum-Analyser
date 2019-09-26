#include <unistd.h>

#define KEYS_PER_OCTAVE 12
#define MAX_NOTES 88
#define NOTES_PER_SCALE 45
#define PENTATONIC 5
#define DIATONIC 7
#define OCTAVES 8
#define SCALES 4 // major / minor / pentatonic major / pent minor

extern void histogramDisplayCB(int whichScale,int whichKey);
extern void flashBPM(int flash);
extern void updateBPMMenuItem(int value);


char *KEY[] = { (char*)"A",
								(char*)"A#",
								(char*)"B",
								(char*)"C",
								(char*)"C#",
								(char*)"D",
								(char*)"D#",
								(char*)"E",
								(char*)"F",
								(char*)"F#",
								(char*)"G",
								(char*)"G#"
							};

char *SCALE[] = { (char*)"Major",
									(char*)"Minor",
									(char*)"Pent Major",
									(char*)"Pent Minor"
								};
								
class mesaHistogram
	{
	public:
		mesaHistogram(sign &_SIGNPOINTER,midiClient &_MIDIPOINTER);
		~mesaHistogram();
		void init(int debug);
		void add(uint *_peak,uint *_peakThreshold);
		void keyTest();
		void draw();
		void setupCamera();

		sign *SIGNPOINTER;
		midiClient *MIDIPOINTER;
		
		int hg[SIGN_WIDTH];
		double pianoKeys[MAX_NOTES];
		double scales[SCALES][KEYS_PER_OCTAVE][DIATONIC * OCTAVES]; // major / minor scales
		int STEPS[SCALES][DIATONIC];
		int scaleHistogram[SCALES][KEYS_PER_OCTAVE][SIGN_WIDTH];
		int notesPerScale[SCALES];
		int whichKey,whichScale;
		char *notes[KEYS_PER_OCTAVE];
		char *noteChar;
		
		uint *peakThreshold;
		int playKey;
		int notePlaying;
	 	int key;
	 	int keyScale;
		int playKeyDelay;
		int PLAYKEYDELAY;
		int inited;
		int keyOdds[SCALES][KEYS_PER_OCTAVE];
		int showKeyHistogram;
	 	float BPM;
		
		int histogramThresholdY;

		int resetHistogram;
		int RESETHISTOGRAM;
	private:
		
	};
	
/////////////////////////
//
//	const
//
///////////////////////////
mesaHistogram::mesaHistogram(sign &_SIGNPOINTER,midiClient &_MIDIPOINTER) : 
																		SIGNPOINTER(&_SIGNPOINTER) ,MIDIPOINTER(&_MIDIPOINTER)
	{
  inited = 0;
  
//	init(1); // debug
	init(0);
	} // end o constructor

/////////////////////////
//
//	deconst
//
///////////////////////////
mesaHistogram::~mesaHistogram()
	{
		
	}

//////////////////////////////////
//
//				void mesaHistogram::init()
//
/////////////////////////////////////
void mesaHistogram::init(int debug)
	{
		
	int i,x,k,n,note,s;
	double f;
	int changed = 0;
  int index = 0;
  int scaleIndex = 0;
  int lastX = -1;
  double lastNote = 2750;


	notePlaying = 0;
  playKeyDelay = 0;
  note =0;
  key = 0;
	
  notes[0] = (char*)"A";
  notes[1] = (char*)"A#";
  notes[2] = (char*)"B";
  notes[3] = (char*)"C";
  notes[4] = (char*)"C#";
  notes[5] = (char*)"D";
  notes[6] = (char*)"D#";
  notes[7] = (char*)"E";
  notes[8] = (char*)"F";
  notes[9] = (char*)"F#";
  notes[10] = (char*)"G";
  notes[11] = (char*)"G#";
  
  
  
 	int steps[SCALES][DIATONIC] = {
 																{2,2,1,2,2,2,1}, // diatonic major w w h w w w h
 																{2,1,2,2,1,2,2}, // diatonic minor 
 																{2,2,3,3,2,0,0}, // pentatonic major
 																{2,3,2,2,3,0,0}	 // pentatonic minor
 																};


	for (s = 0;s < SCALES;s++)
		for(i = 0; i < DIATONIC;i++)
			STEPS[s][i] = steps[s][i];
	
	int notesPerScale[SCALES] = {
															DIATONIC,  
															DIATONIC,
															PENTATONIC,
															PENTATONIC
															};
	
	RESETHISTOGRAM = 10;
	resetHistogram = 0;
	
	for (x = 0;x < SIGN_WIDTH;x++)
		hg[x] = 0;

	 
	// thx to suits@mtu.edu (Byran Suits) for this equation
	//n = 39.86314 log(Fn) - 105.37632
	//n = (int)((39.86314 * log10(freq)) - 105.37632);
	//endwin();
	
	// starting @ A0
	for (k = 0;k < MAX_NOTES;k++)		
			{
			changed = 0;	
			
			for (f = lastNote;f < 2205000;f+=0.01)
				{
				//note = (int)((39.86314 * log10((double)f / 100.000)) -  105.37632) + 47;	 // steps from A4
 					note = (int) (((log10((double)f / (double)100.0) - log10(27.5000)) / log10(1.059463094))); // steps from A0
 				
 				if ((note == k) )
					{
 					pianoKeys[k] = ((double)f / 100.000);
					lastNote = f + 10;  
					
					if (debug == 1)
						printf("pianoKeys[%d] = %2.3fHz (%s)\n",k,pianoKeys[k],
																										SIGNPOINTER->freqToNote(pianoKeys[k]));	
					changed = 1;
					break;
					}  
				
				if (changed == 1)
					break;
				}
			} // for (k = 0;k < MAX_NOTES;k++)		
	
	if (debug == 1)
 		{
	 	printf("A0 = %s\n",SIGNPOINTER->freqToNote(27.5 * 1));
	 	printf("B0 = %s\n\n",SIGNPOINTER->freqToNote(30.868));
	 	printf("A1 = %s\n",SIGNPOINTER->freqToNote(27.5 * 2));
	 	printf("B1 = %s\n\n",SIGNPOINTER->freqToNote(61.736));
	 	printf("A2 = %s\n",SIGNPOINTER->freqToNote(27.5 * 4));
	 	printf("A3 = %s\n",SIGNPOINTER->freqToNote(27.5 * 8));
	 	printf("A4 = %s\n",SIGNPOINTER->freqToNote(27.5 * 16));
	 	printf("A5 = %s\n",SIGNPOINTER->freqToNote(27.5 * 32));
	 	printf("A6 = %s\n",SIGNPOINTER->freqToNote(27.5 * 64));
		}
		
	// reset scale histograms
	for (s = 0 ; s < SCALES;s++)
		for (k = 0 ; k < KEYS_PER_OCTAVE;k++)
			for (x = 0;x < SIGN_WIDTH;x++)
 				scaleHistogram[s][k][x] = 0;
 				
 	//exit(1);
	// calc all the diatonic scales	
	for (s = 0 ; s < SCALES;s++)
		{
		for (k = 0 ; k < KEYS_PER_OCTAVE;k++)
			{
			keyOdds[s][k] = 0; //reset key odds
 	 		index = k;
			
			for (n = 0; n < NOTES_PER_SCALE; n++)
				{
				scales[s][k][n] 	= pianoKeys[index];
				
				x = SIGNPOINTER->freqToX(pianoKeys[index]); 
				scaleHistogram[s][k][x] = 1;//histogramThresholdY;

				//if (s < 2) // diatonic scale
					index += steps[s][(n % notesPerScale[s] )];		
				
		//		if ((s > 1) && (n < 5)) // pentatonic scale
		//			index += steps[s][(n % notesPerScale[s] )];		
				} //for (n = 0; n < DIATONIC * OCTAVES; n++)
			} //	for (k = 0 ; k < KEYS_PER_OCTAVE;k++)
		} //		for (s = 0 ; s < SCALES;s++)
		
//	printf("G# = %s\n",SIGNPOINTER->freqToNote(51.913));// g#

	// show keys major / minor scales
	s = 2;
	for (s = 0 ; s < SCALES;s++)
		{
		for (k = 0 ; k < KEYS_PER_OCTAVE ;k++)
			{
			for (n = 0; n < NOTES_PER_SCALE; n++)
				{
	 			//endwin();
	//			printf("freq to x %d\n",SIGNPOINTER->freqToX(scales[s][k][n]));
				if (debug == 1)
					{
					printf("scales[%d][%d][%d] =  %2.3f (%s) (%s %s) scaleHistogram[%d][%d][%d] =  %d\n",
									s,k,n,scales[s][k][n],
									SIGNPOINTER->freqToNote(scales[s][k][n]),
									notes[k],
									SCALE[s],
									s,k,n,
									scaleHistogram[s][k][SIGNPOINTER->freqToX(scales[s][k][n])]);
					}
	/*	
				printf("index = %d steps[%d] = %d (%d)\n\n",
							index,n,steps[s][(n % notesPerScale[s] ) ],
							(n % notesPerScale[s]) );
	*/	
						
				} // for (n = 0; n < 45; n++)
			} // for (k = 0 ; k < KEYS_PER_OCTAVE ;k++)
		} // for (s = 0 ; s < 2;s++)

  //	 exit(1);
		
	inited = 1;	
	}

///////////////////////////////////
//
//			void mesaHistogram::addRow(uint *_peak,uint *_peakThreshold)
//
//////////////////////////////
void mesaHistogram::add(uint *_peak,uint *_peakThreshold)
	{
	int amp,i,x,z,frequencyIndex;	
								
	peakThreshold = _peakThreshold;

	// go thru peaks ...looking for biggest
//	for (x = 0 ; x < SIGN_WIDTH; x++)
	for (x = 0 ; x < BUFFERFRAMES; x++)
		{	
		// find biggest peak in band range...we have 128 bands...but only
	 	// show 8...16...24 ect...
	 	
		if (_peak[x] > _peakThreshold[x])
			{
			hg[x]++;
			
			
			}
	
 			
		} // for (x = SIGN_WIDTH - 1;x > 0;x -= SIGN_WIDTH / bands)

	} // void mesaSpectrogram::addRow(uint *_peak,uint *_peakThreshold)

	
/////////////////////////
//
//				void mesaHistogram::draw()
//
///////////////////////////
void mesaHistogram::draw()
	{
	int x,index,k,n,s;
	float freq;
 	int found;
	int currentOdds;
	float milliSeconds;
	resetHistogram++;
	
	if (resetHistogram > RESETHISTOGRAM)
		{
		resetHistogram = 0;
		
		keyTest();

		} // if (resetHistogram > RESETHISTOGRAM)  
			
	glPushMatrix();
	
	glColor3f(1.0,0.0,0.0);		
	
	
	for (x=0;x < SIGN_WIDTH;x++)
		{
		glBegin(GL_LINES);

		glVertex3f(x,
							0.0,
							0.0 );

		glVertex3f(x,
							hg[x],
							0.0 );
		glEnd();

		}	

	if (showKeyHistogram)
		{
		glPointSize(1.0);
		
		for (x=0;x < SIGN_WIDTH;x++)
			{
			glBegin(GL_POINTS);			
			glVertex3f(x,
								scaleHistogram[keyScale][key][x],
								0.0 );
			
//			SIGNPOINTER->freqToX(pianoKeys[index])] 
//										= SIGNPOINTER->freqToX(pianoKeys[index]);
			
			glEnd();
			}
	
		
		index = 0;

		 
		} // if (showKeyHistogram)

	if (playKey == 1)
		{
		int velocity = 70;
		
//		playKeyDelay++;		
		
		if (playKeyDelay > PLAYKEYDELAY)
			{
			playKeyDelay = 0;	
			//	MIDIPOINTER->noteOffCheck();
 			MIDIPOINTER->queueNote(scales[keyScale][key][notePlaying], velocity);	

		//		endwin();
			//		printf("key is %d note is %d\n",key,note);
			notePlaying++;

			if (notePlaying > NOTES_PER_SCALE)
				notePlaying = 0;
			} // if (playKeyDelay > PLAYKEYDELAY)
		} // if (playKey == 1)

	glPopMatrix();

 	 
//	endwin();
//	printf("tc = %2.2f\n",tapClock);
		milliSeconds = ((float)(clock() - MIDIPOINTER->tapClock) 
											/ CLOCKS_PER_SEC) * 1000.0;

	if (milliSeconds > (MIDIPOINTER->checkForNotes ) )
		{
		int peakX = 0;
			
		for (x = 0;x < SIGN_WIDTH;x++)	
			if (hg[x] > peakX)
				peakX = x;	

		MIDIPOINTER->queueNote(SIGNPOINTER->calcFreq(peakX,0) ,70);	
	
		flashBPM(MIDIPOINTER->flashOn);
		
		MIDIPOINTER->flashOn	= -MIDIPOINTER->flashOn;
 		//MIDIPOINTER->tapClock = clock() - (float)clock();
		if (MIDIPOINTER->tapOn == -1)
			MIDIPOINTER->tapClock = clock();
		}  
 
	
	if (MIDIPOINTER->tapOn == 1)	
		{
		//	MIDIPOINTER->checkForNotes = milliSeconds;
			updateBPMMenuItem(milliSeconds);
		}
	
	//if (milliSeconds * 1000.0 > MIDIPOINTER->checkForNotes)	
	//	MIDIPOINTER->tapClock = clock();	
	}
	
///////////////////////////////
//
// key test
//
///////////////////////////////
void mesaHistogram::keyTest()
	{
	int s,k,x,found,currentOdds;
	int velocity,peakX;
	
 		for (s = 0;s < SCALES;s++)		
			for (k = 0 ; k < KEYS_PER_OCTAVE;k++)
				{ 
					for (x = 0;x < SIGN_WIDTH;x++)
						{
						// compare incoming histrogram to key histograms
						if ((hg[x] > 0) && (scaleHistogram[s][k][x] > 0))
								keyOdds[s][k]++;
										
						}	// for (x = 0;x < SIGN_WIDTH;x++)
			 	}			//  for (k = 0 ; k < KEYS_PER_OCTAVE;k++)

	currentOdds = 0;
 
		
	for (s = 0;s < SCALES;s++)
		for (k = 0 ; k < KEYS_PER_OCTAVE;k++)
			{
  	//	 	printf("keyOdds[%d][%d] = %d\n",s,k,keyOdds[s][k]);
				
			if (keyOdds[s][k] > currentOdds)
				{
//		 		endwin();
				currentOdds = keyOdds[s][k];
				
 	// 		printf("keyOdds[%d][%d] = %d\n",s,k,keyOdds[s][k]);
		
				whichKey = k;
				whichScale = s;
				}
				
			//reset key odds
			keyOdds[s][k] = 0;
			}

 //	printf("whichScale =%d whichKey = %d\n",whichScale,whichKey);


	histogramDisplayCB(whichScale,whichKey); 
	
	velocity = SIGNPOINTER->clamp(hg[SIGNPOINTER->freqToX(pianoKeys[whichKey + 44])] * 12,
						69,
						100);
 
 	 
 	peakX = 0;
 	
 	for (x = 0;x < SIGN_WIDTH;x++)
		{
		if (hg[x] > peakX)
			peakX = x;
		}
 		
	MIDIPOINTER->queueNote(pianoKeys[whichKey + 44] , velocity);	
 
	// reset input histogram
 	for (x = 0;x < SIGN_WIDTH;x++)
		hg[x] = 0;
	
	}
	
/////////////////////////////////
//
// 	setup camera
//
////////////////////////////////
void mesaHistogram::setupCamera()
	{
	 
	gluLookAt(32.0,2.0,10.0,
						32.0,0.0,-25.0,
						0.0,1.0,0.0);
		 	
	}



	