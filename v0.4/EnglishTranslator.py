import requests
from bs4 import BeautifulSoup
import json
import win32file
import win32pipe
import winreg


class PipeReceiver:  # 管道接收端
    def __init__(self):
        self.pipeHandle = 0
        self.pipeName = ""
        self.pipePath = ""
        self.pipeSize = 0

    def open(self, name, size=4096):
        self.pipeName = name
        self.pipePath = f"\\\\.\\pipe\\{name}"
        self.pipeSize = size

        win32pipe.WaitNamedPipe(
            self.pipePath,
            win32pipe.NMPWAIT_USE_DEFAULT_WAIT)

        self.pipeHandle = win32file.CreateFile(
            self.pipePath,
            win32file.GENERIC_READ | win32file.GENERIC_WRITE,
            win32file.FILE_SHARE_WRITE | win32file.FILE_SHARE_READ,
            None,
            win32file.OPEN_EXISTING,
            0,
            None)
        return self.pipeHandle

    def write(self, msg):
        win32file.WriteFile(self.pipeHandle, str.encode(msg), None)

    def read(self):
        try:
            data = win32file.ReadFile(self.pipeHandle, self.pipeSize)
        except Exception:
            return ""
        return bytes(data[1]).decode("utf-8")

    def close(self):
        win32file.CloseHandle(self.pipeHandle)


def cbENWordSolution(resp):  # 爬剑桥英语单词翻译
    cont = BeautifulSoup(resp.text, "html.parser")  # 处理文本
    res = {}

    pron = cont.find("span", attrs={"class": "pron dpron"})  # 找到读音
    if pron is None:  # 没找到就为空
        res["pronunciation"] = ""
        word = cont.find("span", attrs={"class": "hw dhw"})  # 找到单词(第一个单词认为是正确的单词)
        res["word"] = word.get_text()
    else:  # 找到
        res["pronunciation"] = pron.get_text()
        word = pron.parent.parent.find("span", attrs={"class": "hw dhw"})  # 找到单词(有读音的单词认为是单词原型)
        res["word"] = word.get_text()

    meanings = []  # 存词性, 释义
    blocks = cont.findAll("div", attrs={"class": "pr entry-body__el"})
    for block in blocks:
        text = block.find("span", attrs={"class": "pos"})  # 找到词性
        if text is None:  # 如果没有就找下一个block
            continue
        text = text.get_text()
        meaning = {"type": text}

        needContinue = False  # 之前有找到过这个词性就不用再找了
        for child in meanings:
            if child["type"] == text:
                needContinue = True
                break
        if needContinue:
            continue

        meaning["meanings"] = []
        meaningBlocks = block.findAll("div", attrs={"class": "sense-body dsense_b"})  # 找到释义
        for meaningBlock in meaningBlocks:
            for child in meaningBlock.findAll("div", attrs={"class": "def-block ddef_block"}, recursive=False):
                meaningText = child.find("div", attrs={"class": "def ddef_d db"}).get_text()
                if meaningText[-2:] == ": ":  # 去掉冒号
                    meaningText = meaningText[:-2]
                meaning["meanings"].append(meaningText)

        meanings.append(meaning)  # 将这个词性及其所有意思加到meanings进去

    res["meanings"] = meanings  # 放到结果中
    return res


def ydENWordSolution(resp):  # 爬有道英语单词翻译
    cont = BeautifulSoup(resp.text, "html.parser")  # 处理文本
    res = {}
    meanings = []  # 存词性, 释义
    blocks = cont.find("ul", attrs={"class": "basic"}).findAll("li", attrs={"class": "word-exp"})  # 获取词性，释义
    for block in blocks:
        meaning = {}

        pos = block.find("span", attrs={"class": "pos"})  # 词性， 不一定存在这个标签（有道你真**）
        if pos is None:
            meaning["type"] = "else"
        else:
            text = pos.get_text()

            if text == "n.":  # 统一词性，与剑桥词性统一
                text = "noun"
            elif text == "v." or text == "vt." or text == "vi.":
                text = "verb"
            elif text == "adj.":
                text = "adjective"
            elif text == "adv.":
                text = "adverb"
            elif text == "prep.":
                text = "preposition"
            elif text == "aux.":
                text = "auxiliary verb"
            meaning["type"] = text

        trans = block.find("span", attrs={"class": "trans"})  # 释义
        if trans is None:
            meaning["meanings"] = "unknown"
        else:
            meaning["meanings"] = [trans.get_text()]

        meanings.append(meaning)  # 将词性与释义放进去

    res["meanings"] = meanings  # 放到结果中
    return res


