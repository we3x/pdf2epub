
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

#ifndef PDF2EPUB_SEGMENTATION_HPP
#define PDF2EPUB_SEGMENTATION_HPP

#include <vector> 
#include <core/core.hpp>

#include "matpart.hpp"

#define COLUMNS_SUM 0
#define ROWS_SUM 1

#define DIVIDE_BY_COLUMNS 0 
#define DIVIDE_BY_ROWS 1

/*
 * xy-cuts implementation 
 *
 * Arguments: 
 * 	1: std::vector<int> - sums
 * 	2: int - min whitespace which need to be reached for changing state
 *
 * Returns: vector<int> - cut points
 * */
std::vector<int> xy_cut(std::vector<int>, int max_whitespace = 7);

/*
 * Divides MatPart on small parts using xy-cuts algorithm and puts that parts into childs vector 
 *
 * Arguments: 
 * 	1: MatPart - MatPart object which will be divided 
 * 	2: int - type of division (DIVIDE_BY_ROWS or DIVIDE_BY_COLUMNS)
 * 	3: bool - is it recursive call, it is recommanded to set it to false ... (default: false)
 * */
void divideIntoBlocks(MatPart&, int, bool recursive_call = false);

/*
 * Divides into rows by results given from xy-cuts algorithm 
 *
 * Arguments: 
 * 	1: MatPart - part which will be divided 
 * 	2: std::vector<int> - results
 *
 * Returns: vector<MatPart> - parts
 * */
std::vector<MatPart> divByRow(MatPart, std::vector<int>);


/*
 * Divides into columns by results given from xy-cuts algorithm 
 *
 * Arguments: 
 * 	1:MatPart - part which will be divided 
 * 	std::vector<int> - results
 *
 * Returns: vector<MatPart> - parts
 * */
std::vector<MatPart> divByCol(MatPart, std::vector<int>);

/*
 * Generate histogram by rows/columns
 *
 * Arguments: 
 * 	1: cv::Mat - img 
 * 	2: int - type (COLUMNS_SUM or ROWS_SUM)
 * */
std::vector<int> genSum(cv::Mat, int);

/*
 * Get average size of row in specified part 
 *
 * Arguments: 
 * 	1: MatPart - part which will be processed 
 *
 * Returns: double - average size of row 
 * */
double avgRowSize(MatPart);

#endif // PDF2EPUB_SEGMENTATION_HPP
