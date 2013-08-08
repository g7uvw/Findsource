#include <vector>
#include <iostream>
using namespace std;

#define xdim 1064
#define ydim 800
#define framesize (xdim*ydim)
#define offset 512



int main(int argc, char* argv[])
{
	vector<unsigned int> data(xdim,12345);
	cout<<"size of data vector is :"<<data.size()<<endl;
	vector<double> dark(data.begin(),data.begin()+100);
	cout<<"size of dark vector is :"<<dark.size()<<endl;
	
	cout<<"Byte at index 50 in dark vector is: "<<dark[50]<<endl;
	
	
	return 0;
}

