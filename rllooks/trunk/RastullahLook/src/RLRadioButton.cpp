/************************************************************************
    filename:   RLRadioButton.cpp
    created:    21/5/2004
    author:     Paul D Turner
    
    purpose:    Implementation of Rastullah look Radio Button widget
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "RLRadioButton.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Constants
*************************************************************************/
// type name for this widget
const utf8  RLRadioButton::WidgetTypeName[] = "RastullahLook/RadioButton";

const utf8  RLRadioButton::ImagesetName[]           = "RastullahLook";
const utf8  RLRadioButton::NormalImageName[]        = "RadioButtonNormal";
const utf8  RLRadioButton::HighlightImageName[]     = "RadioButtonHover";
const utf8  RLRadioButton::SelectMarkImageName[]    = "RadioButtonMark";

const float RLRadioButton::LabelPadding             = 4.0f;


/*************************************************************************
    Constructor for Rastullah Look Radio Button objects.
*************************************************************************/
RLRadioButton::RLRadioButton(const String& type, const String& name) :
    RadioButton(type, name)
{
    Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

    // setup cache of image pointers
    d_normalImage       = &iset->getImage(NormalImageName);
    d_hoverImage        = &iset->getImage(HighlightImageName);
    d_selectMarkImage   = &iset->getImage(SelectMarkImageName);
}


/*************************************************************************
    Destructor for RLRadioButton objects.
*************************************************************************/
RLRadioButton::~RLRadioButton(void)
{
}


/*************************************************************************
    render the radio button in the normal state.    
*************************************************************************/
void RLRadioButton::drawNormal(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha_comp));

    //
    // draw the images
    //
    Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_normalImage->getHeight()) * 0.5f), z);
    d_normalImage->draw(pos, clipper, colours);

    if (d_selected)
    {
        d_selectMarkImage->draw(pos, clipper, colours);
    }

    //
    // Draw label text
    //
    absrect.d_top   += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
    absrect.d_left  += d_normalImage->getWidth();
    colours.setColours(d_normalColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


/*************************************************************************
    render the radio button in the hover / highlighted state.
*************************************************************************/
void RLRadioButton::drawHover(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha_comp));

    //
    // draw the images
    //
    Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_hoverImage->getHeight()) * 0.5f), z);
    d_hoverImage->draw(pos, clipper, colours);

    if (d_selected)
    {
        d_selectMarkImage->draw(pos, clipper, colours);
    }

    //
    // Draw label text
    //
    absrect.d_top   += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
    absrect.d_left  += d_hoverImage->getWidth();
    colours.setColours(d_hoverColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


/*************************************************************************
    render the radio button in the pushed state.    
*************************************************************************/
void RLRadioButton::drawPushed(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha_comp));

    //
    // draw the images
    //
    Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_normalImage->getHeight()) * 0.5f), z);
    d_normalImage->draw(pos, clipper, colours);

    if (d_selected)
    {
        d_selectMarkImage->draw(pos, clipper, colours);
    }

    //
    // Draw label text
    //
    absrect.d_top   += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
    absrect.d_left  += d_normalImage->getWidth();
    colours.setColours(d_pushedColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


/*************************************************************************
    render the radio button in the disabled state   
*************************************************************************/
void RLRadioButton::drawDisabled(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha_comp));

    //
    // draw the images
    //
    Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_normalImage->getHeight()) * 0.5f), z);
    d_normalImage->draw(pos, clipper, colours);

    if (d_selected)
    {
        d_selectMarkImage->draw(pos, clipper, colours);
    }

    //
    // Draw label text
    //
    absrect.d_top   += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
    absrect.d_left  += d_normalImage->getWidth();
    colours.setColours(d_disabledColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
    Create, initialise and return a RLRadioButton
*************************************************************************/
Window* RLRadioButtonFactory::createWindow(const String& name)
{
    RLRadioButton* wnd = new RLRadioButton(d_type, name);
    wnd->initialise();

    return wnd;
}

} // End of  CEGUI namespace section
