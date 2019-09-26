#define MAXVertices 2 * 65535 * 3// 128K * 3 (x,y,z) of NURBS control points
#define MAXCONTROLPOINTS 10000 
 
extern void updateMenuVertices(int value); // update menu values

//////////////////////////////
//
//	void errorCallback(GLenum errorCode) 
//
//////////////////////////////
void errorCallback(GLenum errorCode)
	{
  	const GLubyte *estring;
	
	endwin(); 
  
	estring = gluErrorString(errorCode);
	  
	printf("\n\n\"%s!!!\"...\n\n", estring);
	exit(1);
	}

class mesaNURBS
	{
	public:
		mesaNURBS(_GLUfuncptr _NURBSFP);
		~mesaNURBS();

	 	 _GLUfuncptr NURBSFP; // gl vertex callback function pointer

		tsBSpline spline;	

void calcControlPoints();
		
void circControlPoints();
		void draw();
		void incIndex();
		float getX();
		float getY();
		float getZ();

		float NURBSx[MAXVertices];
		float NURBSy[MAXVertices];
		float NURBSz[MAXVertices];
			
		float controlPointsCoords[MAXCONTROLPOINTS * 4];
		
		int controlPoints;
		int activeControlPoint;
		
		int percentRandom;
		float randomMult;
		int vertices;	
		int index;	
		int SPEED;
		int flyMode;
		int circFlightMode;
		int showCurve;

		float offsetX,offsetY,offsetZ;	
		int controlPointsEdited;
		int movedcontrolPointsCoords[MAXCONTROLPOINTS];

		uint degree;			
		uint dimension;
		uint inited;

		float thetaScale,phiScale;

		int NURBSsamplingMethod;
		float NURBSsamplingTolerance;
		float NURBSparametricTolerance;
		int uStep,vStep;		
		float xDiameter,yDiameter,zDiameter;
		
	private:
		void calcCurve();
		int pointDelay;
		int POINTDELAY;
		int samplingMethod[5];
		int speed;
		int recalcs;
		float lastKnot;
	};
	
//////////////////
//
//	constructor
//
//////////////////
mesaNURBS::mesaNURBS(_GLUfuncptr _NURBSFP) : NURBSFP(_NURBSFP)
	{
	int i;
	inited = 0;	
	SPEED = speed = 0;
 	
	samplingMethod[0] = GLU_PATH_LENGTH;
	samplingMethod[1] = GLU_PARAMETRIC_ERROR;
	samplingMethod[2] = GLU_DOMAIN_DISTANCE;
	samplingMethod[3] = GLU_OBJECT_PATH_LENGTH;
	samplingMethod[4] = GLU_OBJECT_PARAMETRIC_ERROR;
	recalcs = 0;
	inited = 1;
	}

////////////////////////////////
//
//	DE constructor
//
/////////////////////////////////
mesaNURBS::~mesaNURBS() 
	{
	// boom	 
	}

