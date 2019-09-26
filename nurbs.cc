#define USE_REALLY_LONG_VARIABLE_NAMES TRUE

#include <stdlib.h> 						// exit,uint
#include "classes/nCurses.cc"
#include "classes/jackTools.cc"
  
//////////////////////////
//
// the prototypes
//
///////////////////////
int main(int argc, char **argv);

void setupMenus();
void setValues();
void setupNURBS();

static void controlPointCB();// __attribute__((cdecl));     // GNU GCC
void recalcNURBSCB();//   __attribute__((cdecl));     // GNU GCC

void setbeatsPerMinuteCallback(int value);
void flashBPM(int flash);
void updateBPMMenuItem(int _checkForNotes);

void updateTunnelVertices(int menuValue);
void recalcTUNNELCB(int menuValue);

static void reCalcThresholdCB();

//void GLAPIENTRY recordFlightPathCB(GLfloat *vertex);
void GLAPIENTRY recordTunnelVeritces(GLfloat *vertex);
void GLAPIENTRY bandEQCB(GLfloat *vertex);
void GLAPIENTRY errorCallback(GLenum errorCode);

void mesaToolsDisplayCB(int menuItemValue);
void histogramDisplayCB(int _whichKey,int _whichScale);
void setKeyHistogramCB(int value);

///////////////////////////
//
//		objects
//
///////////////////////////
effect ANALYSER;	 									 // the spectrum analyser
jackTools JACK((char*)"Ledsign Analyser",BUFFERFRAMES,&ANALYSER);	// audio i/o
nCurses DSPMENU((char*)"LEDSign NURBS"); // ncurses 


////////////////////////////////////////////////////////////
//
//  welcome to main.........
//
/////////////////////////////////////////////////////
int main(int argc, char **argv)
	{
	int keyPressed;
 
 	setupMenus();	
	setValues();	 

	reCalcThresholdCB();
	controlPointCB(); 		// set x,y,z coords of 1st control point on 'control points' menu 
	recalcNURBSCB();
// flatten curve...reset ALL points


//	ANALYSER.MESA.NURBS_FOCUS.calcControlPoints();	

	recalcTUNNELCB(0);
	setKeyHistogramCB(1);
 	
 	DSPMENU.showPanels();	
  	
 	while (1)
		{
		keyPressed = DSPMENU.keyCheck();	
		
		setValues();
			
		if (DSPMENU.quit == 1)
			{
			ANALYSER.ANALYSERLEDSIGN.clear(0);	
			ANALYSER.ANALYSERLEDSIGN.refresh();	
			
//			ANALYSER.MESA.topLedsign.clear(0);
//			ANALYSER.MESA.topLedsign.refresh();

			JACK.jackShutdown();
			exit(1);
			}	
		} // while (1)
  
  return 0;  /* ANSI C requires main to return int. */
  }

