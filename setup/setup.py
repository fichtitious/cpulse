from distutils.core import setup, Extension

if __name__ == "__main__":

    setup(name = "cpulse",
          version = "1.0",
          description = "does beat detection on whatever audio is going out through your speakers (via pulseaudio)",
          ext_modules = [Extension("cpulse",
                                   sources = ["src/beatdetector.c", "src/cpulse.c", "src/pycpulse.c"],
                                   libraries = ["pulse-simple"])])
