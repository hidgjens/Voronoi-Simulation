import scripts.VorVids as vvid
import sys

def main(name):
    vvid.CreateVid(name)

if __name__ == '__main__':
    name = sys.argv[1]
    main(name)
