#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "GL/osmesa.h"

#include <stdlib.h>
#include <stdio.h>
#include <cmath> 			 // pow,sqrt,log10

#include "mesaChars.cc" 
#include "mesaSpectrogram.cc"
#include "mesaMesh.cc"
#include "mesaTunnel.cc"
#include "mesaHistogram.cc"

#define YES 1
#define NO 0
#define TOPSIGN 0
#define BOTTOMSIGN 1
#define MAXHEIGHT 200	
#define CALC 0

extern void mesaToolsDisplayCB(int value);
extern void GLAPIENTRY recordFlightPathCB(GLfloat *vertex);
extern void GLAPIENTRY recordFocusPathCB(GLfloat *vertex);

/////////////////////////////
//
// mesaTools Class def
//
////////////////////////////
class mesaTools
	{
	public:
		mesaTools(midiClient &_MIDIPOINTER);
		~mesaTools();
		
		void signOut();
		void display(int dB);

		mesaNURBS NURBS_CAMERA;
		mesaNURBS NURBS_FOCUS;

		mesaSpectrogram SPECTROGRAM;
		mesaTunnel TUNNEL;
		mesaMesh MESH;
		mesaHistogram HISTOGRAM;
		midiClient *MIDIPOINTER;
		
 		sign topLedsign;

		bool inited;
		int showAxis;
		int showPath;
   		 
		int sourceBlend;
		int destinationBlend;
		int spectrogramType;		

		float cameraX,cameraY,cameraZ;
		float centreX,centreY,centreZ;
		float xScale,yScale,zScale;
		float topPang;
 
		OSMesaContext ctx;
	
		bool busy;
	
	private:
		// private functions		
		void drawAxis(float x,float y,float z);
		double fclamp (double x, double min, double max);
		void drawLetter(charPointer *l);		
		void printStrokedString(char *s);		
		void init();

		void *signOutBuffer;
		void *contextBuffer; 	

		GLuint fogMode[];

		clock_t cameraClock;
	};
	
//////////////////////////////////////////
//
// constructor
//
///////////////////////////////////////
/*
peaks::peaks(sign &_ledsign,midiClient &_MIDIPOINTER) : peaksledsign(&_ledsign) ,
															 freqCursor(_ledsign) ,
															 DBMETER(_ledsign) ,
															 BIGPEAKS(_ledsign) ,
															 MIDIPOINTER(&_MIDIPOINTER)
*/
mesaTools::mesaTools(midiClient &_MIDIPOINTER) : 
												 topLedsign((char*)"192.168.50.12") ,
												 MIDIPOINTER(&_MIDIPOINTER),
												 HISTOGRAM(topLedsign,_MIDIPOINTER),		
												 SPECTROGRAM(topLedsign),
												 NURBS_CAMERA((_GLUfuncptr) &recordFlightPathCB),
												 NURBS_FOCUS((_GLUfuncptr) &recordFocusPathCB),
												 TUNNEL(),
												 MESH()
												 
	{
	inited = false;
 	//init ();
 	}
	
//////////////////////////////////////////
//
// deconstructor
//
///////////////////////////////////////
mesaTools::~mesaTools()
	{
		
	}

//////////////////////////////////////
//
// double effect::fclamp (double x, double min, double max)
//
////////////////////////////////////////
double mesaTools::fclamp (double x, double min, double max)
	{
	 if (x < min) return min;
	  
	 if (x > max) return max;
	  
	 return x;
	}

