/*
Is it possible/safe/sane to pass around a function pointer to a 
static function?

Let�s say I only want to expose a function from one of my files by 
passing out a function pointer to that function. 
Is it safe to declare that function as static? 
Are compilers allowed to do any judo that would invalidate my
function pointer, or make it meaningless outside the context of that
file, since the function is declared as specific to that file?

Not my code, but a (silly) example of what I mean:

void    static   cool_function(void);
void    extern (*cool_function_ptr)(void); // Actually, I�m not sure of where the `extern` goes in a function-
                                           // pointer declaration. Damn you, confusing function-pointer syntax!
                                           
http://stackoverflow.com/questions/5138330/is-it-possible-safe-sane-to-pass-around-a-function-pointer-to-a-static-function                                           


classes/nCurses.cc:288:11: error: invalid conversion from 
					'void (* (*)(int))(int)' to 
					'functionPointer {aka void (*)(int)}' 
					
					[-fpermissive]


I think what this says is ...

"Hey man...you tried to convert a pointer 
to a function pointer that returns an int and takes void
to a function pointer that returns void and takes an int ...!"

i dunno

"Damn you, confusing function-pointer syntax!"

but 

according to cdecl.org -> BEST WEBSITE EVER

void (* (*fp)(int))(int) means

declare fp as pointer to function (int) returning a
pointer to function (int) returning void

best error ever.....damn you cunfusing function pointer sytanx!

wait ....heres another one

void (*& activeCTLPointCallback)(int)

means

declare activeCTLPointCallback as reference to a
pointer to a function (int) returning void 

Warning: Unsupported in C -- 'reference'

void (* (*fp)(float))(int) =
declare fp as pointer to a function (float) returning a 
pointer to function (int) returning void

and the answer is yes....you can

damn you cunfusing function pointer sytanx!

imm dec 26/15
					
   _testFP = activeCTLPointCallback;

*/

#include <ncurses.h>
#include <menu.h>
#include <panel.h>
#include <form.h>
#include <string.h> 						// strlen,strcpy

#define MENUDEBUG 1

#define MAXPANELS 30				// max number of menus / panels / windows
#define MAXMENUITEMS 75			// max # of choices in a menu
#define INITALWINDOWHEIGHT 13
#define MAXMENUSHOWING INITALWINDOWHEIGHT - 1 	// max # of menu options showing on the screen
#define INITALWINDOWWIDTH 40
#define MAXWINDOWHEIGHT 20  
#define MAXTITLELENGTH 50
#define MAXPRINTFORMATS 20

#define MAINMENU 0
#define SUBMENU1 1
#define SUBMENU2 2
#define SUBMENU3 3
#define SUBMENU4 4
#define SUBMENU5 5
#define SUBMENU6 6
#define SUBMENU7 7
#define SUBMENU8 8
#define SUBMENU9 9
#define SUBMENU10 10
#define SUBMENU11 11
#define SUBMENU12 12
#define SUBMENU13 13
#define SUBMENU14 14
#define SUBMENU15 15
#define SUBMENU16 16
#define SUBMENU17 17
#define SUBMENU18 18
#define SUBMENU19 19
#define SUBMENU20 20
#define SUBMENU21 21
#define SUBMENU22 22
#define SUBMENU23 23
#define SUBMENU24 24
#define SUBMENU25 25 
#define SUBMENU26 26
#define SUBMENU27 27
#define SUBMENU28 28
#define SUBMENU29 29
#define SUBMENU30 30

char *WHOLENUMBER[]					 = { (char*)"%2.0f     " };
char *TENTHS[] 				 			 = { (char*)"%0.1f     " };
char *HUNDREDTHS[] 			 		 = { (char*)"%0.2f     " };
char *THOUSANDTHS[] 		 		 = { (char*)"%0.3f     " };
char *TENTHOUSANDTHS[] 		 	 = { (char*)"%0.4f     " };
char *HUNDREDEDTHOUSANDTHS[] = { (char*)"%0.5f     " };
char *HIDE[]				 				 = { (char*)"HIDE" 			 }; 
char *ONOFF[]				 				 = { (char*)"Off"	 , (char*)"On" };
char *TEXTFIELD[]			 			 = { (char*)"TEXTFIELD" }; 
char *IPADDRESS[]			 			 = { (char*)"IPADDRESS" }; 
char *NUMERIC[]			 	 			 = { (char*)"NUMERIC" }; 

