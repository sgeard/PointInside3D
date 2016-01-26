#pragma once

#include "GeoVector.h"

namespace GeoProc
{

	// A 3D Geometry Plane
	class GeoPlane
	{
	public:

		// Plane Equation: A * x + B * y + C * z + D = 0

		double A;
		double B;
		double C;
		double D;

		__declspec(dllexport) GeoPlane(void);
		__declspec(dllexport) ~GeoPlane(void);

		__declspec(dllexport) GeoPlane(double a, double b, double c, double d)
		{
			A = a;
			B = b;
			C = c;
			D = d;
		}

		__declspec(dllexport) GeoPlane(GeoPoint p0, GeoPoint p1, GeoPoint p2)
		{
			
			GeoVector v = GeoVector(p0, p1);

			GeoVector u = GeoVector(p0, p2);

			GeoVector n = u * v;

			// normal vector		
			double a = n.GetX();
			double b = n.GetY();
			double c = n.GetZ();				
			double d = -(a * p0.X + b * p0.Y + c * p0.Z);
			
			A = a;
			B = b;
			C = c;
			D = d;
			
		}

		__declspec(dllexport) GeoPlane operator-() 
		{
			return GeoPlane(-A, -B, -C, -D);
		}

		__declspec(dllexport) friend double operator*(const GeoPoint& pt, const GeoPlane& pl) 
		{
			return (pt.X * pl.A + pt.Y * pl.B + pt.Z * pl.C + pl.D);
		}
	};

}