///////////////////////////////////////
//
// mesa off screen init
//
///////////////////////////////////////
void mesaTools::init()
	{
	int x,z;
		
	GLuint base;

	busy = false;	 
	signOutBuffer = malloc(128 * 48 * 1 * sizeof(GLubyte));

	if (!signOutBuffer) 
		{
	 	printf("Error: couldn't allocate image buffer\n");
	 	return;
	 	}
	
	// Create an RGBA-mode context 
	#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
  	// specify Z, stencil, accum sizes 
  	ctx = OSMesaCreateContextExt( OSMESA_RGBA, 16, 0, 0, NULL );
			#else
   			ctx = OSMesaCreateContext( OSMESA_RGBA, NULL );
	#endif
	
	 if (!ctx) 
		{
	 	printf("OSMesaCreateContext failed!\n");
	 	exit(0);
	   	}

	// Allocate the image/buffer 
	contextBuffer = malloc( 128 * 48 * 3 * 4 * sizeof(GLubyte) );
   
	if (!contextBuffer) 
		{
		printf("Alloc image contextBuffer failed!\n");
		exit(0);
		}

	// Bind the contextBuffer to the context and make it current 
	if (!OSMesaMakeCurrent( ctx, contextBuffer, GL_UNSIGNED_BYTE, 128, 48 * 3 )) 
		{
 		printf("OSMesaMakeCurrent failed!\n");
		exit(0);
		}
/*
    glutInit(NULL,NULL);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc((static)display);
    glutMainLoop();
*/
	//glEnable(GL_DITHER);
	// glDisable(GL_STENCIL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
//	glShadeModel (GL_FLAT);
//	glClearDepth(1.0f);			// Depth Buffer Setup
// may 5/18..i removed this line...add it started working.//	
// glEnable(GL_DEPTH_TEST);//  Enable Z-buffer depth test

	glEnable(GL_CULL_FACE); 

	//glEnable(GL_ALPHA_TEST);

	//glShadeModel(GL_SMOOTH);			

	base = glGenLists (128);
	glListBase(base);
	
	glNewList(base+'0', GL_COMPILE); drawLetter(char0); glEndList();
	glNewList(base+'1', GL_COMPILE); drawLetter(char1); glEndList();
	glNewList(base+'2', GL_COMPILE); drawLetter(char2); glEndList();
	glNewList(base+'3', GL_COMPILE); drawLetter(char3); glEndList();
	glNewList(base+'4', GL_COMPILE); drawLetter(char4); glEndList();
	glNewList(base+'5', GL_COMPILE); drawLetter(char5); glEndList();
	glNewList(base+'6', GL_COMPILE); drawLetter(char6); glEndList();
	glNewList(base+'7', GL_COMPILE); drawLetter(char7); glEndList();
	glNewList(base+'8', GL_COMPILE); drawLetter(char8); glEndList();
	glNewList(base+'9', GL_COMPILE); drawLetter(char9); glEndList();
	
	glNewList(base+'A', GL_COMPILE); drawLetter(charA); glEndList();
	glNewList(base+'B', GL_COMPILE); drawLetter(charB); glEndList();
	glNewList(base+'C', GL_COMPILE); drawLetter(charC); glEndList();
	glNewList(base+'D', GL_COMPILE); drawLetter(charD); glEndList();
	glNewList(base+'E', GL_COMPILE); drawLetter(charE); glEndList();
	glNewList(base+'F', GL_COMPILE); drawLetter(charF); glEndList();
	glNewList(base+'G', GL_COMPILE); drawLetter(charG); glEndList();
	glNewList(base+'H', GL_COMPILE); drawLetter(charH); glEndList();
	glNewList(base+'I', GL_COMPILE); drawLetter(charI); glEndList();
	glNewList(base+'K', GL_COMPILE); drawLetter(charK); glEndList();
	glNewList(base+'N', GL_COMPILE); drawLetter(charN); glEndList();
	glNewList(base+'P', GL_COMPILE); drawLetter(charP); glEndList();
	glNewList(base+'S', GL_COMPILE); drawLetter(charS); glEndList();
	glNewList(base+'T', GL_COMPILE); drawLetter(charT); glEndList();
	glNewList(base+'V', GL_COMPILE); drawLetter(charV); glEndList();
	glNewList(base+'X', GL_COMPILE); drawLetter(charX); glEndList();
	glNewList(base+'Y', GL_COMPILE); drawLetter(charY); glEndList();
	glNewList(base+'b', GL_COMPILE); drawLetter(charb); glEndList();
	glNewList(base+'z', GL_COMPILE); drawLetter(charz); glEndList();
	glNewList(base+'Z', GL_COMPILE); drawLetter(charZ); glEndList();
	glNewList(base+'*', GL_COMPILE); drawLetter(charAsterisk); glEndList();
	glNewList(base+'-', GL_COMPILE); drawLetter(charNeg); glEndList();
	glNewList(base+'+', GL_COMPILE); drawLetter(charPos); glEndList();
	glNewList(base+'.', GL_COMPILE); drawLetter(charDot); glEndList();
	glNewList(base+' ', GL_COMPILE); glTranslatef(1.0, 0.0, 0.0); glEndList();
	 
	  // NURBS_CAMERA icon
	glNewList(base+'@', GL_COMPILE); drawLetter(charAsterisk); glEndList();

	inited = true;
 	}


