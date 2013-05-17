
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

#include "page.hpp"

#include <poppler-document.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>

#include <highgui/highgui.hpp>

#include "document.hpp"


using namespace cv; 
using namespace std;

DocumentPage::DocumentPage(Document* doc, int index, int flags) 
{
	_index = index; 
	_page =  doc->popplerDocument()->create_page(_index);
	if (! (flags & NO_AUTO_RENDER)) 
	{
		render();
	}
}

void DocumentPage::render() 
{
	
	poppler::page_renderer renderer; 
	if (renderer.render_page(_page).save("img.png", "png")) {
			_img = imread("img.png", 0);
	}
}

Mat DocumentPage::img() 
{
	return _img; 
}

vector<char> DocumentPage::text(MatPart part) 
{
	poppler::ustring utxt = _page->text(poppler::rectf(part.x,part.y,part.width,part.height), poppler::page::physical_layout);
	vector<char> utf8arr = utxt.to_utf8();
	string asciiarr = utxt.to_latin1();
	int i = 0; 
	while (asciiarr[i] != '\0') i++;
	if (utf8arr.at(utf8arr.size() - 1) == '\0') utf8arr.erase(utf8arr.end()-1);
	for (i; i<asciiarr.size(); i++) 
	{
		if (asciiarr[i] != '\0') 
		{
			utf8arr.push_back(asciiarr[i]);
		}
	}
	return utf8arr; 
}

poppler::page* DocumentPage::popplerDocumentPage() 
{
	return _page; 
}

DocumentPage::~DocumentPage() 
{
	delete _page; 
}
