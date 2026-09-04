#include "CXMLFile.hpp"
#include "CXMLStream.hpp"
#include <cstring>
#include <string>
#include <fstream>

namespace LibCXML {
	CXMLStream::CXMLStream(uint8_t* srcbuffer, size_t size) {
		this->Open(srcbuffer, size);
	}
	CXMLStream::CXMLStream(std::fstream& fd, CxmlTableDeclaration dec)
	{
		this->Open(fd, dec.tableOffset, dec.tableSize);
	}
	CXMLStream::CXMLStream(std::fstream& fd, size_t offset, size_t size)
	{
		this->Open(fd, offset, size);
	}

	void CXMLStream::Open(uint8_t* srcbuffer, size_t size)
	{
		this->buffer.resize(size);
		memcpy(this->buffer.data(), srcbuffer, size);
	}

	void CXMLStream::Open(std::fstream& fd, size_t offset, size_t size)
	{
		fd.seekg(offset);
		this->buffer.resize(size);
		fd.read((char*)this->buffer.data(), this->buffer.size());
	}

	size_t CXMLStream::Length() {
		return this->buffer.size();
	}

	void CXMLStream::Seek(size_t pos) {
		this->pos = pos;
	}

	char* CXMLStream::ReadStrLen(size_t sz) {
		char* str = (char*)(this->buffer.data() + this->pos);
		pos += sz * sizeof(char);
		return str;
	}

	wchar_t* CXMLStream::ReadWStrLen(size_t sz) {
		wchar_t* str = (wchar_t*)(this->buffer.data() + this->pos);
		pos += sz * sizeof(wchar_t);
		return str;
	}

	char* CXMLStream::ReadStr() {
		char* str = (char*)(this->buffer.data() + this->pos);
		pos += (strlen(str)) * (sizeof(char)) + 1;
		return str;
	}

	wchar_t* CXMLStream::ReadWStr() {
		wchar_t* str = (wchar_t*)(this->buffer.data() + this->pos);
		pos += (wcslen(str)) * (sizeof(wchar_t)) + 1;
		return str;
	}

	float CXMLStream::ReadFloat() {
		float val;
		this->Read(&val, sizeof(float));
		return val;
	}

	double CXMLStream::ReadDouble() {
		double val;
		this->Read(&val, sizeof(double));
		return val;
	}

	int CXMLStream::ReadInt() {
		int val;
		this->Read(&val, sizeof(int));
		return val;
	}

	size_t CXMLStream::Read(void* buf, size_t sz) {
		size_t realsz = sz;
		if ((this->pos + realsz) > this->buffer.size())
			realsz = (this->buffer.size() - this->pos);

		memcpy(buf, this->buffer.data() + this->pos, realsz);

		this->pos += realsz;

		return realsz;
	}

}
