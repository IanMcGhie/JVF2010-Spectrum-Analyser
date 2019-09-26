/*

			FUCKING ONE BY ONE ERROR 
			PLUS ONE.....MINUS ONE....PLUS TWO!!

*/
#include "../tinyspline.h"
#include "mesaNURBS.cc"
#include <time.h>

//#define OGLFT_NO_SOLID TRUE // if GL extrusion lib isnt installed
#define OGLFT_NO_QT

#include <GL/glut.h>
#include <oglft/OGLFT.h>

#define SHOWCONTROLPOINTS 1
#define POLYGONDEBUG 0 
#define MAX_LABELS 7

extern void GLAPIENTRY bandEQCB(GLfloat *vertex);

class mesaSpectrogram  
	{
	public:
		mesaSpectrogram(sign &_SIGNPOINTER);
		~mesaSpectrogram();
	
		void draw(float _cameraX,float _cameraY,float _cameraZ,int _sourceBlend,int _destinationBlend);

		void addRow(uint *_peak, uint *_peakThreshold);
		void drawPolygons(int _x,float _cameraX);
		void drawLabels(int _x,float _cameraX,float _cameraY,float _cameraZ);

		int peakThreshold[SIGN_WIDTH];
		int polygonFace, polygonMode;
	 	int trueTypeFonts;
  		int maxSpectrogramZ;	
		int sg[SIGN_WIDTH][80];

   		uint bands;

		sign *SIGNPOINTER;
		
		float yEyeCentreOffset, zEyeCentreOffset;

		bool inited;

		private:
 		 	void setVertexLineColour(int _x,int _z);
			int pFace[3];
			int pMode[3];
			int pRendering[2];
			int blendFactor[19];
			clock_t addRowSpeed;
			float point_size;
			 
 			 OGLFT::Filled* filled_face;
// 			 OGLFT::Solid* filled_face;
	};
	
////////////////////////////////
//
//	constructor
//
////////////////////////////////
mesaSpectrogram::mesaSpectrogram(sign &_SIGNPOINTER) : SIGNPOINTER(&_SIGNPOINTER)
	{
	#if DEBUG	
		printf("in mesaSpectrogram constructor\n");
	#endif
	
	int l,x,z;
	 
	blendFactor[0] = GL_ZERO;
	blendFactor[1] = GL_ONE;
	blendFactor[2] = GL_SRC_COLOR;
	blendFactor[3] = GL_ONE_MINUS_SRC_COLOR;
	blendFactor[4] = GL_DST_COLOR;
	blendFactor[5] = GL_ONE_MINUS_DST_COLOR;
	blendFactor[6] = GL_SRC_ALPHA;
	blendFactor[7] = GL_ONE_MINUS_SRC_ALPHA;
	blendFactor[8] = GL_DST_ALPHA;
	blendFactor[9] = GL_ONE_MINUS_DST_ALPHA;
	blendFactor[10] = GL_CONSTANT_COLOR;
	blendFactor[11] = GL_ONE_MINUS_CONSTANT_COLOR;
	blendFactor[12] = GL_CONSTANT_ALPHA;
	blendFactor[13] = GL_ONE_MINUS_CONSTANT_ALPHA;
	blendFactor[14] = GL_SRC_ALPHA_SATURATE;
	blendFactor[15] = GL_SRC1_COLOR;
	blendFactor[16] = GL_ONE_MINUS_SRC1_COLOR;
	blendFactor[17] = GL_SRC1_ALPHA;
	blendFactor[18] = GL_ONE_MINUS_SRC1_ALPHA;

	pFace[0] = GL_FRONT;
	pFace[1] = GL_BACK;
	pFace[2] = GL_FRONT_AND_BACK;

	pMode[0] = GL_POINT;
	pMode[1] = GL_LINE;
	pMode[2] = GL_FILL;

	pRendering[0]	= GL_CCW;
	pRendering[1]	= GL_CW;
 
	maxSpectrogramZ = 50;
/*
	// flatten spectrogram array
 	for(z = 0;z < maxSpectrogramZ;z++)	
		for (x=0;x<SIGN_WIDTH;x++)
			sg[x][z] = 0;	
*/	 
 
   
 	addRowSpeed = clock();
	trueTypeFonts = 0;
	 
	point_size = 1;


//	  filled_face = new OGLFT::Translucent( "/home/ian/fonts/TTF/Hack-Bold.ttf", point_size ,100);
  	  filled_face = new OGLFT::Filled( "/home/ian/fonts/TTF/Hack-Regular.ttf", point_size ,100);
//  	  filled_face = new OGLFT::Solid( "/home/ian/fonts/TTF/Hack-Bold.ttf", point_size ,10);
//	  filled_face = new OGLFT::Outline( "/home/ian/fonts/TTF/Hack-Regular.ttf", point_size );
	  filled_face->setHorizontalJustification( OGLFT::Face::CENTER );
	  filled_face->setForegroundColor( 1., 0., 0., 0. );
//	  filled_face->setBackgroundColor(1., 0., 0., 1. );
	  filled_face->setCharacterRotationZ( 0 );
//	  filled_face->setDepth(1);
	  filled_face->setStringRotation( 0 );

	if ( !filled_face->isValid() ) 
		{
		printf("failed to open face. exiting.\n");
		exit( 1 );
		}

	inited = true;
	}
	
