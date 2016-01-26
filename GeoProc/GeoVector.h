#pragma once

#include "GeoPoint.h"

namespace GeoProc
{
	// A 3D Geometry Vector
	class GeoVector
	{
		GeoPoint P0; // vector begin point
		GeoPoint P1; // vector end point

		double X; // vector x axis projection value
		double Y; // vector y axis projection value
		double Z; // vector z axis projection value 

	public:

		__declspec(dllexport) GeoVector(void);
		__declspec(dllexport) ~GeoVector(void);
	
		__declspec(dllexport) GeoVector(GeoPoint p0, GeoPoint p1)
		{
			P0 = p0;
			P1 = p1;
			X = p1.X - p0.X;
			Y = p1.Y - p0.Y;
			Z = p1.Z - p0.Z;
		}		

		__declspec(dllexport) friend GeoVector operator*(const GeoVector& u, const GeoVector& v) 
		{
			double x = u.Y * v.Z - u.Z * v.Y;
			double y = u.Z * v.X - u.X * v.Z;
			double z = u.X * v.Y - u.Y * v.X;
			
			GeoPoint p0 = v.P0;
			GeoPoint p1 = p0 + GeoPoint(x, y, z);

			return GeoVector(p0, p1);
		}

		__declspec(dllexport) double GetX(){return X;}
		__declspec(dllexport) double GetY(){return Y;}
		__declspec(dllexport) double GetZ(){return Z;}

	};

}