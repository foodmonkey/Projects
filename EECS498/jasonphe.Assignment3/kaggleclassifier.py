__author__ = 'Jason jasonphe'
import glob, os, re, string, sys, math, porter, operator

def main(argv):
    """lie = joke. truth = mix"""
    global directory, correct, words, testfile, liecount, truecount
    directory = sys.argv[1]
    testfiles = os.listdir(sys.argv[1])
    files = os.listdir("kaggle.training/")
    """files = files[0:4000] + files[15000:19000]"""

    correct = 0
    words = {}
    """mapping the file to its tokens and counting words in each class because the processing took too long"""
    lies = []
    truths = []
    liecount = {}
    truecount = {}
    for file in files:
        infile = open("kaggle.training/"+ file)
        tex = infile.readlines()
        text = "".join(tex)
        processed = process(text)
        words[file] = processed
        if file[:4] == "joke":
            lies += words[file]
        else:
            truths += words[file]
    lvocab = list(set(lies))
    tvocab = list(set(truths))
    for x in range(0, len(lvocab)):
        liecount[lvocab[x]] = lies.count(lvocab[x])
    for x in range(0, len(tvocab)):
        truecount[tvocab[x]] = truths.count(tvocab[x])
    print("File,Class")
    data = trainNaiveBayes(files)
    for file in testfiles:
        testNaiveBayes(file, data)
        """lieso = sorted(data[0].items(), key=operator.itemgetter(1), reverse=True)
        print lieso
        trueso = sorted(data[1].items(), key=operator.itemgetter(1), reverse=True)
        print trueso"""
    print str(correct)+ "/" + str(len(testfiles)) + " = " + str(correct*1.0/len(testfiles))

def trainNaiveBayes(training):
    global words, liecount, truecount, testfile, directory
    lies = []
    truths = []
    lienum = 0
    truenum = 0
    for file in training:
        if file[:4] == "joke":
            lies += words[file]
            lienum += 1
        else:
            truths += words[file]
            truenum += 1
    voc = list(set(lies + truths))
    liedic = {}
    truedic = {}
    for x in range(0, len(voc)):
        if voc[x] in liecount:
            val = liecount[voc[x]]
        else:
            val = 0
        if val < 0:
                val = 0
        liedic[voc[x]] = (val + 1.0)/(1.0*len(lies) + len(voc))
        if voc[x] in truecount:
            val = truecount[voc[x]]
        else:
            val = 0
        if val < 0:
                val = 0
        truedic[voc[x]] = (val + 1.0)/(1.0*len(truths) + len(voc))
    plie = 1.0*lienum/(lienum + truenum)
    ptrue = 1.0*truenum/(lienum + truenum)
    return [liedic, truedic, plie, ptrue]

def testNaiveBayes(test, data):
    global directory, words

    lieprob = 0
    trueprob = 0
    infile = open(directory + test)
    tex = infile.readlines()
    text = "".join(tex)
    processed = process(text)
    voc = list(set(processed))
    for word in voc:
        if word in data[0]:
            lieprob += math.log(data[0][word])
    for word in voc:
        if word in data[1]:
            trueprob += math.log(data[1][word])
    lieprob *= data[2]
    trueprob *= data[3]
    global correct
    if lieprob > trueprob:
        print str(test) + ",joke"
        correct += 1
    else:
        print str(test) + ",mix"
        if test[:3] == "mix":
            correct += 1
    return


def process(input):
    s2 = tokenizeText(input)
    s3 = removeStopwords(s2)
    pr = []
    from porter import PorterStemmer
    p = PorterStemmer()
    for element in s3:
        pr.append(p.stem(element, 0, len(element)-1))
    return pr


def tokenizeText(str):
    str = re.sub("it's", "it is", str)
    str = re.sub("I'm", "I am", str)
    str = re.sub("can't", "can not", str)
    str = re.sub("let's", "let us", str)
    str = re.sub("they're", "they are", str)
    str = re.sub("we're", "we are", str)
    str = re.sub("haven't", "have not", str)
    str = re.sub("'", " '", str)
    str = re.sub('[.]', '', str)
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
__author__ = 'Jason'
