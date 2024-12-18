#ifndef LIB_CXML_ELEMENT_H
#define LIB_CXML_ELEMENT_H 1
#include "CXMLFile.hpp"
#include "CXMLReader.hpp"
#include "CXMLAttributeBase.hpp"
#include <vector>

namespace LibCXML {

	class CXMLElement {
	private:
		void readCurrentElement();
		void readCurrentAttribute();

		CXMLReader* reader;
		CxmlElementHeader curElemPtr;
		std::string elementName;

		std::vector<CXMLAttributeBase*> attributes;
	public:
		CXMLElement(const std::string& cxmlFile, const std::string& magic);
		~CXMLElement();
		std::string ElementName();
		template <typename T> T GetAttribute(const std::string& attributeName) {
			for (CXMLAttributeBase* attribute : this->attributes) {
				if (attribute->AttributeName() == attributeName) {
					return (T)attribute;
				}
			}
			return NULL;
		}

		bool HasParentElement();
		bool HasPrevSibling();
		bool HasNextSibling();
		bool HasFirstChild();
		bool HasLastChild();


		CXMLElement* ParentElement();
		CXMLElement* PrevSibling();
		CXMLElement* NextSibling();
		CXMLElement* FirstChild();
		CXMLElement* LastChild();
	};

}

#endif
