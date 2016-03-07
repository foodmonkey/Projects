__author__ = 'Jason He jasonphe'
import glob, os, re, string, sys, math

def main(argv):
    english = open("languageIdentification.data/training/English")
    french = open("languageIdentification.data/training/French")
    italian = open("languageIdentification.data/training/Italian")
    test = open("languageIdentification.data/" + sys.argv[1])
    eng = english.readlines()
    str = "".join(eng)
    trainedEng = trainBigramLanguageModel(str)
    fren = french.readlines()
    str = "".join(fren)
    trainedFre = trainBigramLanguageModel(str)
    ital = italian.readlines()
    str = "".join(ital)
    trainedItal = trainBigramLanguageModel(str)
    te= test.readlines()
    text = "".join(te)
    languages = ('English', 'French', 'Italian')
    singles = (trainedEng[0], trainedFre[0], trainedItal[0])
    bigrams = (trainedEng[1], trainedFre[1], trainedItal[1])
    answer = identifyLanguage(text, languages, singles, bigrams)
    ans = "".join(answer)
    print ans

def trainBigramLanguageModel(text):
    freqsing = list(text)
    chars = list(set(freqsing))
    if ' ' in chars:
        chars.remove(' ')
    sing = {}
    for letter in chars:
        sing[letter] = freqsing.count(letter)
    bigrams = []
    for let in range(0, len(freqsing) - 1):
        if(freqsing[let] is not ' ' and freqsing[let] is not '\n' and freqsing[let+1] is not ' '
                and freqsing[let+1] is not '\n'):
            bigrams.append("".join(freqsing[let] + freqsing[let+1]))
    uniqbigrams = list(set(bigrams))
    bigram = {}
    for let in range(0, len(uniqbigrams)):
        bigram[uniqbigrams[let]] = bigrams.count(uniqbigrams[let])

    return (sing, bigram)

def identifyLanguage(text, langs, singles, bigrams):

    lines = re.split('\n', text)
    lines = filter(None, lines)
    answer = []
    linenum = 1
    for line in lines:
        probs = []
        for lang in range(0, len(langs)):
            tempProb = 0.0
            for letter in range(0, len(line) - 1):
                if line[letter] is not ' ' and line[letter+1] is not ' ':
                    if line[letter] + line[letter+1] in bigrams[lang]:
                        bigramfreq = bigrams[lang][line[letter] + line[letter+1]] +0.0
                    else:
                        bigramfreq = 0.0
                    if line[letter] in singles[lang]:
                        singfreq = singles[lang][line[letter]] + 0.0
                    else:
                        singfreq = 0.0
                    tempProb += math.log10((bigramfreq + 1) / (singfreq + len(singles[lang])))
            probs.append(tempProb)
        ans = probs.index(max(probs))
        answer.append(str(linenum) + " " + langs[ans] + "\n")
        linenum += 1

    return answer


if __name__ == "__main__":
    main(sys.argv[1:])