////////////////////////////////
//
//	de constructor
//
////////////////////////////////
mesaSpectrogram::~mesaSpectrogram()
	{
 
	}

///////////////////////////////////
//
//			void mesaSpectrogram::addRow(uint *_peak,uint *_peakThreshold)
//
//////////////////////////////

void mesaSpectrogram::addRow(uint *_peak,uint *_peakThreshold)
	{
	int amp,i,l,x,z;
	

	float elapsedSeconds;

	elapsedSeconds = ((float)clock() - addRowSpeed) / CLOCKS_PER_SEC * 1000.0;
		
//	if (elapsedSeconds < 30) // 30ms
//		return;

	addRowSpeed = clock();		
									
	// shift array back 1 ...via z
	for(z = maxSpectrogramZ - 1;z > 0;z--)	
		for (x=0;x<SIGN_WIDTH;x++)
			 sg[x][z] = sg[x][z - 1];	

	memcpy(peakThreshold,_peakThreshold,SIGN_WIDTH * sizeof(uint));
	
	for (x = 0 ; x < SIGN_WIDTH;x++)
		sg[x][0] = _peak[x];
	} // void mesaSpectrogram::addRow(uint *_peak,uint *_peakThreshold)

//////////////////////////////////
//
//				void mesaSpectrogram::draw(int _cameraX, int _cameraY, int _cameraZ)
//
//////////////////////////////////
 void mesaSpectrogram::draw(float _cameraX, float _cameraY, float _cameraZ,int _sourceBlend,int _destinationBlend)
{
	int x;
	int blank = 1;

	if (!inited)			
		return;

	glPushMatrix();
	// preserving opacity....

	// draw bands from furthest to closest 
	// if we get to camerx....stop
	for (x = 0;x < SIGN_WIDTH ;x += SIGN_WIDTH / bands)
		{
		if ((float)x > _cameraX)  	
			break;

		drawPolygons(x,_cameraX);
	 	drawLabels(x,_cameraX,_cameraY,_cameraZ);	
	
		}

  	// finish drawing the rest from furthest to closest
 	// starting from the other end of x axis
	for (x = SIGN_WIDTH - 1 ;x > 0;x -= SIGN_WIDTH / bands)
		{
		if ((float)x < _cameraX)
			break;

		drawPolygons(x  ,_cameraX);
		drawLabels(x ,_cameraX,_cameraY,_cameraZ);	
		}

	glPopMatrix();	 	
}

