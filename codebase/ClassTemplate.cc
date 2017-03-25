/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

//
// Created by nestal on 3/13/17.
//

#include "ClassTemplate.hh"

#include "EntityType.hh"
#include "Variable.hh"

#include "libclx/Cursor.hh"
#include "libclx/Type.hh"

namespace codebase {

class ClassTemplate::Instance : public DataType
{
public:
	Instance(const ClassRef& ref, bool used, const ClassTemplate *temp) :
		DataType{ref.Name(), ref.ID(), temp->Location(), temp->Parent()},
		m_temp(temp)
	{
		SetUsed(used);
	}
	
	EntityType Type() const override
	{
		return EntityType::instantiated_type;
	}
	
	using DataType::AddBase;
	using DataType::AddField;

private:
	const ClassTemplate *m_temp;
};

void ClassTemplate::VisitChild(const libclx::Cursor& child, const libclx::Cursor& self)
{
	switch (child.Kind())
	{
	case CXCursor_TemplateTypeParameter:
	{
		m_param.push_back({child.Spelling(), child.USR()});
		break;
	}
	
	default:
		DataType::VisitChild(child, self);
		break;
	}
}

/**
 * \brief Instantiate a template by replacing all its template parameters with the arguments
 * \param args  template arguments, i.e. the actual types that will replace the template parameters
 * \return the instantiated class
 */
std::unique_ptr<DataType> ClassTemplate::Instantiate(const ClassRef& ref, bool used) const
{
	auto inst = std::make_unique<Instance>(ref, used, this);

	for (auto&& base : BaseClasses())
	{
		auto arg_idx = Match(base.ID());
		inst->AddBase(
			arg_idx != m_param.size() ? ClassRef{ref.TempArgs().at(arg_idx)} : base
		);
	}
	
	for (auto&& field : Fields())
	{
		auto arg_idx = Match(field.TypeRef().ID());
		if (arg_idx < m_param.size() && arg_idx < ref.TempArgs().size())
			std::cout << "instantiating " << field.DataType() << " " << " as " << ref.TempArgs().at(arg_idx) << std::endl;
		else
			inst->AddField(field);
	}
	
	return inst;
}

std::size_t ClassTemplate::Match(const std::string& usr) const
{
	auto it = std::find_if(m_param.begin(), m_param.end(), [&usr](auto&& param)
	{
		return param.usr == usr;
	});
	return static_cast<std::size_t>(it-m_param.begin());
}

EntityType ClassTemplate::Type() const
{
	return EntityType::class_template;
}
	
} // end of namespace