//////////////////////////////
//
// display
//
/////////////////////////
void mesaTools::display(int dB)
	{
	GLfloat m[16];
	float elapsedSeconds;
 	int i;
float pitch;

	if (!inited)
		init();

	busy = 1;
	
	glDisable(GL_LIGHTING);

//	glClearAccum( 0,0,0,1 ) ;
	glClearColor(0.0f, 0.0f, 0.0f,0.0f); // opaque Black Background
 
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

/*
	GLfloat LightAmbient[]= { 0.0005f, 0.0f, 0.0f, 1.0f };
	GLfloat LightDiffuse[]= { 0.0f, 0.0f, 0.0f, 0.0f };	
	GLfloat LightPosition[]= { 0.0f, 10.0f, 0.0f, 1.0f };	
	 
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);			// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
	
	glEnable(GL_LIGHT1);
  
  	
	glEnable(GL_BLEND);


*/
	glViewport(0, 0, 128, 48);

  	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  	gluPerspective(topPang,2.6,0.0,1.0);
glScalef(xScale,yScale,zScale);


/* 	
    glOrtho(0.0,128.0, 			// left / right
    				0.0,48.0,				// bottom / top
    				0.0,1.0);  // near / far
*/
/*
if (cameraY < 0) {
endwin();
printf("cameraY went negative %2.2f!\n",cameraY);
}
*/
 
	if (NURBS_CAMERA.flyMode == 0) 
		 gluLookAt(	cameraX, cameraY, cameraZ,  
	 			centreX, centreY, centreZ, 
	 			0, 1.0, 0); // positive Y up vector 
 
	if (NURBS_CAMERA.flyMode == 1)  
		{
		elapsedSeconds = ((float)clock() - cameraClock) / CLOCKS_PER_SEC * 1000.0;
		
		if (elapsedSeconds < 5)
		return;

		cameraClock = clock();

pitch = NURBS_CAMERA.getX() - NURBS_FOCUS.getX();
//pitch *=2;

//endwin();
//pitch /= 5.0f;
//printf("pitch -> %2.2f\n",pitch);

		gluLookAt(
			NURBS_CAMERA.getX(),NURBS_CAMERA.getY(),NURBS_CAMERA.getZ(),
			    NURBS_FOCUS.getX() ,
			    NURBS_FOCUS.getY(),
			    NURBS_FOCUS.getZ(),
			//	64.0, 5.0,-10.0,
				0.0,
				1.0, // positive Y up vector 
				0.0);


		NURBS_CAMERA.incIndex();		
		NURBS_FOCUS.incIndex();		
		}
		
//glRotatef(-pitch,  0.0,0.0,1.0);

//endwin();
//printf("%2.2f %2.2f %2.2f",NURBS_CAMERA.getX() ,NURBS_CAMERA.getY() ,NURBS_CAMERA.getZ() );
//printf(" and %2.2f %2.2f %2.2f\n",NURBS_FOCUS.getX() ,NURBS_FOCUS.getY() ,NURBS_FOCUS.getZ() );
		

 	if (spectrogramType == 0)
		TUNNEL.setupCamera();

 	//if ((spectrogramType == 3))	 // histogram
		//	HISTOGRAM.setupNURBS_CAMERA();

	// update camera index in menus
 	mesaToolsDisplayCB(NURBS_CAMERA.index);
	
 	glMatrixMode(GL_MODELVIEW);
 	glLoadIdentity();
//glRotatef(pitch / 4.0,  0.0,0.0,1.0);

 	if (showAxis == 1)
		drawAxis(SIGN_WIDTH,SIGN_HEIGHT,SPECTROGRAM.maxSpectrogramZ);

	switch (spectrogramType)
		{
		case 0:
			TUNNEL.draw();
		break;

		case 1:
			if (NURBS_CAMERA.flyMode == 0) 
				SPECTROGRAM.draw(cameraX, cameraY, cameraZ,
							sourceBlend,destinationBlend);

			if (NURBS_CAMERA.flyMode == 1)
				SPECTROGRAM.draw(NURBS_CAMERA.getX(),
							NURBS_CAMERA.getY(),
							NURBS_CAMERA.getZ(),
							sourceBlend,destinationBlend);
		break;
		
		case 2:
			MESH.draw(&TUNNEL.NURBS,32,0);
		break;	

		case 3:
	 		HISTOGRAM.draw();
		break;
		}
	
// show control points/NURBS?
	if ((NURBS_CAMERA.showCurve == 1 ) && (NURBS_CAMERA.controlPoints != 0))
{
	NURBS_CAMERA.draw();
  		NURBS_FOCUS.draw();
  	}
	
 	glFlush();
	signOut();
	busy = 0;
	}

