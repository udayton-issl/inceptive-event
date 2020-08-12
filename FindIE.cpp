#include "FindIE.h"
#include <stdint.h>

extern void findIE(values *inceptiveEvents, values *valueArray, int multiTriggerWindow)
{
    unsigned long long positiveT[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned long long negativeT[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int positiveIndex = 0;
    int negativeIndex = 0;
    int tIndex = 0;

    //fill arrays for positive and negative polarity
    for(int i = 0; i < 10; i++)
    {
        if(valueArray->t[i] != 0)
        {
            if((((valueArray)->p >> i) & 1) == 1)
            {
                positiveT[positiveIndex] = (valueArray->t[i]);
                positiveIndex++;
            }
            else if((((valueArray)->p >> i) & 1) == 0)
            {
                negativeT[negativeIndex] = (valueArray->t[i]);
                negativeIndex++;
            }
        }
    }
    uint8_t positiveIndicies[positiveIndex];
    uint8_t negativeIndicies[negativeIndex];

    //section determines if an event is an inceptive event and fills the inceptive struct positive
    if(sizeof(positiveIndicies) != 0)
    {
        for(int i = 0; i < (sizeof(positiveIndicies) - 1); i++)
        {
            if((positiveT[i + 1] - positiveT[i]) <= multiTriggerWindow)
            {
                positiveIndicies[i] = 1;
            }
            else
            {
                positiveIndicies[i] = 0;
            }
        }
        positiveIndicies[sizeof(positiveIndicies)] = 0;

        uint8_t tempPositive[positiveIndex];
        tempPositive[0] = 0;

        for(int i = 1; i < sizeof(tempPositive); i++)
        {
            tempPositive[i] = positiveIndicies[i - 1];
        }

        for(int i = 0; i < sizeof(tempPositive) - 1; i++)
        {
            if((tempPositive[i + 1] - tempPositive[i]) == 1)
            {
                positiveIndicies[i] = 1;
            }
            else
            {
                positiveIndicies[i] = 0;
            } 
        }
    }
    
    for(int i = 0; i < positiveIndex; i++)
    {
        if(positiveIndicies[i] == 1)
        {
            inceptiveEvents->t[tIndex] = positiveT[i];
            tIndex++;
        }
    }
    

    //section determines if an event is an inceptive event and fills the inceptive struct negative
    if(sizeof(negativeIndicies) != 0)
    {
        for(int i = 0; i < (sizeof(negativeIndicies) - 1); i++)
        {
            if((negativeT[i + 1] - negativeT[i]) <= multiTriggerWindow)
            {
                negativeIndicies[i] = 1;
            }
            else
            {
                negativeIndicies[i] = 0;
            }
        }
        negativeIndicies[sizeof(negativeIndicies)] = 0;

        uint8_t tempNegative[negativeIndex];
        tempNegative[0] = 0;

        for(int i = 1; i < sizeof(tempNegative); i++)
        {
            tempNegative[i] = negativeIndicies[i - 1];
        }

        for(int i = 0; i < sizeof(tempNegative) - 1; i++)
        {
            if((tempNegative[i + 1] - tempNegative[i]) == 1)
            {
                negativeIndicies[i] = 1;
            }
            else
            {
                negativeIndicies[i] = 0;
            } 
        }
    }
    
    for(int i = 0; i < negativeIndex; i++)
    {
        if(negativeIndicies[i] == 1)
        {
            inceptiveEvents->t[tIndex] = negativeT[i];
            tIndex++;
        }
    }
}