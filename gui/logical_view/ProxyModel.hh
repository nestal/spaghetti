/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#pragma once

#include <QtCore/QSortFilterProxyModel>

namespace gui {

class LogicalModel;

class ProxyModel : public QSortFilterProxyModel
{
public:
	ProxyModel(LogicalModel *model);
	
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
	
	bool IsShowAll() const;
	void SetShowAll(bool value);
	
private:
	bool m_show_all{false};
};
	
} // end of namespace
