/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/23/17.
//

#include "ProxyModel.hh"

#include "LogicalModel.hh"

namespace gui {
namespace logical_view {


ProxyModel::ProxyModel(LogicalModel *model) :
	QSortFilterProxyModel{model}
{
	setSourceModel(model);
}

}} // end of namespace
