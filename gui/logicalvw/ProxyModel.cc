/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include <iostream>
#include "ProxyModel.hh"

#include "LogicalModel.hh"

#include "codebase/Entity.hh"

namespace gui {
namespace logicalvw {

ProxyModel::ProxyModel(LogicalModel *model) :
	QSortFilterProxyModel{model}
{
	setSourceModel(model);
}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	if (m_show_all)
		return true;
	else
	{
		auto source = dynamic_cast<LogicalModel *>(sourceModel());
		auto entity = source->At(source->index(source_row, 0, source_parent));
		
		return entity && entity->IsUsed();
	}
}

bool ProxyModel::IsShowAll() const
{
	return m_show_all;
}

void ProxyModel::SetShowAll(bool value)
{
	if (value != m_show_all)
	{
		m_show_all = value;
		invalidate();
	}
}
	
}} // end of namespace
