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
#include <QtCore/QJsonObject>

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

void Model::Load(const QJsonObject& json)
{
	m_fname  = json["fname"].toString();
	m_line   = static_cast<unsigned>(json["line"].toInt(0));
	m_column = static_cast<unsigned>(json["column"].toInt(0));
	
	emit OnLocationChanged();
}

QJsonObject Model::Save() const
{
	QJsonObject json;
	json.insert("fname",  m_fname);
	json.insert("line",   static_cast<qint64>(m_line));
	json.insert("column", static_cast<qint64>(m_column));
	return json;
}

unsigned Model::Line() const
{
	return m_line;
}

unsigned Model::Column() const
{
	return m_column;
}

void Model::SetLocation(const QString& fname, unsigned line, unsigned column)
{
	m_fname  = fname;
	m_line   = line;
	m_column = column;
	emit OnLocationChanged();
}

bool Model::IsChanged() const
{
	return false;
}
	
}} // end of namespace
