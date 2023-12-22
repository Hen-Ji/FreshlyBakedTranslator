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


# 打开管道
receiver = PipeReceiver()
receiver.open("TranslatorPipe", 65536)

# 读取服务端发来的数据
words = receiver.read()
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


cbUrl = f"https://dictionary.cambridge.org/dictionary/english/{words}"  # 剑桥词典
ydUrl = f"https://youdao.com/result?word={words}&lang=en"  # 有道词典
tlUrl = "https://dict.hjenglish.com/v10/dict/translation/en/cn"  # 沪江小D句子翻译

resJson = None

if (words.find(' ') == -1):  # 没有空格，认为是单词

    # 爬剑桥词典
    resp = requests.get(cbUrl, headers=headers, proxies=proxies)  # 开爬
    cont = BeautifulSoup(resp.text, "html.parser")  # 处理文本

    word = cont.find("span", attrs={"class": "hw dhw"}).get_text()  # 找到单词
    pron = cont.find("span", attrs={"class": "pron dpron"}).get_text()  # 找到读音

    EngGroups = []  # 存词性, 释义
    blocks = cont.findAll("div", attrs={"class": "pr entry-body__el"})
    for block in blocks:
        child = []
        text = block.find("span", attrs={"class": "pos"}).get_text()  # 找到词性

        needContinue = False  # 之前有找到过这个词性就不用再找了
        for ch in EngGroups:
            if ch[0] == text:
                needContinue = True
                break
        if needContinue:
            continue

        child.append(text)

        means = []
        meanBlock = block.findAll("div", attrs={"class": "sense-body dsense_b"})  # 找到释义
        for ch in meanBlock:
            for c in ch.findAll("div", attrs={"class": "def-block ddef_block"}, recursive=False):
                means.append(c.find("div", attrs={"class": "def ddef_d db"}).get_text())
        child.append(means)

        EngGroups.append(child)

    # 爬有道词典
    resp = requests.get(ydUrl, headers=headers, proxies=proxies)  # 开爬
    cont = BeautifulSoup(resp.text, "html.parser")  # 处理文本

    ChiGroups = []
    blocks = cont.find("ul", attrs={"class": "basic"}).findAll("li", attrs={"class": "word-exp"})  # 获取词性，释义
    for block in blocks:
        child = []

        pos = block.find("span", attrs={"class": "pos"})  # 不一定存在这个标签（有道你真**）
        if pos is None:
            child.append("")
        else:
            child.append(pos.get_text())

        trans = block.find("span", attrs={"class": "trans"})
        if trans is None:
            child.append("")
        else:
            child.append(trans.get_text())

        ChiGroups.append(child)

    # 打印
    print(word)
    print(pron)
    for group in EngGroups:
        print(group)
    for group in ChiGroups:
        print(group)

    # 编为json
    datas = {"word": word, "prononciation": pron, "EnglishTranslate": EngGroups, "ChineseTranslate": ChiGroups}
    resJson = json.dumps(datas, ensure_ascii=False)  # python对象转为json字符串，ensure_ascii=False：不输出ascii码，即可以输出中文

else:  # 有空格，认为是句子
    dat = {"content": words}  # 要翻译的句子放进去
    hd = {  # 解决反爬
        "user-agent": "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36",
        "cookie": "HJ_UID=21b57b9d-9b29-81a0-682a-ab94dbe67496; _SREF_20=https%3A%2F%2Fcn.bing.com%2F; _SREG_20=cn.bing.com%7C%7Csearch%7Cdomain; Hm_lvt_d4f3d19993ee3fa579a64f42d860c2a7=1702731291; _SREF_3=; _REF=; TRACKSITEMAP=3%2C23; acw_tc=76b20f6317030388542631715e2e114456537d97045c9c6f7fbffd2fdf24c1; HJ_CST=0; HJ_SID=g45mdv-f332-44a8-92b8-a487449d032b; HJ_SSID_3=g45mdv-19f9-449a-869b-ec6741980d53; HJ_CSST_3=1; _SREG_3=direct%7C%7Cdirect%7Cdirect; _REG=direct%7C%7Cdirect%7Cdirect"
    }
    resp = requests.post(tlUrl, data=dat, headers=hd, proxies=proxies)  # 开爬
    trandict = resp.json()  # 解析为json
    res = trandict["data"]["content"]  # 获取翻译

    # 打印
    print(res)

    # 编为json
    datas = {"translate": res}
    resJson = json.dumps(datas, ensure_ascii=False)

# 输出
print()
print(resJson)

# 向服务端发送数据
receiver.write(resJson)

# 关闭管道
receiver.close()
