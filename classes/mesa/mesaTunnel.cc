#include "../tinyspline.h" // NURBS! NURBS! NURBS! 

//#include "mesaNURBS.cc"
extern void GLAPIENTRY recordTunnelVeritces(GLfloat *vertex);
extern void updateTunnelVertices(int value);
 
class mesaTunnel
	{
	public:
//		mesaTunnel(sign &_signPointer);
		mesaTunnel();
		~mesaTunnel();
		
		mesaNURBS NURBS;
		mesaMesh MESH;
		
//		sign *signPointer;
		void init();
		void draw();
		void setupCamera();
		void drawCircle(float _diameter,float _centreX, float _centreY, float _centre);		
		void applyRotation();
 
		float diameter;
		int bands;
		float yScale;
		float zScale;
		
		int depth;
		int markEveryZ;
		int flyMode;
		int showCurve;
	//	float offsetX,offsetY,offsetZ;
	//	float tunnelStep;
	//	float cameraX,cameraY,cameraZ;
	//	float centreX,centreY,centreZ;
	
	int rotateX, rotateY, rotateZ;

	int upX,upYours,upZ;
	int SPEED;

	int inited;
	private:
		
	};

////////////////////////////////
//
//	constructor
//
////////////////////////////////
//mesaTunnel::mesaTunnel(sign &_signPointer) : signPointer(&_signPointer) ,
mesaTunnel::mesaTunnel() : NURBS((_GLUfuncptr) &recordTunnelVeritces),
														 MESH()
																		//				 MESH((sign&)*signPointer)
	{
	int i;
	inited = 0;

	init();
	}
		
////////////////////////////////
//
//	de constructor
//
////////////////////////////////
mesaTunnel::~mesaTunnel()
	{

	}

/////////////////////////////////
//
//		void mesaTunnel::init()
//
////////////////////////////////////
void mesaTunnel::init()
	{
//	NURBS.thetaScale = 1.0;
//	NURBS.phiScale = 2.0;

	NURBS.degree = 1;
	
	NURBS.offsetX = 20.0;
	NURBS.offsetY = 0.0;
	NURBS.offsetZ = -17.0;
	
	NURBS.xDiameter = 32.0;
	NURBS.yDiameter = 10.0;
	NURBS.zDiameter = 32.0;
	
	//NURBS.calcControlPoints();
	//NURBS.calcCurve();
	
	NURBS.percentRandom = 0;
	NURBS.randomMult = 0;
	
	yScale = 1.0;
	zScale = 1.0;
	 
	SPEED = 1;
	
	upX = 0;
	upYours = 1;
	upZ = 0;
	
	rotateX = 0;
	rotateY = 1;
	rotateZ = 0;
	
	flyMode = 0;
	
	inited = 1;		
	}
	

/////////////////////////////////
//
//	void mesaTunnel::draw()
//
///////////////////////////////
void mesaTunnel::draw()
	{
	int i,b,d = 0,v;
	float startX,startY,startZ;
	int j = 0;
	
	startX = 0;
	startY = 0;
	startZ = 0;
 	
	glColor4f(0.005, 0.0, 0.0, 0.0);

	if (showCurve == 1)
  		NURBS.draw();
  	
// MESH.drawMesh(&NURBS,diameter,NURBS.index / depth);
 	
//	glPushMatrix();	

	j = NURBS.index;
	
	for (i = 0 ;i < depth - 1 ; i++)
		{
		if (j + i > NURBS.vertices - SPEED)
			j = 0;
		
		j++;  
				
		if (i % markEveryZ == 0)
					drawCircle(diameter,
								NURBS.NURBSx[j],	
								NURBS.NURBSy[j],	
								NURBS.NURBSz[j]);

	
		}
		
//	glPopMatrix();		
	updateTunnelVertices(NURBS.index);
	

	}

/////////////////////////////////
//
//	void mesaTunnel::setupTunnelCamera()
//
///////////////////////////////
void mesaTunnel::setupCamera()
	{
	int i,b;

//	if (inited != 1)
//		return;
	
//	if (NURBS.index + SPEED > NURBS.vertices)
//		NURBS.index = 0;
		
	if (flyMode == 1) 
		gluLookAt(NURBS.NURBSx[NURBS.index],
						  NURBS.NURBSy[NURBS.index],
						  NURBS.NURBSz[NURBS.index],  // eye
	
//	0.0,0.0,0.0,						
							NURBS.NURBSx[NURBS.index + (depth)],
							NURBS.NURBSy[NURBS.index + (depth)],
							NURBS.NURBSz[NURBS.index + (depth)],	// centre
							
						//0,1,0);
							upX, upYours,upZ); // positive Y up vector
/*	
	if (flyMode == 0)
		gluLookAt(64.0,20.0,-50.0,
							0.0,0.0,0.0,
							0,1,0
							);		
*/

 	NURBS.index+=SPEED;
 
	if (NURBS.index > NURBS.vertices - SPEED)
		NURBS.index = 0;
	
	}

