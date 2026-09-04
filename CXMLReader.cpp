#include "CXMLFile.hpp"
#include "CXMLReader.hpp"
#include "CXMLElement.hpp"
#include <fstream>
#include <string.h>

namespace LibCXML {
	bool CXMLReader::checkMagicNumber(const char* magic) {
		if (strncmp(this->cxmlHeader.magic, magic, MAGIC_LEN) == 0)
			return true;
		else
			return false;
	}

	CXMLReader::CXMLReader(const std::string& cxmlFilePath, const char* magic) {
		
		this->cxmlFile.open(cxmlFilePath, std::ios::in | std::ios::binary);
		if (this->cxmlFile.is_open()) {
			this->cxmlFile.read((char*)&this->cxmlHeader, sizeof(CxmlFileHeader));

			if (!checkMagicNumber(magic)) {
				throw std::runtime_error("CXML File is invalid or corrupt");
			}

			this->TreeTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.treeTable);
			this->IdTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.idTable);
			this->HashIdTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.hashIdTable);
			this->StringTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.stringTable);
			this->WStringTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.wstringTable);
			this->HashTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.hashTable);
			this->IntArrayTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.intArrayTable);
			this->FloatArrayTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.floatArrayTable);
			this->FileTable = std::make_unique<CXMLStream>(cxmlFile, this->cxmlHeader.fileTable);
		}
		else {
			throw std::runtime_error("CXML file could not be opened.");
		}

	}


	CXMLReader::~CXMLReader() {
		this->cxmlFile.close();
	}

}
