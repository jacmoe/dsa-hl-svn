/************************************************************************
    filename:   RLPopupMenu.cpp
    created:    1/4/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "RLPopupMenu.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{

const utf8 RLPopupMenu::WidgetTypeName[] = "RastullahLook/PopupMenu";

const utf8	RLPopupMenu::ImagesetName[]					= "RastullahLook";
const utf8	RLPopupMenu::TopLeftFrameImageName[]		= "PopupMenuFrameTopLeft";
const utf8	RLPopupMenu::TopRightFrameImageName[]		= "PopupMenuFrameTopRight";
const utf8	RLPopupMenu::BottomLeftFrameImageName[]		= "PopupMenuFrameBottomLeft";
const utf8	RLPopupMenu::BottomRightFrameImageName[]	= "PopupMenuFrameBottomRight";
const utf8	RLPopupMenu::LeftFrameImageName[]			= "PopupMenuFrameLeft";
const utf8	RLPopupMenu::RightFrameImageName[]			= "PopupMenuFrameRight";
const utf8	RLPopupMenu::TopFrameImageName[]			= "PopupMenuFrameTop";
const utf8	RLPopupMenu::BottomFrameImageName[]			= "PopupMenuFrameBottom";
const utf8	RLPopupMenu::BackgroundImageName[]			= "Background";


/*************************************************************************
	Constructor for RLMenubar widgets.	
*************************************************************************/
RLPopupMenu::RLPopupMenu(const String& type, const String& name) :
	PopupMenu(type, name)
{
	d_backgroundColours.d_top_left		= 0xFFF4F3F5;
	d_backgroundColours.d_top_right		= 0xFFF2F2FF;
	d_backgroundColours.d_bottom_left	= 0xFFF4F3F5;
	d_backgroundColours.d_bottom_right	= 0xFFF4F3F5;

	// get images
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// store framesizes
	storeFrameSizes();

	d_frame.setImages(
		&iset->getImage(TopLeftFrameImageName),
		&iset->getImage(TopRightFrameImageName),
		&iset->getImage(BottomLeftFrameImageName),
		&iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName),
		&iset->getImage(TopFrameImageName),
		&iset->getImage(RightFrameImageName),
		&iset->getImage(BottomFrameImageName)
	);

	d_fill.setImage(&iset->getImage(BackgroundImageName));
	d_fill.setHorzFormatting(RenderableImage::HorzStretched);
	d_fill.setVertFormatting(RenderableImage::VertStretched);
}


/*************************************************************************
	Destructor for RLMenubar widgets.
*************************************************************************/
RLPopupMenu::~RLPopupMenu(void)
{
}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void RLPopupMenu::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	const Image* img;
	img = &iset->getImage(LeftFrameImageName);
	d_frameLeftSize = img->getWidth();

	img = &iset->getImage(RightFrameImageName);
	d_frameRightSize = img->getWidth();

	img = &iset->getImage(TopFrameImageName);
	d_frameTopSize = img->getHeight();

	img = &iset->getImage(BottomFrameImageName);
	d_frameBottomSize = img->getHeight();
}


/*************************************************************************
	Resizes the popup menu to exactly fit the items attached to it.
*************************************************************************/
void RLPopupMenu::sizeToContent_impl(void)
{
	// get size of content
	Size csz = getContentSize();

	// get the full size with the frame accounted for and resize the window to this
	Size rsz( csz.d_width+d_frameLeftSize+d_frameRightSize, csz.d_height+d_frameTopSize+d_frameBottomSize );

	setSize(Absolute,rsz);
}


/*************************************************************************
	Return the un-clipped pixel Rect available for rendering menu items
*************************************************************************/
Rect RLPopupMenu::getItemRenderArea(void) const
{
	Rect r = getUnclippedPixelRect();

	Point pos( d_frameLeftSize, d_frameTopSize );
	Size sz( r.getWidth()-d_frameLeftSize-d_frameRightSize, r.getHeight()-d_frameTopSize-d_frameBottomSize );

	r.setPosition( pos );
	r.setSize( sz );

	return r;
}


/*************************************************************************
	Perform the actual rendering of this RLMenubar
*************************************************************************/
void RLPopupMenu::populateRenderCache()
{
	// get the destination screen rect for this window
	Rect absrect(getAbsoluteRect());
	
	// set colour to use for the frame	
	ColourRect colours(d_backgroundColours);
//	colours.modulateAlpha(getEffectiveAlpha());

	//
	// draw the frame
	//
	d_frame.setSize(absrect.getSize());
	d_frame.setColours(colours);
	d_frame.draw(d_renderCache);

	// calculate size for middle title bar segment
	float midWidth		= absrect.getWidth() - d_frameLeftSize - d_frameRightSize;
	float midHeight		= absrect.getHeight() - d_frameTopSize - d_frameBottomSize;

	//
	// draw the centre fill
	//
	const float recip_w = 1.0f / absrect.getWidth();
	const float recip_h = 1.0f / absrect.getHeight();
	
	const float left = d_frameLeftSize*recip_w;
	const float right = left + midWidth*recip_w;
	const float top = d_frameTopSize*recip_h;
	const float bottom = top + midHeight*recip_h;
	
	colours = colours.getSubRectangle(left,right,top,bottom);
	
	d_fill.setSize(Size(midWidth, midHeight));
	d_fill.setColours(colours);
	d_fill.setPosition(Point(d_frameLeftSize, d_frameTopSize));
	d_fill.draw(d_renderCache);
}

//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Factory Methods
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

Window* RLPopupMenuFactory::createWindow(const String& name)
{
    return new RLPopupMenu(d_type, name);
}


void RLPopupMenuFactory::destroyWindow(Window* window)
{
    if (window->getType() == d_type)
    {
		delete window;
	}
}

} // End of  CEGUI namespace section