//////////////////////////////////////////
//
//			void mesaTunnel::drawCircle(float diameter,float offsetX, float offsetY, float offsetZ)
//
/////////////////////////////////////////
void mesaTunnel::drawCircle(float _diameter,float _centreX, float _centreY, float _centreZ)
	{
	int i;
	
	 glPushMatrix();		

//	applyRotation();

//	glRotatef(-90,1.0,0.0,0.0);

	glBegin(GL_LINE_STRIP);
	
	
	for (i = 0;i < 180; i++)
		{
		if (i < 9)	
			glColor4f(0.005, 0.0, 0.0, 0.0);
				else
					glColor4f(1.0, 0.0, 0.0, 0.0);
					
		glVertex3f(_centreX + ((_diameter * 0.5) * cos(2.0 * M_PI * ((float)i / 180.0))),
							 _centreY + ((_diameter * 0.5) * sin(2.0 * M_PI * ((float)i / 180.0))),
							 _centreZ);	
 		}
 	
	glEnd();	
	
 	glPopMatrix();		
	}

///////////////////////////////////////
//
//		void mesaTunnel::applyRotation()
//
///////////////////////////////////////
void mesaTunnel::applyRotation()
	{
	float angleX = 0;
	float angleY = 0;
	float angleZ = 0;
	 
	int nextStep = NURBS.vertices / depth;
	
	if (NURBS.index > NURBS.vertices)
		return;
/* 	 
	// did the NURBS move positive(right) on the x axis?
	if (NURBS.NURBSx[NURBS.index + nextStep] > NURBS.NURBSx[NURBS.index])
		{
		angleX = tan(markEveryZ / (NURBS.NURBSx[NURBS.index + nextStep] - NURBS.NURBSx[NURBS.index])) - 90.0;
		rotateX = 0.5;		
		}

	// did the NURBS move negative(left) on the x axis?
	if (NURBS.NURBSx[NURBS.index + nextStep] < NURBS.NURBSx[NURBS.index])
		{
		angleX = tan(markEveryZ / (NURBS.NURBSx[NURBS.index + nextStep] - NURBS.NURBSx[NURBS.index] )) - 90.0;
		rotateX = -0.5;	
		}
  
 */
	// did the NURBS move positive(up) on the y axis?
	if (NURBS.NURBSy[NURBS.index + nextStep] > NURBS.NURBSy[NURBS.index])
		{
		angleY = 180.0 - tan( (NURBS.NURBSy[NURBS.index + nextStep] - NURBS.NURBSy[NURBS.index]) / markEveryZ ) + 90;
		angleY = -angleY;
		
		rotateY = -1.0;		
		}

	// did the NURBS move negative(down) on the y axis?
	if (NURBS.NURBSy[NURBS.index + nextStep] < NURBS.NURBSy[NURBS.index])
		{
		angleY = 180.0 - tan( (NURBS.NURBSy[NURBS.index] - NURBS.NURBSy[NURBS.index + nextStep] / markEveryZ )) + 90.0;
		angleY = -angleY;

		rotateY = 1.0;		
		}
 
 /*
	//did the NURBS move positive(up) on the z axis? (ITS NEGATIVE!!)
	if (NURBS.NURBSz[NURBS.index + nextStep] < NURBS.NURBSz[NURBS.index])
		{
		angleZ = tan(markEveryZ / (NURBS.NURBSz[NURBS.index] - NURBS.NURBSz[NURBS.index + nextStep])) - 90.0;
		rotateZ = 0.5;		
		}	

	//did the NURBS move negative(down) on the z axis? (ITS NEGATIVE!!)
	if (NURBS.NURBSz[NURBS.index + nextStep] > NURBS.NURBSz[NURBS.index])
		{
		angleZ = tan(markEveryZ / (NURBS.NURBSz[NURBS.index + nextStep] - NURBS.NURBSz[NURBS.index])) - 90.0;
		rotateZ = -0.5;		
		}
	 */
	upX = 0;//cos(angleX);
	upYours = 1;//sin(angleY);
	upZ = 0;//sin(rotateZ);
	

//		glRotatef(angleX,rotateX,0.0,0.0);


 		glRotatef(angleY,rotateY,0.0,0.0);


//		glRotatef(angleZ,0.0,0.0,rotateZ);
		
	}


