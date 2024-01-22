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


def ydJPSolution(resp):  # 爬有道翻译
    cont = BeautifulSoup(resp.text, "html.parser")  # 处理文本

    if cont.find("p", attrs={"class": "trans-content"}):  # 是句子翻译
        return ydJPSentenceSolution(cont)
    else:  # 是单词翻译
        return ydJPWordSolution(cont)


def ydJPWordSolution(cont):  # 日语单词翻译
    res = {}
    word = cont.find("div", attrs={"class": "page-container"}).find("div", attrs={"class": "word-head"}).find("div", attrs={"class": "title"}).next  # 找到单词
    res["word"] = word
    
    pron = cont.find("div", attrs={"head-content"}).next.nextSibling.get_text()  # 找到发音
    if pron == ' ' or pron == '':  # 没找到
        pron = cont.find("div", attrs={"head-content"}).get_text()  # 换种方式找
    res["pronunciation"] = pron

    meanings = []
    for sense in cont.find_all("div", attrs={"each-sense"}):
        meaning = {}
        wordType = sense.find("div", attrs={"class": "word-pos color_text_2"})  # 获取词性（不一定有）
        if wordType is None:
            meaning["type"] = "else"
        else:
            meaning["type"] = wordType.get_text()

        meaning["meanings"] = []
        for child in sense.find_all("div", attrs={"class": "sense-ja"}):  # 获取所有意思
            meaning["meanings"].append(child.get_text())

        meanings.append(meaning)  # 放进去

    res["meanings"] = meanings

    res["isWord"] = True  # 标记为单词
    res["isSentence"] = False
    return res


def ydJPSentenceSolution(cont):  # 日语句子翻译
    res = {}
    res["meaning"] = cont.find("p", attrs={"class": "trans-content"}).get_text()  # 获取翻译

    res["isSentence"] = True  # 标记为句子
    res["isWord"] = False
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

nhUrl = f'https://youdao.com/result?word={words}&lang=ja'

# 爬有道词典
resp = requests.get(nhUrl, headers=headers, proxies=proxies)  # 开爬
resDict = ydJPSolution(resp)  # 获取来自有道词典的释义

if resDict["isSentence"] is True:  # 是句子就补上原句子
    resDict["sentence"] = words

# 编为json
resJson = json.dumps(resDict, ensure_ascii=False)
print(resJson)


# 向服务端发送数据
receiver.write(resJson)

# 关闭管道
receiver.close()
