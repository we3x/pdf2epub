
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

#include "segmentation.hpp"

using namespace std; 
using namespace cv; 

vector<int> xy_cut(vector<int> sum, int max_whitespace) 
{
	vector<int> result; 
	int state = 1; 
	int c = 0; 
	int i;
	
	if (sum.empty()) return result; 

	for (i = 0; i<sum.size(); i++) 
	{
		int p = sum.at(i);
		if (p == 255 && state == 1) c++; 
		else if (p < 255 && state == 0) c++;
		else if (p == 255 && state == 0) 
		{
			c = 1; 
			state = 1; 
		}
		else if (p < 255 && state == 1) 
		{
			if (c >= max_whitespace) 
			{
				result.push_back(i-c);
				result.push_back(i);
			}
			c = 1; 
			state = 0; 
		}
	}
	bool found = false; 
	for (i = 0; i<7 && i<sum.size(); i++) 
	{
		if (sum.at(i) < 255) 
		{
			found = true; 
			break; 
		}
	}
	if (found) result.push_back(0);
	
	if (sum.at(sum.size() - 1) == 255) result.push_back(sum.size() - c);
	else result.push_back(sum.size() - 1);
	sort(result.begin(), result.end());
	if (!found) result.erase(result.begin());
	return result;
}


void divideIntoBlocks(MatPart &part, int type, bool recursive_call) 
{
	int max_whitespace;
	if (type == DIVIDE_BY_ROWS) max_whitespace = 7; 
	else max_whitespace = 10;
	vector<int> sum = genSum(part.mat, type); 
	vector<int> result = xy_cut(sum, max_whitespace);
	if (type == DIVIDE_BY_ROWS) 
	{
		part.childs = divByRow(part, result);
	}
	else 
	{
		part.childs = divByCol(part, result);
	}
	if (part.childs.size() <= 1 && recursive_call) return; 
	for (int i = 0; i<part.childs.size(); i++) 
	{
		divideIntoBlocks(part.childs.at(i), (type + 1) % 2, true);
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

double avgRowSize(MatPart part) 
{
	vector<MatPart> parts = divByRow(part, xy_cut(genSum(part.mat, ROWS_SUM), 1));
	double sum = 0; 
	double count = (double) parts.size();
	for (int i = 0; i<count; i++) 
	{
		sum = sum + parts.at(i).mat.rows;
	}
	if (count == 0) return 0; 
	return (double) (sum / count);
}
