#!/usr/bin/env python

APPNAME = 'beam'
VERSION = '0.1.1'

top = '.'
out = 'build'

def options(opt):
  opt.load('compiler_cxx')
  opt.load('unittestt')

def configure(cnf):
  cnf.load('compiler_cxx')
  cnf.load('unittestt')
  cnf.check_cxx(header_name='boost/assert.hpp')
  cnf.check_cxx(header_name='boost/checked_delete.hpp')
  cnf.check_cxx(header_name='boost/compressed_pair.hpp')
  cnf.check_cxx(header_name='boost/current_function.hpp')
  cnf.check_cxx(header_name='boost/exception/exception.hpp')
  cnf.check_cxx(header_name='boost/noncopyable.hpp')
  cnf.check_cxx(header_name='boost/preprocessor/stringize.hpp')
  cnf.check_cxx(lib='boost_thread')
  cnf.check_cxx(header_name='boost/thread/locks.hpp')
  cnf.check_cxx(header_name='boost/thread/mutex.hpp')
  cnf.check_cxx(header_name='boost/thread/once.hpp')
  cnf.check_cxx(header_name='boost/throw_exception.hpp')
  cnf.check_cxx(header_name='boost/tr1/unordered_map.hpp')
  cnf.env.CXXFLAGS = ['-g', '-W', '-Wall', '-O2']

def build(bld):
  bld.recurse('beam')
