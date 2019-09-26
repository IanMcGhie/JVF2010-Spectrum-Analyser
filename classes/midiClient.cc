#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <alloc.h> // antiquated header
#include <unistd.h> /* close */
#include <ctime>
#include "midi.h"

#define portNumber 6502
#define MAXNOTES 1 



class midiClient
	{
  int sd;
  
	public:
		midiClient(char *hostName);

		void queueNote(float _noteFreq,int _velocity);
		void noteOffCheck();
		void playNote();

		void checkForNotesTap();
		
		float checkForNotes;
		float sustain;

		int midiMode;
		int lastNote;
		int playNoteReset;
		int playSameNote;
		int maxNote;
		int minNote;
//		int notesPlaying[MAXNOTES]; 
		struct midiData MIDIDATA;
 	 

		int octaveShift;
		int inited;
		
		void setBPM();
		clock_t tapClock;
		int resetBPMTimer;			
		int tapOn;
		int flashOn;		


	private:
		int freqToMIDINote(float freq);

		float noteDuration[MAXNOTES];
		int noteQueue[MAXNOTES];
		
		int errorCount;
		int sameNote;
		int notesInQueue;
		clock_t lastCall;
		

};

/////////////////////////////////////
//
//	ctor
//
//////////////////////////////////////
midiClient::midiClient(char *hostName)
	{
  int rc,i;
  
  	
  struct sockaddr_in localAddress, serverAddress;
  struct hostent *host;



	MIDIDATA.MIDICommand = 0;
	MIDIDATA.MIDINote = 0.0;
	MIDIDATA.velocity = 0;

	midiMode 		 = 1;
	
	checkForNotes = 500;// milliseconds
	octaveShift = 0;
	lastNote = 1;
	sameNote = 0;
	sustain = 1000;
	playNoteReset = 50;
	
	
				

	notesInQueue = 0;
	lastCall = clock();

	tapClock = clock();
	resetBPMTimer = 0;
	tapOn = -1;
 	flashOn	  = -1;


	// empty queue
	for (i = 0;i < MAXNOTES;i++)
		{
//		notesPlaying[i] = -1;
		noteDuration[i] = 0; // sustain
		noteQueue[i] = -1; // which MIDI note
		}

return; // i just added this may 5 2018
	
  host = gethostbyname(hostName);
  
  if	(host == NULL)
  	{
    printf("midiClient: unknown host '%s'\n",hostName);
    exit(1);
  	}

  serverAddress.sin_family = host->h_addrtype;
  memcpy((char *) &serverAddress.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
  serverAddress.sin_port = htons(portNumber);

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
 
  if(sd < 0) 
  	{
    perror("midiClient: cannot open socket ");
    exit(1);
  	}

  /* bind any port number */
  localAddress.sin_family = AF_INET;
  localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddress.sin_port = htons(0);
  
  rc = bind(sd, (struct sockaddr *) &localAddress, sizeof(localAddress));
  
  if (rc < 0) 
  	{
    printf("midiClient: cannot bind port TCP %u\n",portNumber);
    perror("error ");
    exit(1);
  	}
				
  /* connect to server */
  rc = connect(sd, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
  
  if (rc < 0) 
  	{
    perror("midiClient: cannot connect ");
    exit(1);
  	}
	
	inited = 1;
	}
 
///////////////////////////////////////////////
//
//		void midiClient::queueNote(float noteFreq,int velocity)
//
///////////////////////////////////////////////
void midiClient::queueNote(float _noteFreq,int _velocity)
	{
	int octave;
	float elapsedSeconds,milliSeconds;
 	int i,queued = -1;
  	
	if (inited != 1)	
		return;
		
	if (midiMode != 1)	
		return;

 	MIDIDATA.MIDINote = freqToMIDINote(_noteFreq);

 	if (MIDIDATA.MIDINote < minNote)
		return;

	if (MIDIDATA.MIDINote > maxNote)
		return;


	if (lastNote == MIDIDATA.MIDINote)
		{
		sameNote++; 

		if (sameNote > playNoteReset)
		  sameNote = 0;

		if (sameNote > playSameNote)
			return;		

		}
//endwin();
//printf("lastnote = %d sameNote = %d playNoteReset = %d MIDIDATA.MIDINote = %d\n",lastNote,sameNote,playNoteReset,MIDIDATA.MIDINote);

	// first check for notes to shut off
 	noteOffCheck();
		
	elapsedSeconds = ((float)clock() - lastCall) / CLOCKS_PER_SEC * 1000.0;
	
	if (elapsedSeconds > (checkForNotes) )
	 	{
		// find queue positioin to put incoming note 
		// or return.....discarding note
		for (i = 0;i < MAXNOTES;i++)
			{
			if ((noteQueue[i] == -1) && (queued == -1)) // found empty spot
				{
				noteDuration[i] = (float)clock() ;
		//		printf("noteduration[%d] is %2.10f\n",i,noteDuration[i] / CLOCKS_PER_SEC);
	
				noteQueue[i] = MIDIDATA.MIDINote;
			 	queued = i;
				
				 
		//		printf("  %d  \r",MIDIDATA.MIDINote);
				MIDIDATA.velocity = 70;//_velocity;
				MIDIDATA.MIDICommand = 0x90; // note on
				
				lastNote = MIDIDATA.MIDINote;
				playNote();
				}
			
			if (queued != -1)
				break;
			} // for (i = 0;i < MAXNOTES;i++)

		lastCall = clock();
		} // 	if (elapsedSeconds > (checkForNotes / 1000.0) )

	}	// void midiClient::queueNote(float noteFreq,int velocity)

/////////////////////////////////////
//
//	void midiClient::playNote(float noteFreq,int velocity)
//
////////////////////////////////////
void midiClient::playNote()
	{	
		  
	send(sd, (void *)&MIDIDATA, sizeof(MIDIDATA), 0);
	}
 
/////////////////////////////////////////
//
// int midiClient::freqToMIDINote(float freq)
//
/////////////////////////////////////////
int midiClient::freqToMIDINote(float freq)
	{
 	int i,n;
  
	// thx to suits@mtu.edu (Byran Suits) for this equation
	//n = 39.86314 log(Fn) - 105.37632
	n = (int)((39.86314 * log10(freq)) - 105.37632);
 
// 	octave=(n / 12) + 4; 
 
	return n + 69;
	}

/////////////////////////////////////
//
//			void midiClient::noteOffCheck()
//
///////////////////////////////////////
void midiClient::noteOffCheck()
	{
	int i;
 	float elapsedSeconds;
   
	  
	// check for notes in queue to shut off
	for (i = 0;i < MAXNOTES;i++)
		{
		elapsedSeconds = ((float)clock() - noteDuration[i]) / CLOCKS_PER_SEC * 1000.0;

//		if ((elapsedMilliseconds > sustain / CLOCKS_PER_SEC * 100) && (noteQueue[i] != -1))
		if ((elapsedSeconds > (sustain)) && (noteQueue[i] != -1))
			{
			MIDIDATA.MIDICommand = 0;	 // note off command
			MIDIDATA.MIDINote = noteQueue[i];
			MIDIDATA.velocity = 0;
		
		//endwin();	 		
	 	//	printf("shutting %d off ",MIDIDATA.MIDINote);
  	//printf("elapsed milliseconds %2.2f sustain = %2.2f\n ",elapsedSeconds,sustain / 1000.0);

	 		playNote();

			noteDuration[i] = 0;
			noteQueue[i] = -1; // clear item from queue
			 
			}
		}
	   
	}


//////////////////////////////
//
//void midiClient::checkForNotesTap()
// 
/////////////////////////////
void midiClient::checkForNotesTap()
	{
//	float elapsedSeconds;
	 	
	tapOn = -tapOn;
	
	if (tapOn == 1)
		{
		tapClock = clock();
		checkForNotes = 0;
		}

	}
	
