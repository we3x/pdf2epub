
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

#ifndef PDF2EPUB_PAGE_HPP
#define PDF2EPUB_PAGE_HPP

#include "document.hpp"
#include "matpart.hpp"

#include <core/core.hpp>

#include <poppler-page.h>
#include <poppler-rectangle.h>

#include <vector>

class DocumentPage 
{

	cv::Mat _img;
	int _index;
	poppler::page *_page; 

public: 
	/*
	 * Flags for defining page flags on page creation 
	 *
	 * Values: 
	 * 	1: NO_AUTO_RENDER - Do not render page on creation (need to call DocumentPage::render() function after creating object)
	 * */
	static const int NO_AUTO_RENDER = 1;

	/*
	 * Constructor - makes a new page from a document on the defined index (page number) 
	 *
	 * Arguments: 
	 * 	1: Document* - document which represents a Document class of a working document 
	 * 	2: int - idex of a needed page 
	 * 	3: int - flags to be set on the creating object (default: 0). Seed DocumentPageFlags for more... 
	 * */
	DocumentPage(Document*, int, int flags = 0);
	
	/*
	 * Render a page to image which can be returned with DocumentPage::img() function 
	 * */
	void render();
	
	/*
	 * Returns rendered image of a page 
	 *
	 * Returns: cv::Mat
	 * */
	cv::Mat img(); 

	std::vector<char> text(MatPart);

	/*
	 * Return pointer to the poppler::page of a page 
	 *
	 * Returns: poppler::page* - poppler::page of a page 
	 * */
	poppler::page* popplerDocumentPage();

	/*
	 * Destructor - destroys garbage 
	 * */
	~DocumentPage();

};
#endif 