///////////////////////////////////////
//
//		void mesaNURBS::draw()
//
/////////////////////////////////////
void mesaNURBS::draw()
	{ 	
	int i;
	char controlPointNumber[3];

	if (inited != 1)
		{
		printf("\nIn NURBS and spline isnt inited....its over\n");
		return;	
		//exit(1);
		}
 
	if (controlPoints == 0)
		{
		printf("\nno control points\n");
		return;
		}
	
	pointDelay++;  
	
	// draw nurbs
	glPushMatrix();
		
	glBegin(GL_LINE_STRIP);    	
  
  glColor3f(1,0.0,0.0);

  for (i = 0; i < vertices ; i++) 
  	{
  	glVertex3f(NURBSx[i],
		 NURBSy[i],
		 NURBSz[i]);
	}

	glEnd();

	// draw control points
	glPointSize(3.0);
	glColor3f(1.0,0.0,0.0);
	
  for (i = 0; i < controlPoints; i++) 
		{
		glPointSize(2.0);
		glColor3f(1.0,0.0,0.0);
  		
  	if (pointDelay > POINTDELAY / 2)
			if (i == activeControlPoint - 1)
    		glPointSize(4.0);

  	if (pointDelay > POINTDELAY)
  		pointDelay = 0;

  	glBegin(GL_POINTS);    	
  	
  	glVertex3f(controlPointsCoords[(i * 4) + 0],
							 controlPointsCoords[(i * 4) + 1],
							 controlPointsCoords[(i * 4) + 2]);
							 
		glEnd();
  	} // for (i = 0; i < controlPoints; i++) 

	glTranslatef(controlPointsCoords[((activeControlPoint - 1) * 4 ) + 0],
							 controlPointsCoords[((activeControlPoint - 1) * 4 ) + 1],
							 controlPointsCoords[((activeControlPoint - 1) * 4 ) + 2]);

	sprintf(controlPointNumber,"%d",activeControlPoint);
	
	glColor3f(1.0,0.0,0.0);
	glTranslatef(0.0,0.4,0.0); 
	
	glScalef(1.0,5.0,1.0);   
	glCallLists(strlen(controlPointNumber), GL_BYTE, (GLbyte *)controlPointNumber); // show control point number 
	glScalef(1.0,1.0,1.0);   
 
 	
	// show NURBS eye z centre	
	glPointSize(3.0);
	glColor3f(1.0,0.0,0.0);
	// camera position
	glColor3f(1.0,0.0,0.0);

	// draw line from camera to nurbs z eye centre
	glBegin(GL_LINES);
	
	glVertex3f(NURBSx[index],NURBSy[index],NURBSz[index]);
	glEnd();

	// go to the NURBS
/*
	// draw evaluation
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);

	tsDeBoorNet net;

	ts_bspline_evaluate(&spline, uStep, &net);
	
	glBegin(GL_POINTS);
	    glVertex3fv(net.result);
	 glEnd();
	
	ts_deboornet_free(&net);
*/	 		 
	glPopMatrix();
	} 

/////////////////////////////////
//
//		void incIndex();
//
////////////////////////////////
void mesaNURBS::incIndex()
	{
	int i;	

	if (index > vertices)
		{

	for (i = 1;i < controlPoints; i++)
		movedcontrolPointsCoords[i] = -1;
		index = 0;
		calcControlPoints();
		}

	if (index < 0)
		{
	for (i = 1;i < controlPoints; i++)
		movedcontrolPointsCoords[i] = -1;
		calcControlPoints();
		index = vertices;
		}
	
	index += SPEED;
	}

//////////////////////////////
//
// 	getX()
//
///////////////////////////////
float mesaNURBS::getX()
	{
	return NURBSx[index];	
	}

//////////////////////////////
//
// 	getY()
//
///////////////////////////////
float mesaNURBS::getY()
	{
	return NURBSy[index];	
	}

//////////////////////////////
//
// 	getZ()
//
//////////////////////////////
float mesaNURBS::getZ()
	{
	return NURBSz[index];	
	}
	
//////////////////////////////////
//
// calc NURBS (welcome to the watermelon)
//
/*
	Control Points
	
	The control points are a list of at least degree+1 points.
	One of easiest ways to change the shape of a NURBS curve is to move 
	its control points.
	
	The control points have an associated number called a weight.
	With a few exceptions, weights are positive numbers. 
	When a curve�s control points all have the same weight (usually 1),
	the curve is called non-rational, otherwise the curve is called rational. 
	The R in NURBS stands for rational and indicates that a NURBS
	curve has the possibility of being rational. 
	In practice, most NURBS curves are non-rational. A few NURBS
	curves, circles and ellipses being notable examples, are always
	rational.
*/
//
//////////////////////////
void mesaNURBS::calcControlPoints()
	{
	int i,point;
	time_t t;
	int start = 1;
recalcs++;
	if (controlPoints == 0)
		return;
 /* 
	if (circFlightMode > 0) {
		circControlPoints();
		return;
		}
*/	   

srand((unsigned) time(&t));
 
      for (i = 0;i < MAXCONTROLPOINTS; i++)
                movedcontrolPointsCoords[i] = -1;

for (i = 0;i < controlPoints;i++ )
  	{

  	if (movedcontrolPointsCoords[i] == -1)
	  	{
		controlPointsCoords[(i * 4) + 0] = offsetX + (float)(rand() % (int)(xDiameter) );
	        controlPointsCoords[(i * 4) + 1] = offsetY + (float)(rand() % (int)(yDiameter) );
		controlPointsCoords[(i * 4) + 2] = offsetZ + (float)(rand() % (int)(zDiameter) );
                controlPointsCoords[(i * 4) + 3] = 1.0;

		if ((percentRandom > 1) && (randomMult > 0))
			if ((rand() % 100) < percentRandom) {
			controlPointsCoords[(i * 4) + 1] = controlPointsCoords[(i * 4) + 1] * randomMult;
			
			//endwin();
			//printf("rnd pt #%d (%2.2f, %2.2f, %2.2f) rand = %d perrnd = %d\n", i,  controlPointsCoords[(i * 4) + 0], controlPointsCoords[(i * 4) + 1], controlPointsCoords[(i * 4) + 2], rand() % 100,percentRandom);
			}
		}   // 	if (movedcontrolPointsCoords[i] != -1)
//endwin();
//printf("i = %d (%2.2f , %2.2f, %2.2f) xDiameter -> %2.2f\n",i, controlPointsCoords[(i * 4) + 0], controlPointsCoords[(i * 4) + 1], controlPointsCoords[(i * 4) + 2],xDiameter);
 
	} // for (i = 0;i < controlPoints;i++ )
//printf("--------------------------------\n");
//printf("x0 = %2.2f y0 = %2.2f z0 = %2.2f x%d = %2.2f y%d = %2.2f z%d = %2.2f\n\n",controlPointsCoords[0],controlPointsCoords[1],controlPointsCoords[2], i ,controlPointsCoords[i * 4],i ,controlPointsCoords[(i * 4) + 1],i ,controlPointsCoords[(i * 4) + 2] );


	calcCurve();
}

