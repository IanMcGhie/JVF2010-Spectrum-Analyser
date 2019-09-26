
class cursor
{
private:
		
public:
	cursor(sign &_SIGNPOINTER);
	~cursor();
	
	void updateCursor(float scale);	
	
	//cursor& operator=(const cursor&);
		
	uint CURSOR_MODE;
	uint cursorX;
	uint cursorY;
	
  sign *SIGNPOINTER;

};

//////////////////////////////////////
//
// cursor constructor
//
/////////////////////////////////////
cursor::cursor(sign &_SIGNPOINTER) : SIGNPOINTER(&_SIGNPOINTER) 
	{
	CURSOR_MODE = -1;
	cursorX 		= 46; // 1kHz
	cursorY 		= 15;
	}

//////////////////////////////////////
//
// cursor destructor
//
/////////////////////////////////////
cursor::~cursor()
	{
		
	}

//////////////////////////////////////////
//
//	update cursor
//
///////////////////////////////////////
void cursor::updateCursor(float scale)
	{
  uint flags =0;
	uint k,nIndex=0;
	
 	
	uint freq;
	char freqStr[13];
	char* note;
	 
	for (k=0;k <= cursorX;k++)
		nIndex += SIGNPOINTER->calcIndex(k,scale);	
		
	flags = 256; // add the cursor crosshairs
		 	
	freq = (uint)(nIndex * SIGNPOINTER->nIndexFreq);
				 	
	if (SIGNPOINTER->tunerMode == 1)
		{
		note = SIGNPOINTER->freqToNote(freq);
		snprintf(freqStr, 13, "%s",note);
		} else { 
						snprintf(freqStr, 13, "%dHz@%2.0fdB", freq, (10.0 * pow(10.0,((14 + 47 - cursorY) / 48.0 ))) / 0.757); 
						}
 
		
	if (cursorX > 64)
		flags = flags | 8; // left align text 

	SIGNPOINTER->strToSign((char*)freqStr,cursorX ,cursorY-8,flags); 
	
	
	}

