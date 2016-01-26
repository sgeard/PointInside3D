// lasProc1.cpp : Defines the entry point for the console application.
//
#include "StdAfx.h"

#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#include <math.h>
#else
#include <cstdio>
#include <cmath>
#include <cstdlib>
#endif
#include <iostream>

#include "GeoPolygonProc.h"

using namespace GeoProc;
using namespace std;

// Initialize cube 
GeoPoint CubeVertices[8] = 
{  
	GeoPoint(-27.28046,         37.11775,        -39.03485),
	GeoPoint(-44.40014,         38.50727,        -28.78860),
	GeoPoint(-49.63065,         20.24757,        -35.05160),
	GeoPoint(-32.51096,         18.85805,        -45.29785),
	GeoPoint(-23.59142,         10.81737,        -29.30445),
	GeoPoint(-18.36091,         29.07707,        -23.04144),
	GeoPoint(-35.48060,         30.46659,        -12.79519),
	GeoPoint(-40.71110,         12.20689,        -19.05819)
};

#ifndef _WIN32
static int fopen_s(FILE** s, const char* n, const char* m)
{
  *s = fopen(n,m);
  return (*s != NULL ? 0 : 1);
}
#endif

int _tmain(int, _TCHAR**)
{
			
	FILE * rbFile, * wbFile,  * wFile;
	
	// LAS public header variables
	unsigned long offset_to_point_data_value;
	unsigned long variable_len_num;
	unsigned char record_type_c;
	unsigned short record_len;
	unsigned long record_num;
	double x_scale, y_scale, z_scale;
	double x_offset, y_offset, z_offset;
	
	// Output text file of inside points
	fopen_s(&wFile, "../CubeOutput/cube_inside.txt", "w");
	
	// Output LAS file of inside points
	fopen_s(&wbFile, "../CubeOutput/cube_inside.las", "wb");	

	// Input LAS file
	if(fopen_s(&rbFile, "../LASInput/cube.las" , "rb" ) == 0)		
	{
		// Offset bytes and data types are based on LAS 1.2 Format 
    
		// Read LAS public header
		fseek(rbFile, 96, 0); 
		fread(&offset_to_point_data_value, 1, 4, rbFile);
		
		fread(&variable_len_num, 1, 4, rbFile); // Unused
		fread(&record_type_c, 1, 1, rbFile);    // Unused
		fread(&record_len, 1, 2, rbFile);
		fread(&record_num, 1, 4, rbFile);

		fseek(rbFile, 131, 0);	
		fread(&x_scale, 1, 8, rbFile);
		fread(&y_scale, 1, 8, rbFile);
		fread(&z_scale, 1, 8, rbFile);
		fread(&x_offset, 1, 8, rbFile);
		fread(&y_offset, 1, 8, rbFile);
		fread(&z_offset, 1, 8, rbFile);
		
		// Copy LAS header
		unsigned char *bufHeader = (unsigned char *)malloc(offset_to_point_data_value);
		
		// Get LAS header
		fseek(rbFile, 0, 0);
		fread(bufHeader, 1, offset_to_point_data_value, rbFile);
		
		// Write LAS header 
		fwrite(bufHeader, 1, offset_to_point_data_value, wbFile);
	
		// Read LAS points
		int x, y, z; // LAS point record coordinates			
		double ax, ay, az; // LAS point actual coordinates
		double xmin, xmax,ymin, ymax,zmin, zmax; // Cube boundary
		int insideCount = 0;		

		// Point record buffer
		unsigned char *bufRecord = (unsigned char *)malloc(record_len);
				
		// Create polygon instance 
		GeoPolygon *polygonInst = new GeoPolygon(CubeVertices, 8);

		// Create main process instance 
		GeoPolygonProc *procInst = new GeoPolygonProc(polygonInst);

		// Get cube boundary 
		procInst->GetBoundary(&xmin, &xmax,&ymin, &ymax,&zmin, &zmax);

		// Process point records
		for(unsigned int i=0;i<record_num;i++)
		{	

			int record_loc = offset_to_point_data_value + record_len * i;
			
			fseek(rbFile, record_loc, 0);

			// Record coordinates
			//int x, y, z;
			fread(&x, 1, 4, rbFile);
			fread(&y, 1, 4, rbFile);
			fread(&z, 1, 4, rbFile);
			
			// Actual coordinates
			ax = (x*x_scale)+x_offset;
			ay = (y*y_scale)+y_offset;
			az = (z*z_scale)+z_offset;	
			
			// Get point record
			fseek(rbFile, record_loc, 0);	
			fread(bufRecord, 1, record_len, rbFile);	
					
			// Filter out the points outside of boundary to reduce computing
			if(ax>xmin && ax<xmax && ay>ymin && ay<ymax && az>zmin && az<zmax)
			{					
				// Main Process to check if the point is inside of the cube				
				if(procInst->PointInside3DPolygon(ax, ay, az))
				{										
					// Write the actual coordinates of inside point to text file
					fprintf(wFile, "%15.6f %15.6f %15.6f \n", ax, ay, az);					
					// Write to binary LAS file
					fwrite(bufRecord, 1, record_len, wbFile);		
					insideCount++;
				}													
			}
					
		}
							
		// Update total record numbers in output binary LAS file
		fseek(wbFile, 107, 0);				
		fwrite(&insideCount, 1, 4, wbFile);
								
		fclose ( rbFile );		
		fclose ( wbFile );
		fclose ( wFile );	

		free(bufHeader);
		free(bufRecord);
		cout << insideCount << " points inside" << endl;
	}
	  
	return 0;
}







