#!/usr/bin/env python

APPNAME = 'beam'
VERSION = '0.1.0'

top = '.'
out = 'build'

def options(opt):
  opt.load('compiler_cxx')
  opt.load('unittestt')

def configure(cnf):
  cnf.load('compiler_cxx')
  cnf.load('unittestt')
  cnf.env.CXXFLAGS = ['-g', '-W', '-Wall', '-O2']

def build(bld):
  bld.recurse('beam')
  bld.recurse('tests')