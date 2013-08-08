#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#define xdim 1064
#define ydim 800
#define framesize (xdim*ydim)
#define offset 512

vector<unsigned int> data;

int main(int argc, char* argv[])
{
	ifstream datafile;
	
	datafile.open("2013-07-31a.mcd",ios::in | ios::binary);
	
	datafile.seekg(0, ios::end);	//check we can read it, and it's not zero length
	streampos length(datafile.tellg());
	cout<<"Opened file, file size: "<<length<<" bytes"<<endl;
	
	if (length) {
			
		data.resize(length);
		//load whole file
		cout<<"loading whole datafile"<<endl;
		//datafile.read((char *) &data,  sizeof (uint16_t));
		datafile.seekg(0,std::ios_base::beg);
		datafile.read((char *) &data.front(), length);

		datafile.close();
		cout<<"Loaded "<<data.size()<<" bytes of data into vector"<<endl;
		cout<<"First 20 bytes of the data:"<<endl;
		for (int i=0;i<512;i++)
		{
			cout <<std::hex<<data[i]<< " ";
		}
		cout<<endl;
		
		cout<<"chomping darkframe"<<endl;
		vector<double> dark(data.begin(),data.begin()+offset);	
		cout<<"Constructed vector<double> of size "<<dark.size()<<endl;
		cout<<"Byte at index 50 in dark vector is: "<<dark[50]<<endl;
		
	}

		return 0;
}

