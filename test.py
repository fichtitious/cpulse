import cpulse

if __name__ == "__main__":

    level = 0
    while 1:
        level += 1 if cpulse.pulse() else -2
        level = level if level >= 0 else 0
        print "." * int(level/12)