///////////////////////////////////////////
//
//
//   circControlPoints()
//
//
/////////////////////////////////////////
void mesaNURBS::circControlPoints() {
int i;
float x,y,z;
bool goodPoint =false;
offsetX = -30; 

for (i = 0;i < controlPoints;i++ )
        {
       if (movedcontrolPointsCoords[i] == -1)
                {
	goodPoint = false;	

		while (!goodPoint) {
			x = (float)(rand() % (int)(xDiameter ));
			y = (float)(rand() % (int)(yDiameter ));
			z = (float)(rand() % (int)(30 )); 
//endwin();
// printf("%2.2f %2.2f %2.2f    %2.2f %2.2f %2.2f \n",x,y,z,xDiameter,yDiameter,zDiameter);
		if ((x >= (xDiameter * 0.5 )) && (z >= (zDiameter * 0.5))) // && (y >= (int)(yDiameter / 3.0) ) && (z >= (int)(zDiameter / 3.0) ))
			{
			goodPoint=true;
	//		endwin();
	//		printf("good point! %d, %d, %d i = %d\n",x,y,z,i);
			}
		}

           	controlPointsCoords[(i * 4) + 0] = offsetX + (x / 2);
                controlPointsCoords[(i * 4) + 1] = offsetY + (y / 2);
                controlPointsCoords[(i * 4) + 2] = offsetZ + (z / 2);
                controlPointsCoords[(i * 4) + 3] = 1.0;
		}
	}

	calcCurve();
}

