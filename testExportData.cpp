#include "inputCameraData_generated.h"
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;
using namespace Camera::Data;

int main()
{
    ifstream infile;
    infile.open("outputCameraData.bin", ios::binary);
    infile.seekg(0, ios::end);
    int length = infile.tellg();
    infile.seekg(0, ios::beg);
    char *theInputData = new char[length];
    infile.read(theInputData, length);
    infile.close();

    auto receivedInputData = GetDataCollection(theInputData);
    auto importDataVector = receivedInputData->collectedData();
    auto importInceptiveVector = receivedInputData->collectedInceptive();
    auto importTrailingVector = receivedInputData->collectedTraling();

    FILE *fa = fopen("importTest.csv", "w");

    for(int i = 0; i < importDataVector->size(); i++)
    {
        int x = importDataVector ->Get(i)->x();
        int y = importDataVector ->Get(i)->y();
        unsigned long long t = importDataVector ->Get(i)->time();
        int p = importDataVector ->Get(i)->polarity();
        fprintf(fa, "%d,%d,%llu,%d\n", x, y, t, p);
    }
    fclose(fa);

    FILE *fb = fopen("inceptiveTest.csv", "w");

    for(int i = 0; i < importInceptiveVector->size(); i++)
    {
        int x = importInceptiveVector ->Get(i)->x();
        int y = importInceptiveVector ->Get(i)->y();
        unsigned long long t = importInceptiveVector ->Get(i)->time();
        int trailing = importInceptiveVector ->Get(i)->trailingNum();
        fprintf(fb, "%d,%d,%llu,%d\n", x, y, t, trailing);
    }
    fclose(fb);

    FILE *fc = fopen("trailingTest.csv", "w");

    for(int i = 0; i < importTrailingVector->size(); i++)
    {
        int x = importTrailingVector ->Get(i)->x();
        int y = importTrailingVector ->Get(i)->y();
        unsigned long long t = importTrailingVector ->Get(i)->time();
        fprintf(fc, "%d,%d,%llu\n", x, y, t);
    }
    fclose(fc);
    
    return 0;
}