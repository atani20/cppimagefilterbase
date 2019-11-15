#include <iostream>
#include <fstream> 
#include <vector>
#include <sstream>
#include "png_toolkit.h"
#include "filter.h"
#include "rect.h"
using namespace std;

vector <pair<string, Rect>> ParseConfig(string fileName) {
	ifstream file(fileName);
	string str;
	vector <pair<string, Rect>> config;
	while (getline(file, str)) {
		int pos = str.find(" ");
		if (pos == std::string::npos)
			continue;
		string filterName = str.substr(0, pos);
		str.erase(0, pos + 1);
		stringstream sStr(str);
		vector<int> coef;
		for (int i; sStr >> i;) {
			coef.push_back(i);
		//	if (sStr.peek() == ' ')
			//	sStr.ignore();
		}
		if (coef.size() == 4) {
			Rect rc(coef[0], coef[1], coef[2], coef[3]);
			config.push_back(make_pair(filterName, rc));
		}
	}
	file.close();
	return config;
}

int main( int argc, char *argv[] )
{
    try
    {
		if (argc != 4) {
			throw "Not enough arguments";
			return 0;
		}
        png_toolkit studTool;
        studTool.load(argv[2]);
		vector <pair<string, Rect>> config = ParseConfig(argv[1]);
		image_data imgData = studTool.getPixelData();
		for (auto& curEditor: config) {
			Filter *filter;
			if (curEditor.first == "Red") 
				filter = new Red;
			else if (curEditor.first == "Threshold") 
				filter = new Threshold;
			else if (curEditor.first == "Blur")
				filter = new Blur;
			else if (curEditor.first == "Edge")
				filter = new Edge;
			else 
				continue;
			curEditor.second.transform(imgData.h, imgData.w);
			filter->set(imgData, curEditor.second);
			delete filter;
		}
        studTool.save(argv[3]);

    }
    catch (const char *str)
    {
        std::cout << "Error: " << str << std::endl;
        return 1;
    }

    return 0;
}
