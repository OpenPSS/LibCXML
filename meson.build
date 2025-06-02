project('CXML', 'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++20'])


cpp = meson.get_compiler('cpp')
dirbase = meson.current_source_dir()
include_libs = include_directories('.')

libCXML = static_library('CXML',
        'CXMLAttribute.cpp',
        'CXMLAttributeBase.cpp',
        'CXMLElement.cpp',
        'CXMLReader.cpp',
        'CXMLStream.cpp',
		include_directories: [ include_libs ],
        install : true)
		
libCXML_dep = declare_dependency(include_directories: include_libs, link_with: libCXML)