#include <iostream> 
#include <fstream> 
#include <cstdio>
#include "dataStructure.h"
#include "FindIE.h"
#include "FindTE.h"
#include <time.h>
#include "inputCameraData_generated.h"
#include <thread>
  
using namespace std; 
using namespace Camera::Data;
flatbuffers::FlatBufferBuilder builder;

int main() 
{ 
    clock_t begin = clock();

    int multiTriggerWindow = 20000;
    int sensorDim[2] = {260, 346}; 

    values * valueArray;
    values * inceptiveEvents;
    values * trailingEvents;

    valueArray =  new (nothrow) values[sensorDim[0] * sensorDim[1]];
    inceptiveEvents =  new (nothrow) values[sensorDim[0] * sensorDim[1]];
    trailingEvents =  new (nothrow) values[sensorDim[0] * sensorDim[1]];

//import data begin
    clock_t importBegin = clock();

    ifstream infile;
    infile.open("inputCameraData.bin", ios::binary);
    infile.seekg(0, ios::end);
    int length = infile.tellg();
    infile.seekg(0, ios::beg);
    char *theInputData = new char[length];
    infile.read(theInputData, length);
    infile.close();

    auto receivedInputData = GetDataCollection(theInputData);
    auto importDataVector = receivedInputData->collectedData();

    dataPoint * totalDataPointsExport;
    totalDataPointsExport = new (nothrow) dataPoint[importDataVector->size()];


    //imports the data into a sorted array of structs
    for(unsigned int i = 0; i < importDataVector->size(); i++)
    {
        fillTimeArray(&(valueArray[(importDataVector->Get(i)->x() - 1) * sensorDim[0] + (importDataVector->Get(i)->y() - 1)]), importDataVector->Get(i)->time(), importDataVector->Get(i)->polarity());
    }


    clock_t importEnd = clock();
    double time_import_spent = (double)(importEnd - importBegin) / CLOCKS_PER_SEC;
    printf("%f seconds of runtime for importation \n", time_import_spent);

//import data end

    clock_t findEvents = clock();

    //thread to call a function to copy the data from the buffer into a format that can
    //be saved again on the output buffer
    //thread t1(convertOriginalData, importDataVector, totalDataPointsExport);
    //t1.join();

    //loop to find all the inceptive and trailing events
    for(int i = 0; i < (sensorDim[0] * sensorDim[1]); i++)
    {
        findIE(&inceptiveEvents[i], &valueArray[i], multiTriggerWindow);
        findTE(&trailingEvents[i], &valueArray[i], multiTriggerWindow);

        //loop to find how many trailing events each inceptive event has
        int k = 0;
        for(int j = 0; j < 9; j++)
        {
            if(inceptiveEvents[i].t[k] == 0 || trailingEvents[i].t[j] == 0)
            {
                j = 15;
            }
            else if (trailingEvents[i].t[j] > inceptiveEvents[i].t[k + 1])
            {
                k++;
                j--;
            }
            else if(trailingEvents[i].t[j] > inceptiveEvents[i].t[k])
            {
                inceptiveEvents[i].trailingNum[k] = inceptiveEvents[i].trailingNum[k] + 1;
            }
        }
    }
    clock_t findEventsEnd = clock();
    double time_clock_spent = (double)(findEventsEnd - findEvents) / CLOCKS_PER_SEC;
    printf("%f seconds of runtime for processing \n", time_clock_spent);

//import timer
    clock_t exportBegin = clock();

    inceptiveEventsBuffer * inceptiveEventsArray;
    inceptiveEventsArray = new (nothrow) inceptiveEventsBuffer[200000];
    int inceptiveCounter = 0;

    trailingEventsBuffer * trailingEventsArray;
    trailingEventsArray = new (nothrow) trailingEventsBuffer[200000];
    int trailingCounter = 0;

    //save data from inceptive events and trailing events arrays into vectors of structs
    //that can be received by the buffer
    for(int i = 0; i < (sensorDim[0] * sensorDim[1]); i++)
    {
        for(int j = 0; j < 10; j++)
        {
            
            if(inceptiveEvents[i].t[j] != 0)
            {
                inceptiveEventsArray[inceptiveCounter] = inceptiveEventsBuffer(((i / sensorDim[0]) + 1), 
                ((i % sensorDim[0]) + 1), inceptiveEvents[i].t[j], inceptiveEvents[i].trailingNum[j]);
                inceptiveCounter++;
            }
            if(trailingEvents[i].t[j] != 0)
            {
                trailingEventsArray[trailingCounter] = trailingEventsBuffer(((i / sensorDim[0]) + 1), ((i % sensorDim[0]) + 1), trailingEvents[i].t[j]);
                trailingCounter++;
            }
            if((inceptiveEvents[i].t[j] == 0) && (trailingEvents[i].t[j] == 0))
            {
                j = 11;
            }
            
        }
    }

    convertOriginalData( importDataVector, totalDataPointsExport);

    //assemble the vectors that go into the root table
    auto totalInceptiveEvents = builder.CreateVectorOfStructs(inceptiveEventsArray, inceptiveCounter);
    auto totalTrailingEvents = builder.CreateVectorOfStructs(trailingEventsArray, trailingCounter);
    auto messyDataExport = builder.CreateVectorOfStructs(totalDataPointsExport, importDataVector->size());

    //create the buffer object
    auto totalMess = CreateDataCollection(builder, messyDataExport, totalInceptiveEvents, totalTrailingEvents);

    builder.Finish(totalMess);

    //save to outputCamerData.bin
    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();
    ofstream ofile("outputCameraData.bin", ios::binary);
    ofile.write((char *)buf, size);
    ofile.close();


    clock_t exportEnd = clock();
    double time_clock_spent_export = (double)(exportEnd - exportBegin) / CLOCKS_PER_SEC;
    printf("%f seconds of runtime for exporting \n", time_clock_spent_export);


    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f seconds of runtime\n", time_spent);

    return 0; 
} 