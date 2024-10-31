from setuptools import setup, Extension

module = Extension('lightfork', sources = ['lightfork.c'])

setup(name = 'lightfork',
      version = '1.0',
      description = 'This is lightfork',
      ext_modules = [module])