////////////////////////////////
//
//	void mesaSpectrogram::drawPolygons(int _x,int _cameraX)
//
//
// gl only draws CONVEX POLYGONS and doesnt draw CONCAVE POLYGONS
// so you cannot draw CONCAVE POLYGONS and must draw CONVEX POLYGONS
// if you try to draw CONCAVE POLYGONS it wont work 
// you must break up the concave polygons into CONVEX POLYGONS...and draw those
// because GL only draws CONVEX POLYGONS.
//
////////////////////////////////
void mesaSpectrogram::drawPolygons(int _x,float _cameraX)
	{
	int z,length = 0;
	int peakY = 100;
 	int amp,ampPlusOne,ampPlusTwo;
	bool DEBUG = false;
	int threshold = peakThreshold[_x];
  	  		/*
        char *POLYGONMODE[] =   {       (char*)"GL_POINT",
                                                                (char*)"GL_LINE",
                                                                (char*)"GL_FILL"
                                                                };

        char *POLYGONFACE[] =   {       (char*)"GL_FRONT",
                                                                (char*)"GL_BACK",
                                                                (char*)"GL_FRONT_AND_BACK"
                                                                };

        char *POLYGONRENDERING[] =      {
                                                                (char*)"GL_CCW",
                                                                (char*)"GL_CW"
                                                                };

*/	
 	glPolygonMode(pFace[0], pMode[2]);

	/*
	In a scene composed entirely of opaque closed surfaces,
	back-facing polygons are never visible.
	Eliminating these invisible polygons has the obvious benefit
	of speeding up the rendering of the image.
	To enable and disable elimination of back-facing polygons, call glEnable
	and glDisable with argument GL_CULL_FACE.
	*/



	glPushMatrix();
	//glScalef(xScale,yScale,zScale);


	glEnable(GL_CULL_FACE);

	if (((float)_x ) < _cameraX )
		glFrontFace(GL_CCW);
			else
		  		glFrontFace(GL_CW);

	glPointSize(1.0);

	if ((_x == 64) && DEBUG) {
		endwin();
		printf("in drawPolygons()\n");
	}

	for(z =0;z < maxSpectrogramZ ;z++)
{
                  amp= sg[_x][z];
                        ampPlusOne = sg[_x][z + 1];
                        ampPlusTwo = sg[_x][z + 2];
       
		// floor amp
		if (amp - threshold < 0)
		amp = threshold;
		
		// floor amp
		if (ampPlusOne - threshold < 0)
			ampPlusOne = threshold;
				 
		glColor4f(0.0,0.0,0.0,0.0);
		glBegin(GL_POLYGON);

	   	glVertex3f((float)_x ,
	                             0.0,
	                              (float)(z));

	   	glVertex3f((float)_x ,
	                             (float)(amp - threshold) ,
	                              (float)z);

	   	glVertex3f((float)_x ,
	                             (float)(ampPlusOne - threshold),
				     (float)(z + 1) );

	   	glVertex3f((float)_x  ,
	                             0.0,
	                             (float)(z + 1) );

	 	glEnd();

if ((amp > threshold) && (ampPlusOne < threshold))  {
	            glVertex3f((float)_x ,
	                             0.0,
	                             (float)(z));
}

		if ((amp > threshold) || (ampPlusOne > threshold)) {
			glBegin(GL_LINES); 
	 		glColor4f(0.005,0.0,0.0,0.0);
			
	   		if ((amp - threshold > 5) || (ampPlusOne - threshold > 5)) 
		                      glColor4f(0.006,0.0,0.0,0.0);
			   
			glVertex3f((float)_x ,
		                               (float)(amp - threshold),
		                               (float)(z ));

			   glVertex3f((float)_x,
			                    (float)(ampPlusOne - threshold),
			                    (float)(z + 1));
			glEnd();
			}
		} //for(z = 0 ;z < maxSpectrogramZ ;z++)	
 	 
	if (length > 0) 
		{
	            glVertex3f((float)_x  ,
	                             0.0,
	                             (float)(z));

		if ((_x == 64) && DEBUG)
			printf("finished poly outside of loop length = %d\n",length);
			glEnd();
		}
	glPopMatrix();
}

