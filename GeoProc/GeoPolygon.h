#pragma once

#include "GeoPoint.h"

namespace GeoProc
{
	// A 3D Polygon
	class GeoPolygon
	{
		// Vertices of the 3D polygon
		GeoPoint *V;

		// Number of Vertices
		int N;

		// Indexes of Vertices
		int *I;

	public:
			
		__declspec(dllexport) GeoPolygon(void);

		__declspec(dllexport) ~GeoPolygon(void)
		{ 
			delete[] V; 
			delete[] I;
		};

		__declspec(dllexport) GeoPolygon(GeoPoint *v, int n)
		{
			N = n;
		
			I = new int[n];

			V = new GeoPoint[n];

			for(int i=0;i<n;i++)
			{
				V[i] = v[i];			
				I[i] = i;
			}		
		}

		__declspec(dllexport) int GetN()
		{
			return N;
		}

		__declspec(dllexport) int * GetI()
		{
			return I;
		}

		__declspec(dllexport) GeoPoint * GetV()
		{
			return V;
		}

		__declspec(dllexport) void SetV(GeoPoint *v, int n)
		{
			GeoPolygon(v, n);
		}

		__declspec(dllexport) int GetMaxFaceCount()
		{		
			return N * (N-1) * (N-2);
		}

		__declspec(dllexport) int GetMaxFaceVerticeCount()
		{
			return N-1;
		}
	};

}

