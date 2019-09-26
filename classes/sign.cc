#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <cmath> 			 // pow,sqrt,log10

#define SAMPLERATE 44100

#define SIGN_WIDTH 128
#define SIGN_WIDTH_BYTES 32
#define SIGN_HEIGHT 48
#define PORTNUM 5001
#define BUFFERFRAMES 2048

//#define DB_MAX 99
 
class sign
	{
	const char* dev;   		// name of sequencer device
	
	int seqfd;    // sequencer file descriptor
	int status;   // for error checking
	int device;   // for selecting which MIDI output to use
	int	numMidi;  // number of available MIDI outputs
	int lastNote; 
	int sd;				// socket descriptor
	
	struct message
		{  
   char data[SIGN_HEIGHT][SIGN_WIDTH_BYTES];   

		};				

	message msg; // buffer to send to sign
		
	public:
		sign(char *host);
	
		int getxy(unsigned int x, unsigned int y);
		void clear(int colour);
		void refresh();
		void setxy( uint x,  uint y,  uint gamma);
		char* freqToNote(double freq);
		int freqToX(double freq);
		void strToSign(char string[12],uint X,uint Y,uint flags);

//		sign& operator=(const sign&);

		void charOut(int X,int Y,int s,uint bright);	

		uint calcIndex(uint x,int xScale);
		int clamp (int x, int min, int max);
		float fclamp (float x, float min, float max);
	  int octaveShift;
 		
 	  int zoom;
 	  int zoomBand;
	  int errorCount;
 
		uint start[5];
	 	float calcFreq(uint i,int xScale);

 		uint freqs[128];
		uint tunerMode; 
		uint crossHairDelay;
		uint crossHair;
		uint nyquist;
		int weightingView;
		int weighting;
		int inited;
		float nIndexFreq;
	
	private:

	};

////////////////////////////////////////////////
//
//  sign contructor
//
/////////////////////////////////////////////////
sign::sign(char *host)
{
  int rc;
  struct sockaddr_in localAddr, servAddr;
  struct hostent *h;
  uint i,f=0;
  errorCount = 0;
  inited = 0;
   
  h = gethostbyname(host);
  
  if(h == NULL) 
  	{
    printf("signclient: unknown host '%s'\n",host);
    exit(1);
  	}

  servAddr.sin_family = h->h_addrtype;
  memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  servAddr.sin_port = htons(PORTNUM);

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  
  if(sd < 0) 
  	{
    perror("signclient: cannot open socket ");
    exit(1);
  	}

  /* bind any port number */
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(0);
  
  rc = bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr));
  
  if(rc < 0) 
  	{
    printf("signclient: cannot bind port TCP %u\n",PORTNUM);
    perror("error ");
    exit(1);
  	}
				
  /* connect to server */
  rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  
  if(rc < 0) 
  	{
    perror("\n\nsign.cc:118 cannot connect....\n\n");
    exit(1);
  	}
 
	start[0] = 0;  // x coords
  	start[1] = 12; // of peak bands
  	start[2] = 61; // ...
  	start[3] = 100; // ..
  	start[4] = 127;
 
	zoom 	  = 0;
	zoomBand  = 0;
	tunerMode = 0;
	crossHair = 24;
	crossHairDelay = 0;
 
 	octaveShift = 0;
	
  	nyquist = SAMPLERATE / 2;

	nIndexFreq=(float)SAMPLERATE / (float)(BUFFERFRAMES);


	inited = 1;
 /*
	for (i=0;i<128;i++)
		{
		f+=(uint)(calcIndex(i) * nIndexFreq);
 		freqs[i] =f;
 		}
 */
 	
 
/*
         seqfd = -1;               // sequencer file descriptor

 dev = "/dev/sequencer";   // name of sequencer device
         status;                   // for error checking
         device = 17;               // for selecting which MIDI output to use
    numMidi = 0;                   // number of available MIDI outputs
   
  seqfd = open(dev, O_WRONLY, 0);
  
     if (seqfd < 0) {
      printf("Error: cannot open %s\n", dev);
//      exit(1);
   
 
   }
*/
//   if (argc >= 2) {
 //     device = argv[1][0] - '0';
  // }
