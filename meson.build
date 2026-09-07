project('CXML', 'cpp',
  version: '0.1',
  default_options: ['warning_level=0', 'cpp_std=c++20', 'default_library=static'])


cpp = meson.get_compiler('cpp')
dirbase = meson.current_source_dir()
include_libs = include_directories('.')



cargs = []
largs = []
if target_machine.system() == 'darwin'
    cargs += ['-arch', 'arm64', '-arch', 'x86_64']
    largs += ['-arch', 'arm64', '-arch', 'x86_64']
endif


libCXML = static_library('CXML',
        'CXMLAttribute.cpp',
        'CXMLAttributeBase.cpp',
        'CXMLElement.cpp',
        'CXMLReader.cpp',
        'CXMLStream.cpp',
		include_directories: [ include_libs ],
        cpp_args: cargs,
		link_args: largs
)
		
libCXML_dep = declare_dependency(include_directories: include_libs, link_with: libCXML)
