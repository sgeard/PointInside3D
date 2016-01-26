#pragma once

namespace GeoProc
{
	// A 3D Geometry Point 
	class GeoPoint
	{			

	public:
	
		double X;		
		double Y;
		double Z;	

		__declspec(dllexport) GeoPoint(void);
		__declspec(dllexport) ~GeoPoint(void);

		__declspec(dllexport) GeoPoint(double x, double y, double z)		
		{
			X=x;
			Y=y;
			Z=z;	
		}	

		__declspec(dllexport) friend GeoPoint operator+(const GeoPoint& p0, const GeoPoint& p1) 
		{
			return GeoPoint(p0.X + p1.X, p0.Y + p1.Y, p0.Z + p1.Z);
		}
	
	};

}