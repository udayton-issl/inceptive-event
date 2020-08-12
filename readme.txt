The software we share in this GitHub is copyrighted by the University of Dayton and Intelligent Signal Systems Laboratory (PI Keigo Hirakawa). We do not guarantee the code's accuracy. Acknowledgments must be made for the use of our codes in your work or publications. Both of the following citations should be listed:

- R. Wes Baldwin, Mohammed Almatrafi, Jason R. Kaufman, Vijayan Asari, and Keigo Hirakawa. "Inceptive Event Time-Surfaces for Object Classification Using Neuromorphic Cameras." International Conference on Image Analysis and Recognition. Springer, Cham, 2019.
https://link.springer.com/chapter/10.1007%2F978-3-030-27272-2_35

- Ryan Goettemoeller, R. Wes Baldwin, Keigo Hirakawa, “Inceptive Event Filtering,” C++ software (Version 1.0), available from https://github.com/rgoettemoeller/Inceptive-Event-Filtering, 2020.


The “inceptive event filtering” is a data reduction technique for neuromorphic cameras. The events are labeled as “inceptive” (IE), “trailing” (TE), and “noise” (NE). IE events mark the moment an edge arrives at a particular pixel, and represents the edge shapes and timing most faithfully. Inceptive events are typically less than 20% of overall neuromorphic camera’s output, meaning there is 5x reduction of data. TE events are events following the IE events, and they are proportional in number to the log edge magnitudes. Since there is a lag associated with TE events, it is less reliable way to represent the object shapes (when compared to IE events). Thus we found that the “count” of trailing events (rather than timing) is most meaningful use of TE.
This software is designed to take event sequences from a neuromorphic camera, and output IE events only, or IE events plus trailing event counts (but not their timing). 



To compile the main program, do
g++ -o main main.cpp dataStructure.cpp FindIE.cpp FindTE.cpp
To import the data, we use flatbuffer. Compile and run fillBuffer.cpp (may need flatc installed). We also output data using flatbuffer.
To test the output of the program that saves the data into a csv, compile and run testExportData.cpp.

Developed and tested on Ubuntu. For questions, refer to issl.udayton.edu and https://link.springer.com/chapter/10.1007%2F978-3-030-27272-2_35
