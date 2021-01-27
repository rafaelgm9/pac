import os, glob
from setuptools import setup, Extension

sources = glob.glob(os.path.join('src', '*.c'))
headers = glob.glob(os.path.join('src', '*.h'))

module = Extension('pac',
                   define_macros=[('MAJOR_VERSION', '1'),
                                  ('MINOR_VERSION', '0')],
                   sources=sources,
                   depends=headers,
                   extra_compile_args=['-fopenmp', '-O3'],
                   extra_link_args=['-fopenmp', '-O3'])

setup(name='pac',
      version='1.0',
      description='Module for counting pairs of particles in a cubic volume.',
      author='Rafael Garcia',
      author_email='rgarciamar@email.arizona.edu',
      ext_modules=[module],
      packages=['pac'],
      zip_safe=False)
