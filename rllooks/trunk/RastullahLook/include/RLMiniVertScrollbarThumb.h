/************************************************************************
	filename: 	RLMiniVertScrollbarThumb.h
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Interface to thumb for Taharez mini vertical scroll bar.
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
#ifndef _RLMiniVertScrollbarThumb_h_
#define _RLMiniVertScrollbarThumb_h_

#include "RLModule.h"
#include "elements/CEGUIThumb.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Thumb class as used in the Taharez mini vertical scroll bar.
*/
class RASTULLAHLOOK_API RLMiniVertScrollbarThumb : public Thumb
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// Image names
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	NormalImageName[];				//!< Name of the image to use for normal rendering (used for sizes only).
	static const utf8	NormalTopImageName[];			//!< Name of the image to use for normal rendering (top end).
	static const utf8	NormalMiddleImageName[];		//!< Name of the image to use for normal rendering (mid section).
	static const utf8	NormalBottomImageName[];		//!< Name of the image to use for normal rendering (bottom end).
	static const utf8	HighlightTopImageName[];		//!< Name of the image to use for hover / highlighted rendering (top end).
	static const utf8	HighlightMiddleImageName[];		//!< Name of the image to use for hover / highlighted rendering (mid section).
	static const utf8	HighlightBottomImageName[];		//!< Name of the image to use for hover / highlighted rendering (bottom end).


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	RLMiniVertScrollbarThumb(const String& type, const String& name);
	virtual ~RLMiniVertScrollbarThumb(void);


protected:
	/*************************************************************************
		Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		render the thumb in the normal state.
	*/
	virtual void	drawNormal(float z);

	/*!
	\brief
		render the thumb in the hover / highlighted state.
	*/
	virtual void	drawHover(float z);

	/*!
	\brief
		render the thumb in the pushed state.
	*/
	virtual void	drawPushed(float z)		{drawHover(z);}

	/*!
	\brief
		render the thumb in the disabled state
	*/
	virtual void	drawDisabled(float z);


	/*************************************************************************
		Overridden events
	*************************************************************************/
	virtual void	onSized(WindowEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	const Image*	d_normalImage;				//!< Image to render in normal state (used to get sizes only)
	const Image*	d_normalTopImage;			//!< Image to render in normal state (top end)
	const Image*	d_normalMiddleImage;		//!< Image to render in normal state (mid section)
	const Image*	d_normalBottomImage;		//!< Image to render in normal state (bottom end)
	const Image*	d_highlightTopImage;		//!< Image to render in highlighted state (top end).
	const Image*	d_highlightMiddleImage;		//!< Image to render in highlighted state (mid section).
	const Image*	d_highlightBottomImage;		//!< Image to render in highlighted state (bottom end).
};


/*!
\brief
	Factory class for producing RLMiniVertScrollbarThumb objects
*/
class RASTULLAHLOOK_API RLMiniVertScrollbarThumbFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	RLMiniVertScrollbarThumbFactory(void) : WindowFactory(RLMiniVertScrollbarThumb::WidgetTypeName) { }
	~RLMiniVertScrollbarThumbFactory(void){}


	/*!
	\brief
		Create a new Window object of whatever type this WindowFactory produces.

	\param name
		A unique name that is to be assigned to the newly created Window object

	\return
		Pointer to the new Window object.
	*/
	Window*	createWindow(const String& name);


	/*!
	\brief
		Destroys the given Window object.

	\param window
		Pointer to the Window object to be destroyed.

	\return
		Nothing.
	*/
	virtual void	destroyWindow(Window* window)	 { if (window->getType() == d_type) delete window; }
};


} // End of  CEGUI namespace section


#endif	// end of guard _RLMiniVertScrollbarThumb_h_