class nCurses
	{
	public:
	
		nCurses(char *title);
		~nCurses();

 //	nCurses& operator=(const nCurses&);

	 	
	 	void addMenuItem(int _panelID,char *_menuItem,float _value,
	 									float _min,float max,float _decBy,float _incBy,
	 									char* _units,char **_printFormat, ...);

	  void addNote(int _panel,char *_menuNote);		
	  void editTextField();
	  int keyCheck();
		void showPanels();	
		void setMenuItemValue(int _menuID,int _menuItemID,float _value);		
		float getMenuItemValue(int _menuID,int _menuItemID);
		void flash(int _menu,int _menuItem,int _flashOn,char *_text);
		
		WINDOW *menuWindow[MAXPANELS];
		
		int getCurrentPanelID();
		int getMenuRow(int _panel);
		int	quit;

		// define functionPointer as a 
		// new type... that is a pointer to functions that take an int 
		// and returns void
		typedef void(*functionPointer)(int);  

		// this is a two dimensional array of function pointers
		// to functions that take an int...and return void ... no really...
		// check out the typedef
	  functionPointer menuItemCallbackFP[MAXPANELS][MAXMENUITEMS]; 
	 
	private:
		void centreText(WINDOW *_win, int _starty, int _startx, int _width, char *_string, chtype _color);
		void incSetting(int _menuRow,int _mult);
	  void decSetting(int _menuRow,int _mult);			
		void drawScrollBar();
		int getParentPanelID(int _panelID);
		 
		PANEL  *panel[MAXPANELS];
		MENU   *menu[MAXPANELS];
		ITEM   **menuItems[MAXPANELS][MAXMENUITEMS];
  	char   *menuTitle[MAXPANELS];
		char   *units[MAXPANELS][MAXMENUITEMS];
		char   *printFormat[MAXPANELS][MAXMENUITEMS][MAXPRINTFORMATS];
		char   *myNote[MAXPANELS];
		char	 *callback[MAXPANELS][MAXMENUITEMS];
		int    keyPressed;
		int    currentPanel;
		int    menuRow[MAXPANELS];			
		int    choices[MAXPANELS];
		int    width[MAXPANELS];
		int    height[MAXPANELS];
  	int    subMenu[MAXPANELS][MAXMENUITEMS];
		int    panelID[MAXPANELS];
		int    firstMenuItem[MAXMENUITEMS];
		int    currentSelection[MAXMENUITEMS];
		float  value[MAXPANELS][MAXMENUITEMS];
		float  min[MAXPANELS][MAXMENUITEMS], max[MAXPANELS][MAXMENUITEMS];
		float  decBy[MAXPANELS][MAXMENUITEMS], incBy[MAXPANELS][MAXMENUITEMS];
	};
	
//////////////////////////////////////
//
//	constructor
//
///////////////////////////////////////////	
nCurses::nCurses(char *_title)
	{
//#ifdef MENUDEBUG		
//	printf("nCurses constructor title is %s\n",_title);	
//#endif

	int i,j;
 
	// reset panel arrays
	for (i = 0;i < MAXPANELS;i++)
		{
		choices[i] = 0;
		menuRow[i] = 0;
		firstMenuItem[i] = 0;
		currentSelection[i] = 0; 
		width[i]  = INITALWINDOWWIDTH;	  // set initial window width
		height[i] = INITALWINDOWHEIGHT;	// set initial window height
		
		// reset menu item values
		for (j=0;j<MAXMENUITEMS;j++)
			{
			value[i][j] = 0.0;
			menuItemCallbackFP[i][j] = NULL;
			callback[i][j]=(char*)""; // seg faults if these arent intited to ....something
			}
		}
	
	currentPanel = MAINMENU;
	currentPanel = SUBMENU18;
	
	quit = FALSE;

	menuTitle[currentPanel] = _title; 	

	// initialize ncurses
	initscr();
	
	// check for colour
	if(has_colors() == FALSE)
		{	
		endwin();
		printf("Your terminal does not support colour....it's over.\n");
		exit(1);
		}

	// Good, we got colour
	start_color();
	
	// set up some colour pairs
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);	
	init_pair(6, COLOR_GREEN, COLOR_BLACK);	
	
	// allocate memory for main menu
	menuWindow[0] = newwin(1,1, 1, 1);
	panel[0] = new_panel(menuWindow[0]);	 
	menu[0] = new_menu((ITEM**)menuItems[0]);

	// i dont know what this does
	cbreak();
	
	// make sure typed chars do not show on screen
	noecho();
	}

