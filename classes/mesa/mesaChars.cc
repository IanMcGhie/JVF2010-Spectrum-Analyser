#define PT 1
#define STROKE 2
#define END 3


		typedef struct charpoint 
			{
  		GLfloat   x, y;
  		int    type;
			} charPointer;

	charPointer char0[] = 
		{
  		{ 0, 0, PT}, {0, 1, PT}, {1, 1, PT}, {1, 0, PT}, 
  		{0, 0, END}
		};

	charPointer char1[] = 
		{
  		{0.5,0,PT }, {0.5, 1, END}
		};

	charPointer char2[] = 
		{
  		{1,0,PT }, { 0, 0, PT},
  		{0, 0.5, PT}, {1, 0.5, PT},
  		{1, 1, PT} , {0,1,END}
		};

	charPointer char3[] = 
		{
  		{0.0, 0.0, PT}, {1.0, 0.0, STROKE},
  		{0.0, 0.6, PT}, {0.8, 0.6, STROKE},
  		{0.0, 1.0, PT}, {1.0, 1.0, STROKE},
  		{1.0, 0.0, PT}, {1.0, 1.0, END}
		};

	charPointer char4[] = 
		{
  		{1,0,PT }, { 1, 1, STROKE}, 
  		{0, 1, PT}, {0, 0.5, PT}, 
  		{0, 0.5, PT} , {1,0.5,END}
		};
	
	charPointer char5[] = 
		{
  		{0,0,PT }, { 1, 0, STROKE}, 
  		{1, 0, PT}, {1, 0.5, STROKE}, 
  		{1, 0.5, PT} , {0,0.5,STROKE},
  		{0.0, 0.5, PT} , {0,1.0,STROKE},
  		{0, 1, PT} , {1,1,END}
		};

	charPointer char6[] = 
		{
  		{0,0,PT }, { 1, 0, STROKE},
  		{1, 0, PT}, {1, 0.5, STROKE}, 
  		{1, 0.5, PT} , {0,0.5,STROKE} ,
  		{0, 0, PT} , {0,1,END} 
  		
		};
	charPointer char7[] = 
		{
  		{0,1,PT }, { 1, 1, STROKE}, 
  		{1, 1, PT}, {0, 0, END}

		};
	charPointer char8[] = 
		{
  		{0,0,PT }, { 1, 0, STROKE},
  		{1, 0, PT}, {1, 1, STROKE},
  		{1, 1, PT} , {0,1, STROKE},
  		{0, 1, PT} , {0,0, STROKE},
  		{0, 0.5, PT} , {1,0.5, END}
		};
	charPointer char9[] = 
		{
  		{1,0,PT }, { 1, 1, STROKE}, 
  		{1, 1, PT}, {0, 1, STROKE},
  		{0, 1, PT} , {0,0.5,STROKE},
  		{0, 0.5, PT} , {1,0.5, END}
		};
 
	charPointer charA[] = 
		{
  		{0, 0, PT}, {0, 1, PT}, 
  		{0.5, 1, PT}, {1, 1, PT}, 
   		{1, 0.75, PT}, {1, 0, STROKE}, 
   		{0, 0.75, PT}, {1, 0.75, END}
		};

	charPointer charB[] = 
		{
  		{0, 0, PT}, {0, 1, STROKE}, 
  		{0, 1, PT}, {0.75, 1, STROKE}, 
   		{0.75, 1, PT}, {1,.65, STROKE}, 
   		{1, 0.65, PT}, {0, 0.65, STROKE},
   		{0.0, 0.0, PT}, {0.75,0, STROKE}, 
   		{0.75, 0, PT}, {1.0,.65, END}
		};
	charPointer charC[] = 
		{
  		{0, 0, PT}, {0, 1, STROKE}, 
  		{0, 1, PT}, {1, 1, STROKE}, 
   		{0, 0, PT}, {1, 0, END}

		};
	charPointer charD[] = 
		{
  		{0, 0, PT}, {0.75, 0.0, STROKE}, 
  		{0.75, 0.0, PT}, {1.0, 0.25, STROKE}, 
   		{1.0, 0.25, PT}, {1.0, 0.75, STROKE}, 
   		{1.0,0.75 ,PT}, {0.75, 1, STROKE},
   		{0.75, 1, PT}, {0, 1, STROKE},
   		{0, 1, PT}, {0, 0, END}
   		
		};

	charPointer charE[] = 
	{
  	{0.0, 0, PT}, {0, 1.0, STROKE},
    {0, 1.0, PT}, {1.0, 1.0, STROKE},
    {0, 0.5, PT}, {1.0, 0.5, STROKE},
    {0, 0.0, PT}, {1.0, 0.0, END}

	};
		
	charPointer charF[] = 
		{
  		{0, 0, PT}, {0, 1, PT}, 
  		{0.5, 1, PT}, {1, 1, PT}, 
   		{1, 0.75, PT}, {1, 0, STROKE}, 
   		{0, 0.75, PT}, {1, 0.75, END}
		};
		
	charPointer charG[] = 
		{
  		{0, 0, PT}, {0, 1, STROKE}, 
  		{0, 1, PT}, {1, 1, STROKE}, 
  		{0, 0, PT}, {1, 0, STROKE}, 
  		{1, 0, PT}, {1.0, 0.5, STROKE}, 
   		{0.7, 0.5, PT}, {1.0, 0.5, END}
		};

	charPointer charH[] = 
		{
  		{0.0, 0.0, PT}, {0.0, 1.0, STROKE}, 
  		{0.0, 0.5, PT}, {1.0, 0.5, STROKE}, 
   		{1.0, 0.0, PT}, {1, 1, END} 

		};

	charPointer charI[] = 
		{
		{0,1,PT }, {1, 1, STROKE},
		{0.5,0,PT }, {0.5, 1, STROKE},
		{0,0,PT }, {1, 0, END}
		};

	charPointer charK[] = 
		{
		{0,0,PT }, {0, 1, STROKE},
		{0,0.5,PT }, {1, 1, STROKE},
		{0,0.5,PT }, {1, 0, END}
		};


	charPointer charN[] = 
		{
		{0,0,PT }, {0, 1, STROKE},
		{0.0,1,PT }, {1, 1, STROKE},
		{1,0,PT }, {1, 1, END}
		};

	charPointer charP[] = 
		{
  		{0, 0, PT}, {0, 1, PT},  
  		{0.4, 1.0, PT}, {0.5, 0.9, PT}, {0.5, 0.6, PT}, 
  		{0.4, 0.5, PT}, {0, 0.5, END}
		};

	charPointer charR[] = 
		{
  		{0, 0, PT}, {0, 10, PT},  {4, 10, PT}, {5, 9, PT}, {5, 6, PT}, 
  		{4, 5, PT}, {0, 5, STROKE}, {3, 5, PT}, {5, 0, END}
		};
