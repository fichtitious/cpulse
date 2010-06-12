import cpulse

if __name__ == "__main__":

    while 1:
        p = cpulse.pulse()
        print "%i %f" % (1 if p > 0 else 0, p)
        if p > 1 or p < -1:
            raise AssertionError