//////////////////////////////////////
//
//	constructor (de)
//
///////////////////////////////////////////	
nCurses::~nCurses()
	{
	#ifdef MENUDEBUG
		printf("\n** in nCurses deconstructor\n");
		exit(1);
	#endif	
	}

/////////////////////////////////
//
// addMenuItem(int panelID,char *menuItem,float start,float min,float max,float decBy,float incBy,char* units,char *printFormat, ...)
//
/////////////////////////////////
void nCurses::addMenuItem(int _panelID,char *_menuItemName,float _value,float _min,float _max,float _decBy,float _incBy,char* _units,char **_printFormat, ...) 
	{
	int startX,startY,i,maxX;
	
	va_list varArgs;	// where we are gunna hold the variable args (...) (in the function call)

	int menuSub = 0;  // varname of place we want vararg to go

	if ((_value == 0.0) && (_min 	== 0.0) && (_max == 0.0) &&
			(_incBy == 0.0) && (_decBy == 0.0))
		{
	  va_start(varArgs, _printFormat); 	// .. line up for the arg name BEFORE the ... ?
	  menuSub = va_arg(varArgs,int);  	// ok...assign it
		va_end(varArgs); 									// done with var args
		menuTitle[menuSub] = _menuItemName;	
		}	
			
	if (strcmp(_units,"CALLBACK") == 0)	
		 {
		  va_start(varArgs, _printFormat); 	// .. line up for the arg name BEFORE the ... ?
		  menuItemCallbackFP[_panelID][choices[_panelID]] = va_arg(varArgs,functionPointer);  // ok...assign it
		  va_end(varArgs); 											// done with var arg
 		  callback[_panelID][choices[_panelID]] = (char*)"CALLBACK"; // set callback flag
			}
					
	// make sure we dont go out of bounds
	if (choices[_panelID] > MAXMENUITEMS)
		{
		endwin();
		printf("nCurses line 318 ish ....Make MAXMENUITEMS at least %d in nCurses.cc...Thank you.\n",choices[_panelID] + 1);
		exit(0);
		}

	// store menuitem info
	panelID[_panelID] 								 = _panelID;
	value[_panelID][choices[_panelID]] = _value;
	min[_panelID][choices[_panelID]]	 = _min;
	max[_panelID][choices[_panelID]]	 = _max;
	decBy[_panelID][choices[_panelID]] = _decBy;
	incBy[_panelID][choices[_panelID]] = _incBy;
	units[_panelID][choices[_panelID]] = _units;

	maxX = 0;
	
	for (i = 0 ; i < MAXPRINTFORMATS ;i++)
		printFormat[_panelID][choices[_panelID]][i] = _printFormat[i];

	// hack
	// make gl blend menu & spectrogram & NURBS props window really wide
	if ((_panelID == 19) || 
			(_panelID == 13) ||
			(_panelID == 22) ||
			(_panelID == 23))		
				width[_panelID] = 70;
		
	if (_panelID == 28) 	
		width[_panelID] = 50;
		
	subMenu[_panelID][choices[_panelID]] = menuSub;

	// free memory from old panel , and menu 	
 	unpost_menu(menu[_panelID]);
 	free_menu(menu[_panelID]);
 	del_panel(panel[_panelID]);

	// Create the window to be associated with the menu 			
	startY = (LINES - height[_panelID]) / 2;	// Calculating for a center placement 
	startX = (COLS - width[_panelID]) / 2;	// of the window		

	menuWindow[_panelID] = newwin(height[_panelID],width[_panelID], startY , startX);
		
	// create panel & menu & menu items
	panel[_panelID] = new_panel(menuWindow[_panelID]);	 
  menuItems[_panelID][choices[_panelID]] = (ITEM**)new_item(_menuItemName, (char*)"");
	menu[_panelID] = new_menu((ITEM**)menuItems[_panelID]);

	// Set main window and sub window 
	set_menu_win(menu[_panelID], menuWindow[_panelID]);
 	set_menu_sub(menu[_panelID], derwin(menuWindow[_panelID], 0, 0, 3, 1));

	set_menu_format(menu[_panelID], MAXMENUSHOWING , 1); // menu window attribs
	set_menu_mark(menu[_panelID], " * ");		

	post_menu(menu[_panelID]);		
	
	choices[_panelID]++;
	
	if (height[_panelID] < MAXWINDOWHEIGHT)
		height[_panelID]++;		
	}

