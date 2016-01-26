#pragma once

#include "GeoPoint.h"
#include "GeoVector.h"
#include "GeoFace.h"
#include "GeoPlane.h"
#include "GeoPolygon.h"

namespace GeoProc
{
	// 3D Polygon Process 
	class GeoPolygonProc
	{
		// Polygon Boundary
		double X0, X1, Y0, Y1, Z0, Z1;	

		// Polygon faces
		GeoFace *Faces;

		// Polygon face planes
		GeoPlane *FacePlanes;

		// Number of faces
		int NumberOfFaces;	

		// Maximum point to face plane distance error, point is considered in the face plane if its distance is less than this error
		double MaxDisError;

		double Get3DPolygonUnitError(GeoPolygon *polygon);

		void Get3DPolygonBoundary(GeoPolygon *polygon, double *xmin, double *xmax,double *ymin, double *ymax,double *zmin, double *zmax);
	
		bool PointInside3DPolygon(double x, double y, double z, GeoPlane *facePlanes, int numberOfFaces);

		bool PointInside3DPolygon(GeoPoint P, GeoPlane *facePlanes, int numberOfFaces);	
	
		void GetConvex3DFaces(GeoPolygon *polygon, double maxError, GeoFace *faces, GeoPlane *facePlanes, int *numberOfFaces);

	public:

		__declspec(dllexport) GeoPolygonProc(void);
		__declspec(dllexport) ~GeoPolygonProc(void);
			
		__declspec(dllexport) GeoPolygonProc(GeoPolygon *polygonInst);

		__declspec(dllexport) void GetBoundary(double *xmin, double *xmax,double *ymin, double *ymax,double *zmin, double *zmax)
		{
			*xmin = this->X0;
			*xmax = this->X1;
			*ymin = this->Y0;
			*ymax = this->Y1;
			*zmin = this->Z0;
			*zmax = this->Z1;
		}
	
		__declspec(dllexport) bool PointInside3DPolygon(double x, double y, double z);

	};

}