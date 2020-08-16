# AI-Adversarial_Search
A c++ implementation of a connect 4 game playing AI using search for move selection

Name: Christian Prather 
CWID: 10711855
Structure: All the code is written in a single max_connect.cpp file with functions to handle the minimax calculation utilizing alpha-beta pruning as well as functions for score calculation and player moves. Some of the code used is from reference file provided however functions such as score calculations were rewritten as the provided example was very incorrect.
Run instructions: This is compiled using cmake any version above 3.11 will be sufficent and should not throw errors
    Linux: 
        1) Enter the build directory
        2) run 'cmake ..' to generate the make files 
        3) run make -j8 (note j8 specifies using 8 threads only allocate as many as your comuter can handle, if unsure just run make with no flag)
        4) An executable called compute_a_posteriori will be generated in the build folder you may now run it with ./maxconnect4 and then add any sequence you want depending on the mode you would like 
            Ex) ./maxconnect4 one-move input1.txt output1.txt 10 
                (arguments: mode, input file, output file, depth to look)
            Ex) maxconnect4 interactive input1.txt computer-next 10
                (arguments: mode, input file, who plays next, depth to look)

        5) In interactive mode human.txt and computer.txt files will be generated in the build folder after every players turn.
        6) In one-move mode the specified output.txt file passed in as an argument will be generated with the computers move.
    Windows:
        I recomend importing the project into clion and compilng that way as the c++ compiler on windows can be tricky
      