//////////////////////////
//
//	void nCurses::flash(int _menu,int _menuItem,int _flashOn)
//
/////////////////////////
void nCurses::flash(int _menu,int _menuItem,int _flashOn,char *_text)
	{
	int line;
	
	line = _menuItem + 3;

	if (_flashOn == 1)
		{		
		wattron(menuWindow[_menu],A_REVERSE);
		mvwprintw(menuWindow[_menu],line,33,"%s",_text);
		wattroff(menuWindow[_menu],A_REVERSE);
		} else {
				//		wattron(menuWindow[_menu],A_REVERSE);
						mvwprintw(menuWindow[_menu],line,33,"%s",_text);
					//	wattroff(menuWindow[_menu],A_REVERSE);
			
						}
//	wrefresh(menuWindow[_menu]);	
	}

///////////////////////////////////////////
//
// void nCurses::keyCheck()
//
//////////////////////////////////////////////
int nCurses::keyCheck()
	{
	int i,change = 0;
	
	keyPressed = wgetch(menuWindow[currentPanel]);
			
	switch (keyPressed)
		{
		case KEY_UP:

	  	if (menuRow[currentPanel] > 0)
	  		{
	  		// move to next selected item							 
				menu_driver(menu[currentPanel], REQ_UP_ITEM);	
			
				// and keep track of it		
				menuRow[currentPanel]--; // actual menu item index (0 to MAXMENUITEMS)

				// if we are at the top of menu...and heading up....scroll menu
				if (firstMenuItem[currentPanel] > 0 && currentSelection[currentPanel] == 0)
					firstMenuItem[currentPanel]--;
					else
						currentSelection[currentPanel]--;	// or just move selection  (0 to MAXMENUSHOWING)
				
				change++;
				}
		break;	

		case KEY_DOWN:
				// are there more items to scroll?
				if (menuRow[currentPanel] < choices[currentPanel] - 1)
					{
					// move to next selected item							 
					menu_driver(menu[currentPanel], REQ_DOWN_ITEM);	

					// and keep track of it
					menuRow[currentPanel]++;	// actual menu item index (0 to MAXMENUITEMS)
					currentSelection[currentPanel]++; // item selected on screen index (0 to MAXMENUSHOWING)
					
					// if we are at the bottom of menu (on screen) ...scroll to next item
					if (currentSelection[currentPanel] == MAXMENUSHOWING)
						{
						currentSelection[currentPanel]--;
						firstMenuItem[currentPanel]++;
						}
							
					change++;
					}
		break;	

		case 67: // SHIFT RIGHT
			incSetting(menuRow[currentPanel],5);
		 	change++;
		break;	

		case 68:  // SHIFT LEFT
			decSetting(menuRow[currentPanel],5);
		 	change++;
		break;	
	
		case KEY_LEFT:
			decSetting(menuRow[currentPanel],1);
			change++;	
		break;	

		case KEY_RIGHT:
			incSetting(menuRow[currentPanel],1);
			change++;	
		break;	

		case 82: case 114: // R / r
	 		if (currentPanel > 0)
				{
				currentPanel = getParentPanelID(currentPanel);
				change++;
				}
		break;

		case 10: // Enter
			if ((value[currentPanel][menuRow[currentPanel]] == 0.0) &&
					(min[currentPanel][menuRow[currentPanel]] 	== 0.0) &&
					(max[currentPanel][menuRow[currentPanel]] 	== 0.0) &&
					(incBy[currentPanel][menuRow[currentPanel]] == 0.0) &&
					(decBy[currentPanel][menuRow[currentPanel]] == 0.0))
						{
						currentPanel = subMenu[currentPanel][menuRow[currentPanel]];
			 			change++;			
						}

			// edit text field
			if ((strcmp((char*)printFormat[currentPanel][menuRow[currentPanel]][0],TEXTFIELD[0]) == 0) ||
				 (strcmp((char*)printFormat[currentPanel][menuRow[currentPanel]][0],IPADDRESS[0]) == 0) ||
				 (strcmp((char*)printFormat[currentPanel][menuRow[currentPanel]][0],NUMERIC[0]) == 0)
				 )
				{
				editTextField();
				}
				
			if (strcmp(callback[currentPanel][menuRow[currentPanel]],(char*)"CALLBACK") == 0)
				(*menuItemCallbackFP[currentPanel][menuRow[currentPanel]])(-1); // <- o my god
		break;
		
		case 113: case 81: // q / Q
			clear();
			endwin();

			// free memory from old panel , and menu 	
		  for (i = 0 ; i < MAXPANELS ; i++)
		  	{
		  	 unpost_menu(menu[i]);
		  	 free_menu(menu[i]);
		 		 del_panel(panel[i]);
		 		}

		 	quit = 1;
		break;
		}		// switch (keyPressed)
  
	if (change == 1)
		showPanels();
	
	return keyPressed;
	}

