// findsource.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdint.h>


using namespace std;

#define xdim 1020
#define ydim 800
#define framesize (xdim*ydim)
#define offset (data.begin()+256)
#define darkoffset (offset)
#define darkend (darkoffset + framesize)
#define lightoffset (offset+framesize)
#define lightend (lightoffset+framesize)
#define frameoffset (lightend+(framenum*framesize))
#define frameend (frameoffset+framesize)
#define linestart (linecounter*xdim)
#define lineend (linestart + xdim)
#define linepixel (y * xdim +x)
#define range 500
#define nproj 27
#define PEPPERTHRESHOLD 0.8

vector<uint16_t> data;
vector<vector<double> > proj;

vector<vector<double> > widtharray;				//500*27
vector<vector<double> > edgearray;				//500*2
//vector<vector<float> > light;					//xdim*ydim
//vector<vector<float> > dark;					//xdim*ydim
//vector<vector<float>> proj;			

unsigned int frame,line, pixel,peppercount = 0;

int _tmain(int argc, _TCHAR* argv[])
	{

cout<<sizeof(double)<<endl;

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

	datafile.open("d:\\data\\2013-07-31a.mcd",ios::in | ios::binary);

	datafile.seekg(0, ios::end);	//check we can read it, and it's not zero length
	streampos length(datafile.tellg());
	cout<<"Opened a.mcd, file size: "<<length<<" bytes"<<endl;

	if (length) {
		//datafile.seekg(512, ios::beg); //skip 512 bytes of header
		//cout<<"SEEKed to end of header. Current position is :" <<datafile.tellg()<<endl;

		data.resize(length);
		//load whole file
		datafile.seekg(0,std::ios_base::beg);
		cout<<"loading whole datafile"<<endl;
		datafile.read((char *) &data.front(),  length);

		cout<<"chomping darkframe"<<endl;
		vector<double> dark(darkoffset,darkend);

		cout<<"test bytes from dark frame: "<<dark[759]<<dark[760]<<","<<dark[761]<<","<<dark[762]<<","<<dark[763]<<","<<dark[764]<<","<<dark[765]<<","<<dark[766]<<","<<dark[767]<<","<<dark[768]<<","<<dark[769]<<endl;
		ofstream darkfileu("d:\\data\\darkframe.bin", ios::out | ios::binary);
		darkfileu.write((char*)&*(dark.begin()),dark.size() * sizeof(double));

		cout<<"chomping lightframe"<<endl;
		vector<double> light(lightoffset,lightend);
		cout<<"test byte from light frame: "<<light[1000]<<endl;
		ofstream lightfileu("d:\\data\\lightframe.bin", ios::out | ios::binary);
		lightfileu.write((char*)&*(light.begin()),light.size() * sizeof(double));

		proj.resize(nproj);
		for (int framenum=0;framenum<nproj;framenum++)
			{
			cout<<"chomping projection "<<framenum+1 <<"of "<<nproj<<endl;
			proj[framenum].assign (frameoffset,frameend);
			cout<<"test byte from data frame: "<<proj[framenum][200]<<endl;
			}

		/*access vectors via usuall width*row+col access
		should be much faster*/




		cout<<"Dark frame correction"<<endl;
		for (int pix = 0; pix <framesize; pix ++)
			{
			light[pix] -= dark[pix];
			}
		//ofstream lightfile("d:\\data\\lightframecorrected.bin", ios::out | ios::binary);
		//lightfile.write((char*)&*(light.begin()),light.size() * sizeof(double));


		for (frame = 0; frame < nproj; frame ++)
			{
			cout<<"Dark frame correction, frame: " <<frame<<endl;
			for (int pix = 0; pix <framesize; pix ++)
				{
				proj[frame][pix] -= dark[pix];
				}
			}
		
		//ofstream projections("d:\\data\\projections.bin", std::ofstream::binary);
		//for(size_t i = 0; i < proj.size(); i++ )
		//	{
		//	if ( proj[i].size() > 0 )
		//		{
		//		//const char* projbuffer = static_cast<const char*>(&proj[i][0]);
		//		projections.write((char*)&*proj[i].begin(), proj[i].size()*sizeof(double));
		//		}
		//	}



				for (frame = 0; frame < nproj; frame ++)
					{
					peppercount = 0;
					cout<<"light frame correction, frame: " <<frame<<endl;

					for (pixel = 0; pixel < framesize; pixel ++)
						{
						if (light[pixel] !=0)
							proj[frame][pixel] = proj[frame][pixel] / light[pixel];
						else
							proj[frame][pixel] = proj[frame][pixel] / light[pixel]+1;

						//depepper
						if (pixel != 0 && pixel < framesize-1)
							{
							double test = proj[frame][pixel] - proj[frame][pixel-1]*0.5 - proj[frame][pixel+1]*0.5;
							if (test > PEPPERTHRESHOLD)
								{
								proj[frame][pixel] = proj[frame][pixel+1] + proj[frame][pixel-1] / 2;
								peppercount++;
								}

							}
						}
					cout<<"Pepper count this frame: "<<peppercount<<endl;
					}

		}

	//ofstream depepprojections("d:\\data\\depepprojections.bin", std::ofstream::binary);
	//	for(size_t i = 0; i < proj.size(); i++ )
	//		{
	//		if ( proj[i].size() > 0 )
	//			{
	//			//const char* projbuffer = static_cast<const char*>(&proj[i][0]);
	//			depepprojections.write((char*)&*proj[i].begin(), proj[i].size()*sizeof(double));
	//			}
	//		}
	
	//find edges
	vector<double> edges;
	edges.resize(2);
	for (frame = 0; frame < nproj; frame ++)
		{
		int a = 0;
		double h = 0;
		unsigned int ystart = (ydim-range)/2;
		unsigned int yend = ystart + range -1;
		edges[0]=0;
		edges[1]=0;
		for (unsigned int y = ystart; y < yend; y++)
			{
			double x=1;
			while (proj[frame][linepixel] > 0.5)
				{
				x++;
				}
			cout<<"x[0] = "<<x<<endl;
			edges[0] += x;
			h = 0.5 - proj[frame][linepixel]/(proj[frame][linepixel-1] - proj[frame][linepixel]);
			edges[0] -= h;
			
			edgearray[a][0]=(x-h);
			
			//proj[frame][linepixel] = 254; //mark pixel

			x +=10;	//make sure we're away from the edge before we look for the next edge

			while (proj[frame][linepixel] < 0.5)
				{
				x++;
				}
			cout<<"x[1] = "<<x<<endl;
			edges[1] += x;
			h = 0.5 - proj[frame][linepixel]/(proj[frame][linepixel-1] - proj[frame][linepixel]);
			edges[1] -= h;
			
			edgearray[a][1]=(x-h);
			
			//proj[frame][linepixel] = 254; //mark pixel

			a++;

			edges[0] /= range;
			edges[1] /= range;

			//cout <<"Edges at "<<edges[0]<<" and "<<edges[1]<<endl; 
			}


		}


	//ofstream edgesfile("d:\\data\\edges.bin", std::ofstream::binary);
	//	for(size_t i = 0; i < proj.size(); i++ )
	//		{
	//		if ( proj[i].size() > 0 )
	//			{
	//			//const char* projbuffer = static_cast<const char*>(&proj[i][0]);
	//			edgesfile.write((char*)&*proj[i].begin(), proj[i].size()*sizeof(double));
	//			}
	//		}


	//recover some RAM
	/*light.~vector();
	dark.~vector();
	proj.~vector();*/


	return 0;
	}




