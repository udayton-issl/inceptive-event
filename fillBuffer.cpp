#include "inputCameraData_generated.h"
#include <cstdio>
#include "dataStructure.h"
#include <iostream>
#include <stdint.h>
#include <fstream>

using namespace Camera::Data;
using namespace std;

int main() {

    int multiTriggerWindow = 20000;
    int sensorDim[2] = {260, 346}; 

    values * valueArray;

    valueArray =  new (nothrow) values[sensorDim[0] * sensorDim[1]];

    values * rawData;
    rawData = new (nothrow) values[400000];

    // Build up a serialized buffer algorithmically:
  flatbuffers::FlatBufferBuilder builder;

  FILE *fp; 
    int count = 0;  // Line counter (result) 
    char filename[] = "sampleDataC.csv";
    int x = 0;
    int y = 0;
    unsigned long long t = 0;
    short p = 0;
    int counter = 0;

    fp = fopen(filename, "r");

    //vector<flatbuffers::Offset<dataPoint>> collectedData_vector;
    //vector<dataPoint> collectedData_vector;
    dataPoint * dataArray;
    dataArray = new (nothrow) dataPoint[400000];


    while(fscanf(fp, "%d,%d,%llu,%hd", &x, &y, &t, &p) == 4)
    {
        //auto pointOfData = dataPoint(x, y, t, p);
        //collectedData_vector.push_back(collectedData_vector);
        dataArray[counter] = dataPoint(x, y, t, p);

        //printf("%d,%d,%llu,%hd\n", x, y, t, p);
        counter++;
    }
//flatbuffers::Offset<flatbuffers::Vector<const Camera::Data::dataPoint *>> collectedData
    auto totalDataPoints = builder.CreateVectorOfStructs(dataArray, counter);

    auto totalMess = CreateDataCollection(builder, totalDataPoints);

    builder.Finish(totalMess);

    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();
    ofstream ofile("inputCameraData.bin", ios::binary);
    ofile.write((char *)buf, size);
    ofile.close();


    dataPoint * dataMessArray;
    dataMessArray = new (nothrow) dataPoint[400000];

    auto dataConglomerate = GetDataCollection(builder.GetBufferPointer());

    auto dates = dataConglomerate->collectedData();

    for (unsigned int i = 0; i < dates->size(); i++) {
        uint16_t x = dates ->Get(i)->x();
        uint16_t y = dates ->Get(i)->y();
        uint16_t p = dates ->Get(i)->polarity();
        uint64_t time = dates->Get(i)->time();
        //auto x = timeStruct->x;
    cout << x << '\t' << y << '\t' << time << '\t' << p << '\n';
  }

  cout << "the end";
  

    return 0;
}