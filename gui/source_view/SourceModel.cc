/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 2/12/17.
//

#include "SourceModel.hh"
#include <QtCore/QJsonObject>

namespace gui {
namespace source_view {


SourceModel::SourceModel(const QString& fname, QObject *parent) :
	QObject{parent},
	m_fname{fname}
{
}

void SourceModel::AddEntity(const std::string&, const QPointF&)
{
}

void SourceModel::SetRect(const QRectF&)
{
}

std::string SourceModel::Name() const
{
	return m_fname.toStdString();
}

void SourceModel::SetName(const QString&)
{
}

bool SourceModel::CanRename() const
{
	return false;
}

void SourceModel::Load(const QJsonObject& json)
{
	m_fname  = json["fname"].toString();
	m_line   = static_cast<unsigned>(json["line"].toInt(0));
	m_column = static_cast<unsigned>(json["column"].toInt(0));
	
	emit OnLocationChanged();
}

QJsonObject SourceModel::Save() const
{
	QJsonObject json;
	json.insert("fname",  m_fname);
	json.insert("line",   static_cast<qint64>(m_line));
	json.insert("column", static_cast<qint64>(m_column));
	return json;
}

unsigned SourceModel::Line() const
{
	return m_line;
}

unsigned SourceModel::Column() const
{
	return m_column;
}

void SourceModel::SetLocation(const QString& fname, unsigned line, unsigned column)
{
	m_fname  = fname;
	m_line   = line;
	m_column = column;
	emit OnLocationChanged();
}

bool SourceModel::IsChanged() const
{
	return false;
}

void SourceModel::UpdateCodeBase(const codebase::EntityMap*)
{
}
	
}} // end of namespace
