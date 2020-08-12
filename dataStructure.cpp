#include "dataStructure.h"
#include "inputCameraData_generated.h"

using namespace std;
using namespace Camera::Data;

//imports the data into a sorted array of structs
void fillTimeArray(values *ValuesElement, unsigned long long timeValue, short p)
{
    for(int i = 0; i < sizeof(ValuesElement->t); i++)
    {
        if(ValuesElement->t[i] == 0)
        {
            ValuesElement->t[i] = timeValue;
            ValuesElement->p = ((ValuesElement->p) << 1) | p;
            break;
        }
    }
}

//copies the data to a medium that can be stored in the buffer
//This was a workaround to a bug encountered where I could not just save
//the same vector of import data as it was into a new buffer.
void convertOriginalData(const flatbuffers::Vector<const Camera::Data::dataPoint*>* importDataVector, dataPoint *exportedData)
{
            for(int i = 0; i < importDataVector->size(); i++){
        exportedData[i] = dataPoint(importDataVector->Get(i)->x(), importDataVector->Get(i)->y(), importDataVector->Get(i)->time(), importDataVector->Get(i)->polarity());
    }
}