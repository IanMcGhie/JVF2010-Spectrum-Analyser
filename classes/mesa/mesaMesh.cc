#define MAXBANDS 16
#define MAXDEPTH 16

class mesaMesh 
	{
	public:
		mesaMesh();
		~mesaMesh();

		void draw(mesaNURBS *NURBSPtr,int _tunnelStep,float diameter);
		void addRow(uint *_peak,uint *_peakThreshold);
		void init();
		int inited;
		
		int mapUStride, mapVStride;
		int mapUOrder, mapVOrder;
		int mapU2,mapV2;
		int meshType[3];
		int mType;
		int meshU2,meshV2;
		float xScale,yScale,zScale;
		
		int depth;
		int bands;
		int refresh;
		int REFRESH;
		int peakFrequencies[SIGN_WIDTH];

		int mapGridUN,mapGridVN;
		int mapGridU1,mapGridV1;
		int mapGridU2,mapGridV2;

		int colourU2, colourUStride, colourUOrder;
		int colourV2, colourVStride, colourVOrder;
				
		uint *peakThreshold;
		uint sg[SIGN_WIDTH][MAXDEPTH];
		
		float cMapRedScale;
	
	private:
	
	};

////////////////////////////////
//
// ctor
//
//////////////////////////////
mesaMesh::mesaMesh()
	{
	init();
	}

////////////////////////////////
//
// dtor
//
//////////////////////////////
mesaMesh::~mesaMesh()
	{
	
	}

 
///////////////////////////////////
//
//			void mesaSpectrogram::addRow(uint *_peak,uint *_peakThreshold)
//
//////////////////////////////
void mesaMesh::addRow(uint *_peak,uint *_peakThreshold)
	{
	int amp,i,x,z,frequencyIndex;	
	
	REFRESH++;
	
//	if (REFRESH > refresh)
// 		return;
		
	REFRESH = 0;								
	
	peakThreshold = _peakThreshold;

	// shift array back 1 ...via z
//	for(z=depth - 1;z != 0;z--)	
//		for (x=0;x<SIGN_WIDTH;x++)
//			 sg[x][z] = sg[x][z - 1];	

	for(z=depth - 1;z > 0;z--)	
		for (x=0;x<SIGN_WIDTH;x++)
			 sg[x][z] = sg[x][z - 1];	

	// go thru peaks ...looking for biggest
	for (x = 0 ; x < SIGN_WIDTH- (SIGN_WIDTH / bands); x+= SIGN_WIDTH / bands)
		{	
		// find biggest peak in band range...we have 128 bands...but only
	 	// show 8...16...24 ect...
	 	amp = 0;
	 	
		for (i = 0; i < bands;i++)
			if (_peak[x + i] > amp)
				{
				amp = _peak[x + i];
				frequencyIndex = x + i;
				}
	
		// replace all peaks in band range with biggest peak found
		for (i = 0;i < bands ;i++ )
			{
			peakFrequencies[x + i] = frequencyIndex;
			sg[x + i][0] = amp;
			}
					
		} // for (x = SIGN_WIDTH - 1;x > 0;x -= SIGN_WIDTH / bands)
 
 inited = 1;
	}
	