/////////////////////////////
//
// int nCurses::getParentPanel(int _currentPanel)
//
//////////////////////////////
int nCurses::getParentPanelID(int _panelID)
	{
	int i,j;
	
	// iterate through all the menus
 	for (i = 0;i < MAXPANELS;i++)
 		for (j = 0;j < choices[i];j++)		// through all the menuitems
 			if (_panelID == subMenu[i][j])	// looking for the parent panelID
 				return panelID[i];  					// and return it...
	
	// or crash
	endwin();
	printf("\n\nCouldnt find parent panel ID for panel %d, sorry.\n",panelID);
	exit(0);
	}

/////////////////////////////
//
// int nCurses::getCurrentPanel(int _currentPanel)
//
//////////////////////////////
int nCurses::getCurrentPanelID()
	{
	return panelID[currentPanel];
	}

//////////////////////////////
//
// void nCurses::drawScrollBar()
//
////////////////////////////////
void nCurses::drawScrollBar()
	{
	int i;
	
	// top of scroll bar	
	mvwaddch(menuWindow[currentPanel], 3, width[currentPanel] - 3, ACS_URCORNER);
	mvwaddch(menuWindow[currentPanel], 3, width[currentPanel] - 4, ACS_HLINE);
	mvwaddch(menuWindow[currentPanel], 3, width[currentPanel] - 5, ACS_ULCORNER);

	// bottom of scrollbar
	mvwaddch(menuWindow[currentPanel], 3 + MAXMENUSHOWING, width[currentPanel] - 3, ACS_LRCORNER);
	mvwaddch(menuWindow[currentPanel], 3 + MAXMENUSHOWING, width[currentPanel] - 4, ACS_HLINE);
	mvwaddch(menuWindow[currentPanel], 3 + MAXMENUSHOWING, width[currentPanel] - 5, ACS_LLCORNER);
	
	// draw vert lines of scrollbar
	for (i = 0 ; i < MAXMENUSHOWING - 1 ;i++)
		{
		// left vert
		mvwaddch(menuWindow[currentPanel], 4 + i, width[currentPanel] - 3, ACS_VLINE);
		
		// draw diamond
		if (i == (int)(((menuRow[currentPanel] + 1) / (float)choices[currentPanel] ) * (float)MAXMENUSHOWING) ) 
			mvwaddch(menuWindow[currentPanel], 4 + i, width[currentPanel] - 4, ACS_DIAMOND);
				else
					mvwprintw(menuWindow[currentPanel],4 + i , width[currentPanel] - 4, " ");
		
		// right vert
		mvwaddch(menuWindow[currentPanel], 4 + i, width[currentPanel] - 5, ACS_VLINE);
		} // for (i = 0 ; i < MAXMENUSHOWING - 1 ;i++)
	}