/*
   status = ioctl(seqfd, SNDCTL_SEQ_NRMIDIS, &numMidi);
   if (status != 0) {
      printf("Error: cannot access MIDI devices on soundcard\n");
//      exit(1);
   }
//   printf("\nThere are: %d MIDI output devices available\n", numMidi);
   if (device >= numMidi) {
      printf("You specified an invalid MIDI device\n");
      //exit(1);
     }

     */

  printf("clearing ledsign @ %s with zeros\n",host);
	clear(0);
	refresh();
}

///////////////////////////////
//
// int clamp (int x, int min, int max)
//
//////////////////////////////
int sign::clamp (int x, int min, int max)
	{
	  if (x < min) 
	  	return min;
	    	
	  if (x > max) 
	  	return max;
	  
  	return x;
	}

///////////////////////////////
//
// int clamp (int x, int min, int max)
//
//////////////////////////////
float sign::fclamp (float x, float min, float max)
	{
	  if (x < min) 
	  	return min;
	    	
	  if (x > max) 
	  	return max;
	  
	  return x;
	}

/////////////////////////////////
//
//  refresh
//
//////////////////////////////////
void sign::refresh()
	{	
 	int bytesSent;
  
 	bytesSent = send(sd, (void *)&msg.data, sizeof(msg.data), 0);

 	if (bytesSent != sizeof(msg))
 		{
 		printf("oh oh..array overflow!\n.....we sent %d bytes to the sign and only got %d back iam outa here(sign.cc around line 198ish)\n",sizeof(msg),bytesSent);
	 	}
 	 
	}

////////////////////////////////
//
// clear()
//
/////////////////////////////////
void sign::clear(int colour)
	{
  memset(msg.data,colour | 
  							 (colour << 2) | 
  							 (colour << 4) | 
  							 (colour << 6),
  							 sizeof(msg.data));
  
	}

///////////////////////////////////////////////
//
// Set a pixel in the display buffer to a given gamma value
// 0=off, 1=dim, 2=medium, 3=bright
// coordinates are cartisian
// with 0,0 starting at the top left
//
///////////////////////////////////////////////
void sign::setxy(uint x, uint y, uint gamma)
	{
	char byte;	
  uint bitPair, pattern;
  uint blackPattern;
  
  // this used to be    
  // if ((y > 47) || (x > 127))
  // one day... i changed it to 
  // if ((y > SIGN_HEIGHT) || (x > SIGN_WIDTH))
  // creating a off by one error that took days to find
  // imm jan 21/16
  //
  
  if ((y > SIGN_HEIGHT - 1) || (x > SIGN_WIDTH - 1))
		return;
		 
  if ((y < 0) || (x < 0))
		return;

	if (gamma > 3)
		gamma = 3;

  // bit pair offset 0,2,4 or 6
    bitPair = 6 - ((x % 4) * 2);  
	//  unsigned int pattern = gamma << bitpair;
  
  // on? ...shift 000000xx into byte 
  // from the right 'OR' & or it (xx - pixel colour)
  pattern = gamma << bitPair;
 
 	// off? 
  blackPattern =  3 << bitPair;
  blackPattern = ~blackPattern; // invert pattern
  // shift 11111100 in from the right and AND it
 /*
  if ((blackPattern < 0) || (blackPattern > 255)) 
   {
   	endwin();
   printf("black pattern is %d\n",blackPattern);	
   exit(1);
  }
  	*/
  // which byte
  byte = x / 4;

	if ((byte > 31) || (byte < 0))
		{
		printf("byte is %d\n",byte);
		exit(1);
		}

	if (gamma > 0)
		msg.data[y][byte]=msg.data[y][byte] | pattern;
			else
				msg.data[y][byte]=msg.data[y][byte] & blackPattern;
				
			
	}
  
