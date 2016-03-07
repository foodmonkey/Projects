__author__ = 'Jason He jasonphe'
import glob, os, re, string, sys, porter

def main(argv):
    files = os.listdir(sys.argv[1])
    file = files[0]
    stemmed = []
    for file in files:
        text = ""
        infile = open(sys.argv[1] + file)
        a = infile.readline()
        while a:
            text += removeSGML(a)
            a = infile.readline()
        tok = tokenizeText(text)
        removed = removeStopwords(tok)
        from porter import PorterStemmer
        p = PorterStemmer()
        for element in removed:
            stemmed.append(p.stem(element, 0, len(element)-1))
    print "Words " + str(len(stemmed))
    unique = list(set(stemmed))
    print "Vocabulary " + str(len(unique))
    wordfrequency = [(unique[x], stemmed.count(unique[x])) for x in range(0,len(unique))]
    sort = sorted(wordfrequency, key = getKey, reverse = True)
    for i in range(0,49):
        print sort[i]

def getKey(item):
    return item[1]

def removeSGML(str):
    fixed = re.split('<.*?>', str)
    str = "".join(fixed)
    return str

def tokenizeText(str):
    str = re.sub("it's", "it is", str)
    str = re.sub("I'm", "I am", str)
    str = re.sub("can't", "can not", str)
    str = re.sub("let's", "let us", str)
    str = re.sub("they're", "they are", str)
    str = re.sub("we're", "we are", str)
    str = re.sub("haven't", "have not", str)
    str = re.sub("'", " '", str)
    fix = re.split('\n|\s[.]\s|\s[-]\s|\s|,', str)
    fix = filter(None, fix)
    return fix

def removeStopwords(tok):
    stops = open("stopwords.txt")
    stopwords = stops.readlines()
    stopwords = [a.strip('\n| ') for a in stopwords]
    fix = [a for a in tok if a not in stopwords and a is not "."]
    return fix

if __name__ == "__main__":
    main(sys.argv[1:])