/*

{0, 1, PT}, {1, 0, PT}, 
{4, 0, PT}, {5, 1, PT}, 
{5, 4, PT},{4, 5, PT}, 

{1, 5, PT}, {0, 6, PT}, 
{0, 9, PT}, {1, 10, PT},    
{4, 10, PT}, {5, 9, END}   

*/
	charPointer charS[] = 
		{
  		{0, 0.1, PT}, {0.1, 0, PT},
  		{0.4, 0, PT}, {0.5, 0.4, PT},
  		{0.5, 0.4, PT},	{0.4, 0.5, PT}, 
  		{0.1, 0.5, PT}, {0, 0.6, PT}, 
  		{0, 0.9, PT}, {0.1, 1, PT}, 
  		{0.625, 1, PT}, {0.5,0.9, END}
		};

	charPointer charT[] = 
		{
		{0,1,PT }, {1, 1, STROKE},
		{0.5,1,PT }, {0.5, 0, END},
		};

	charPointer charV[] = 
		{
		{0,1,PT }, {0.5, 0.0, STROKE},
		{0.5,0.0,PT }, {1.0, 1.0, END},
		};

	charPointer charX[] = 
		{
  		{0, 0, PT}, {1, 1, STROKE},  
  		{0, 1, PT}, {1, 0, END}
		};

	charPointer charY[] = 
		{
  		{0.5, 0, PT}, {0.5, 0.5, STROKE},  
  		{0, 1, PT}, {0.5, 0.5, STROKE},
  		{1, 1, PT}, {0.5, 0.5, END}
		};

	charPointer charb[] = 
		{
  		{0.0, 0.0, PT}, {0.5, 0.0, STROKE},  
  		{0.5, 0.0, PT}, {0.5, 0.5, STROKE},
  		{0.5, 0.5, PT}, {0.0, 0.5, STROKE},
  		{0.0, 1.0, PT}, {0.0, 0.0, END}
  		
		};
