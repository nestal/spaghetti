/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/4/17.
//

#pragma once

#include <QtCore/QtGlobal>

namespace gui {
namespace classgf {

struct ItemRenderingOptions;

/**
 * \brief Interface for the BaseItem to access the ClassView
 *
 * The purpose of Viewport is the expose functions from ClassView to the items
 * being drawn on the view. In the ClassItem::paint() function, the ClassItem
 * will want to read the rendering options, which belongs to the ClassView
 * that the item is drawing itself on. This means that if there are two
 * views attached to the same scene, the items can be drawn using two sets
 * of rendering options correspond to each view.
 */
class Viewport
{
protected:
	~Viewport() = default;
	
public:
	virtual const classgf::ItemRenderingOptions& Setting() const = 0;
	virtual qreal ZoomFactor() const = 0;
};

}} // end of namespace
