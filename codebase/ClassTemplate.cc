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
	Instance(const codebase::TypeRef& ref, bool used, const ClassTemplate *temp) :
		DataType{ref.Name(), ref.ID(), temp->Location(), temp->Parent()},
		m_temp(temp),
		m_args{ref.TempArgs()}
	{
		assert(m_temp);
		assert(ref.TemplateID() == m_temp->ID());
		SetUsed(used);
		
		for (auto&& base : temp->BaseClasses())
			AddBase(temp->Match(base, ref.TempArgs()));
		
		for (auto&& field : temp->Fields())
		{
			std::cout << "instantiating " << field.TypeRef() << std::endl;
			AddField(field.ReplaceType(temp->Match(field.TypeRef(), ref.TempArgs()), this));
		}
	}
	
	EntityType Type() const override
	{
		return EntityType::instantiated_type;
	}
	
	codebase::TypeRef GetTypeRef() const override
	{
		assert(m_temp);
		return codebase::TypeRef{ID(), CXType_Unexposed}.
			SetName(Name()).
			SetTemplate(m_temp->ID(), m_args);
	}
	
private:
	const ClassTemplate *m_temp;
	std::vector<codebase::TypeRef> m_args;
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
std::unique_ptr<DataType> ClassTemplate::Instantiate(const codebase::TypeRef& ref, bool used) const
{
	return std::make_unique<Instance>(ref, used, this);
}


EntityType ClassTemplate::Type() const
{
	return EntityType::class_template;
}

codebase::TypeRef ClassTemplate::Match(const codebase::TypeRef& type, const std::vector<codebase::TypeRef>& args) const
{
	auto it = std::find_if(m_param.begin(), m_param.end(), [&type](auto&& param)
	{
		return param.usr == type.ID();
	});
	return it == m_param.end() ? type : args.at(it-m_param.begin());

}
	
} // end of namespace
