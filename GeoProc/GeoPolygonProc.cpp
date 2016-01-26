#include "StdAfx.h"

#include "math.h"

#include "GeoPolygonProc.h"

double MaxUnitMeasureError = 0.001;

namespace GeoProc
{
	GeoPolygonProc::GeoPolygonProc(void)
	{
	}
	GeoPolygonProc::~GeoPolygonProc(void)
	{
	}

	GeoPolygonProc::GeoPolygonProc(GeoPolygon *polygonInst)
	{
			int MaxFacesCount = polygonInst->GetMaxFaceCount();
		
			GeoFace *faces = new GeoFace[MaxFacesCount];

			GeoPlane *facePlanes = new GeoPlane[MaxFacesCount];
						
			int numberOfFaces;		
												
			double x0, x1,y0, y1,z0, z1; 

			// Get boundary 
			this->Get3DPolygonBoundary(polygonInst, &x0, &x1,&y0, &y1,&z0, &z1);		

			// Get maximum point to face plane distance error, point is considered in the face plane if its distance is less than this error
			double maxDisError = this->Get3DPolygonUnitError(polygonInst);
						
			// Get face planes		
			this->GetConvex3DFaces(polygonInst, maxDisError, faces, facePlanes, &numberOfFaces);

			// Set data members
			this->X0 = x0;
			this->X1 = x1;
			this->Y0 = y0;
			this->Y1 = y1;
			this->Z0 = z0;
			this->Z1 = z1;
			this->Faces = faces;
			this->FacePlanes = facePlanes;
			this->NumberOfFaces = numberOfFaces;
			this->MaxDisError = maxDisError;
	}

	void GeoPolygonProc::Get3DPolygonBoundary(GeoPolygon *polygon, 
		double *xmin, double *xmax,double *ymin, double *ymax,double *zmin, double *zmax)
	{	
		GeoPoint *vertices = polygon->GetV();
		int n = polygon->GetN();

		*xmin = *xmax = vertices[0].X;
		*ymin = *ymax = vertices[0].Y;
		*zmin = *zmax = vertices[0].Z;

		for(int i=1;i<n;i++)
		{
			if(vertices[i].X < *xmin) *xmin = vertices[i].X;
			if(vertices[i].Y < *ymin) *ymin = vertices[i].Y;
			if(vertices[i].Z < *zmin) *zmin = vertices[i].Z;
			if(vertices[i].X > *xmax) *xmax = vertices[i].X;
			if(vertices[i].Y > *ymax) *ymax = vertices[i].Y;
			if(vertices[i].Z > *zmax) *zmax = vertices[i].Z;
		}		
	
	}

	double GeoPolygonProc::Get3DPolygonUnitError(GeoPolygon *polygon)
	{
		GeoPoint *vertices = polygon->GetV();
		int n = polygon->GetN();

		double measureError = 0;

		double xmin, xmax,ymin, ymax,zmin, zmax; 

		this->Get3DPolygonBoundary(polygon, &xmin, &xmax,&ymin, &ymax,&zmin, &zmax);
			
		measureError = ( (fabs(xmax) + fabs(xmin) + fabs(ymax) + fabs(ymin) + fabs(zmax) + fabs(zmin) ) / 6 * MaxUnitMeasureError);
	
		return measureError;
	}

	bool GeoPolygonProc::PointInside3DPolygon(double x, double y, double z)
	{	
		GeoPoint P = GeoPoint(x,y,z);	
			
		return this->PointInside3DPolygon(P, this->FacePlanes, this->NumberOfFaces);					
	}

	bool GeoPolygonProc::PointInside3DPolygon(double x, double y, double z, GeoPlane *facePlanes, int numberOfFaces)
	{
	
		GeoPoint P = GeoPoint(x,y,z);	
			
		return this->PointInside3DPolygon(P, facePlanes, numberOfFaces);					
	}

	bool GeoPolygonProc::PointInside3DPolygon(GeoPoint P, GeoPlane *facePlanes, int numberOfFaces)
	{ 	
		
		for (int i=0; i < numberOfFaces; i++)   
		{
		        							
			double dis = P * facePlanes[i];

			// If the point is in the same half space with normal vector for any facet of the cube, then it is outside of the 3D polygon		
			if(dis > 0) 
			{					
				return false;
			}

		}
	
		// If the point is in the opposite half space with normal vector for all 6 facets, then it is inside of the 3D polygon
		return true;
	}