/*
	charPointer charA[] = 
		{
  		{0, 0, PT}, {0, 1, PT}, 
  		{0.5, 1, PT}, {1, 1, PT}, 
   		{1, 0.75, PT}, {1, 0, STROKE}, 
   		{0, 0.75, PT}, {1, 0.75, END}
		};
*/
	charPointer charAsterisk[] = 
		{
  		{0.5, 0.0, PT}, {0.5, 1.0, STROKE},  
  		{0.0, 0.5, PT}, {1.0, 0.5, STROKE},  
  		{0.0, 0.0, PT}, {1.0, 1.0, END}

		};

	charPointer charNeg[] = 
		{
  		{0.1, 0.5, PT}, {0.9, 0.5, END}

		};

	charPointer charPos[] = 
		{
  		{0.1, 0.5, PT}, {0.9, 0.5, STROKE},  
  		{0.5, 0.9, PT}, {0.5, 0.1, END}
		};

	charPointer charZ[] = 
		{
  		{0.2, 1.0, PT}, {1.0, 1.0, STROKE},  
  		{1.0, 1.0, PT}, {0.0, 0.0, STROKE},
  		{0.0, 0.0, PT}, {1.0, 0.0, END}
		};

	charPointer charz[] = 
		{
  		{0.0, 0.5, PT}, {0.5, 0.5, STROKE},  
  		{0.4, 0.5, PT}, {0.0, 0.0, STROKE},
  		{0.0, 0.0, PT}, {0.5, 0.0, END} // ??? wtf
		};

	charPointer charDot[] = 
		{
  		{0.12, 0, PT}, {0.12, 0.12, STROKE},  
  		{0.12, 0.12, PT}, {0.37, 0.12, STROKE},  
  		{0.37, 0.12, PT}, {0.37, 0.0, STROKE},  
  		{0.37, 0.0, PT}, {0.12, 0.0, END}
		};

	charPointer charCamera[] = 
		{
  	// lens
  	 {0.3, 1.0, PT}, {0.5, 0.8, STROKE},  
  	 {0.3, 0.6, PT}, {0.5, 0.8, STROKE},  
  	 {0.3, 1.0, PT}, {0.3, 0.6, STROKE},  

//  		{0.2, 0.6, PT}, {0.6, 0.6, STROKE},  
  	//camera	
  		{0.5, 1.0, PT}, {1.0, 1.0, STROKE},  
  		{1.0, 1.0, PT}, {1.0, 0.4, STROKE},  
  		{1.0, 0.4, PT}, {0.5, 0.4, STROKE},
  		{0.5, 0.4, PT}, {0.5, 1.0, STROKE},
  	// stand
  		{0.5, 0.0, PT}, {0.7, 0.4, STROKE},
  		{1.0, 0.0, PT}, {0.7, 0.4, END}

		};

