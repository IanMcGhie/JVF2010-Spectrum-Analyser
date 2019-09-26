#define DB_MAX 99

class dBMeter 
{
private:
	
	int refresh;

	
public:
	dBMeter(sign &_ledsign);
	~dBMeter();
   
   	dBMeter& operator=(const dBMeter&);

  void updateMeter(int _dB,uint aw);
   	
  sign &ledsign;
  int weighting;
	
	int REFRESH;
	int dB;
};

//////////////////////////////////////
//
// dBMeter constructor
//
/////////////////////////////////////
dBMeter::dBMeter(sign &_ledsign) : ledsign(_ledsign) 
	{
	REFRESH = 4;
	refresh = 0;
	}

//////////////////////////////////////
//
// dBMeter destructor
//
/////////////////////////////////////
dBMeter::~dBMeter()
	{
		
	}

//////////////////////////////////////////
//
//	db meter
//
///////////////////////////////////////
void dBMeter::updateMeter(int _dB,uint weighting)
	{
	uint flags,x,k,warn,pattern;
	char dBString[7];	
		
	if (_dB < -2147483646)
		_dB = 0; 	
	
	refresh++;
	
	if (refresh > REFRESH)
		{
		refresh = 0;
		
	 	dB = _dB;
	 	}
	  
		
	flags = 0; 
	x 		= 105; // text x coord
   
	warn  = 1;

	if (dB >= DB_MAX)
		flags = flags | 16; // turn on bright bit

 	switch (weighting)
 		{
 		case 0:
			snprintf(dBString, 7, "%ddB", dB);  			
 		break;	
 		
 		case 1:
			snprintf(dBString, 7, "%ddBA", dB); 
			x -= 5;
 		break;
 		
 		case 2:
			snprintf(dBString, 7, "%ddBC", dB); 
			x -= 5;
 		break;

 		case 3:
			snprintf(dBString, 7, "%ddBI", dB); 
			x -= 5;
 		break;
 		}
	
	if (dB < 0)
		x -= 5;
	
 	if (dB > 99) // right align text if its 100(3 digits)
 		x -= 5;
 
 	ledsign.strToSign(dBString,x ,16,flags);					

 	// the meter ... each led is 6 dB
 	if (abs(dB) > 0)
 	for (k=0;k < abs(dB)/6;k++)
 		{
 		pattern = 0;	
 			
 		if (k > 5)
 			warn = 2;
 	
 		if (k > 10)
 			warn = 3;
 			
 		if (k % 3)
 			pattern = 3;
 			
 		ledsign.setxy(123 - k,22 ,warn & pattern);
 		ledsign.setxy(123 - k,23 ,warn & pattern);
 		}
  }

