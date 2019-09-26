#include <jack/jack.h>
#include "LEDSignAnalyser.cc"

/////////////////////////
//
// global stuff
//
/////////////////////////
int process(jack_nframes_t _frames, void *_buffer);	
void jackError(const char* msg);
void jackInfoError(const char* msg);
int jackXrun (void* msg);
int error = 0;

jack_client_t *client;
jack_default_audio_sample_t *jinL,*jinR;
jack_default_audio_sample_t *joutL,*joutR;
jack_port_t *inputPorts[2],*outputPorts[2];

// jackTools class def
class jackTools	
	{
	public:
		jackTools(char _clientName[20],uint _bufferFrames,effect *_effectIn);
		~jackTools();
		
		void init();
		
		void jackShutdown();
		int jackProcess(jack_nframes_t _frames);
		int bufferFrames;
	
  	//effect& operator=(effect const&);
	
		effect *effectPointer;
		int inited;

		const char *clientName; // const...fuck off
		const char *serverName;
		const char **ports;
	};

/////////////////////////////
//
// constructor
//
/////////////////////////////
jackTools::jackTools(char _clientName[20],uint _bufferFrames,effect *_effectIn)
	{
	printf("in jackTools constructor\n");	
	inited = 0;
	
	clientName = _clientName;
	bufferFrames = _bufferFrames;
	effectPointer = _effectIn;

	init();
	}
	
///////////////////////////
//
// deconstructor
//
/////////////////////////
jackTools::~jackTools() 
	{
	printf("in jackTools deconstructor\n");	
	}