///////////////////////////////////////////////////
//
//			void mesaMesh::draw(mesaNURBS *NURBSPtr,int _tunnelStep,float diameter)
//
////////////////////////////////////////////////// 
void mesaMesh::draw(mesaNURBS *NURBSPtr,int _tunnelStep,float diameter)
	{
	// make sure addRow has been called (populating the sg and threshold arrays)	
	if (!inited)
		return;
			
	int i,j,u,v;
	int amp,threshold,error;
	int toggle = 0;
	int toggle2 = 0;
	
	GLfloat ctrlpoints[MAXBANDS][MAXDEPTH][3];
	GLfloat colourMap[MAXBANDS][MAXDEPTH][4];
 
	for (u = 0; u < bands; u++)
		for (v = 0; v < depth;v++)
			{
			if ((_tunnelStep * v) > NURBSPtr->vertices)
				_tunnelStep = 0;
				
			amp = sg[u * (SIGN_WIDTH / bands)][v] * yScale;	
 			threshold = peakThreshold[u * (SIGN_WIDTH / bands)];
  
	//		ctrlpoints[v][u][0] = (u * (SIGN_WIDTH / bands))  ;

ctrlpoints[v][u][2] = NURBSPtr->NURBSz[_tunnelStep * v];
//ctrlpoints[v][u][2] += (v * zScale) ;

ctrlpoints[v][u][0] = NURBSPtr->NURBSx[_tunnelStep * v];
ctrlpoints[v][u][0] *= cos(2.0 * M_PI * ((u + 1.0) / bands)) ;
//			ctrlpoints[u][v][2] = -v ;
 	
 			if ((amp > threshold) || (toggle == 1) || (toggle2 == 1))
	 			{
				ctrlpoints[v][u][1] = NURBSPtr->NURBSy[_tunnelStep * v];
				ctrlpoints[v][u][1] *= sin(2.0 * M_PI * ((u + 1.0) / bands));
	 			ctrlpoints[v][u][1] += (amp - threshold);
	 			}
	 				else
				//		ctrlpoints[v][u][1] = diameter * sin(2.0 * M_PI * ((u + 1.0) / bands));
	 					ctrlpoints[v][u][1] =	0;




// DONT SCALE THE VERTICES!
// IT ALSO SCALES THE COLOR MAP!
//...BASTARDS
//			ctrlpoints[v][u][0] *= xScale; 
//			ctrlpoints[v][u][2] *= zScale;

		 	colourMap[v][u][0] = 0.00;
		 	colourMap[v][u][1] = 0.00;
		 	colourMap[v][u][2] = 0.00;
		 	colourMap[v][u][3] = 0.0;
 
		 	if (((amp > threshold) || (toggle == 1)))
				{
				colourMap[v][u][0] = 0.005;
	 	 		colourMap[v][u][3] = 1.0;

			 	toggle = !toggle;
				}
					
		 	if ((((amp > (float)threshold * 1.1) ) || (toggle2 == 1)))
		 		{ 
		 		colourMap[v][u][0] = cMapRedScale; 
 		 		colourMap[v][u][3] = 1.0;
				
 			toggle = 0;
		// 		colourMap[u][v+1][0] = 1.0; 
		 		toggle2 = !toggle2;
		 		}
		
		 	}

	// control points
  glMap2f(GL_MAP2_VERTEX_3,
  			 0, mapU2, mapUStride, mapUOrder,
          0, mapV2, mapVStride, mapVOrder,
          &ctrlpoints[0][0][0]);
  
  glEnable(GL_MAP2_VERTEX_3);

	error = glGetError();
	
	if (error)
		{	

		printf("\nctrlpoint map error\n");
	//	exit(1);
		}
      
	// colour map
  glMap2f(GL_MAP2_COLOR_4, 
		       0, colourU2 , colourUStride, colourUOrder, 
		       0, colourV2 , colourVStride, colourVOrder,
			     &colourMap[0][0][0]);

   glEnable(GL_MAP2_COLOR_4);
     
	error = glGetError();
	
	if (error)
		{
		printf("\ncolour map error\n");
		exit(1);
		}
 

	glPointSize(1.0); 	

	glPushMatrix();
  glColor3f(005.0, 0.0, 0.0);

//    glRotatef(90.0,0.0, 0.0, 1.0);
	
	switch (meshType[mType])
		{
		case 6502: 	
			glMapGrid2f(mapGridUN, 0.0, mapGridU2  , mapGridVN, 0.0, mapGridV2  );
		
			error = glGetError();
			
			if (error)
				{
				printf("\nGL ERROR: %s\n",error);
				exit(1);
				}

			glEvalMesh2(GL_LINE,0.0,meshU2  ,0.0,meshV2  );
		break;

		case GL_LINE_STRIP:
	
			for (i = 0; i <=mapGridU1; i++)
				{      
			  glBegin(GL_LINE_STRIP); 
			
			  for (j = 0; j <=mapGridV1; j++) 
					glEvalCoord2f(mapGridU1 + i * (mapGridU2 - mapGridU1) / (mapGridU1 + 1),
											  mapGridV1 + j * (mapGridV2 - mapGridV1) / (mapGridV1 + 1)); 

			  glEnd(); 
			} 
	
/*
			for (j = 0; j <= bands; j++) 
				{ 
			  glBegin(GL_LINE_STRIP); 
			
 			  for (i = 0; i <= depth; i++)  
				  glEvalCoord2f(i,j); 

			  glEnd(); 
				}
*/
			break;  
  
  
  	case GL_POINTS:
 			glBegin(GL_POINTS); 

			for (i = 0; i < mapGridU1; i++) 
		  	for (j = 0; j < depth; j++) 
					glEvalCoord2f(mapGridU1 + i * (mapGridU2 - mapGridU1) / (mapGridU1 + 1),
											  mapGridV1 + j * (mapGridV2 - mapGridV1) / (mapGridV1 + 1)); 

			glEnd();
		break;
		}     
 	
  glPopMatrix();

	}


///////////////////////////////
//
// init()
//
////////////////////////////
void mesaMesh::init()
	{
	int i,j,u,v,x,y,z;
	int amp,threshold;
	
	REFRESH = 0;
	refresh = 50;
 	
 	inited = 0;
 	
	for (x = 0;x<SIGN_WIDTH;x++)
		for(z = 0;z < 96;z++)	
			 sg[x][z] = 0;	

 	meshType[0] = GL_POINTS;
	meshType[1] = GL_LINE_STRIP;
	meshType[2] = 6502;
	
	
	mType		 	 = 2;
	
	cMapRedScale = 0.015;
	xScale		 = 1.0;
	yScale		 = 1.0;
	zScale		 = 16.0;
	meshU2			= 16;
	meshV2			= 16;
	bands			 = 16;
	depth			 = 16;
	
	mapU2			 = 16;
	mapV2			 = 16;
	mapUStride = 3;
	mapVStride = 48;
	mapUOrder		= 16;
	mapVOrder		= 16;
	
	
	mapGridUN		= 16;
	mapGridVN		= 16;
	mapGridU2		= 16;
	mapGridV2		= 16;

	
	colourU2			= 16;
	colourV2			= 16;
	colourUStride	= 4;
	colourVStride	= 64;
	colourUOrder	= 16;
	colourVOrder	= 16;
		
	}

