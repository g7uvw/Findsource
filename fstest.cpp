// findsource.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdint.h>


using namespace std;

#define xdim 1064
#define ydim 800
#define framesize (xdim*ydim)
#define offset (data.begin()+512)
#define darkoffset (offset)
#define darkend (darkoffset + framesize)
#define lightoffset (offset+framesize)
#define lightend (lightoffset+framesize)
#define frameoffset (lightend+(framenum*framesize))
#define frameend (frameoffset+framesize)
#define range 500
#define nproj 27
#define PEPPERTHRESHOLD 0.6

vector<unsigned int> data;
vector<vector<double> > proj;

vector<vector<float> > widtharray;				//500*27
vector<vector<float> > edgearray;				//500*2
//vector<vector<float> > light;					//xdim*ydim
//vector<vector<float> > dark;					//xdim*ydim
//vector<vector<float>> proj;			

unsigned int frame,line, pixel,peppercount = 0;

int main(int argc, char* argv[])
{


	//resize the arrays to be large enough
	//m * n is the size of the array
	
	/*dark.resize(xdim*ydim);
	light.resize(xdim*ydim);*/

	widtharray.resize(500);
    for(int i = 0 ; i < 500 ; ++i)
    {
    	widtharray[i].resize(27);
    }

	edgearray.resize(500);
    for(int i = 0 ; i < 500 ; ++i)
    {
    	edgearray[i].resize(2);
    }



	//unsigned int b;
	//read file
	ifstream datafile;
	
	datafile.open("2013-07-31a.mcd",ios::in | ios::binary);
	
	datafile.seekg(0, ios::end);	//check we can read it, and it's not zero length
	streampos length(datafile.tellg());
	cout<<"Opened a.mcd, file size: "<<length<<" bytes"<<endl;
	
	if (length) {
	
	data.resize(length);
	
	//load whole file
	datafile.seekg(0,std::ios_base::beg);
	cout<<"loading whole datafile"<<endl;
	datafile.read((char *) &data.front(),  length);
	
	cout<<"chomping darkframe"<<endl;
	vector<double> dark(darkoffset,darkend);
	cout<<"test byte from dark frame: "<<dark[1000]<<endl;
	
	cout<<"chomping lightframe"<<endl;
	vector<double> light(lightoffset,lightend);
	cout<<"test byte from light frame: "<<light[1000]<<endl;

	proj.resize(nproj);
	for (int framenum=0;framenum<nproj;framenum++)
		{
		cout<<"chomping projection "<<framenum+1 <<"of "<<nproj<<endl;
		proj[framenum].assign (frameoffset,frameend);
		cout<<"test byte from data frame: "<<proj[framenum][200]<<endl;
	
	}

	//data.~vector();	//free up ram


		/*access vectors via usuall width*row+col access
		should be much faster*/

	
	
		}
		


	//cout<<"Fixing bright lines in frame"<<endl;
	//for (line = 0; line <ydim; line ++)
	//	{
	//	fdark[line][xdim-1]=fdark[line][xdim-2];
	//	flight[line][xdim-1]=flight[line][xdim-2];
	//	}

	//for (frame = 0; frame < nproj; frame ++)
	//	{
	//	for (line = 0; line <ydim; line ++)
	//		{
	//		fproj[frame][line][xdim-1] = fproj[frame][line][xdim-2];
	//		}
	//	}

	//cout<<"Dark frame correction"<<endl;
	//for (line = 0; line <ydim; line ++)
	//	{
	//	for (pixel = 0; pixel < xdim; pixel ++)
	//		{
	//		//cout << light[line][pixel]<< " - " <<dark[line][pixel] <<" = "<< light[line][pixel]-dark[line][pixel] <<endl;
	//		flight[line][pixel]-=fdark[line][pixel];
	//		//cout << light[line][pixel] <<endl;
	//		}
	//	}

	//for (frame = 0; frame < nproj; frame ++)
	//	{
	//	cout<<"Dark frame correction, frame: " <<frame<<endl;
	//	for (line = 0; line <ydim; line ++)
	//		{
	//		for (pixel = 0; pixel < xdim; pixel ++)
	//			{
	//			fproj[frame][line][pixel] -= fdark[line][pixel];
	//			}
	//		}
	//	}

	//cout<<"Floatifying projections"<<endl;
	//vector<vector<vector<double> > > projections;
	//projections.resize(nproj);
	//for (int i = 0; i < nproj; i++)
	//	{
	//	projections[i].resize(ydim);
	//	for (int j = 0; j < ydim; j++)
	//		{
	//		projections[i][j].resize(xdim);
	//		}
	//	}

	//for (frame = 0; frame < nproj; frame ++)
	//	{
	//	peppercount = 0;
	//	cout<<"light frame correction, frame: " <<frame<<endl;
	//	for (line = 0; line <ydim; line ++)
	//		{
	//		for (pixel = 0; pixel < xdim; pixel ++)
	//			{
	//			if (flight[line][pixel] !=0)
	//				projections[frame][line][pixel] = fproj[frame][line][pixel] / flight[line][pixel];
	//			else
	//				projections[frame][line][pixel] = fproj[frame][line][pixel] / (flight[line][pixel]+1);

	//			//depepper
	//			if (pixel != 0 && pixel < xdim-1)
	//				{
	//				double test = projections[frame][line][pixel] - projections[frame][line][pixel-1]*0.5 - projections[frame][line][pixel+1]*0.5;
	//				if (test > PEPPERTHRESHOLD)

	//					{
	//					projections[frame][line][pixel] = projections[frame][line][pixel+1] + projections[frame][line][pixel-1] / 2;
	//					peppercount++;
	//					}

	//				}
	//			}
	//		}
	//	cout<<"Pepper count this frame: "<<peppercount<<endl;
	//	}

	//recover some RAM
//	/*light.~vector();
//	dark.~vector();
//	proj.~vector();*/


	return 0;
}


	