	void GeoPolygonProc::GetConvex3DFaces(GeoPolygon *polygon, double maxError, GeoFace *faces, GeoPlane *facePlanes, int *actualFaceCount)
	{		
		GeoPoint *vertices = polygon->GetV();

		int maxFaceVerticesCount = polygon->GetMaxFaceVerticeCount();

		int maxFaceCount = polygon->GetMaxFaceCount();

		int n = polygon->GetN();
	
		int **faceVerticeIndex = new int *[maxFaceCount];
	
		int faceCount = 0;
	
		GeoPlane *fpOutward = new GeoPlane[maxFaceCount];	

		int *pointInSamePlaneIndex = new int[maxFaceVerticesCount];
	
		int invalidIndex = -1;
	
		for(int i=0; i< n; i++)
		{ 
			// triangle point 1
			GeoPoint p0 = vertices[i];

			for(int j= i+1; j< n; j++)
			{
				// triangle point 2
				GeoPoint p1 = vertices[j];

				for(int k = j + 1; k<n; k++)
				{
					// triangle point 3
					GeoPoint p2 = vertices[k];
		
					GeoPlane trianglePlane = GeoPlane(p0, p1, p2);
					
					int onLeftCount = 0;
					int onRightCount = 0;
									
					int m = 0;
				
					for(int l = 0; l < n ; l ++)
					{
						if(l != i && l != j && l != k) // any point other than the 3 triangle points
						{
							GeoPoint p = vertices[l];

							double dis = p * trianglePlane;

							if(fabs(dis) < maxError) // next point is in the triangle plane
							{							
								pointInSamePlaneIndex[m] = l;
								m++;
							}
							else
							{
								if(dis < 0)
								{
									onLeftCount ++;								
								}
								else
								{
									onRightCount ++;
								}

							}
						}
					}
				

					if(onLeftCount == 0 || onRightCount == 0) // This is a face for a CONVEX 3d polygon. For a CONCAVE 3d polygon, this maybe not a face.
					{
						faceVerticeIndex[faceCount]  = new int[maxFaceVerticesCount];
						for(int p = 0; p < maxFaceVerticesCount; p ++)
						{											
							faceVerticeIndex[faceCount][p] = invalidIndex;						
						}	

						// triangle plane
						faceVerticeIndex[faceCount][0] = i;
						faceVerticeIndex[faceCount][1] = j;
						faceVerticeIndex[faceCount][2] = k;								

						if(m > 0) // there are other vetirces in this triangle plane
						{
							for(int p = 0; p < m; p ++)
							{											
								faceVerticeIndex[faceCount][3+p] = pointInSamePlaneIndex[p];
							}						
						}

						if(onRightCount == 0)
						{												
							fpOutward[faceCount] = trianglePlane; 
						}
						else if (onLeftCount == 0)
						{						
							fpOutward[faceCount] = -trianglePlane; 
						}

						faceCount++;

					}
					else
					{					
						// possible reasons:
						// 1. the plane is not a face of a convex 3d polygon, it is a plane crossing the convex 3d polygon.
						// 2. the plane is a face of a concave 3d polygon
					}

				} // k loop
			} // j loop		
		} // i loop

	
		// sort index descending
		for(int i=0; i< faceCount; i++)
		{
			for(int j=0; j<maxFaceVerticesCount; j++)
			{
				for(int k=0;k<maxFaceVerticesCount;k++)
				{
					if(faceVerticeIndex[i][j] > faceVerticeIndex[i][k])
					{
						int temp = faceVerticeIndex[i][j];
						faceVerticeIndex[i][j] = faceVerticeIndex[i][k];
						faceVerticeIndex[i][k] = temp;
					}
				}
			}
		}	
	
		// actual face count
		*actualFaceCount = 0;

		// actual unqiue face index
		int *actualUnqiueFaceIndex = new int[faceCount];

		// remove duplicates and return face planes and actual face count
		for(int i=0; i< faceCount; i++)
		{
			bool isUnique = true;
			for(int j=i + 1; j<faceCount; j++)
			{
			
				bool isDuplicate = true;
				for(int k=0;k<maxFaceVerticesCount;k++)
				{				
					if(faceVerticeIndex[i][k] != faceVerticeIndex[j][k])
					{
						isDuplicate=false;
						break;
					}
				}

				if(isDuplicate)
				{
					isUnique=false;
					break;
				}
			}

			if(isUnique)
			{			
						
				facePlanes[*actualFaceCount] = GeoPlane(fpOutward[i].A, fpOutward[i].B, fpOutward[i].C, fpOutward[i].D);		

				actualUnqiueFaceIndex[*actualFaceCount] = i;

				(*actualFaceCount)++;
			}
		}
	
	
		// Return faces
		for(int i=0;i<(*actualFaceCount);i++) // face 
		{
			int actualFacePointCount = 0;
		
			for(int j=0;j<maxFaceVerticesCount;j++) // face vertice 
			{
				if(faceVerticeIndex[i][j] == invalidIndex)							
				{												
					break;
				}
				actualFacePointCount ++;
			}

			GeoPoint *gp = new GeoPoint[actualFacePointCount];

			int *vi = new int[actualFacePointCount];

			int idx = actualUnqiueFaceIndex[i];

			for(int j=0;j<actualFacePointCount;j++)
			{			
				vi[j] = faceVerticeIndex[idx][j];
				gp[j] = GeoPoint(vertices[vi[j]].X, vertices[vi[j]].Y, vertices[vi[j]].Z);
			}

			faces[i] = GeoFace(gp, vi, actualFacePointCount);

		}

		// Free memory
		for(int i = 0; i < faceCount; i++ )
		{
			delete [] faceVerticeIndex[i] ;
		}
		delete[] faceVerticeIndex;
		delete[] fpOutward;
		delete[] actualUnqiueFaceIndex;
		delete[] pointInSamePlaneIndex;
	}

}