////////////////////////////////
//
//  sign::getxy(unsigned int x, unsigned int y)
//
//////////////////////////////////
int sign::getxy(unsigned int x, unsigned int y)
 	{
 		printf("\n\n\fix 326 ish....sign.ccn\n");
 	exit(1);
	
  if ((y > 47) || (x > 127))
  	return 4;

  // which byte
  unsigned int byte=x / 4;

  // bit pair offset
  unsigned int bitpair= 6 - ((x % 4) * 2); // the pixels are stored in reverse

  // create  mask to mask out bits we're going to replace
  // (00's at insertion point and 1's everywhere else)
  unsigned int mask=(3 << bitpair);

 	return ((msg.data[y][byte] & mask) >> bitpair);
 	}



//////////////////////////////////////
//
// peaks calcFreq
//
/////////////////////////////////////
 
float sign::calcFreq(uint x,int xScale)
	{
	float freq;
	int i,nIndex;
 
	// calc freq 
	nIndex =3;  

	for (i = 0;i < x ;i++)  
		nIndex += calcIndex(i,xScale);	
	
	freq = (float)nIndex * (44100.0 / (float)BUFFERFRAMES);
	 	
	return freq;
	}

////////////////////////////
//
//	int sign::freqToX(double freq)
//
/////////////////////////////
int sign::freqToX(double freq)
	{
	int x;
	
	int nIndex = 3;
 
	for (x = 0;x < SIGN_WIDTH;x++)	
		{
		nIndex += calcIndex(x,0);// * (double)(SAMPLERATE / BUFFERFRAMES);
		//		endwin();		
 	//	printf("x = %d nIndex = %d freq = %2.2f calced freq = %2.2f\n",x,nIndex,freq,(double)nIndex * (44100.0 / 2048.0));
		
		if ((double)(nIndex * (44100.0 / 2048.0)) >= freq)
			break;
 		}
		
	return x;	
	}
	
///////////////////////////////////////////
//
//		void sign::freqToMidi(char *)
//
//////////////////////////////////////////
/*
void sign::freqToMidi(int freq)
	{
 	int octave,midiNote;
 	char* note;

 	note=freqToNote(freq);
 	note[0]-=64;
	octave=(note[2] - 49) + octaveShift; // convert from ascii
	
	midiNote = (int)note[0] +(octave * 12);

	if (note[1] == 35) // #
		midiNote++;

	if (note[1] == 98) // b
		midiNote--;
		
	lastNote = midiNote;
	 //	printf("played a %d\n",midiNote);
	}
*/
/* 
/////////////////////////////////
//
// playnote
//
///////////////////////////////
void sign::playnote(int note) 
	{

  unsigned char outpacket[12];

  outpacket[0] = SEQ_MIDIPUTC;
  outpacket[1] = 0x90;              // note-on command
  outpacket[2] = device;
  outpacket[3] = 0;

  outpacket[4] = SEQ_MIDIPUTC;
  // outpacket[5] will be the keynumber
  outpacket[6] = device;
  outpacket[7] = 0;

  outpacket[8] = SEQ_MIDIPUTC;
  outpacket[9] = 127;              // attack velocity
  outpacket[10] = device;
  outpacket[11] = 0;
}
*/


