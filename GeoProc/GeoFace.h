#pragma once

#include "GeoPoint.h"

namespace GeoProc
{
	// A Face of a 3D Polygon
	class GeoFace
	{
		// Vertices in one face of the 3D polygon
		GeoPoint *V;

		// Number of vertices
		int N;

		// Vertices Index
		int *I;

	public:
		__declspec(dllexport) GeoFace(void);
		__declspec(dllexport) ~GeoFace(void)
		{
			delete[] V;
			delete[] I;
		};
	
		__declspec(dllexport) GeoFace(GeoPoint* p, int *vi, int n)
		{
			N=n;

			V = new GeoPoint[n];

			I = new int[n];

			for(int i=0;i<n;i++)
			{
				V[i] = p[i];
				I[i] = vi[i];
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
	};
}