///////////////////////////////////////////////////
//
//	void signout()
//
////////////////////////////////////////////////// 
void mesaTools::signOut()
	{
	int i, x, y;
	
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); // ttf oglft fonts are distorted without this
//glDisable( GL_DITHER );
	topLedsign.clear(0);
	
	glReadPixels(0, 0, 128, 48, GL_RED, GL_UNSIGNED_BYTE, signOutBuffer);
		
	for (y=0; y<48; y++) 
		for (x=0; x<128;x+=4) 
	   		for (i=0;i<4;i++)
  				topLedsign.setxy(x+i,47-y,((GLubyte*)signOutBuffer)[y*128+x+i] & 3 ) ;	

	topLedsign.refresh();				
	}

	
//////////////////////////////////////////////////////////////
// 
//  drawLetter() interprets the instructions from the array
//  for that letter and renders the letter with line segments.
//
//////////////////////////////////////////////////////////
void mesaTools::drawLetter(charPointer *l)
{

//glLineWidth(1.0);	

 glBegin(GL_LINE_STRIP);
   
 while (1) 
	{
	switch (l->type) 
      		{
		case PT:
			glVertex2fv(&l->x);
		break;
		
		case STROKE:
			glVertex2fv(&l->x);
			glEnd();
			glBegin(GL_LINE_STRIP);
		break;

		case END:
			glVertex2fv(&l->x);
			glEnd();
			glTranslatef(2.0, 0.0, 0.0);
		return;
		}
	l++;
	}

endwin();
printf("\nwhoops...\n\n");
exit(0);	
}

///////////////////////////////////////
//
// void mesaTools::printStrokedString(char *s,float x,float y,float z)
//
///////////////////////////////////////
void mesaTools::printStrokedString(char *s)
	{
  	GLsizei len = strlen(s);
    
	glCallLists(len, GL_BYTE, (GLbyte *)s);
	}
 
//////////////////////////////////
//
//			mesaTools::drawAxis(x,y,z)	 	
//
//////////////////////////////////
void mesaTools::drawAxis(float x,float y,float z)	 	
	{
	int i,j,k;

	glPushMatrix();  
  
	glPointSize(1.0);
	glBegin(GL_POINTS);
	glColor4f(0.005, 0.0, 0.0,0.0);
		
	for (i = 0 ; i < x;i++)
		glVertex3f(i  , 0.0, 0.0);
	
	glEnd();
	
	// y axis	 	
	glBegin(GL_LINES);	
	glColor4f(1.0,  0.0, 0.0 ,0.0);
	glVertex3f(0.0,  0, 0.0 );
	glVertex3f(0.0,  y, 0.0 );
	glEnd();			

	// z axis
	glBegin(GL_POINTS);
	glColor4f(1.0,  0.0, 0.0 ,0.0);
	
	for (i = -z ; i < z;i++)
		glVertex3f(0.0,  0.0, i );
	glEnd();

	// bright dots @ end of axis
	glPointSize(2.0);
	
	glBegin(GL_POINTS);
	glColor4f(1.0,  0.0, 0.0 ,0.0);
	glVertex3f(SIGN_WIDTH - 1,  0.0, 0.0 );
	glVertex3f(-SIGN_WIDTH - 1,  0.0, 0.0 );

	glVertex3f(0.0, SIGN_HEIGHT - 1, 0.0 );
	//glVertex3f(0.0, -SIGN_HEIGHT - 1, 0.0 );

	glVertex3f(0.0, 0.0, z );
//	glVertex3f(0.0, 0.0,-z );
	
	// x axis centre
	glVertex3f(SIGN_WIDTH / 2,0.0,0.0);
	glEnd();
	
	glTranslatef(0.0,1.0,0.0);

	glRotatef(90,0.0,1.0,0.0);
//	glScalef(1.0,2.0,1.0);   
//	printStrokedString((char*)" NEGATIVE Z AXIS");   
	//glScalef(1.0,1.0,1.0); 

	glPopMatrix(); 
	}