//////////////////////////////////////
//
// analyser calcIndex
//
/////////////////////////////////////
uint sign::calcIndex(uint x,int _xScale)
	{
	// have to init array from here because
	// calcIndex() is called before constructor
	// is called
	start[0] = 0;  // x coords
  start[1] = 12; // of peak bands
  start[2] = 61; // ...
  start[3] = 100; // ..
  start[4] = 127;
	
	if ((zoom == 1) && (x == 0))
		{
		switch (zoomBand)
			{
			case 0:
				return 0; // 22 hz steps	
			break;
			
			case 1:
				return start[1] + 110;; // 2649Hz
			break;
			
			case 2:
				return start[2] + 187 ; // 5462Hz
			break;
			
			case 3: 
				return start[3] + 260;
			break;
			} 
		}  
		
		if ((zoom == 1) && (x > 0))
		{ 
		switch (zoomBand)
			{
			case 0:
				return 1; // 22 hz steps	
			break;
			
			case 1:
				return 1; // 22 hz steps
			break;
			
			case 2:
				return 1; // 22 Hz steps
			break;
			
			case 3: 
				return 5; // 110 hz steps
			break;
			} 		
		}
		
		if (zoom != 1)
			 {
			 // $2 computer microophone scale (42hz - 15khz) - log scale ...sorta
			 if (_xScale == 0)
					{
					if (x >= start[3])
						return 27; // 792 Hz steps (36 * 22hz steps)
							else if ((x >= start[2]) && (x < start[3]))
								return 5; // 44 Hz steps
									else if ((x >= start[1]) && (x < start[2]))
										return 1; // 22 Hz steps
											else
												return 1; // 22 Hz steps
					}

			 if (_xScale == 1)
		 	 	return 1; 

			 if (_xScale == 2)
				return 8;

		 	} 
	}


/////////////////////////////////////////
//
// char* freqTonote(freq)
//
/////////////////////////////////////////
char* sign::freqToNote(double freq)
	{
	static char freqString[5]; // this is static becasue were returning it....it has to hang around
 
	int i,n,octave; 

	char newnotes[12] = {'A','A','B','C','C','D','D','E','F','F','G','G'};
	char sharps[12] 	= {' ','#',' ',' ','#',' ','#',' ',' ','#',' ','#'};
  
 	
  //freq = 27.500;
 	//freq = 29.135;
 	//freq = 30.868;

	// thx to suits@mtu.edu (Byran Suits) for this equation
	//n = 39.86314 log(Fn) - 105.37632
	//n = (int)(((double)39.86314 * log10(freq )) -  (double)105.37632) + 47;	 // steps from A4
	n = (int) (((log10((double)freq) - log10(27.5000)) / log10(1.059463094))); // steps from A0

	 
		octave=((n + 9) / 12); // C1 and higher
 
 	//n = (int)(((log10((double)freq / (double)100.0) - log10(27.5000)) / log10(1.059463094)));

 	n=(n % 12);
	
//	if (n < 0)
//			n+=12;

	
	//if (n > 0)
		 
//	endwin();
	
	//printf("n is %d octave is %d\n",n ,octave);
	 
	freqString[0] = newnotes[n];
	freqString[1] = sharps[n];
	freqString[2] = char(octave + 48);
	freqString[3] = '\0';

	return freqString;
	}


//////////////////////////////////////////
//
//	void sign::strToSign(char string[12],uint X,uint Y,uint flags)
//
///////////////////////////////////////
void sign::strToSign(char string[12],uint X,uint Y,uint flags)
	{
 	uint l;
 	uint len  	= strlen(string);	
	uint bright = 1;
	 
	if (flags & 16)
		bright = 3;
	
	if (flags & 32)
		X+=5; // left align for big peaks
	
	crossHairDelay++;
			
	// add the cursor cross hairs
	if ((flags & 256))
		{
		if (crossHairDelay > 30)
			{
			crossHairDelay = 0;
			crossHair++;
			
			if (crossHair > 27)
				crossHair=24;	
			}

		charOut(X - 2, Y + 6, crossHair, bright); // crosshairs

 		if (flags & 8) // left align for freq/db string near cursor
			X = X - (5 * len);
		}

	for (l = 0;l < len;l++)
		{	
		switch (string[l])
			{
			case 32: // space (but dont move X...dont print the space)
				charOut(X, Y, 30, bright);
			//	X+=5;
			break;

			case 65: // A
				charOut(X, Y, 10, bright);
				X+=5;	
			break;

			case 66: // B
				charOut(X, Y, 11, bright);
				X+=5;	
			break;

			case 67: // C
				charOut(X, Y, 12, bright);
				X+=5;	
			break;

			case 68: // D
				charOut(X, Y, 13, bright);
				X+=5;	
			break;

			case 69: // E
				charOut(X, Y, 14, bright);
				X+=5;	
			break;

			case 70: // F
				charOut(X, Y, 15, bright);
				X+=5;	
			break;

			case 71: // G
				charOut(X, Y, 16, bright);
				X+=5;	
			break;

 			case 72: // H 
				charOut(X, Y, 17, bright); 				
				X+=5;	
 			break;

 			case 73: // I 
				charOut(X, Y, 18, bright); 				 				 				
				X+=5;	
 			break;

			case 98: // b
				charOut(X, Y, 20, bright);
				X+=5;	
			break;

			case 100: // d
				charOut(X, Y, 21, bright);
				X+=5;	
			break;
			
 			case 122: // z 
				charOut(X, Y, 22, bright); 				 				
				X+=5;	
	 		break;
 		
			case 64:  // @ symbol
				charOut(X, Y, 28, bright); 				 								
				X+=5;			
			break;	
			
			case 35: // # symbol
				charOut(X,Y,29,bright);
				X+=5;
			break;

			case 45: // minus symbol
				charOut(X,Y,31,bright);
				X+=5;
			break;
				 
			default: // 0 - 9
 				charOut(X, Y, string[l] - 48, bright);
				X+=5;	
			break;
			}
		}
	}



