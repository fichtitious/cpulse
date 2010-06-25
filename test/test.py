import cpulse

if __name__ == "__main__":

    while 1:
        bass, treble = cpulse.pulse()
        print "%s %s" % ("+" if bass else " ", "+" if treble else " ")