////////////////////////
//
//	jackInit
//  void jackTools::init(char _clientName[20],uint bufferFrames,effect &effectIn)
//
/////////////////////////
//void jackTools::init()
void jackTools::init()
	{
	int i;

	jack_options_t options;
	jack_status_t status;
	
 	serverName = NULL;
	options = JackNullOption;

	// open a client connection to the JACK server
	client = jack_client_open(clientName, options, &status, serverName);

	if (client == NULL) 
		{
		endwin(); // shutdown ncurses
		
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		
		if (status & JackServerFailed) 
			fprintf (stderr, "Unable to connect to JACK server\n");
		
		exit(1);
		}

	if (status & JackServerStarted) 
		fprintf (stderr, "JACK server started\n");
		
	clientName = jack_get_client_name(client);

	fprintf (stderr, "unique name `%s' assigned\n", clientName);


	// tell the JACK server to call `process()' whenever
	//  there is work to be done.

	//jack_set_process_callback (_client, process, 0);
	jack_set_process_callback (client, process, this);

	jack_set_error_function (jackError);
	jack_set_info_function (jackInfoError);
	jack_set_xrun_callback(client,jackXrun,this);
	
	// tell the JACK server to call `jack_shutdown()' if
	//   it ever shuts down, either entirely, or if it
	//   just decides to stop calling us.
	
	//jack_on_shutdown (_client, jackShutdown, this);
	//jack_on_shutdown (client, jackShutdown, 0);
	
	jack_set_buffer_size(client,bufferFrames); 	

	// create two ports 
  	inputPorts[0] = jack_port_register (client, "inputL", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	inputPorts[1] = jack_port_register (client, "inputR", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
   
 	outputPorts[0] = jack_port_register (client, "outputL",	JACK_DEFAULT_AUDIO_TYPE ,	JackPortIsOutput , 0);
	outputPorts[1] = jack_port_register (client, "outputR",	JACK_DEFAULT_AUDIO_TYPE , 	JackPortIsOutput , 0);

	// Tell the JACK server that we are ready to roll.  Our
	// process() callback will start running now. 
	if (jack_activate (client)) 
		{
		endwin(); // shutdown ncurses
		fprintf (stderr, "cannot activate client");
		exit(1);
		}

	// Connect the ports.  You can't do this before the client is
	// activated, because we can't make connections to clients
	// that aren't running.  Note the confusing (but necessary)
	// orientation of the driver backend ports: playback ports are
	// "input" to the backend, and capture ports are "output" from
	// it.
	
	// get a list of ports we can connect to for inputs 
	ports = jack_get_ports (client, NULL, NULL, 	JackPortIsPhysical  | JackPortIsOutput );

	if (ports == NULL) 
		{
		endwin(); // shutdown ncursesx
		fprintf(stderr, "no physical capture ports\n");
		exit(1);
		}
  
for(i=0;i < sizeof(ports);i++)  {
	printf("list of input ports  %s\n",jack_get_ports(client,NULL,NULL,0)[i]);
  }

  // hook jack up to capture ports
	if (jack_connect (client, ports[0], jack_port_name (inputPorts[0]))) 
		fprintf (stderr, "cannot connect input ports\n");

	if (jack_connect (client, ports[1], jack_port_name (inputPorts[1]))) 
		fprintf (stderr, "cannot connect input ports\n");

	free (ports);
/*
	// hook up baudline out to NURBS in
	printf("hooking up baudline to NURBS....maybe\n\n");

	ports = jack_get_ports (client, NULL, NULL,	 JackPortIsInput);

for(i=0;i < sizeof(ports);i++)  {
	printf("list of output ports %s\n",jack_get_ports(client,NULL,NULL,0)[i]);
  }


	if (jack_connect (client, ports[0], jack_port_name (inputPorts[0]))) 
		fprintf (stderr, "cannot connect input ports\n");

	if (jack_connect (client, ports[1], jack_port_name (inputPorts[1]))) 
		fprintf (stderr, "cannot connect input ports\n");

	if (ports == NULL) 
		{
		endwin(); // shutdown ncurses
		printf("%s\n",ports[2]);
			printf("%s\n", jack_port_name (inputPorts[0]));
	//	fprintf(stderr, "no physical playback ports\n");
		exit(1);
		}

	free (ports);  
	 */
	inited = 1;
	}
//////////////////////////////
//
// int jackTools::jackProcess(jack_nframes_t _frames)	
//
///////////////////////////		
int jackTools::jackProcess(jack_nframes_t _frames)	
	{
	uint f,i,j;
 	
	jinL = (jack_default_audio_sample_t*)jack_port_get_buffer (inputPorts[0], _frames );  
	jinR = (jack_default_audio_sample_t*)jack_port_get_buffer (inputPorts[1], _frames );  

	joutL = (jack_default_audio_sample_t*)jack_port_get_buffer (outputPorts[0], _frames);
	joutR = (jack_default_audio_sample_t*)jack_port_get_buffer (outputPorts[1], _frames);

	effectPointer->tick(jinL,jinR,joutL,joutR);	
	//_effect.newTick(_jinL,_jinR,_joutL,_joutR);	
	
	return 0;
	 
}

//////////////////////////////
//
// jackShutdown
//
/////////////////////////////
void jackTools::jackShutdown()
	{
	printf("Jack is closing...\n");
	 
	jack_deactivate(client);	
	jack_client_close(client);
	}
 
//////////////////////////
//
// process callback to the our  callback jackProcess
//
//////////////////////////
int process(jack_nframes_t _frames, void *_buffer)
	{

	return static_cast<jackTools*>(_buffer)->jackProcess(_frames);
	}

//////////////////////////////////////
//
// void jackError (const char* msg)
//
///////////////////////////////////////
void jackError (const char* msg)
	{
	printf("void jackError (const char* msg) Jack is freaking out.....\n%s\n",msg);
	error = 1;
	}

//////////////////////////////////////
//
// void jackInfoError (const char* msg)
//
///////////////////////////////////////
void jackInfoError (const char* msg)
	{
	printf("void jackInfoError (const char* msg) Jack is freaking out.....\n%s\n",msg);
	error = 1;
	}

//////////////////////////////////////
//
// void jackInfoError (const char* msg)
//
///////////////////////////////////////
int jackXrun (void * msg)
	{
	printf("xrun detected - %s\r",msg);
	error = 1;

	return 0;
	}

