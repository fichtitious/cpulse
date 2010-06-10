from distutils.core import setup, Extension

if __name__ == "__main__":

    setup(name = "_cpulse",
          version = "1.0",
          description = "pulseaudio beat detector with aubio",
          ext_modules = [Extension("_cpulse", 
                                   sources = ["src/cpulse.c", "src/pycpulse.c"], 
                                   libraries = ["pulse-simple", "aubio"])])
