import _cpulse

class Pulser(object):

    def __init__(self):
        _cpulse.start()

    def __del__(self):
        _cpulse.stop()

    def pulse(self):
        return _cpulse.pulse()

if __name__ == "__main__":

    pulser = Pulser()
    for i in xrange(100):
        print pulser.pulse()
