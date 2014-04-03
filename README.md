QtTrader
========

Qt Trading software

This project was forked from qtstalker (http://qtstalker.sourceforge.net/) 2013-01-10. This was due to a number of reasons, but first and most important is that I since the summer 2012 have poked around in the sourcecode made a few bug corrections and a few changes to the project without having somewere to commit my work. At the same them I've been waiting for qtstalker to gain some momenum in the development but since nothing major have happend to that project since summer and it not open for collaboration I see no other path then to fork. With a fork I also gain a number of stuff. Most important: I can change to git.

=======

Dependencies

1. Linux
2. Qt 4.5		http://qt-project.org/downloads
3. TA-lib 0.3.0		http://www.ta-lib.org/
4. QWT 6.0.0		http://sourceforge.net/projects/qwt/

Please not that is have been reported that QWT breaks API between minor revisions. Use Exactly 6.0.0 (Installable in Ubuntu 12.10)

For the Nordnet plug-in additional dependencies exist

1. qjson
2. openssl

Both is installable with the synaptic package manager in Ubuntu 12.10

To be able to use the nordnet plugin you will have to get an account at Nordnet. For more info and details see, https://api.test.nordnet.se/projects/api 


=======

Build instructions

To be done!

=======

Run

To be done!

=======

FAQ

Q: I get "qttrader: error while loading shared libraries: libQTTRADER.so.0: cannot open shared object file: No such file or directory" when i try to start qttrader
A: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/

Q: qwt_plot.h is not found when I am building in QtCreator
A: Current project setup does not work in QtCreator out-of-the-box. Under Projects->Build Settings remove the QMake build step and add a custom Process step: ./configure

Q: Debuging in QtCreator creates strange results!
A: Make sure that Working directory is pointing to the latest build. Under Projects->Run Settings add a deploy step "Make install" and change Working directory to "~./qttrader_debug"