/////////////////////////////////////////
//
//				void mesaSpectrogram::drawLabels(int _x,int _eyeX,int _eyeY,int _eyeZ)
//
/////////////////////////////////////////////////
void mesaSpectrogram::drawLabels(int _x,float _cameraX,float _cameraY,float _cameraZ)
	{

return;
	char labelString[6] ="12345";


	int peakLength = 0; 
 	int peakHeight = 0;
 	int minPeakLength = 0;
 	int minPeakHeight = 0;
	int lastLabelZ = 0;
	float freq;
	float xFontScale = 1.0;
 	float yFontScale = 1.0;

	glColor4f(1.0,0.0,0.0,0.0);

	freq =  (float)(_x + 1)   * (44100.0 / (float)BUFFERFRAMES);

	// convert freq to string
	if (freq > 1000)
		{
		// if freq is even...dont print beyond decimal			
		if (((int)freq % 2) == 0)
			snprintf (labelString, 10, "%2fK", (float)freq / 1000.0); 
				else
					snprintf (labelString, 10, "%2.1fK",(float) freq ); 
		} else {
			snprintf ( labelString, 10, "%dHz",(int)freq); 
			}

	minPeakLength = (int)(strlen(labelString) +2 );

 	for (int z = 0;z < maxSpectrogramZ ;z++) {
// 	for (int z = maxSpectrogramZ - 1; z > 0 ;z--) {
		bool skipLabel = false;

			
 
		// dont draw labels that are too far from the cameras

//endwin();
//printf("%2.2f\n",sqrt(( ((float)_x - _cameraX) * ((float)_x - _cameraX)) + (((float)peakHeight - _cameraY) * ((float)peakHeight - _cameraY)) + (((float)z - _cameraZ) * ((float)z - _cameraZ))));

skipLabel = false;

if (sqrt(( ((float)_x - _cameraX) * ((float)_x - _cameraX)) + (((float)peakHeight - _cameraY) * ((float)peakHeight - _cameraY)) + (((float)z - _cameraZ) * ((float)z - _cameraZ))) > 50.0 )
	skipLabel = true;

	
		//if ((peakLength > minPeakLength) && (fabs((float)_x - _cameraX) < 50.0 ) && (fabs((float)z - _cameraZ) < 20.0)) 
		if ((peakLength > minPeakLength) && (z - lastLabelZ  > 30  )) 
 		 	{
lastLabelZ = z;
			glPushMatrix();

			// centre label on the y axis
	//		glTranslatef(0.0,(peakHeight ) / 2    ,0.0);
			
			glTranslatef((float)_x ,0.0, z - (minPeakLength / 2) ); 
 
			if  ((float)_x > _cameraX) { // draw label on right side of peak
				 glFrontFace(GL_CW);
				glRotatef(270.0, 0.0, 1.0, 0.0);
				} else {
					 glFrontFace(GL_CCW);
					// draw label on other right side
					glRotatef(90.0, 0.0, 1.0, 0.0);
					}	

			if  (trueTypeFonts == 1)
				{
				filled_face->setCharacterRotationX( 0 );
				filled_face->setCharacterRotationY( 0 );
				filled_face->setCharacterRotationZ( 0 );
				filled_face->setStringRotation(0 );
				//filled_face->setForegroundColor( 1, 0., 0., 0. );
				//filled_face->setBackgroundColor(0., 0., 0., 0. );
				glTranslatef(0.0,0.0,_x );	// the z & x axis are exchanged because of the rotation above
				glRotatef(90.0,0.0,1.0, 0.0); 	// rotate text so label isnt backwards...

				//glRotatef(180,0.0,1.0,0.0);
				filled_face->setCharacterRotationX( 0 );
				filled_face->setCharacterRotationY( 0);
				//filled_face->setStringRotation(180 );
				filled_face->setCharacterRotationZ(0 );

				//glTranslatef(0.0,10.0,0.0);
	
				filled_face->setForegroundColor( 1.0, 0., 0.,0. );
				filled_face->setBackgroundColor(1.0, 0., 0., 0. );
				} 	
  			
			xFontScale =  1.0; 
	//		yFontScale = (float)(peakHeight * 0.5)  ;
 yFontScale = 1.0;
endwin();
printf("hey ... whats this -> %2.2f cameraX is %2.2f z is %d yFontScale is %2.2f minPeakLength is %d\n",  fabs((float)z - _cameraZ)  ,_cameraX,z,yFontScale,minPeakLength);

			glScalef(xFontScale  ,  yFontScale , 1.0);

			glCallLists(strlen(labelString), GL_BYTE, labelString);

			peakLength = 0;
			peakHeight = 0;
			lastLabelZ = z;
			glPopMatrix();
			} // if (peakLength > MAX_LABEL_LENGTH )) 
		} // for (z = 0;z < maxSpectrogramZ ;z++)
}


