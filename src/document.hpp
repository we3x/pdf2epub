
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

#ifndef PDF2EPUB_DOCUMENT_HPP
#define PDF2EPUB_DOCUMENT_HPP

#include <poppler-document.h> 
#include <string>

class Document 
{
	poppler::document* _document; 

public: 
	/*
	 * Constructor - open a new PDF document 
	 *
	 * Arguments: 
	 * 	1: std::string - filename 
	 * */
	Document(std::string);

	/*
	 * Returns pointer to the poppler::document 
	 *
	 * Returns: poppler::document* 
	 * */
	poppler::document* popplerDocument();
	
	/*
	 * Return page count
	 *
	 * Returns: int
	 * */
	int pageCount(); 

	/*
	 * Destructor 
	 * */
	~Document();
private: 
	Document(const Document &);
	Document& operator=(const Document &);
};

#endif 