////////////////////////////////
//
// void showPanels();
//
/////////////////////////////////
void nCurses::showPanels()
	{
	int i,j;
	int line = 3; // y screen coord of first menu item
	int menuItem = 0;
	int changed = 0;

	if (width[currentPanel] > COLS)
		{
		endwin();
		printf("Min screen width is %d columns. Make terminal window bigger. NOW!\n",width[currentPanel]);
		exit(0);	
		}
			
	// hook keyboard up to current panel
	for (i = 0;i < MAXPANELS;i++)
		{
		if (i == currentPanel)
			{
			show_panel(panel[i]);
			keypad(menuWindow[i], TRUE);
			nodelay(menuWindow[currentPanel],TRUE); // get kb input waiting for c/r
			//nodelay(_menuWindow[_currentPanel],FALSE); // get kb input without waiting for c/r
			} else {
							hide_panel(panel[i]);	
							keypad(menuWindow[i], FALSE);
				   		}
		}

	// put menuitem values on screen
	for (i = firstMenuItem[currentPanel]; i < firstMenuItem[currentPanel] + MAXMENUSHOWING ;i++)
			{
			changed=0;
	
			// if we go past the array index....get out
			if (i > choices[currentPanel] - 1)
				break;
				
			if (MENUDEBUG)
				mvwprintw(menuWindow[currentPanel],line,1,"%d",i);
	
			// dont show a value
			if (strcmp((char*)printFormat[currentPanel][i][0] ,HIDE[0]) == 0)
				{	
				mvwprintw(menuWindow[currentPanel],line,25,"       "); // clear test in case a bigger string was there and we scrolled
				mvwprintw(menuWindow[currentPanel],line++,32, "%s" , units[currentPanel][i]);
				changed++;
				}
	 
			// show a number
			if ((strcmp(printFormat[currentPanel][i][0] ,WHOLENUMBER[0]) == 0) ||
					(strcmp(printFormat[currentPanel][i][0] ,TENTHS[0]) == 0) ||
					(strcmp(printFormat[currentPanel][i][0] ,HUNDREDTHS[0]) == 0) ||
					(strcmp(printFormat[currentPanel][i][0] ,THOUSANDTHS[0]) == 0) ||
					(strcmp(printFormat[currentPanel][i][0] ,TENTHOUSANDTHS[0]) == 0) ||
					(strcmp(printFormat[currentPanel][i][0] ,HUNDREDEDTHOUSANDTHS[0]) == 0 )
				 )
					{
					mvwprintw(menuWindow[currentPanel],line,26, 
										printFormat[currentPanel][i][0] ,
										value[currentPanel][i]);							
							
					if (strcmp(callback[currentPanel][i],(char*)"CALLBACK") != 0)
									mvwprintw(menuWindow[currentPanel],line++,33, "%s" , units[currentPanel][i]);
										else
											line++;
										
					changed++;
					}
	
			// show text field
			if ((strcmp((char*)printFormat[currentPanel][i][0],TEXTFIELD[0]) == 0) ||
				(strcmp((char*)printFormat[currentPanel][i][0],IPADDRESS[0]) == 0) ||
				(strcmp((char*)printFormat[currentPanel][i][0],NUMERIC[0]) == 0) 
				)
				{
				if (strlen((char*)printFormat[currentPanel][i][0]) > 5)
					{	
					mvwprintw(menuWindow[currentPanel],line++,26, "%0.4s ..." , units[currentPanel][i]);
					} else { 
									mvwprintw(menuWindow[currentPanel],line++,26, "%s     " ,units[currentPanel][i] );
								 } 
				changed++;
				}		
		 
			
			
			// if we havent printed anything yet..
			// it must be an array of things
			// like ON,OFF ect...
			// show a string
			// from the _printFormat array
			// but dont print CALLBACK on the screen
			if (changed == 0)
				if (strcmp((char*)"CALLBACK",units[currentPanel][i]) == 0)
						mvwprintw(menuWindow[currentPanel],line++,26,"%s           ",	
											printFormat[currentPanel][i][(int)value[currentPanel][i]]);
						
						else
							mvwprintw(menuWindow[currentPanel],line++,26,"%s %s     ",	
							printFormat[currentPanel][i][(int)value[currentPanel][i]],
							units[currentPanel][i]);
			} // for (i = _firstMenuItem[_currentPanel];i < _firstMenuItem[_currentPanel] + MAXMENUSHOWING ;i++)

	// show menu note at bottom of screen
	if (myNote[currentPanel] != NULL)
		mvwprintw(menuWindow[currentPanel],height[currentPanel] - 4 , 1, myNote[currentPanel]);

	box(menuWindow[currentPanel], 0, 0);		

	// centre menu title
	centreText(menuWindow[currentPanel], 1, 0, width[currentPanel] , menuTitle[currentPanel], COLOR_PAIR(1));

	// draw horiz line under title
	mvwaddch(menuWindow[currentPanel], 2, 0, ACS_LTEE);
	mvwhline(menuWindow[currentPanel], 2, 1, ACS_HLINE, width[currentPanel]);
	mvwaddch(menuWindow[currentPanel], 2, width[currentPanel] - 1 , ACS_RTEE);
	
	// if there are more items in the menu than can be
	// displayed on the screen.....draw a scrollbar
	if (choices[currentPanel] > MAXMENUSHOWING)
		drawScrollBar();

	if (currentPanel == MAINMENU)
		mvwprintw(menuWindow[currentPanel],height[currentPanel] - 2, 1, "Q - Exit / Enter - Select Menu");
			else
				mvwprintw(menuWindow[currentPanel],height[currentPanel]-2, 1, "Press R to return");
	
 	update_panels();

	// Show it on the screen 
	doupdate();
	}

