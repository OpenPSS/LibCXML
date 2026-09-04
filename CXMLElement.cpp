#include "CXMLElement.hpp"
#include "CXMLFile.hpp"
#include "CXMLAttribute.hpp"


namespace LibCXML {

	CXMLElement::CXMLElement(const std::string& cxmlFile, const std::string& magic) {
		this->reader = std::make_unique<CXMLReader>(cxmlFile, magic.c_str());
		this->reader->TreeTable->Seek(0);
		this->attributes = std::vector<CXMLAttributeBase*>();
		readCurrentElement();
	}

	void CXMLElement::readCurrentAttribute() {
		CxmlAttributeHeader attributeHeader;
		reader->TreeTable->Read(&attributeHeader, sizeof(CxmlAttributeHeader));

		reader->StringTable->Seek(attributeHeader.name);
		std::string attributeName = std::string(reader->StringTable->ReadStr());

		std::vector<float> floatArr;
		std::vector<int> intArr;
		std::vector<uint8_t> buffer;
		int ref;

		switch (attributeHeader.type) {
		case TYPE_INT:
			this->attributes.push_back(new CXMLAttribute<int>(attributeName, attributeHeader.value));
			break;
		case TYPE_FLOAT:
			this->attributes.push_back(new CXMLAttribute<float>(attributeName, (float)attributeHeader.value));
			break;
		case TYPE_STRING:
			reader->StringTable->Seek(attributeHeader.value);
			this->attributes.push_back(new CXMLAttribute<std::string>(attributeName, std::string(reader->StringTable->ReadStr(), attributeHeader.sz)));
			break;
		case TYPE_WSTRING:
			reader->WStringTable->Seek(attributeHeader.value * sizeof(wchar_t));
			this->attributes.push_back(new CXMLAttribute<std::wstring>(attributeName, std::wstring(reader->WStringTable->ReadWStr(), attributeHeader.sz)));
			break;
		case TYPE_HASH:
			reader->HashTable->Seek(attributeHeader.value * sizeof(int));
			this->attributes.push_back(new CXMLAttribute<int>(attributeName, reader->HashTable->ReadInt()));
			break;
		case TYPE_INTEGER_ARRAY:
			reader->IntArrayTable->Seek(attributeHeader.value * sizeof(int));
			intArr.resize(0);
			intArr.reserve(attributeHeader.sz);
			for (uint32_t i = 0; i < attributeHeader.sz; i++)
				intArr.push_back(reader->IntArrayTable->ReadInt());
			this->attributes.push_back(new CXMLAttribute<std::vector<int>>(attributeName, intArr));
			break;
		case TYPE_FLOAT_ARRAY:
			reader->FloatArrayTable->Seek(attributeHeader.value * sizeof(float));
			floatArr.resize(0);
			floatArr.reserve(attributeHeader.sz);
			for (uint32_t i = 0; i < attributeHeader.sz; i++)
				floatArr.push_back(reader->FloatArrayTable->ReadFloat());
			this->attributes.push_back(new CXMLAttribute<std::vector<float>>(attributeName, floatArr));
			break;
		case TYPE_FILE:				
			reader->FileTable->Seek(attributeHeader.value);
			buffer.resize(attributeHeader.sz);
			reader->FileTable->Read(buffer.data(), attributeHeader.sz);
			this->attributes.push_back(new CXMLAttribute<CXMLStream>(attributeName, buffer.data(), attributeHeader.sz));
			break;
		case TYPE_ID_REF:
			reader->IdTable->Seek(attributeHeader.value);
			ref = reader->IdTable->ReadInt();
			this->attributes.push_back(new CXMLAttribute<std::string>(attributeName, std::string(reader->IdTable->ReadStr())));
			break;
		case TYPE_ID:
			reader->IdTable->Seek(attributeHeader.value);
			this->attributes.push_back(new CXMLAttribute<std::string>(attributeName, std::string(reader->IdTable->ReadStr())));
			break;
		case TYPE_ID_HASH_REF:
			reader->HashIdTable->Seek(attributeHeader.value);
			ref = reader->HashIdTable->ReadInt();
			this->attributes.push_back(new CXMLAttribute<int>(attributeName, reader->HashIdTable->ReadInt()));
			break;
		case TYPE_ID_HASH:
			reader->HashIdTable->Seek(attributeHeader.value * sizeof(int));
			this->attributes.push_back(new CXMLAttribute<int>(attributeName, reader->HashIdTable->ReadInt()));
			break;
		case TYPE_NONE:
		default:
			break;
		}
	}

	void CXMLElement::readCurrentElement() {
		reader->TreeTable->Read(&curElemPtr, sizeof(CxmlElementHeader));
		reader->StringTable->Seek(this->curElemPtr.elementName);
		elementName = std::string(this->reader->StringTable->ReadStr());

		for (CXMLAttributeBase* attributeBase : this->attributes) {
			delete attributeBase;
		}

		this->attributes.clear();

		if (this->curElemPtr.numAttributes != -1) {
			for (uint32_t i = 0; i < this->curElemPtr.numAttributes; i++) {
				readCurrentAttribute();
			}
		}
	}

	std::string CXMLElement::ElementName() {
		return elementName;
	}

	CXMLElement* CXMLElement::PrevSibling() {
		if (curElemPtr.prevSibling == -1)
			return NULL;

		reader->TreeTable->Seek(curElemPtr.prevSibling);
		readCurrentElement();
		return this;
	}

	CXMLElement* CXMLElement::FirstChild() {
		if (curElemPtr.firstChild == -1)
			return NULL;

		reader->TreeTable->Seek(curElemPtr.firstChild);
		readCurrentElement();
		return this;
	}


	CXMLElement* CXMLElement::LastChild() {
		if (curElemPtr.lastChild == -1)
			return NULL;

		reader->TreeTable->Seek(curElemPtr.lastChild);
		readCurrentElement();
		return this;
	}

	CXMLElement* CXMLElement::NextSibling() {
		if (curElemPtr.nextSibling == -1)
			return NULL;

		reader->TreeTable->Seek(curElemPtr.nextSibling);
		readCurrentElement();
		return this;
	}

	CXMLElement* CXMLElement::ParentElement() {
		if (curElemPtr.parentElement == -1)
			return NULL;

		reader->TreeTable->Seek(curElemPtr.parentElement);
		readCurrentElement();
		return this;
	}

	CXMLElement::~CXMLElement() {

		for (CXMLAttributeBase* attributeBase : this->attributes) {
			delete attributeBase;
		}
		this->attributes.clear();
	}

	bool CXMLElement::HasParentElement() {
		if (curElemPtr.parentElement == -1)
			return false;
		else
			return true;
	}
	bool CXMLElement::HasPrevSibling() {
		if (curElemPtr.prevSibling == -1)
			return false;
		else
			return true;
	}
	bool CXMLElement::HasNextSibling() {
		if (curElemPtr.nextSibling == -1)
			return false;
		else
			return true;
	}
	bool CXMLElement::HasFirstChild() {
		if (curElemPtr.firstChild == -1)
			return false;
		else
			return true;
	}
	bool CXMLElement::HasLastChild() {
		if (curElemPtr.lastChild == -1)
			return false;
		else
			return true;
	}

}
