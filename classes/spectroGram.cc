
class spectroGram
	{

	private:
	
	uint sg[SIGN_WIDTH][SIGN_HEIGHT];
	uint delay;
	uint DELAY;
	
	public:
	spectroGram();
	~spectroGram();
	sign sgLedsign;
	void draw();
	void add(uint *peak,uint *bigPeakThreshold);
//	void add(uint *peak,uint* oldPeak);
	};
	
///////////////////////////////////////////////
//
//	constructor!!!!!!!!!!!!!!
//
/////////////////////////////////////////////
spectroGram::spectroGram() : sgLedsign((char*)"192.168.50.12")
	{
	int x,y;
	
	DELAY=0;
	delay=0;

	sgLedsign.clear(0);
	sgLedsign.refresh();

	for (y=0;y<SIGN_HEIGHT;y++)
		for (x=0;x<SIGN_WIDTH;x++)
			sg[x][y] = 0;
	}
	
//////////////////////////////////////////////
//
//	destructor
//
//////////////////////////////////////////////
spectroGram::~spectroGram()
	{
	
	}

//////////////////////////////////////////////
//
//	draw spectroGram
//
//////////////////////////////////////////////
void spectroGram::draw()
	{
	int x,y;
			
	for (y=0;y<SIGN_HEIGHT;y++)		
		for (x=0;x<SIGN_WIDTH;x++)
			sgLedsign.setxy(x,y,sg[x][y]); 

	sgLedsign.refresh();
	sgLedsign.clear();
	}

/////////////////////////////////////////
//
// add spectroGram info to array
//
/////////////////////////////////////////
void spectroGram::add(uint *peak,uint *bigPeakThreshold)
	{
	 return;

	 
	int i,j,x,y;
	
	delay++;
	
	if (delay > DELAY)
		{
		delay =0;
	
		// shift array up 1 ...via y
		for (y=1;y<SIGN_HEIGHT;y++)		 
			for (x=0;x<SIGN_WIDTH;x++)
				sg[x][y-1] = sg[x][y];	

		for (i=0;i<128;i++)
			{
			sg[i][47] = 0;	

			if (peak[i] > bigPeakThreshold[i])
				sg[i][47] = 1;
					
			if (peak[i] > bigPeakThreshold[i] * 1.85)
				sg[i][47] = 3;

 			
				
				
			}
		}
	}