//////////////////////////////
//
//		void setupMenus()
//
//////////////////////////////
void setupMenus()
	{
	int subMenu;
	
	char* WEIGHTING[] = {	(char*)"",
							(char*)"A",
							(char*)"C",
							(char*)"I468" };

	char *ZOOMBAND[]  = { 	(char*)"Lo",
							(char*)"Mid",
							(char*)"Mid/High",
							(char*)"High" };
													 
	char *OLDPEAKDISPLAY[]	= { (char*)"Array",
								(char*)"Stack" };

	char *XSCALE[]	= { (char*)"Log1",
						(char*)"Log2",
						(char*)"Lin" };

	char *DISPLAYTYPE[] = { (char*)"Analyser",
							(char*)"Scope" };

	char *SPECTROGRAM[] = { (char*)"Tunnel",
							(char*)"3D",
							(char*)"Mesh",
							(char*)"Histogram"};
/*	
char *BLENDBUFFERTYPE[] = { (char*)"GL_ZERO",
								(char*)"GL_ONE",
								(char*)"GL_SRC_COLOR",
								(char*)"GL_ONE_MINUS_SRC_COLOR",
								(char*)"GL_DST_COLOR",
								(char*)"GL_ONE_MINUS_DST_COLOR",
								(char*)"GL_SRC_ALPHA",
								(char*)"GL_ONE_MINUS_SRC_ALPHA",
								(char*)"GL_DST_ALPHA",
								(char*)"GL_ONE_MINUS_DST_ALPHA",
								(char*)"GL_CONSTANT_COLOR",
								(char*)"GL_ONE_MINUS_CONSTANT_COLOR",
								(char*)"GL_CONSTANT_ALPHA",
								(char*)"GL_ONE_MINUS_CONSTANT_ALPHA",
								(char*)"GL_SRC_ALPHA_SATURATE",
								(char*)"GL_SRC1_COLOR",
								(char*)"GL_ONE_MINUS_SRC1_COLOR",
								(char*)"GL_SRC1_ALPHA",
								(char*)"GL_ONE_MINUS_SRC1_ALPHA"
							   };

	char *DEPTHBUFFERTYPE[] = { (char*)"GL_NEVER",
 								(char*)"GL_LESS",
 								(char*)"GL_EQUAL",
 								(char*)"GL_LEQUAL",
 								(char*)"GL_GREATER",
 								(char*)"GL_NOTEQUAL",
								(char*)"GL_GEQUAL",
								(char*)"GL_ALWAYS"
								};
*/
	char *POLYGONMODE[] = 	{	(char*)"GL_POINT",
 								(char*)"GL_LINE",
 								(char*)"GL_FILL"
								};

	char *POLYGONFACE[] = 	{	(char*)"GL_FRONT",
 								(char*)"GL_BACK",
 								(char*)"GL_FRONT_AND_BACK"
								};

	char *POLYGONRENDERING[] = 	{ 
								(char*)"GL_CCW",
 								(char*)"GL_CW"
								};
														
	char* NURBSSAMPLINGMETHOD[] =	{ 
								(char*)"GLU_PATH_LENGTH",
								(char*)"GLU_PARAMETRIC_ERROR",
								(char*)"GLU_DOMAIN_DISTANCE",
								(char*)"GLU_OBJECT_PATH_LENGTH",
								(char*)"GLU_OBJECT_PARAMETRIC_ERROR" };

	char *MESHTYPE[] = { (char*)"Points",
						 (char*)"Line Strip",
	  	 				 (char*)"Mesh" };
 
	 
														
	/*
	 void nCurses::addMenuItem (int panelID,
					char *menuItem,
					float start,
					float min,
					float max,
					float decBy,
					float incBy,
					char* units,
					uint useOpts,
					...)
 	*/

 	// main menu
	DSPMENU.addMenuItem(MAINMENU,(char*)"Gain",1.0 ,-1.0 ,1.0 ,-0.005, 0.005,(char*)"",THOUSANDTHS);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Display Type",0.0,0.0,1.0,-1.0,1.0,(char*)"",DISPLAYTYPE);
 	DSPMENU.addMenuItem(MAINMENU,(char*)"Tuner Mode",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(MAINMENU,(char*)"dB Meter Update",3.0 ,1.0 ,11.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(MAINMENU,(char*)"GL Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU1);
	DSPMENU.addMenuItem(MAINMENU,(char*)"MIDI Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU20);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Threshold Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU5);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Cursor Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU6);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Oscilloscope Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU7);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Peak Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU8);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Scale Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU9);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Weighting Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU10);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Zoom Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU11);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Text Field",0.0,0.0,10.0,-1.0,1.0,(char*)"127.0.0.1",TEXTFIELD);
	DSPMENU.addMenuItem(MAINMENU,(char*)"Number Field",0.0,0.0,10.0,-1.0,1.0,(char*)"1000",NUMERIC);
 
	// gl submenu		
	DSPMENU.addMenuItem(SUBMENU1,(char*)"Spectogram Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU13);
	DSPMENU.addMenuItem(SUBMENU1,(char*)"NURBS Camera Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU15);
	DSPMENU.addMenuItem(SUBMENU1,(char*)"Mesh Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU24);
 	DSPMENU.addMenuItem(SUBMENU1,(char*)"Tunnel Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU21);
	DSPMENU.addMenuItem(SUBMENU1,(char*)"Camera Coords Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU2);
	DSPMENU.addMenuItem(SUBMENU1,(char*)"Show Axis",1.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU1,(char*)"H Perspective",14.0,0.0 ,180.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);

	// Camera  coords submenu
	DSPMENU.addMenuItem(SUBMENU2,(char*)"Camera X",64.0 ,-1500.0 ,1500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU2,(char*)"Camera Y",80.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU2,(char*)"Camera Z",146.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU2,(char*)"Screen Centre X",64.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU2,(char*)"Screen Centre Y",2.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU2,(char*)"Screen Centre Z",0.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	
	// lighting submenu
	DSPMENU.addMenuItem(SUBMENU3,(char*)"Lighting",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	

	// threshold submenu	
	DSPMENU.addMenuItem(SUBMENU5,(char*)"Threshold View",1.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU5,(char*)"Threshold X",17,0,64,-1,1,(char*)"CALLBACK",WHOLENUMBER,&reCalcThresholdCB);
	DSPMENU.addMenuItem(SUBMENU5,(char*)"Threshold Y",22,0,48,-1,1,(char*)"CALLBACK",WHOLENUMBER,&reCalcThresholdCB);
	DSPMENU.addMenuItem(SUBMENU5,(char*)"Threshold Curve",-0.0007,-2.0,1.0,-0.0001,0.0001,(char*)"CALLBACK", TENTHOUSANDTHS,&reCalcThresholdCB);
 	
	// cursor submenu
	DSPMENU.addMenuItem(SUBMENU6,(char*)"Cursor Mode",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU6,(char*)"Cursor X",46,0,128,-1,1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU6,(char*)"Cursor Y",17,0,47,-1,1,(char*)"",WHOLENUMBER);
 	
 	// scope submenu
 	DSPMENU.addMenuItem(SUBMENU7,(char*)"Scope Gain",24,0,10000,-1,1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU7,(char*)"Time Base",0.0078125,-0.0909090909,1.0,-0.00006103515625,0.00006103515625,(char*)"",HUNDREDEDTHOUSANDTHS); // 2Hz
	DSPMENU.addMenuItem(SUBMENU7,(char*)"Trigger Level",24.0,-24.0,24.0,-1.0,1.0,(char*)"",WHOLENUMBER);

	// peak submenu
	DSPMENU.addMenuItem(SUBMENU8,(char*)"Peak Delay",150,0,300,-1,1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU8,(char*)"Old Peak Delay",75,0,250,-1,1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU8,(char*)"Big Peak Delay",50,0,200,-1,1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU8,(char*)"Old Peak Display",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",OLDPEAKDISPLAY);
	DSPMENU.addMenuItem(SUBMENU8,(char*)"Feedback Delay",5,0,100,-1,1,(char*)"",WHOLENUMBER);
	
	// scale submenu
	DSPMENU.addMenuItem(SUBMENU9,(char*)"X Axis Scale",0.0,0.0,2.0,-1.0,1.0,(char*)"",XSCALE);
	DSPMENU.addMenuItem(SUBMENU9,(char*)"Y Axis Scale",1.0,0.0,50.0,-0.01,0.01,(char*)"",HUNDREDTHS);
	
	// weighting submenu
	DSPMENU.addMenuItem(SUBMENU10,(char*)"Weighting",1.0,0.0,3.0,-1.0,1.0,(char*)"",WEIGHTING);
	DSPMENU.addMenuItem(SUBMENU10,(char*)"Weighting View",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	
	// zoom submenu
	DSPMENU.addMenuItem(SUBMENU11,(char*)"Zoom",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU11,(char*)"Zoom Band",0.0,0.0,3.0,-1.0,1.0,(char*)"", 	ZOOMBAND);
	
	// fog submenu
//	DSPMENU.addMenuItem(SUBMENU12,(char*)"Fog Filter",0.0 ,0.0 ,3.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
//	DSPMENU.addMenuItem(SUBMENU12,(char*)"Fog Density",0.001 ,-1.0 ,1.0 ,-0.001, 0.001,(char*)"",THOUSANDTHS);
//	DSPMENU.addMenuItem(SUBMENU12,(char*)"Fog Colour",0.00 ,-1.00 ,1.00 ,-0.001, 0.001,(char*)"",THOUSANDTHS);
 	
	// spectrogram submenu
	DSPMENU.addMenuItem(SUBMENU13,(char*)"Type",1.0,0.0,3.0,-1.0,1.0,(char*)"",SPECTROGRAM);
	DSPMENU.addMenuItem(SUBMENU13,(char*)"True Type Fonts",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU13,(char*)"X Scale",1.0 ,-100.0 ,100.0 ,-0.1, 0.1,(char*)"",TENTHS);
	DSPMENU.addMenuItem(SUBMENU13,(char*)"Y Scale",0.4 ,-1.0 ,200.0 ,-0.05, 0.05,(char*)"",HUNDREDTHS);
	DSPMENU.addMenuItem(SUBMENU13,(char*)"Z Scale (speed)",1.0 ,-50.0 ,50.0 ,-0.05,0.05,(char*)"",HUNDREDTHS);
	DSPMENU.addMenuItem(SUBMENU13,(char*)"Bands",128.0 ,2.0 ,128.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
 	

	// NURBS submeu
	DSPMENU.addMenuItem(SUBMENU15, (char*)"Display Options",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU16);
	DSPMENU.addMenuItem(SUBMENU15, (char*)"Control Points",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU17);
	DSPMENU.addMenuItem(SUBMENU15, (char*)"Offsets / Scale",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU18);
	DSPMENU.addMenuItem(SUBMENU15, (char*)"NURBS Properties",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU22);
	DSPMENU.addMenuItem(SUBMENU15, (char*)"NURBS Vertices",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU15, (char*)"Current Camera Vertex",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
		
	// NURBS options menu	
	DSPMENU.addMenuItem(SUBMENU16, (char*)"Show Curve",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU16, (char*)"Fly Mode",1.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU16, (char*)"Camera Speed",3.0 ,-100.0 ,100.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU16, (char*)"Camera Vertices",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU16, (char*)"Current Camera Vertex",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);

	// NURBS control points submenu
	DSPMENU.addMenuItem(SUBMENU17, (char*)"# Control Points",600.0 ,4.0 ,10000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);	
	DSPMENU.addMenuItem(SUBMENU17, (char*)"Circular Ctl Points",1.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU17, (char*)"Percent Random",30.0 ,0.0 ,100.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);	
	DSPMENU.addMenuItem(SUBMENU17, (char*)"Random Mult",2.7 ,0.0 ,10.0 ,-0.1, 0.1,(char*)"CALLBACK",TENTHS,&recalcNURBSCB);	
	DSPMENU.addMenuItem(SUBMENU17, (char*)"Control Point",0.0 ,0.0 ,10000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&controlPointCB);
	DSPMENU.addMenuItem(SUBMENU17, (char*)"X",0.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU17, (char*)"Y",0.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU17, (char*)"Z",0.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU17, (char*)"NURBS Vertices",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU17, (char*)"Current NURBS_CAMERA Vertex",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);

	// NURBS offsets / scale menu
	DSPMENU.addMenuItem(SUBMENU18, (char*)"Theta Scale",0.80 ,-180.0 ,180.0 ,-0.01, 0.01,(char*)"CALLBACK",HUNDREDTHS,&recalcNURBSCB);		
	DSPMENU.addMenuItem(SUBMENU18, (char*)"Phi Scale",  1.99, -180.0 ,180.0 ,-0.01, 0.01,(char*)"CALLBACK",HUNDREDTHS,&recalcNURBSCB);	
	// changing the degree makes the curve break...jumpy NURBS_CAMERA @ end of flight
	DSPMENU.addMenuItem(SUBMENU18, (char*)"Curve Degree",3.0 ,1.0 ,300.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);

        DSPMENU.addMenuItem(SUBMENU18, (char*)"Camera X Offset",-50.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Y Offset",5.0  ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Z Offset",15.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"X Diameter",128.0,1.0 ,1000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Y Diameter",15.0,1.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Z Diameter",45.0,1.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);

        DSPMENU.addMenuItem(SUBMENU18, (char*)"Focus X Offset",0.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Y Offset",-5.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Z Offset",0.0 ,-500.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"X Diameter",120.0,1.0 ,1000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Y Diameter",10.0,1.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
        DSPMENU.addMenuItem(SUBMENU18, (char*)"Z Diameter",1.0,1.0 ,500.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);

	DSPMENU.addMenuItem(SUBMENU18, (char*)"NURBS Vertices",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);

	// blend menu
/*
	DSPMENU.addMenuItem(SUBMENU19, (char*)"fe Color",1.0,0.0 ,1.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU19, (char*)"Blending",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU19, (char*)"Source Buffer",7.0,0.0,18.0,-1.0,1.0,(char*)"",BLENDBUFFERTYPE);
	DSPMENU.addMenuItem(SUBMENU19, (char*)"Destination Buffer",1.0,0.0,18.0,-1.0,1.0,(char*)"",BLENDBUFFERTYPE);
*/ 	
	// MIDI menu
	DSPMENU.addMenuItem(SUBMENU20, (char*)"Histogram Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU28);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"MIDI Mode",1.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"Play Same Note",1.0 ,0.0 ,10.0 ,-1, 1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"Same Note Count Reset",10.0 ,1.0 ,250.0 ,-1, 1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"Check For Notes",150.0 ,0.0 ,10000.0 ,-5, 5,(char*)"CALLBACK",WHOLENUMBER,&setbeatsPerMinuteCallback);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"Sustain",100.0 ,-5000.0 ,5000.0 ,-5, 5,(char*)"ms",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"Octave Shift From A4",0.0 ,-10.0 ,10.0 ,-1, 1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"MIDI Max Note",108.0 ,0.0 ,108.0 ,-1, 1,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU20, (char*)"MIDI Min Note",68.0 ,27.0 ,169.0 ,-1, 1,(char*)"",WHOLENUMBER);

	// Tunnel submenu
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Tunnel NURBS Properties",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU23);
 	DSPMENU.addMenuItem(SUBMENU21, (char*)"Fly Mode",1.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
 	DSPMENU.addMenuItem(SUBMENU21, (char*)"Show Flight Path",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
 	DSPMENU.addMenuItem(SUBMENU21, (char*)"Theta Scale",2.0 ,-100.0 ,100.0 ,-0.01, 0.01,(char*)"CALLBACK",HUNDREDTHS,&recalcTUNNELCB);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Phi Scale",2.0 ,-100.0 ,100.0 ,-0.01, 0.01,(char*)"CALLBACK",HUNDREDTHS,&recalcTUNNELCB);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Tunnel View Depth",64.0 ,1.0 ,5000.0 ,-1, 1,(char*)"",WHOLENUMBER);	
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Mark Every Z",6.0 ,1.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Diameter",6.0 ,1.0 ,100.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Speed",6.0 ,-25.0 ,25.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Bands",10.0 ,1.0 ,127.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Z Scale",1.75 ,-100.0 ,100.0 ,-0.5, 0.5,(char*)"",TENTHS);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Tunnel Vertices",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU21, (char*)"Current Tunnel Vertex",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	
	// Spectrogram flight nurbs options
	DSPMENU.addMenuItem(SUBMENU22, (char*)"Sampling Method",3.0,0.0 ,4.0 ,-1.0, 1.0,(char*)"CALLBACK",NURBSSAMPLINGMETHOD,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU22, (char*)"Sampling Tolerance",1,-1.0 ,150.0 ,-0.05, 0.05,(char*)"CALLBACK",TENTHS,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU22, (char*)"Parametric Tolerance",1,0.0 ,15.0 ,-0.1, 0.1,(char*)"CALLBACK",TENTHS,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU22, (char*)"U Step",200.0 ,1.0 ,5000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU22, (char*)"V Step",1.0 ,1.0 ,5000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcNURBSCB);
	DSPMENU.addMenuItem(SUBMENU22, (char*)"NURBS Vertices",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU22, (char*)"Current NURBS_CAMERA Vertex",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);

	// tunnel nurbs options
	DSPMENU.addMenuItem(SUBMENU23, (char*)"Sampling Method",4.0,0.0 ,4.0 ,-1.0, 1.0,(char*)"CALLBACK",NURBSSAMPLINGMETHOD,&recalcTUNNELCB);
	DSPMENU.addMenuItem(SUBMENU23, (char*)"Sampling Tolerance",1.0,-1.0 ,50.0 ,-0.1, 0.1,(char*)"CALLBACK",TENTHS,&recalcTUNNELCB);
	DSPMENU.addMenuItem(SUBMENU23, (char*)"Parametric Tolerance",1.0,-1.0 ,50.0 ,-0.1, 0.1,(char*)"CALLBACK",TENTHS,&recalcTUNNELCB);
	DSPMENU.addMenuItem(SUBMENU23, (char*)"U Step",17.0 ,1.0 ,5000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcTUNNELCB);
	DSPMENU.addMenuItem(SUBMENU23, (char*)"V Step",17.0 ,1.0 ,5000.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&recalcTUNNELCB);
	DSPMENU.addMenuItem(SUBMENU23, (char*)"NURBS Vertices",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU23, (char*)"NURBS Vertex",0.0 ,0.0 ,0.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);

	// Mesh submenu
	DSPMENU.addMenuItem(SUBMENU24, (char*)"Mesh Type",2.0,0.0 ,2.0 ,-1.0, 1.0,(char*)"",MESHTYPE);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"glMap2f Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU25);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"glMapGrid2f Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU26);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"glColorMap2f Menu",0.0 ,0.0 ,0.0 ,0.0, 0.0,(char*)"",HIDE,SUBMENU27);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"CMap Bright Red", 0.015 ,-1.0 ,100.0 ,-0.001, 0.001,(char*)"",THOUSANDTHS);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"X Scale", 1.0 ,-1.0 ,100.0 ,-0.1, 0.1,(char*)"",TENTHS);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"Y Scale", 1.0 ,-1.0 ,100.0 ,-0.1, 0.1,(char*)"",TENTHS);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"Z Scale", 1.0 ,-1.0 ,100.0 ,-0.1, 0.1,(char*)"",TENTHS);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"Mesh U2",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"Mesh V2",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"Bands",16.0 ,1.0 ,96.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU24, (char*)"Depth",16.0 ,1.0 ,96.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	
	DSPMENU.addMenuItem(SUBMENU25, (char*)"glMap2f U2",16.0 ,1.0 ,128.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU25, (char*)"glMap2f V2",16.0 ,1.0 ,96.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU25, (char*)"glMap2f U Stride",3 ,3.0 ,10000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);	
	DSPMENU.addMenuItem(SUBMENU25, (char*)"glMap2f V Stride",3 * 16,3.0 ,10000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU25, (char*)"glMap2f U Order",16.0 ,1.0 ,32.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU25, (char*)"glMap2f V Order",16.0 ,1.0 ,32.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);

	DSPMENU.addMenuItem(SUBMENU26, (char*)"glMapGrid2f UN",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU26, (char*)"glMapGrid2f VN",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU26, (char*)"glMapGrid2f U2",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU26, (char*)"glMapGrid2f V2",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	
	DSPMENU.addMenuItem(SUBMENU27, (char*)"Colour U2",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU27, (char*)"Colour V2",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU27, (char*)"Colour U Stride",4.0 ,4.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU27, (char*)"Colour V Stride",64.0 ,4.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU27, (char*)"Colour U Order",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU27, (char*)"Colour V Order",16.0 ,1.0 ,1000.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	
	// histogram menu
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Play Key",0.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Key",0.0 ,0.0 ,11.0 ,-1.0, 1.0,(char*)"",KEY);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Scale",0.0 ,0.0 ,3.0 ,-1.0, 1.0,(char*)"",SCALE);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Play Key Delay",2.0 ,1.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Histogram Y Threshold",1.0 ,1.0 ,47.0 ,-1.0, 1.0,(char*)"CALLBACK",WHOLENUMBER,&setKeyHistogramCB);
//	DSPMENU.addMenuItem(SUBMENU28, (char*)"Threshold Y",19,0,48,-1,1,(char*)"CALLBACK",WHOLENUMBER,&reCalcThresholdCB);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Show Key Histogram",1.0 ,0.0 ,1.0 ,-1.0, 1.0,(char*)"",ONOFF);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Reset Histogram",15.0 ,1.0 ,500.0 ,-1.0, 1.0,(char*)"",WHOLENUMBER);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"BPM Tap",45.0 ,1.0 ,200.0 ,-0.1, 0.1,(char*)"",TENTHS);
	DSPMENU.addMenuItem(SUBMENU28, (char*)"Detected Key",0.0 ,0.0 ,11.0 ,-1.0, 1.0,(char*)"",KEY);
	 
	// add menu note to bottom of main menu
	DSPMENU.addNote(MAINMENU,(char *)"Use cursor keys\n Shift L/R Inc/Dec Faster");

	// add menu notes to the rest of the menus
	for (subMenu = 1; subMenu < MAXPANELS;subMenu++)
		DSPMENU.addNote(subMenu,(char *)"Shift L/R Inc/Dec Faster");
	}

////////////////////////////////
//
//	void setValues()
//
///////////////////////////// 
void setValues()
	{
	int menuIndex = 0;
	
	// Main menu
	ANALYSER.gain				= DSPMENU.getMenuItemValue(MAINMENU,menuIndex++);	
	ANALYSER.displayType 			= DSPMENU.getMenuItemValue(MAINMENU,menuIndex++);
	ANALYSER.ANALYSERLEDSIGN.tunerMode	= DSPMENU.getMenuItemValue(MAINMENU,menuIndex++);
	ANALYSER.PEAKS.DBMETER.REFRESH	= DSPMENU.getMenuItemValue(MAINMENU,menuIndex++);

	// GL submenu
	menuIndex = 5;
	ANALYSER.MESA.showAxis	= DSPMENU.getMenuItemValue(SUBMENU1,menuIndex++);
	ANALYSER.MESA.topPang =  DSPMENU.getMenuItemValue(SUBMENU1,menuIndex++);

//endwin();
//printf("\%d  ------- %2.2f\n", ANALYSER.MESA.showAxis,ANALYSER.MESA.topPang);
	// GL NURBS_CAMERA coords submenu
	menuIndex = 0;
	ANALYSER.MESA.cameraX = DSPMENU.getMenuItemValue(SUBMENU2,menuIndex++);
	ANALYSER.MESA.cameraY = DSPMENU.getMenuItemValue(SUBMENU2,menuIndex++);
	ANALYSER.MESA.cameraZ = DSPMENU.getMenuItemValue(SUBMENU2,menuIndex++);
	ANALYSER.MESA.centreX = DSPMENU.getMenuItemValue(SUBMENU2,menuIndex++);
	ANALYSER.MESA.centreY = DSPMENU.getMenuItemValue(SUBMENU2,menuIndex++);
	ANALYSER.MESA.centreZ = DSPMENU.getMenuItemValue(SUBMENU2,menuIndex++); 
 
 	// threshold submenu
	menuIndex = 0;
	ANALYSER.PEAKS.parabolaView	= DSPMENU.getMenuItemValue(SUBMENU5,menuIndex++);
	ANALYSER.PEAKS.parabolaX		= DSPMENU.getMenuItemValue(SUBMENU5,menuIndex++);
	ANALYSER.PEAKS.parabolaY 		= DSPMENU.getMenuItemValue(SUBMENU5,menuIndex++);
	ANALYSER.PEAKS.parabolaWidth	= DSPMENU.getMenuItemValue(SUBMENU5,menuIndex++); 
 	
	// cursor submenu
	menuIndex = 0;
	ANALYSER.PEAKS.CURSOR.CURSOR_MODE	= DSPMENU.getMenuItemValue(SUBMENU6,menuIndex++);
	ANALYSER.PEAKS.CURSOR.cursorX		= DSPMENU.getMenuItemValue(SUBMENU6,menuIndex++);
	ANALYSER.PEAKS.CURSOR.cursorY		= DSPMENU.getMenuItemValue(SUBMENU6,menuIndex++); 
	
	// oscilloscope menu
	menuIndex = 0;
	ANALYSER.SCOPE.scopeGain		= DSPMENU.getMenuItemValue(SUBMENU7,menuIndex++);
	ANALYSER.SCOPE.timeBase		= DSPMENU.getMenuItemValue(SUBMENU7,menuIndex++);
	ANALYSER.SCOPE.triggerLevel 	= DSPMENU.getMenuItemValue(SUBMENU7,menuIndex++); 
	
	// peaks submenu
	menuIndex = 0;
	ANALYSER.PEAKS.PEAKDELAY				= DSPMENU.getMenuItemValue(SUBMENU8,menuIndex++);
	ANALYSER.PEAKS.OLDPEAKDELAY				= DSPMENU.getMenuItemValue(SUBMENU8,menuIndex++);
	ANALYSER.PEAKS.BIGPEAKS.BIGPEAKDELAY		= DSPMENU.getMenuItemValue(SUBMENU8,menuIndex++);
	ANALYSER.PEAKS.oldPeakDisplay		 		= DSPMENU.getMenuItemValue(SUBMENU8,menuIndex++); 
	ANALYSER.PEAKS.BIGPEAKS.FEEDBACKCHECKDELAY	= DSPMENU.getMenuItemValue(SUBMENU8,menuIndex++); 

	// scale submenu
	menuIndex = 0;	
	ANALYSER.xScale 				= DSPMENU.getMenuItemValue(SUBMENU9,menuIndex++);
	ANALYSER.PEAKS.xScale			= DSPMENU.getMenuItemValue(SUBMENU9,menuIndex++);
	
	// weighting submenu
	menuIndex = 0;
	ANALYSER.ANALYSERLEDSIGN.weighting		= DSPMENU.getMenuItemValue(SUBMENU10,menuIndex++);
	ANALYSER.ANALYSERLEDSIGN.weightingView	= DSPMENU.getMenuItemValue(SUBMENU10,menuIndex++);

	// zoom submenu
	menuIndex = 0;	
	ANALYSER.ANALYSERLEDSIGN.zoom	= DSPMENU.getMenuItemValue(SUBMENU11,menuIndex++);
	ANALYSER.ANALYSERLEDSIGN.zoomBand 	= DSPMENU.getMenuItemValue(SUBMENU11,menuIndex++); 

 
	// gl spectrogram submenu
	menuIndex = 0;
	ANALYSER.MESA.spectrogramType			= DSPMENU.getMenuItemValue(SUBMENU13,menuIndex++);
	ANALYSER.MESA.SPECTROGRAM.trueTypeFonts	= DSPMENU.getMenuItemValue(SUBMENU13,menuIndex++);
	ANALYSER.MESA.xScale		= DSPMENU.getMenuItemValue(SUBMENU13,menuIndex++); 
	ANALYSER.MESA.yScale		= DSPMENU.getMenuItemValue(SUBMENU13,menuIndex++);
	ANALYSER.MESA.zScale		= DSPMENU.getMenuItemValue(SUBMENU13,menuIndex++); 
	ANALYSER.MESA.SPECTROGRAM.bands		= DSPMENU.getMenuItemValue(SUBMENU13,menuIndex++); 
	
	// gl blend menu
	menuIndex = 0;
ANALYSER.MESA.sourceBlend	= DSPMENU.getMenuItemValue(SUBMENU19,menuIndex++);
	ANALYSER.MESA.destinationBlend = DSPMENU.getMenuItemValue(SUBMENU19,menuIndex++);

	// MIDI menu
	menuIndex = 1;
	ANALYSER.MIDICLIENT.midiMode		= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	ANALYSER.MIDICLIENT.playSameNote 	= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	ANALYSER.MIDICLIENT.playNoteReset	= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	ANALYSER.MIDICLIENT.checkForNotes	= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	ANALYSER.MIDICLIENT.sustain		= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	ANALYSER.MIDICLIENT.octaveShift		= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	ANALYSER.MIDICLIENT.maxNote		= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	ANALYSER.MIDICLIENT.minNote		= DSPMENU.getMenuItemValue(SUBMENU20,menuIndex++);
	 
	// Tunnel menu 
	menuIndex = 1;
	ANALYSER.MESA.TUNNEL.flyMode			= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.showCurve		= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.NURBS.thetaScale	= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.NURBS.phiScale		= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.depth 			= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.markEveryZ		= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.diameter			= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.SPEED			= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.bands 			= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);
	ANALYSER.MESA.TUNNEL.zScale 			= DSPMENU.getMenuItemValue(SUBMENU21,menuIndex++);

	// spectrogtam flight NURBS Properties
	menuIndex = 0;
	ANALYSER.MESA.NURBS_CAMERA.NURBSsamplingMethod 		= DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.NURBSsamplingTolerance	= DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.NURBSparametricTolerance	= DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.uStep			= DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.vStep			= DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);

        menuIndex = 0;
        ANALYSER.MESA.NURBS_FOCUS.NURBSsamplingMethod		= DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.NURBSsamplingTolerance       = DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.NURBSparametricTolerance     = DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.uStep                        = DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.vStep                        = DSPMENU.getMenuItemValue(SUBMENU22,menuIndex++);

	// tunnel NURBS Properties
	menuIndex = 0;
	ANALYSER.MESA.TUNNEL.NURBS.NURBSsamplingMethod 		= DSPMENU.getMenuItemValue(SUBMENU23,menuIndex++);
	ANALYSER.MESA.TUNNEL.NURBS.NURBSsamplingTolerance	= DSPMENU.getMenuItemValue(SUBMENU23,menuIndex++);
	ANALYSER.MESA.TUNNEL.NURBS.NURBSparametricTolerance	= DSPMENU.getMenuItemValue(SUBMENU23,menuIndex++);
	ANALYSER.MESA.TUNNEL.NURBS.uStep				= DSPMENU.getMenuItemValue(SUBMENU23,menuIndex++);
	ANALYSER.MESA.TUNNEL.NURBS.vStep				= DSPMENU.getMenuItemValue(SUBMENU23,menuIndex++);

	// mesh Properties
	menuIndex = 0;
	ANALYSER.MESA.MESH.mType 		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	menuIndex+=3;
	ANALYSER.MESA.MESH.cMapRedScale 	= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	ANALYSER.MESA.MESH.xScale 		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	ANALYSER.MESA.MESH.yScale 		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	ANALYSER.MESA.MESH.zScale 		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	ANALYSER.MESA.MESH.meshU2		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	ANALYSER.MESA.MESH.meshV2		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	ANALYSER.MESA.MESH.bands		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	ANALYSER.MESA.MESH.depth		= DSPMENU.getMenuItemValue(SUBMENU24,menuIndex++);
	
	menuIndex = 0;
	ANALYSER.MESA.MESH.mapU2	= DSPMENU.getMenuItemValue(SUBMENU25,menuIndex++);
	ANALYSER.MESA.MESH.mapV2 	= DSPMENU.getMenuItemValue(SUBMENU25,menuIndex++);
	ANALYSER.MESA.MESH.mapUStride 	= DSPMENU.getMenuItemValue(SUBMENU25,menuIndex++);
	ANALYSER.MESA.MESH.mapVStride 	= DSPMENU.getMenuItemValue(SUBMENU25,menuIndex++);
	ANALYSER.MESA.MESH.mapUOrder	= DSPMENU.getMenuItemValue(SUBMENU25,menuIndex++);
	ANALYSER.MESA.MESH.mapVOrder	= DSPMENU.getMenuItemValue(SUBMENU25,menuIndex++);
	
	menuIndex = 0;
	ANALYSER.MESA.MESH.mapGridUN	= DSPMENU.getMenuItemValue(SUBMENU26,menuIndex++);
	ANALYSER.MESA.MESH.mapGridVN	= DSPMENU.getMenuItemValue(SUBMENU26,menuIndex++);
	ANALYSER.MESA.MESH.mapGridU2	= DSPMENU.getMenuItemValue(SUBMENU26,menuIndex++);
	ANALYSER.MESA.MESH.mapGridV2	= DSPMENU.getMenuItemValue(SUBMENU26,menuIndex++);

	menuIndex = 0;
	ANALYSER.MESA.MESH.colourU2	= DSPMENU.getMenuItemValue(SUBMENU27,menuIndex++);
	ANALYSER.MESA.MESH.colourV2	= DSPMENU.getMenuItemValue(SUBMENU27,menuIndex++);
	ANALYSER.MESA.MESH.colourUStride = DSPMENU.getMenuItemValue(SUBMENU27,menuIndex++);
	ANALYSER.MESA.MESH.colourVStride = DSPMENU.getMenuItemValue(SUBMENU27,menuIndex++);
	ANALYSER.MESA.MESH.colourUOrder = DSPMENU.getMenuItemValue(SUBMENU27,menuIndex++);
	ANALYSER.MESA.MESH.colourVOrder = DSPMENU.getMenuItemValue(SUBMENU27,menuIndex++);

	// histogram menu
	menuIndex = 0;
	ANALYSER.MESA.HISTOGRAM.playKey		= DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
	ANALYSER.MESA.HISTOGRAM.key			= DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
	ANALYSER.MESA.HISTOGRAM.keyScale		= DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
	ANALYSER.MESA.HISTOGRAM.PLAYKEYDELAY	= DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
	ANALYSER.MESA.HISTOGRAM.histogramThresholdY = DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
	ANALYSER.MESA.HISTOGRAM.showKeyHistogram = DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
	ANALYSER.MESA.HISTOGRAM.RESETHISTOGRAM = DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
	ANALYSER.MESA.HISTOGRAM.BPM			 = DSPMENU.getMenuItemValue(SUBMENU28,menuIndex++);
 	
 	setupNURBS();
	}

//////////////////////////////////
//
//	void setupNURBS(void);
//
///////////////////////////////////
void setupNURBS()
	{
	int menuIndex;
	 	
	// NURBS menu
	menuIndex = 0;
	ANALYSER.MESA.NURBS_CAMERA.showCurve	= DSPMENU.getMenuItemValue(SUBMENU16, menuIndex++);		
	ANALYSER.MESA.NURBS_CAMERA.flyMode	= DSPMENU.getMenuItemValue(SUBMENU16, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.SPEED	= DSPMENU.getMenuItemValue(SUBMENU16, menuIndex++);

	menuIndex = 0;	
	ANALYSER.MESA.NURBS_FOCUS.showCurve	= DSPMENU.getMenuItemValue(SUBMENU16, menuIndex++);		
	ANALYSER.MESA.NURBS_FOCUS.flyMode	= DSPMENU.getMenuItemValue(SUBMENU16, menuIndex++);
	ANALYSER.MESA.NURBS_FOCUS.SPEED		= DSPMENU.getMenuItemValue(SUBMENU16, menuIndex++);

	// NURBS display options
	menuIndex = 0;
	ANALYSER.MESA.NURBS_CAMERA.controlPoints = DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);		
	ANALYSER.MESA.NURBS_CAMERA.circFlightMode	= DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.percentRandom = DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);		
	ANALYSER.MESA.NURBS_CAMERA.randomMult	 = DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);		
	
	menuIndex = 0;
	ANALYSER.MESA.NURBS_FOCUS.controlPoints = DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);		
	ANALYSER.MESA.NURBS_FOCUS.circFlightMode	= DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);
	ANALYSER.MESA.NURBS_FOCUS.percentRandom = DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);		
	ANALYSER.MESA.NURBS_FOCUS.randomMult	 = DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.activeControlPoint = DSPMENU.getMenuItemValue(SUBMENU17, menuIndex++);				
 
	// NURBS offsets / scale
	menuIndex = 0;
	ANALYSER.MESA.NURBS_CAMERA.thetaScale = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex);
	ANALYSER.MESA.NURBS_FOCUS.thetaScale = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.phiScale	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex);
	ANALYSER.MESA.NURBS_FOCUS.phiScale	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.degree 	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex);		
	ANALYSER.MESA.NURBS_FOCUS.degree 	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);		
	ANALYSER.MESA.NURBS_CAMERA.offsetX	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.offsetY	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.offsetZ	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.xDiameter	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.yDiameter	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.zDiameter	= DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	ANALYSER.MESA.NURBS_FOCUS.offsetX      = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.offsetY      = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.offsetZ      = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.xDiameter    = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.yDiameter    = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
        ANALYSER.MESA.NURBS_FOCUS.zDiameter    = DSPMENU.getMenuItemValue(SUBMENU18, menuIndex++);
	 
	if (ANALYSER.MESA.NURBS_CAMERA.controlPoints != 0)
		{
		DSPMENU.setMenuItemValue(SUBMENU15, 4, ANALYSER.MESA.NURBS_CAMERA.vertices);		
		DSPMENU.setMenuItemValue(SUBMENU16, 3, ANALYSER.MESA.NURBS_CAMERA.vertices);		
		DSPMENU.setMenuItemValue(SUBMENU17, 8, ANALYSER.MESA.NURBS_CAMERA.vertices);		
		DSPMENU.setMenuItemValue(SUBMENU18, 15, ANALYSER.MESA.NURBS_CAMERA.vertices);		
		DSPMENU.setMenuItemValue(SUBMENU22, 5, ANALYSER.MESA.NURBS_CAMERA.vertices);		
		}
	}

//////////////////////////////
//
//				void beatsPerMinuteCallback(int value)
//
//////////////////////////////
void updateBPMMenuItem(int _checkForNotes)
	{
//	DSPMENU.setMenuItemValue(SUBMENU28,8,value);	
	
 	DSPMENU.setMenuItemValue(SUBMENU20,4,_checkForNotes);
	mvwprintw(DSPMENU.menuWindow[SUBMENU20] ,7 ,26 , "%d  ", _checkForNotes);
	}
	
//////////////////////////////
//
//				void beatsPerMinuteCallback(int value)
//
//////////////////////////////
void setbeatsPerMinuteCallback(int value)
	{
	if (value == -1)
		ANALYSER.MIDICLIENT.checkForNotesTap();
	}
	
//////////////////////////////
//
//							void flashBPM(int flash)
//
//////////////////////////////
void flashBPM(int flash)
	{
	DSPMENU.flash(SUBMENU20,4,ANALYSER.MIDICLIENT.flashOn,(char*)"ms");
	}
	
//////////////////////////////
//
//		void set key histgram threshold y  callback (int menuValue);
//
//////////////////////////////
void setKeyHistogramCB(int value)
	{
	
	//ANALYSER.MESA.HISTOGRAM.init(0); 
	}
	
//////////////////////////////
//
//		void recalcTUNNELCB(int menuValue);
//
//////////////////////////////
void recalcTUNNELCB(int menuValue)
	{
	setValues();
	
	ANALYSER.MESA.TUNNEL.init();
	
	updateTunnelVertices(0);
	}

//////////////////////////////////////
//								nCurses Callback
//
//		void recalcNURBSCB(int menuItemValue) 	
//
//////////////////////////////////////
void recalcNURBSCB() 	
	{
	int controlPointIndex,menuIndex = 0;
 	
	controlPointIndex = ANALYSER.MESA.NURBS_CAMERA.activeControlPoint * 4;

//controlPointIndex--;

	// keep track of the control points you move
	if ((DSPMENU.getCurrentPanelID() == 17) &&
			(DSPMENU.getMenuRow(17) > 1) && (DSPMENU.getMenuRow(17) < 5))
		{
	//	ANALYSER.MESA.NURBS_CAMERA.movedcontrolPointsCoords[ANALYSER.MESA.NURBS_CAMERA.controlPointsEdited] = ANALYSER.MESA.NURBS_CAMERA.activeControlPoint;
	//	ANALYSER.MESA.NURBS_CAMERA.controlPointsEdited++;
		}
		
 	menuIndex = 5;
 	ANALYSER.MESA.NURBS_CAMERA.controlPointsCoords[((controlPointIndex) * 4) + 0] = DSPMENU.getMenuItemValue(SUBMENU17,menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.controlPointsCoords[((controlPointIndex) * 4) + 1] = DSPMENU.getMenuItemValue(SUBMENU17,menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.controlPointsCoords[((controlPointIndex) * 4) + 2] = DSPMENU.getMenuItemValue(SUBMENU17,menuIndex++);
	ANALYSER.MESA.NURBS_CAMERA.controlPointsCoords[((controlPointIndex) * 4) + 3] = 1.0;

	setupNURBS();
	
	// flatten curve...reset ALL points
	ANALYSER.MESA.NURBS_CAMERA.calcControlPoints();	
	ANALYSER.MESA.NURBS_FOCUS.calcControlPoints();	
	//ANALYSER.MESA.NURBS_CAMERA.calcCurve();	
		
	// after NURBS's is calculated...update vertices menu values
	DSPMENU.setMenuItemValue(SUBMENU15, 4, ANALYSER.MESA.NURBS_CAMERA.vertices);		
	DSPMENU.setMenuItemValue(SUBMENU16, 3, ANALYSER.MESA.NURBS_CAMERA.vertices);		
	DSPMENU.setMenuItemValue(SUBMENU17, 8, ANALYSER.MESA.NURBS_CAMERA.vertices);		
	DSPMENU.setMenuItemValue(SUBMENU18, 15, ANALYSER.MESA.NURBS_CAMERA.vertices);		
	DSPMENU.setMenuItemValue(SUBMENU22, 5, ANALYSER.MESA.NURBS_CAMERA.vertices);		

	DSPMENU.showPanels();
	}	
	
//////////////////////////////
//
//		void GLAPIENTRY recordTunnelVeritces(GLfloat *vertex) 
//
/////////////////////////////

void GLAPIENTRY recordTunnelVeritces(GLfloat *vertex) 
		{
 	 	ANALYSER.MESA.TUNNEL.NURBS.NURBSx[ANALYSER.MESA.TUNNEL.NURBS.vertices] = vertex[0];
		ANALYSER.MESA.TUNNEL.NURBS.NURBSy[ANALYSER.MESA.TUNNEL.NURBS.vertices] = vertex[1];
		ANALYSER.MESA.TUNNEL.NURBS.NURBSz[ANALYSER.MESA.TUNNEL.NURBS.vertices] = vertex[2];
		ANALYSER.MESA.TUNNEL.NURBS.vertices++;
		}
			
//////////////////////////////////////
//					nCurses Callback
// void controlPointCB(int menuItemValue) // __attribute__((cdecl));     // GNU GCC
//
//////////////////////////////////////
void controlPointCB() 
	{
	int menuIndex;
	int controlPoints	= DSPMENU.getMenuItemValue(SUBMENU17,0);
	int controlPointIndex	= DSPMENU.getMenuItemValue(SUBMENU17,4); 
	
	// dont inc past the number of total control points
	if (controlPointIndex > controlPoints - 1) {  
		 DSPMENU.setMenuItemValue(SUBMENU17,4,controlPointIndex - 1);	
		return;	
	}

	if (ANALYSER.MESA.NURBS_CAMERA.controlPoints != 0)
		{
		menuIndex = 5;
		DSPMENU.setMenuItemValue(SUBMENU17, menuIndex++, ANALYSER.MESA.NURBS_CAMERA.controlPointsCoords[controlPointIndex * 4 + 0]);
		DSPMENU.setMenuItemValue(SUBMENU17, menuIndex++, ANALYSER.MESA.NURBS_CAMERA.controlPointsCoords[controlPointIndex * 4 + 1]);
		DSPMENU.setMenuItemValue(SUBMENU17, menuIndex++, ANALYSER.MESA.NURBS_CAMERA.controlPointsCoords[controlPointIndex * 4 + 2]);
		}
	 
	DSPMENU.showPanels();	

	mvwprintw(DSPMENU.menuWindow[SUBMENU15] ,7 ,26 , "%d  ", ANALYSER.MESA.NURBS_CAMERA.vertices);
	mvwprintw(DSPMENU.menuWindow[SUBMENU16] ,6 ,26 , "%d  ", ANALYSER.MESA.NURBS_CAMERA.vertices);
	mvwprintw(DSPMENU.menuWindow[SUBMENU17] ,8 ,26 , "%d  ", ANALYSER.MESA.NURBS_CAMERA.vertices);
	mvwprintw(DSPMENU.menuWindow[SUBMENU18] ,8 ,26 , "%d  ", ANALYSER.MESA.NURBS_CAMERA.vertices);
	mvwprintw(DSPMENU.menuWindow[SUBMENU22] ,8 ,26 , "%d  ", ANALYSER.MESA.NURBS_CAMERA.vertices);
	}

//////////////////////////////////////
//				 
//		void updateTunnelVertices(int value) 
//
//////////////////////////////////////
void updateTunnelVertices(int menuValue) 
	{
	DSPMENU.setMenuItemValue(SUBMENU21, 10, ANALYSER.MESA.TUNNEL.NURBS.vertices );		
	DSPMENU.setMenuItemValue(SUBMENU21, 11, ANALYSER.MESA.TUNNEL.NURBS.index);		// tunnel vertex

	DSPMENU.setMenuItemValue(SUBMENU23, 5, ANALYSER.MESA.TUNNEL.NURBS.vertices );		
	DSPMENU.setMenuItemValue(SUBMENU23, 6, ANALYSER.MESA.TUNNEL.NURBS.index);		// tunnel vertex
	
	mvwprintw(DSPMENU.menuWindow[SUBMENU23] ,8 ,26 , "%d  ", ANALYSER.MESA.TUNNEL.NURBS.vertices + 1);
	mvwprintw(DSPMENU.menuWindow[SUBMENU23] ,9 ,26 , "%d  ", ANALYSER.MESA.TUNNEL.NURBS.index);
	}	

////////////////////////////////
//
//		mesa histogram callback
//
///////////////////////////////
void histogramDisplayCB(int _whichScale,int _whichKey)
	{
	DSPMENU.setMenuItemValue(SUBMENU28, 9, _whichKey);		
	mvwprintw(DSPMENU.menuWindow[SUBMENU28] ,12 ,26 , "%s%s           ",
				 KEY[_whichKey],SCALE[_whichScale]);
	
	}
	
//////////////////////////////////////
//					Mesa Callback
//		void mesaToolsDisplayCB(int menuItemValue) 
//
//////////////////////////////////////
void mesaToolsDisplayCB(int value) 
	{
	// update NURBS menus with current NURBS index(NURBS_CAMERA)
	
	DSPMENU.setMenuItemValue(SUBMENU15, 4, value);		
	DSPMENU.setMenuItemValue(SUBMENU16, 4, value);			
	DSPMENU.setMenuItemValue(SUBMENU17, 9, value);		
	DSPMENU.setMenuItemValue(SUBMENU22, 6, value);		
	
	// dont call showPanels()...just update the parts of menu that changed

	mvwprintw(DSPMENU.menuWindow[SUBMENU15] ,8 ,26 , "%d  ", value);
	mvwprintw(DSPMENU.menuWindow[SUBMENU16] ,7 ,26 , "%d  ", value);
	mvwprintw(DSPMENU.menuWindow[SUBMENU17] ,12 ,26, "%d  ", value);
	mvwprintw(DSPMENU.menuWindow[SUBMENU22] ,9 ,26 , "%d  ", value);
 	}

						
///////////////////////////////////
//
//		recalc threshold parabola
//
//////////////////////////////////						
void reCalcThresholdCB()
	{						
	setValues();

	ANALYSER.PEAKS.calcThreshold();
	}


//////////////////////////////
//
//		void mesarecordFlightPathCB(GLfloat *vertex)
//
//////////////////////////////

void GLAPIENTRY recordFlightPathCB(GLfloat *vertex) 
	{
  	ANALYSER.MESA.NURBS_CAMERA.NURBSx[ANALYSER.MESA.NURBS_CAMERA.vertices] = vertex[0];
	ANALYSER.MESA.NURBS_CAMERA.NURBSy[ANALYSER.MESA.NURBS_CAMERA.vertices] = vertex[1];
	ANALYSER.MESA.NURBS_CAMERA.NURBSz[ANALYSER.MESA.NURBS_CAMERA.vertices] = vertex[2];
	ANALYSER.MESA.NURBS_CAMERA.vertices++;
	}

//////////////////////////////
//
//				void GLAPIENTRY recordFocusPathCB(GLfloat *vertex) 
//
//////////////////////////////
void GLAPIENTRY recordFocusPathCB(GLfloat *vertex) 
		{
 	 	ANALYSER.MESA.NURBS_FOCUS.NURBSx[ANALYSER.MESA.NURBS_FOCUS.vertices] = vertex[0];
		ANALYSER.MESA.NURBS_FOCUS.NURBSy[ANALYSER.MESA.NURBS_FOCUS.vertices] = vertex[1];
		ANALYSER.MESA.NURBS_FOCUS.NURBSz[ANALYSER.MESA.NURBS_FOCUS.vertices] = vertex[2];
		ANALYSER.MESA.NURBS_FOCUS.vertices++;
		}

