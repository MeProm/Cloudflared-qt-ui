# Cloudflare-UI-program
<div align=center>
  
  ## Using Cloudflare's command line was too cumbersome, and many commands were hard to remember, so I developed this program using qt.
  ## <a href="https://github.com/MeProm">This is my user interface，You can support me</a>
  ## *If you need other languages, please send an email to 2510037056@qq.com. Only Chinese is supported now*
</div>

## 程序可用功能

* 支持登录、创建隧道功能、dns配置记录/设置、Tunnel自动获取(本地)、自动补全数据开启隧道
* 程序分为三个部分**注意使用顺序**
* 目前暂不支持翻译bushi~需要翻译的可以自行修改源码或加上tr()发给我，**作者太懒了（不要对一个初中生要求太多）**
* **支持自动创建yaml配置文件**
* **支持保存程序使用的配置文件格式:JSON**

|        程序目前支持系统                              |        界面美化         |  最新版本  | 开发套件 |
|:--------------------------------------------------: | :-----------------: |:---------------:|:--------: |
|       win   x64 x32(等待光速网络下载32编译套件)                                        |      即将使用        |     1.0.0  |qt|
|      **等待更新（不要对一个初中生要求太多）**       |                     |  |  |

# 使用方法
## 0.注册cloudflare账号**自己去整**
## 1.点击登录，此时弹出cmd窗口，不要管，会跳转浏览器
* **报错解决1**
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/20689524-c7b0-4840-bf21-aac6af6ea7e4)
* **应该是之前登录配置过，但没有配置好**建议清空%USERPROFILE%\.cloudflared\下所有文件重新登录
* **报错解决all**
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/731261a7-4634-4369-ae16-e717b56b525f)
* 程序使用system执行cmd命令，由于system函数为阻塞函数，主程序可能显示未响应，关闭后面的cmd窗口就行了，**1.0.1版本已解决**
## 2.创建隧道
* 正常执行：![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/cd653441-f833-4786-bfd4-79bb932505ec)
## 3.dns代理
* 正常执行：![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/c2aff5e0-2a30-4b7d-9ce4-8f17c9797b33)
* **报错解决2**
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/f0acaa54-d17a-49cf-b66e-86262f0c5f4a)
* 网络太差，等一等，多来几次，实在不行换个网，开个科技
* **报错解决3**
* 有时会出现(A ,AAAA ,CNAME ,already)字样，表示已经被使用了，去域名的dns部分删了即可
## 4.点击**打开**按键打开config.yaml，没有会自动创建，<em>手残整错了可以直接重置一手</em>
## 5.点击**保存**按键保存config.yaml以及config.json，等一会会出现一个cmd，用于验证config.yaml配置是否正常**如果没有ok字样，建议点击重置**
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/503da5ef-03aa-41b8-8023-4b58d19b2643)
* 
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/28f0bae5-b3ff-4e70-ad51-488524e90b3b)
* 
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/f7617a5e-e326-45c9-9676-16c6d620d2b7)
## 6.点击启动
* 正常执行：
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/aecafc3c-8890-4800-a37a-ba3be5edb701)
## 其他功能：
### 1.多个XXXX.json配置文件时询问用户选取哪个，设置config文件，如果和上次使用的不一样，记得点击保存验证
![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/a3f15d6f-710c-470f-8d7d-b67276e224c4)

![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/6cc0bc78-0437-4eaa-80e8-bf4cc8d18e13)
### 2.自动创建
* 创建json![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/42c11cab-82a7-4245-b116-07934d8a3608)
* 
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/49ff9386-b7b0-4272-9eeb-faef191c6bdb)
* 
* 创建yaml![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/50146b9e-923f-4f02-8704-e01328d27180)
* 
* ![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/ebb8c814-8636-4686-8178-c39964e41f13)
* 
* yaml保存检测![image](https://github.com/MeProm/Cloudflare-UI-program/assets/105576804/6395c441-cd24-4f96-99e8-735683748f0d)








