from setuptools import setup, Extension

module = Extension('lightfork', sources = ['lightfork.c'])

setup(name = 'PackageName',
      version = '1.0',
      description = 'This is a demo package',
      ext_modules = [module])
