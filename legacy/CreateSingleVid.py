from scripts.VorVids import CreateVid
import sys

def main(name):
    CreateVid(name)

if __name__ == '__main__':
    name = sys.argv[1]
    main(name)
