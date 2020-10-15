#pragma once
#include <cstddef>
using namespace std;

class MedianFilter
{
public:
	float filterBuff[100] = { 0 };
	float filterBuff_BACKUP[100] = { 0 };
	int filterLength = 30;
	short writePointer = 0;

	MedianFilter() {};

	~MedianFilter() {};

	// Function for calculating median 
	double getMedian(float a[], int n)
	{
		// First we sort the array 
		sort(a, a + n);

		// check for even case 
		if (n % 2 != 0)
			return (double)a[n / 2];

		return (double)(a[(n - 1) / 2] + a[n / 2]) / 2.0;
	}

	float doFiltering(float inputSample)
	{
		float output = 0;
		float tempArray[100] = { 0.0 };
		filterBuff[writePointer] = inputSample;
		for (int i = 0; i < filterLength; i++)
			tempArray[i] = filterBuff[i];
		output = getMedian(tempArray, filterLength);

		writePointer = (writePointer + 1) % filterLength;

		return output;
	}
};