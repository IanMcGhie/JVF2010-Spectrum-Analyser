all: nurbs tinySplineTest

nurbs: nurbs.cc \
				classes/LEDSignAnalyser.cc \
				classes/nCurses.cc \
				classes/jackTools.cc \
				classes/sign.cc \
				classes/mesa/mesaChars.cc \
				classes/peaks.cc \
				classes/dBMeter.cc \
				classes/cursor.cc \
				classes/bigPeaks.cc \
				classes/spectroGram.cc \
				classes/oscilloscope.cc \
				classes/mesa/mesaTools.cc \
				classes/tinyspline.h \
				classes/mesa/mesaSpectrogram.cc \
				classes/mesa/mesaNURBS.cc \
				classes/midiClient.cc \
				classes/mesa/mesaTunnel.cc	\
				classes/mesa/mesaMesh.cc \
				classes/mesa/mesaHistogram.cc
			
		g++ nurbs.cc -lfreetype -lOGLFT -ltinyspline -lGL -lOSMesa -Iglut -lGLU -lform -lpanel -lmenu -lncurses -lfftw3 -lm -ljack -g -o nurbs -I/usr/local/include -I/usr/include/freetype2

tinySplineTest: tinySplineTest.cc \
								classes/tinyspline.h

		g++ tinySplineTest.cc -ltinyspline -lglut -lGL  -lGLU -lm -g -o tinySplineTest -I/usr/local/include 
