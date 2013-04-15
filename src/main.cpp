
/*
 * Copyright (c) 2013 Stefan Nozinic
 *
 * This file is part of the pdf2epub. Pdf2epub is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * Pdf2epub is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <poppler-document.h> 
#include <poppler-page.h>
#include <poppler-page-renderer.h>

#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

#include <iostream>

using namespace std; 
using namespace cv;

#define COLUMNS_SUM 0
#define ROWS_SUM 1

#define DIVIDE_BY_COLUMNS 0 
#define DIVIDE_BY_ROWS 1

// Function which does nothing 
void pass() {char byte;}

typedef struct MatPart MatPart;

struct MatPart
{
	Mat mat; 
	int x,y,height,width; 
	vector<MatPart> childs;
};

// Define functions
vector<int> xy_cut(vector<int>, int);
Rect getRectAround(vector<Point>);
void divideIntoBlocks(Mat, vector<MatPart>&, int, bool);
void processDocument(string, int, int);
void draw_xy_lines(Mat&, vector<int>);
void print_int_vector(vector<int>);
vector<MatPart> divByRow(MatPart, vector<int>);
vector<MatPart> divByCol(MatPart, vector<int>);
vector<int> genSum(Mat, int);
void showParts(Mat, MatPart);
void showPartAsRect(Mat, MatPart, string);

void print_int_vector(vector<int> v) 
{
	cout << "\n\nVector: " << v.size() << " elements:\n"; 
	for (int i = 0; i<v.size(); i++) 
	{
		cout << i << ": " << v.at(i) << endl; 
	}
	cout << "\n";
}

vector<int> xy_cut(vector<int> sum, int max_whitespace = 7) 
{
	vector<int> result; 
	int state = 1; 
	int c = 0; 
	for (int i = 0; i<sum.size(); i++) 
	{
		int p = sum.at(i);
		if (p == 255 && state == 1) c++; 
		else if (p < 255 && state == 0) c++;
		else if (p == 255 && state == 0) 
		{
			c = 1; 
			state = 1; 
			//result.push_back(i);
		}
		else if (p < 255 && state == 1) 
		{
			if (c >= max_whitespace) 
			{
				result.push_back(i-1);
				result.push_back(i - c);
			}
			c = 0; 
			state = 0; 
		}
	}
	if (result.empty()) 
	{
		return result;
	}
	result.push_back(sum.size() - c);
	sort(result.begin(), result.end());
	result.erase(result.begin());
	return result;
}

Rect getRectAround(vector<Point> points) 
{
	int maxx = -1, maxy = -1, miny = -1, minx = -1;
	int i;
	// Find maxx 
	for (i = 0; i<points.size(); i++) 
	{
		if (maxx == -1 || maxx > points.at(i).x) maxx = points.at(i).x;
	}
	
	// Find maxy 	
	for (i = 0; i<points.size(); i++) 
	{
		if (maxy == -1 || maxy > points.at(i).y) maxy = points.at(i).y;
	}

	// Find minx 
	for (i = 0; i<points.size(); i++) 
	{
		if (minx == -1 || minx < points.at(i).x) minx = points.at(i).x;
	}

	// Find miny
	for (i = 0; i<points.size(); i++) 
	{
		if (miny == -1 || miny < points.at(i).y) miny = points.at(i).y;
	}
	return Rect(maxx, maxy, minx - maxx, miny - maxy);
}

void divideIntoBlocks(MatPart &part, int type, bool recursive_call = false) 
{
	vector<int> sum = genSum(part.mat, type); 
	print_int_vector(sum);
	vector<int> result = xy_cut(sum, 10);
	print_int_vector(result);
	if (type == DIVIDE_BY_ROWS) 
	{
		part.childs = divByRow(part, result);
	}
	else 
	{
		part.childs = divByCol(part, result);
	}
	cout << part.childs.size() << endl;
	if (part.childs.size() <= 1 && recursive_call) return; 
	for (int i = 0; i<part.childs.size(); i++) 
	{
		divideIntoBlocks(part.childs.at(i), (type + 1) % 2, true);
	}
}

vector<int> genSum(Mat img, int type) 
{
	if (type == ROWS_SUM) 
	{
		
		vector<int> sum; 
		for (int i = 0; i<img.rows; i++) 
		{
			int z = 0; 
			for (int j = 0; j<img.cols; j++) 
			{
				z = z + (int) img.data[img.step[0]*i + img.step[1]*j];
			}
			sum.push_back(z / img.cols);
		}
		return sum;
	}
	else 
	{
		vector<int> sum; 
		for (int i = 0; i<img.cols; i++) 
		{
			int z = 0; 
			for (int j = 0; j<img.rows; j++) 
			{
				z = z + (int) img.data[img.step[0]*j + img.step[1]*i];
			}
			sum.push_back(z / img.rows);
		}
		return sum;
	}
}

void processDocument(string filename, int first_page_index, int last_page_index) 
{
	
	poppler::document* document = poppler::document::load_from_file(filename);
	
	if (last_page_index < first_page_index) last_page_index = document->pages();

	for (int i = first_page_index; i<last_page_index; i++) {
		poppler::page* page = document->create_page(i);
		poppler::page_renderer renderer; 
		if (renderer.render_page(page).save("img.png", "png")) {
			MatPart whole; 
			whole.mat = imread("img.png", 0);
			whole.x = 0; 
			whole.y = 0; 
			whole.height = whole.mat.cols; 
			whole.width = whole.mat.rows;
			divideIntoBlocks(whole, DIVIDE_BY_ROWS);
			showParts(whole.mat, whole);
		}
		else 
		{
			cout << "Saving page " << i << " failed...\n";
		}
	}
	//cout << page->text().to_latin1() << endl; 
}

void draw_xy_lines(Mat &img, vector<int> points) 
{
	if (points.size() % 2 == 1) 
	{
		points.push_back(points.size());
	}
	for (int i = 0; i<points.size(); i = i + 2) 
	{
		rectangle(img, Point(0, points.at(i)), Point(img.cols, points.at(i+1)), Scalar(0), -1);
	}
}

vector<MatPart> divByRow(MatPart src, vector<int> points) 
{
	vector<MatPart> parts; 
	for (int i = 0; i<points.size(); i += 2) 
	{
		MatPart part; 
		part.x = src.x;
		part.y = src.y + points.at(i); 
		part.width = src.mat.cols;
		part.height = points.at(i+1) - points.at(i);
		part.mat = Mat(src.mat, Rect(0, points.at(i), part.width, part.height));
		parts.push_back(part); 
	}
	return parts; 
}

vector<MatPart> divByCol(MatPart src, vector<int> points) 
{
	vector<MatPart> parts;
	for (int i = 0; i<points.size(); i += 2) 
	{
		MatPart part;
		part.x = src.x + points.at(i);
		part.y = src.y;
		part.width = points.at(i+1) - points.at(i);
		part.height = src.mat.rows;
		part.mat = Mat(src.mat, Rect(points.at(i), 0, part.width, part.height));
		parts.push_back(part);
	}
	return parts;
}

void showPartAsRect(Mat img, MatPart part, string wn = "img") 
{
	rectangle(img, Point(part.x, part.y), Point(part.x + part.width, part.y + part.height), Scalar(0), 2);
	imshow(wn, img); 
	waitKey(0);
}

void showParts(Mat img, MatPart whole) 
{
	namedWindow("img");
	if (whole.childs.empty()) 
	{
		showPartAsRect(img, whole);
	}
	else 
	{
		for (int i = 0; i<whole.childs.size(); i++) 
		{
			showParts(img, whole.childs.at(i)); 
		}
	}
}

int main(int argc, char* argv[]) 
{
	processDocument(
	  "/me/data/Documents/Books/Computer science/Algorithms/Introduction to Algorithms, 3nd Ed - Thomas H. Cormen.pdf", 0, 30);
	processDocument("/me/data/Documents/Books/Computer science/Scientific methods in computer science.pdf", 0, -1);
	processDocument("/me/data/Documents/Books/Computer science/c c++/NCURSES-Programming-HOWTO.pdf", 0, 15);
	processDocument("/me/data/Documents/Books/Math/Matematiranje.in.rs/4. Godina/5. Integrali/2.INTEGRALI ZADACI - I deo.pdf",0,-1);
	processDocument("/me/data/Documents/EBook tutorials/xy-cuts.pdf", 0, -1);
	return 0; 
}
