
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

#include "document.hpp"
#include "page.hpp" 
#include "debug.hpp"
#include "matpart.hpp"
#include "segmentation.hpp"
#include "img_debug.hpp"

#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

#include <iostream>

using namespace std; 
using namespace cv;


// Define functions
void processDocument(string, int, int);

void processDocument(string filename, int first_page_index, int last_page_index = -1) 
{
	cout << "Processing " << filename << endl; 

	Document doc(filename);

	if (last_page_index < first_page_index) last_page_index = doc.pageCount();

	for (int i = first_page_index; i<last_page_index; i++) {
		cout << "\tPage at index: "<< i << endl; 
		DocumentPage p(&doc, i);
		MatPart whole = genMatPartFromMat(p.img());
		divideIntoBlocks(whole, DIVIDE_BY_ROWS);
		showParts(whole.mat, whole, "Rendered page", &p);
	}
}

int main(int argc, char* argv[]) 
{
	processDocument("/me/data/Programming/Projects/Projects/pdf2epub/test/testdoc1.pdf", 0, -1);
	processDocument(
	  "/me/data/Documents/Books/Computer science/Algorithms/Introduction to Algorithms, 3nd Ed - Thomas H. Cormen.pdf", 0, 30);
	processDocument("/me/data/Documents/Books/Computer science/Scientific methods in computer science.pdf", 0, -1);
	processDocument("/me/data/Documents/Books/Computer science/c c++/NCURSES-Programming-HOWTO.pdf", 0, 15);
	processDocument("/me/data/Documents/Books/Math/Matematiranje.in.rs/4. Godina/5. Integrali/2.INTEGRALI ZADACI - I deo.pdf",0,-1);
	processDocument("/me/data/Documents/EBook tutorials/xy-cuts.pdf", 0, -1);
	return 0; 
}
