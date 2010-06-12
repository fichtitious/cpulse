import cpulse

if __name__ == "__main__":

    while 1:
        isPeak, isIncreasing = cpulse.pulse()
        print "%i %s" % (isPeak, "+" if isIncreasing else "")