///////////////////////////////////
//
//void charOut(int X,int Y,int s,uint bright)
//
/////////////////////////////////////
void sign::charOut(int X,int Y,int s,uint bright)
	{
	uint ledChrs[][30] =
 		{ 
 		// 0
 		{0,1,1,0,0,
 		 1,0,0,1,0,
 		 1,0,0,1,0,
 		 1,0,0,1,0,
 		 0,1,1,0,0},
 	
 	// 1
   	{0,0,1,0,0,
   	 0,1,1,0,0,
   	 0,0,1,0,0,
   	 0,0,1,0,0,
   	 0,1,1,1,0}, 

	// 2   	 
   	{0,1,1,0,0,
 		 1,0,0,1,0,
  	 0,0,1,0,0,
 		 0,1,0,0,0,
 	 	 1,1,1,1,0},
 	 	 
	// 3
		{0,1,1,0,0,
 		 1,0,0,1,0,
 		 0,0,1,1,0,
 		 1,0,0,1,0,
 		 0,1,1,0,0},

	// 4

 		{1,0,1,0,0,
 		 1,0,1,0,0,
 		 1,1,1,1,0,
 		 0,0,1,0,0,
 		 0,0,1,0,0},

	// 5
 		{1,1,1,0,0,
 		 1,0,0,0,0,
 		 1,1,1,0,0,
 		 0,0,1,0,0,
 		 1,1,1,0,0},

	// 6
 	 	{0,1,1,0,0,
 		 1,0,0,0,0,
 		 1,1,1,0,0,
 		 1,0,0,1,0,
		 0,1,1,0,0},

	// 7
 	  {1,1,1,1,0,
 		 0,0,0,1,0,
 		 0,0,1,0,0,
 		 0,1,0,0,0,
 		 1,0,0,0,0},

	// 8
 		{0,1,1,0,0,
 		 1,0,0,1,0,
 		 0,1,1,0,0,
 		 1,0,0,1,0,
 		 0,1,1,0,0},

	// 9
		{0,1,1,0,0,
 		 1,0,0,1,0,
 		 0,1,1,1,0,
 		 0,0,0,1,0,
 		 0,1,1,0,0},

	// A
   	{0,1,1,0,0,
 		 1,0,0,1,0,
 		 1,1,1,1,0,
 		 1,0,0,1,0,
 		 1,0,0,1,0},

	// B
   	{1,1,1,0,0,
 		 1,0,0,1,0,
 		 1,1,1,0,0,
 		 1,0,0,1,0,
 		 1,1,1,0,0},
	
	// C
   	{0,1,1,0,0,
 		 1,0,0,1,0,
 		 1,0,0,0,0,
 		 1,0,0,1,0,
 		 0,1,1,0,0},
	
	// D
   	{1,1,1,0,0,
 		 1,0,0,1,0,
 		 1,0,0,1,0,
 		 1,0,0,1,0,
 		 1,1,1,0,0},
	
	// E
   	{1,1,1,1,0,
 		 1,0,0,0,0,
 		 1,1,1,0,0,
 		 1,0,0,0,0,
 		 1,1,1,1,0},
	
	// F
   	{1,1,1,1,0,
 		 1,0,0,0,0,
 		 1,1,1,0,0,
 		 1,0,0,0,0,
 		 1,0,0,0,0},

	// G
   	{0,1,1,0,0,
 		 1,0,0,0,0,
 		 1,0,1,1,0,
 		 1,0,0,1,0,
 		 0,1,1,0,0},

	// H
   	{1,0,0,1,0,
 		 1,0,0,1,0,
 		 1,1,1,1,0,
 		 1,0,0,1,0,
 		 1,0,0,1,0},

	// I
   	{0,1,1,1,0,
 		 0,0,1,0,0,
 		 0,0,1,0,0,
 		 0,0,1,0,0,
 		 0,1,1,1,0},

	// K
 		{1,0,0,1,0,
 		 1,0,1,0,0,
 		 1,1,1,0,0,
 		 1,0,1,0,0,
 		 1,0,0,1,0},

	// b
   	{1,0,0,0,0,
 		 1,0,0,0,0,
 		 1,1,1,0,0,
 		 1,0,0,1,0,
 		 1,1,1,0,0},

	// d
   	{0,0,0,1,0,
 		 0,0,0,1,0,
 		 0,1,1,1,0,
 		 1,0,0,1,0,
 		 0,1,1,1,0},

	// z
 		{0,0,0,0,0,
 		 1,1,1,1,0,
 		 0,0,1,0,0,
 		 0,1,0,0,0,
 		 1,1,1,1,0},

		// .
	  {0,0,0,0,0,
	 	 0,0,0,0,0,
	 	 0,0,0,0,0,
	 	 0,1,1,0,0,
	 	 0,1,1,0,0},
		
		// cursor cross hairs
	 	{0,0,3,0,0,
	 	 0,0,3,0,0,
	 	 1,1,0,1,1,
	 	 0,0,1,0,0,
	 	 0,0,1,0,0},
		// cursor cross hairs
	 	{0,0,1,0,0,
	 	 0,0,1,0,0,
	 	 1,1,0,3,3,
	 	 0,0,1,0,0,
	 	 0,0,1,0,0},
		// cursor cross hairs
	 	{0,0,1,0,0,
	 	 0,0,1,0,0,
	 	 1,1,0,1,1,
	 	 0,0,3,0,0,
	 	 0,0,3,0,0},
		// cursor cross hairs
	 	{0,0,1,0,0,
	 	 0,0,1,0,0,
	 	 3,3,0,1,1,
	 	 0,0,1,0,0,
	 	 0,0,1,0,0},
		
		// @ symbol
	 	{0,1,1,0,0,
	 	 1,0,0,1,0,
	 	 1,0,1,1,0,
	 	 1,0,0,0,0,
	 	 0,1,1,1,0} ,

		// # symbol
	 	{0,1,0,1,0,
	 	 0,1,0,1,0,
	 	 1,1,1,1,1,
	 	 0,1,0,1,0,
	 	 0,1,0,1,0} ,

		// space
	 	 {0,0,0,0,0,
	 		0,0,0,0,0,
	 		0,0,0,0,0,
	 		0,0,0,0,0,
	 	  0,0,0,0,0} ,
	 	  
	 	  // minus sign
	 	  {0,0,0,0,0,
	 	   0,0,0,0,0,
	 	   0,1,1,1,0,
	 	   0,0,0,0,0,
	 	 	 0,0,0,0,0 }
	 	};

		int x,y;
			
		for (x=0;x<5;x++)
			for (y=0;y<5;y++)
				setxy(X + x,Y + y,ledChrs[s][y * 5 + x] * bright);
	}
