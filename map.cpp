#include "map.h"


Map::Map(ClpData& data):mData(data)
{
	mData = data;
	int id = 0;

    for (int type = 0; type < data.numPacketTypes; type++)
	{ 
		for (int packet = 0; packet < data.number[type]; packet++)
		{                
			Node n;
			n.id = id++;
			n.typeId = type;
			n.orientations = data.orient[type];										
			n.orientation = 0;
			n.volume = data.wid[type] * data.len[type] * data.hei[type];
			n.weight = data.wei[type];
			mPackets.push_back(n);
		}
	}

	mPheromones = new double**[mPackets.size()];
	
	//sort(mPackets.begin(), mPackets.end());

	/*for (Node n : mPackets) {
		cout << n.id << " " << n.volume << " " << n.orientation << endl;
	}*/

}

void Map::initPheromones(double initialPhValue) 
{      
	for (size_t i = 0; i < mPackets.size(); i++) 
	{
		mPheromones[i] = new double*[mPackets.size()];
		for (size_t j = 0; j < mPackets.size(); j++) 
		{			
			int orientations = mPackets.at(i).orientations * mPackets.at(j).orientations;
			mPheromones[i][j] = new double[orientations];
			for (int oi = 0; oi < mPackets.at(i).orientations; oi++)
			{	
				for (int oj = 0; oj < mPackets.at(j).orientations; oj++)
				{
					if (i == j ) 
						setPhValue(i,j,oi,oj,0);
					else
						setPhValue(i,j,oi,oj,initialPhValue);
				}
			}
		}
	}
}

void Map::evaporate() 
{
	//cout << "Evaporating map " << mPheromones << endl;
	#pragma omp parallel for 
    for (size_t i = 0; i < mPackets.size(); i++) 
	{
		for (size_t j = 0; j < mPackets.size(); j++) 
		{
			if (i == j ) continue;

			for (int oi = 0; oi < mPackets.at(i).orientations; oi++)
			{					 
				for (int oj = 0; oj < mPackets.at(j).orientations; oj++)
				{			
					double oldPhValue = getPhValue(i,j,oi,oj);
					double newPh = oldPhValue * (1-evaporationRate);
					setPhValue(i,j,oi,oj,newPh);				
				}
			}
		}
	}
}

void Map::saveHeatMap(const string filename) 
{
	int sizex = mPackets.size();
	int sizey = mPackets.size();

	
	heatmap_t* hm = heatmap_new(sizex,sizey);

	for (Node xpack : mPackets) {
		for (Node ypack : mPackets) {
			int i = xpack.id;
			int oi = xpack.orientation;
			int j = ypack.id; 
			int oj = ypack.orientation;
			double ph = getPhValue(i,j,oi,oj);
			heatmap_add_weighted_point(hm, i, j, ph);
		}
	}	
	std::vector<unsigned char> image(sizex*sizey*4);
    heatmap_render_default_to(hm, &image[0]);
   	heatmap_free(hm);

    if(unsigned error = lodepng::encode(filename, image, sizex, sizey)) {
        std::cerr << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
        return;
    }
}

/*void Map::saveHeatMap(const string filename) 
{
	int sizex = mPackets.size();
	int sizey = mPackets.size();
	
	CImg<unsigned char> image(sizex,sizey,1,3,0);
	
	for (Node xpack : mPackets) {
		for (Node ypack : mPackets) {
			int i = xpack.id;
			int j = ypack.id; 
			double ph = getPhValue(i,j,0);
			int red = ((255 * ph) / mMaxPh);
			unsigned char color_mag[] = { red, red, red };
			image.draw_point(i,j,color_mag);
			//image.draw_circle(i,j,10,color_mag);
		}
	}	
	const char * c = filename.c_str();
	image.save(c);
}*/


void Map::setPhValue(int i, int j, int oi, int oj, double p) 
{ 
	if (mMinPh > -1 && mMaxPh > -1) {
		if (p<=mMinPh) p = mMinPh;
		if (p>=mMaxPh) p = mMaxPh;		
	} 
	int o = oi*2 + oj;
	mPheromones[i][j][o] = p; 	
}

double Map::getPhValue(int i, int j, int oi, int oj) { 
	int o = oi*2 + oj;
	return mPheromones[i][j][o]; 
};


void Map::printPheromones() 
{
	cout << "Print pheromones:" <<endl;
	int n = mPackets.size();
	for (int i = 0; i < n; i++)
	{	
		for (int j = 0; j < n; j++)
		{
			int oi = mPackets.at(i).orientation;
			int oj = mPackets.at(j).orientation;
			cout << getPhValue(i,j,oi,oj) << ",";
		}
	}

	cout << endl;
}
