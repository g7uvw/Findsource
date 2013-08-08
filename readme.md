Just some tools in progress for working with MCD datafiles and for getting autofocus working on MuCaT-3
Nothing very interesting. Sorry

MCD dataformat is 512 bytes of header:
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
  
Followed by a dark frame and a light frame and a number of projections, data is 16bit unsigned. For the example MCD file (2013-07-13a.mcd) the sizes are 1020x800. 