//////////////////////////////////
//
//		void mesaNURBS::calcCurve()
//
//////////////////////////////////
void mesaNURBS::calcCurve()
	{
	int i;

 if (controlPoints == 0)
 		{
 		printf("spline has no control points!\n");	
		return;
		}
		
	GLUnurbsObj *NURBSCurve;

  NURBSCurve = gluNewNurbsRenderer();
  
  // curve properties
	// if you dont set this to GLU_NURBS_TESSELLATOR...you get nothing
  gluNurbsProperty(NURBSCurve, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);

// GLU_SAMPLING_METHOD can be 
// GLU_PATH_LENGTH,
// GLU_PARAMETRIC_ERROR,
// GLU_DOMAIN_DISTANCE,
// GLU_OBJECT_PATH_LENGTH, or
// GLU_OBJECT_PARAMETRIC_ERROR.
                        
  gluNurbsProperty(NURBSCurve, GLU_SAMPLING_METHOD,samplingMethod[NURBSsamplingMethod]);
	
	switch (samplingMethod[NURBSsamplingMethod])
		{
		case GLU_PATH_LENGTH: 
		case GLU_OBJECT_PATH_LENGTH:
			gluNurbsProperty(NURBSCurve, GLU_SAMPLING_TOLERANCE ,NURBSsamplingTolerance); 
		break;

		case GLU_PARAMETRIC_ERROR:
	  	case GLU_OBJECT_PARAMETRIC_ERROR:
		gluNurbsProperty(NURBSCurve, GLU_PARAMETRIC_TOLERANCE ,NURBSparametricTolerance); 
		break;

 		
		}
   			
//	gluNurbsProperty(nurbsCurve, GLU_DISPLAY_MODE, GLU_FILL);  
	gluNurbsProperty(NURBSCurve, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);  
  	gluNurbsProperty(NURBSCurve, GLU_AUTO_LOAD_MATRIX, TRUE);

	gluNurbsProperty(NURBSCurve, GLU_U_STEP, uStep);// flight resolutions (how big the nurbsxyz array is)
	gluNurbsProperty(NURBSCurve, GLU_V_STEP, vStep);// flight resolutions (how big the nurbsxyz array is)
	
  	gluNurbsCallback(NURBSCurve, GLU_NURBS_ERROR, 
						   (_GLUfuncptr) errorCallback );

//	gluNurbsProperty(NURBSCurve, GLU_CULLING, GL_TRUE); // culling bad...maybe
  		
	gluNurbsCallback(NURBSCurve, GLU_NURBS_BEGIN, NULL); 
	gluNurbsCallback(NURBSCurve, GLU_NURBS_END, NULL); 

	gluNurbsCallback(NURBSCurve, GLU_NURBS_VERTEX, (_GLUfuncptr)  NURBSFP );

//	  gluNurbsCallback(NURBSCurve, GLU_NURBS_VERTEX,
//	  					 			(_GLUfuncptr)  recordFlightPathCB );

 
  // degree,dimension,# ctrl points,TS_CLAMPED
  //The forth parameter (TS_CLAMPED) ensures that B-Splines 
  //are tangent to the first and last control point. That's it!
 //  The order of a NURBS curve is positive whole number equal to (degree+1)

// If you want to create a NURBS of degree n with m control points in 3D,
// then you have to create a B-Spline of degree n with m control points in 4D
// tsBSpline nurbs;
// ts_bspline_new(n, 4, m, TS_CLAMPED, &nurbs);

ts_bspline_new(degree,4, 
  							 controlPoints,
  							 TS_CLAMPED,//TS_OPENED,//TS_CLAMPED, TS_NONE
  							 &spline); 		


	/*
	Degree
	The degree is a positive whole number.
	This number is usually 1, 2, 3 or 5, but can be any positive whole
	number. NURBS lines and polylines are usually degree 1, NURBS circles
	are degree 2, and most free-form curves are degree 3 or 5.
	Sometimes the terms linear, quadratic, cubic, and quintic are used.
	Linear means degree 1, quadratic means degree 2, cubic means degree 3,
	and quintic means degree 5.
	
	You may see references to the order of a NURBS curve.
	The order of a NURBS curve is positive whole number equal to (degree+1). Consequently, the degree is equal to (order-1).
	
	It is possible to increase the degree of a NURBS curve and not change
	its shape. Generally, it is not possible to reduce a NURBS curve�s
	degree without changing its shape.
	
	
	https://www.rhino3d.com/nurbs
	*/

		// WHATS THIs?....a mesh
	//	glMap1f(GL_MAP1_VERTEX_3,0.0,1.0,1.0,16,&curvecontrolPointsCoords[0][0][0]);
	//	glEnable(GL_MAP1_VERTEX_3);
	//	glMapGrid1f(cameraSpeed,0.0,1.0);
	//	glEvalMesh1(GL_LINE,0,cameraSpeed);
		
 

 
	vertices = 0;				
	index = 0;
	
	gluBeginCurve(NURBSCurve);

//printf("ok...n_knots= %d knots = %2.2f dim = %d order = %d\n",spline.n_knots,spline.knots,spline.dim,spline.order); 

	
	gluNurbsCurve(NURBSCurve, 
	    				  spline.n_knots, 
	    				  spline.knots, 
	    				  spline.dim  , //Specifies the offset (as a number of single-precision floating-point values) 
		    				 					 	 // between successive curve control points.
	    				  controlPointsCoords, 
	    				 	spline.order,// order is dergee + 1 
	   			 		  GL_MAP1_VERTEX_4
							 );
								


 gluEndCurve(NURBSCurve);
	
	vertices-=4; // off by one
	
	gluDeleteNurbsRenderer(NURBSCurve);

	ts_bspline_free(&spline); // free dynamically allocated memory
  //updateMenuVertices(vertices);
  
  inited = 1;
	}

 