def hjENSentenceSolution(resp):  # 爬沪江小D英语句子翻译
    res = {}
    trandict = resp.json()  # 解析为json
    meaning = trandict["data"]["content"]  # 获取翻译
    res["meaning"] = meaning  # 放进去
    return res


# 打开管道
receiver = PipeReceiver()
receiver.open("TranslatorPipe", 65536)

# 读取服务端发来的数据
words = receiver.read()
words = words.strip()  # 删去开头和结尾的空格
print(words)

# 爬取数据
headers = {
    "user-agent": "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36"
}

proxies = {}  # 没有代理就默认，有代理则把https代理改为http，否则有代理时爬不了网页

try:
    settings = winreg.OpenKeyEx(winreg.HKEY_CURRENT_USER, 'Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings', 0, winreg.KEY_ALL_ACCESS)  # 获取注册表
    if winreg.QueryValueEx(settings, "ProxyEnable")[0] == 1:  # 有开启代理
        url = str(winreg.QueryValueEx(settings, "ProxyServer")[0])  # 获取代理url
        proxies = {
            'http': "http://" + url,
            'https': "http://" + url
        }

except Exception:
    pass


if words.find(' ') == -1:  # 没有空格，认为是英文单词

    # 爬剑桥词典
    cbUrl = f"https://dictionary.cambridge.org/dictionary/english/{words}"  # 剑桥词典
    resp = requests.get(cbUrl, headers=headers, proxies=proxies)  # 开爬
    wordDict = cbENWordSolution(resp)  # 获取来自剑桥词典的单词，发音与释义
    print(wordDict)

    # 爬有道词典
    words = wordDict["word"]
    ydUrl = f"https://youdao.com/result?word={words}&lang=en"  # 有道词典
    resp = requests.get(ydUrl, headers=headers, proxies=proxies)  # 开爬
    ydDict = ydENWordSolution(resp)  # 获取来自有道词典的释义
    print(ydDict)

    # 合并
    for ydMeaning in ydDict["meanings"]:
        isFound = False
        for meaning in wordDict["meanings"]:
            if meaning["type"] == ydMeaning["type"]:  # 找到有相同词性的
                meaning["meanings"] = ydMeaning["meanings"] + meaning["meanings"]  # 中文意思放在最前面
                isFound = True
                break
        if isFound is False:  # 没找到相同的就单独放
            wordDict["meanings"].append(ydMeaning)

    # 编为json
    wordDict["isWord"] = True  # 标记为单词
    wordDict["isSentence"] = False
    resJson = json.dumps(wordDict, ensure_ascii=False)  # python对象转为json字符串，ensure_ascii=False：不输出ascii码，即可以输出中文
    print(resJson)

else:  # 有空格，认为是英文句子
    tlUrl = "https://dict.hjenglish.com/v10/dict/translation/en/cn"  # 沪江小D句子翻译
    dat = {"content": words}  # 要翻译的句子放进去
    hd = {  # 解决反爬
        "user-agent": "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36",
        "cookie": "HJ_UID=13b58d1c-5132-8421-e5cd-ba1234567890"
    }
    resp = requests.post(tlUrl, data=dat, headers=hd, proxies=proxies)  # 开爬
    sentenceDict = hjENSentenceSolution(resp)  # 获取来自沪江小D的翻译

    # 编为json
    sentenceDict["isSentence"] = True  # 标记为句子
    sentenceDict["isWord"] = False
    sentenceDict["sentence"] = words
    resJson = json.dumps(sentenceDict, ensure_ascii=False)
    print(resJson)

# 向服务端发送数据
receiver.write(resJson)

# 关闭管道
receiver.close()
