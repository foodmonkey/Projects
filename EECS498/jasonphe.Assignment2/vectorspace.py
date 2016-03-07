__author__ = 'Jason'
import glob, os, re, string, sys, porter, math, operator

def main(argv):
    docscheme = sys.argv[1]
    quescheme = sys.argv[2]
    testqueries = open(sys.argv[4])
    global count
    count = 0
    index = {}
    files = os.listdir(sys.argv[3])
    for file in files:
        count += 1
        infile = open(sys.argv[3] + file)
        a = infile.readlines()
        str = "".join(a)
        indexDocument(str, docscheme, quescheme, index)

    global dlength
    dlength = {}
    for i in range(1, count+1):
        dlength[i] = 0.0
    for j in index:
        for k in range(0, len(index[j])):
            temp = index[j][k]
            dlength[temp[0]] += math.pow(temp[1]*math.log10(count/len(index[j])),2)
    query = testqueries.readline()
    global l

    l = []
    """
    global match
    global retrieved
    retrieved = 0
    match = 0
    rel = open("cranfield.reljudge.test")
    a = rel.readline()
    while a:
        l.append((a.split()))
        a = rel.readline()"""
    while query:
        retrieveDocuments(query, index, docscheme, quescheme)
        query = testqueries.readline()
    """
    print match
    print retrieved"""


def indexDocument(str, docscheme, quescheme, index):
    processed = process(str)
    unique = list(set(processed))
    max = 0
    for i in range(0, len(unique)):
        temp = processed.count(unique[i])
        if temp > max:
            max = temp
    if docscheme == "nxx":
        for k in range(0, len(unique)):
            index.setdefault(unique[k], []).append((count, 0.5*processed.count(unique[k])/max + 0.5))
    else:
        for k in range(0, len(unique)):
            index.setdefault(unique[k], []).append((count, 1.0*processed.count(unique[k])/max))

def retrieveDocuments(query, index, docscheme, quescheme):
    processed = process(query)
    unique = list(set(processed))
    max = 0
    for i in range(0, len(unique)):
        temp = processed.count(unique[i])
        if temp > max:
            max = temp
    qlength = 0.0
    similarity = {}
    for i in range(1, count+1):
        similarity[i] = 0.0
    for i in range(1, len(unique)):
        if unique[i] in index:
            qlength += math.pow(1.0*processed.count(unique[i])/max*math.log10(1.0*count/len(index[unique[i]])), 2)
            for j in range(0, len(index[unique[i]])):
                docs = index[unique[i]]
                if len(docs) > 0:
                    if quescheme == "bpx":
                        if (1.0*count-len(docs))/len(docs) > 0:
                            similarity[docs[j][0]] += docs[j][1] * math.log10((1.0*count-len(docs))/len(docs))
                    else:
                        similarity[docs[j][0]] += (1.0*processed.count(unique[i])/max)*math.log10(1.0*count/len(docs))* docs[j][1] * math.log10(1.0*count/len(docs))
    if docscheme == "tfidf":
        for k in similarity:
            temp = math.sqrt(qlength) * math.sqrt(dlength[k])
            if temp != 0:
                similarity[k] /= math.sqrt(qlength) * math.sqrt(dlength[k])
    sort = sorted(similarity.items(), key=operator.itemgetter(1), reverse=True)
    for i in range(0, len(sort)):
        if sort[i][1] != 0:
            print str(processed[0]) + " " + str(sort[i][0]) + " " + str(sort[i][1])
    """
    global match, retrieved
    for i in range(0, 10):
        if sort[i][1] != 0:
            retrieved += 1
            if [str(processed[0]), str(sort[i][0])] in l:
                match += 1
    """

def process(input):
    s1 = removeSGML(input)
    s2 = tokenizeText(s1)
    s3 = removeStopwords(s2)
    pr = []
    from porter import PorterStemmer
    p = PorterStemmer()
    for element in s3:
        pr.append(p.stem(element, 0, len(element)-1))
    return pr


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
    main(sys.argv[4:])