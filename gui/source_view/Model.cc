/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#include "Model.hh"

namespace gui {
namespace source_view {


Model::Model(const QString& fname, QObject *parent) :
	QObject{parent},
	m_fname{fname}
{
}

void Model::AddEntity(const std::string&, const QPointF&)
{
}

void Model::SetRect(const QRectF&)
{
}

std::string Model::Name() const
{
	return m_fname.toStdString();
}

void Model::SetName(const QString&)
{
}

bool Model::CanRename() const
{
	return false;
}
	
}} // end of namespace