/////////////////////////////////////
//
// void nCurses::centreText(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
//
// stolen from the ncurses tutorial	
////////////////////////////////////
void nCurses::centreText(WINDOW *_win, int _starty, int _startx, int _width, char *_string, chtype _color)
	{
	int length,temp, x, y;
	
	if (_win == NULL)
		return;
	
	getyx(_win, y, x);
	
	if (_startx != 0)
		x = _startx;
	
	if (_starty != 0)
		y = _starty;
	
	if (_width == 0)
		_width = 80;

	if (_string == NULL)
		return;

	length = strlen(_string);
	temp = (_width - length) / 2;
	x = _startx + temp;
	
	wattron(_win, _color);
	
	if (MENUDEBUG)
		mvwprintw(_win, y, x - 5, "%s (Panel #%d)", _string,currentPanel);
		else
			mvwprintw(_win, y, x, "%s", _string);
	
	wattroff(_win, _color);
	}

//////////////////////////////
//
// incSetting(_menuRow)
//
////////////////////////////
void nCurses::incSetting(int _row,int _mult)
	{
	// if menu item is a submenu...pointing to another memu
	// you cant inc/dec/change the value
	if ((value[currentPanel][_row] == 0.0) &&
			(min[currentPanel][_row] == 0.0) &&
			(max[currentPanel][_row] == 0.0) &&
			(decBy[currentPanel][_row] == 0.0) &&
			(incBy[currentPanel][_row] == 0.0))
				return;
	 
	if ((value[currentPanel][_row] - 1 + (incBy[currentPanel][_row] * _mult)) < max[currentPanel][_row])	
		{
		value[currentPanel][_row] += incBy[currentPanel][_row] * _mult;

		// "just pass the function pointer..." .... man i wanna hit that guy 
		if (strcmp((char*)callback[currentPanel][_row],(char*)"CALLBACK") == 0)
	  	(*menuItemCallbackFP[currentPanel][_row])(value[currentPanel][_row]); // <- o my god
		}
}

//////////////////////////////
//
// decSetting(_menuRow)
//
////////////////////////////
void nCurses::decSetting(int _row,int _mult)
	{
	// if menu item is a submenu...pointing to another memu
	// you dont inc/dec/change the value
	if ((value[currentPanel][_row] == 0.0) &&
			(min[currentPanel][_row] == 0.0) &&
			(max[currentPanel][_row] == 0.0) &&
			(decBy[currentPanel][_row] == 0.0) &&
			(incBy[currentPanel][_row] == 0.0))
				return;
						
	if ((value[currentPanel][_row] - 1 - (decBy[currentPanel][_row] * _mult)) > min[currentPanel][_row] )
		{
		value[currentPanel][_row]	+= decBy[currentPanel][_row] * _mult;	

		if (strcmp((char*)callback[currentPanel][_row],(char*)"CALLBACK") == 0)
	  	(*menuItemCallbackFP[currentPanel][_row])(value[currentPanel][_row]); // <- o my god
		}
	}

/////////////////////////////////
//
// void addNote(char *_menuNote);
//
/////////////////////////////
void nCurses::addNote(int _panel,char *_menuNote)
	{
	myNote[_panel] = _menuNote;
	}

////////////////////////////////
//
//	int nCursese::getMenuRow(int panel)
//
////////////////////////////////	
int nCurses::getMenuRow(int _panel)
	{
	return menuRow[_panel];	
	}

//////////////////////////////////////
//
// void nCurses::getMenuItemValue(int menuID,intmenuItem)
//
///////////////////////////////////// 
float nCurses::getMenuItemValue(int _menuID,int _menuItemID)
	{
	return value[_menuID][_menuItemID];
	}

/////////////////////////////////////////
//
//	nCurses::void setMenuItemValue(int menuID,int menuItemID,float value)
//
///////////////////////////////////////
void nCurses::setMenuItemValue(int _menuID,int _menuItemID,float _value)
	{
	value[_menuID][_menuItemID] = _value;
	}
	
