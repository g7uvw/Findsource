// mcd2xml.cpp 
// dumps an MCD header to a bastardisation of XML
// David Mills 2013

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct thead
  {
    short xsize,ysize,zsize,lmarg,rmarg,tmarg,bmarg,tzmarg,bzmarg,\
     num_samples,num_proj,num_blocks,num_slices,bin,gain,speed,pepper,issue,num_frames,spare_int[13];
    float scale,offset,voltage,current,thickness,pixel_size,distance,exposure,\
     mag_factor,gradient,spare_float[2];
	long posdistance,slices_per_block,z,theta;
    char time[26],duration[12],owner[21],user[5],specimen[32],scan[32],\
     comment[64],spare_char[192];
  };


int main(int argc, char* argv[])
{


    // If the user didn't provide a filename command line argument,
    // print an error and exit.
    if (argc <= 1)
    {
		cout << "MCD2XML - dumps MCD header to XML file of same name" <<endl;
        cout << "Usage: " << argv[0] << " <Filename>" << endl;
        exit(1);
    }

	struct thead mcd_header;
 
	ifstream mcd (argv[1]);
    if (!mcd.is_open())
		{
        cout<<"Cannot open " <<argv[1] <<" is_open() returned false. error state"<<endl;
		exit(1);
		}
    else
		{
		mcd.read(reinterpret_cast<char*>(&mcd_header),512);
		mcd.close();
		}

	string xmlfilename = argv[1];
	
	xmlfilename += ".xml";
	ofstream xml(xmlfilename.c_str(),ios::out|ios::trunc);
	
    if (!xml.is_open())
		{
        cout<<"Cannot open " <<xmlfilename <<" for writing."<<endl;
		exit(1);
		}
	
	xml <<  "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>" <<endl;
	xml <<  "<!--Document created by: mcd2xml-->" <<endl;
	xml <<  "<mcd_header>" <<endl;
	xml <<  "<mcd_filename>" << argv[1] << "</mcd_filename>" <<endl;
	xml <<  "<dimensions>" << mcd_header.xsize <<","<<mcd_header.ysize<<","<<mcd_header.zsize << "</dimensions>" <<endl;
	xml << 	"<scan_date_time>" << mcd_header.time  <<  "</scan_date_time>" <<endl;
	xml << 	"<scan_duration>" << mcd_header.duration <<  "</scan_duration>" <<endl;
	xml << 	"<scan_operator>" << mcd_header.user <<  "</scan_operator>" <<endl;	
	xml <<  "<specimen_owner>" << mcd_header.owner  <<  "</specimen_owner>" <<endl;
	xml << 	"<specimen_info>" << mcd_header.specimen <<  "</specimen_info>" <<endl;
	xml << 	"<scan_info>" << mcd_header.scan <<  "</scan_info>" <<endl;
	xml << 	"<scan_comment>" << mcd_header.comment <<  "</scan_comment>" <<endl;
	xml << 	"<slices_per_block>" << mcd_header.slices_per_block <<  "</slices_per_block>" <<endl;
	xml << 	"<number_of_blocks>" << mcd_header.num_blocks <<  "</number_of_blocks>" <<endl;
	xml <<  "<number_of_projections>" << mcd_header.num_proj <<  "</number_of_projections>" <<endl;
	xml << 	"<number_of_slices>" << mcd_header.slices_per_block <<  "</number_of_slices>" <<endl;
	xml << 	"<number_of_frames>" << mcd_header.num_frames <<  "</number_of_frames>" <<endl;
	xml << 	"<voxel_size>" << mcd_header.pixel_size <<  "</voxel_size>" <<endl;
	xml << 	"<voltage>" << mcd_header.voltage <<  "</voltage>" <<endl;
	xml << 	"<current>" << mcd_header.current <<  "</current>" <<endl;
	xml <<  "</mcd_header>" <<endl;

	return 0;
}
