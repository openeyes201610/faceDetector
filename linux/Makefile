DST = main
CFLAGS = -O3 -I/usr/include/ -I/usr/local/include/
LIBS = libface_detector.so -lopencv_core -lopencv_imgproc -lopencv_highgui

$(DST):main.cpp 
	g++ $^ -o $@ $(CFLAGS) $(LIBS)

clean: 
	rm $(DST)
	