//////////////////////////////////////
//
// 	void nCurses::editTextField()
//
/////////////////////////////////////
void nCurses::editTextField()
	{
	int i,startX,startY,rows,cols,ch;
	int insert = 1;
	WINDOW *editTextWindow;
	PANEL *editTextPanel;

	FIELD *field[1];
	FORM  *form;

	// hook keyboard up to current panel
	for (i=0;i<MAXPANELS;i++)
		{
		hide_panel(panel[i]);	
		keypad(menuWindow[i], FALSE);
		}

	field[0] = new_field(1, INITALWINDOWWIDTH , 2, 0, 0, 0);
	field[1] = NULL;

	set_field_buffer(field[0], 0,(char*)units[currentPanel][menuRow[currentPanel]]);

	// Set field options 
	field_opts_off(field[0], O_BLANK); //  When this option is on, entering a character at the first field position erases the entire field 

	set_field_back(field[0], A_UNDERLINE); 	// Print a line for the option 	
	field_opts_off(field[0], O_AUTOSKIP);  	// Don't go to next field when this 
											 	// Field is filled up 

	if ((strcmp(printFormat[currentPanel][menuRow[currentPanel]][0],TEXTFIELD[0]) == 0) ||
			(strcmp(printFormat[currentPanel][menuRow[currentPanel]][0],IPADDRESS[0]) == 0)  )
			set_field_type(field[0],TYPE_ALPHA);

	// Create the form and post it 
	form = new_form(field);

	// Create the window to be associated with the menu 			
	startY = (LINES - INITALWINDOWHEIGHT) / 2;	// Calculating for a center placement 
	startX = (COLS - INITALWINDOWWIDTH) / 2;	// of the window		

	// Calculate the area required for the form 
	scale_form(form, &rows, &cols);

	editTextWindow = newwin(rows + 6,cols + 4, startY, startX);
	editTextPanel = new_panel(editTextWindow);	 

	// Set main window and sub window 
  set_form_win(form, editTextWindow);
  set_form_sub(form, derwin(editTextWindow, rows, cols, 3, 1));

	box(editTextWindow, 0, 0);	

	centreText(editTextWindow, 1, 0, cols + 4 , (char*)"Edit Text Field", COLOR_PAIR(1));
	
	// draw horiz line under title
	mvwaddch(editTextWindow, 2, 0, ACS_LTEE);
	mvwhline(editTextWindow, 2, 1, ACS_HLINE, INITALWINDOWWIDTH + 3);
	mvwaddch(editTextWindow, 2,INITALWINDOWWIDTH + 3, ACS_RTEE);

	post_form(form);

	mvwprintw(editTextWindow,rows + 5, 2, "Edit text, then hit enter");

	show_panel(editTextPanel);
	
 	update_panels();
	
	// Show it on the screen 
	doupdate();

	//	cbreak();
	
	// make sure typed chars show on screen
	keypad(editTextWindow, TRUE);
	//	nodelay(_editTextWindow,FALSE); // get kb input without waiting for c/r

	//echo();
	
	form_driver(form,REQ_INS_MODE); // turn insert on
	//form_driver(_form,REQ_OVL_MODE); // turn insert off
	form_driver(form,REQ_BEG_FIELD);
	
	while((ch = wgetch(editTextWindow)) != 10)
		{	
		switch(ch)
			{	
			case KEY_LEFT:
				form_driver(form,REQ_PREV_CHAR);
			break;
	
			case KEY_RIGHT:
				form_driver(form,REQ_NEXT_CHAR);
			break;
	
			case 127: // back space
				form_driver(form,REQ_PREV_CHAR);
				form_driver(form,REQ_DEL_CHAR);
			break;
	
			case 330: // delete
				form_driver(form,REQ_DEL_CHAR);
			break;
	
			case KEY_IC:
				insert = -insert;
	
				if (insert == 1)
					form_driver(form,REQ_INS_MODE);	 
						else
							form_driver(form,REQ_OVL_MODE);		
			break;
	
			default:
				// If this is a normal character, it gets printed
				form_driver(form, ch);
			break;
			}
	//mvwprintw(_editTextWindow,_rows + 4,2, "key = %d  ",ch);
	}

	form_driver(form,REQ_VALIDATION);

	units[currentPanel][menuRow[currentPanel]] = field_buffer(field[0], 0);
	
	free_field(field[0]); 
	unpost_form(form);
	
	//free_form(_form);
	
	del_panel(editTextPanel);
	delwin(editTextWindow);
	
	clear();
	cbreak();
	noecho();
	showPanels();
	}


