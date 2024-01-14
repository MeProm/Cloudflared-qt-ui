#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QWidget>
#include <QPropertyAnimation>
#include <QSettings>
#include <QProcessEnvironment>
#include <QPixmap>
#include <QVBoxLayout>
#include <QApplication>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <windows.h>
#include <tchar.h>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonValue>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

QString dnsnames;
QString domains;
const QString APP_VERSION = "1.0.1";
const QString githubUsername = "MeProm";
const QString githubRepo = "Cloudflare UI program";
const QString currentVersion = "1.0.1";
bool nojson=false;
QString selectedFileName;
QString yamlContent = "# Tunnel UUID，就是同目录下的json文件的文件名\n"
                      "tunnel: xxxxxxxxxxxxxxxxxxxxx\n"
                      "# 鉴权文件路径，注意根据自己的路径修改，下面的例子是win的路径\n"
                      "credentials-file: C:\\Users\\xxxxx\\.cloudflared\\xxxxxxxxxxxxxx.json\n"
                      "protocol: h2mux\n\n"
                      "# 服务配置\n"
                      "ingress:\n"
                      "  # hostname是刚才添加DNS记录时指定的域名\n"
                      "  - hostname: home.xxxxx.cn\n"
                      "    # service是需要暴露的服务，比如这里反代了5244端口\n"
                      "    service: http://127.0.0.1:8888\n"
                      "  - hostname: home-desktop.xxxxx.cn\n"
                      "    # 这里的示例是windows 远程桌面服务\n"
                      "    service: rdp://localhost:3389\n"
                      "  # 最后记得添加一个默认404\n"
                      "  - service: http_status:404";
void MainWindow::createYaml(const QString& filePath, const QString& Content)
{
    // 创建QFile对象
    QFile file(filePath);

    // 打开文件，以文本写入模式
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // 创建QTextStream对象
        QTextStream out(&file);

        // 写入YAML内容
        out << Content;

        // 关闭文件
        file.close();

        QMessageBox::information(this, "成功", "创建yaml成功");
    }
    else
    {
        QMessageBox::critical(this, "错误", "发生了错误--无法创建yaml文件，请检查权限或点击“打开”检查yaml文件是否存在");
        startVibration();
    }
}

void MainWindow::createJson(const QString& filePath){
    if(nojson==true){
        return;
    }
    QJsonObject defaultParams;
    defaultParams["path"] = "";
    defaultParams["Tunnel-ID"] = "";

    // 将默认参数保存到 JSON 文件
    QJsonDocument jsonDoc(defaultParams);

    QFile configFile(filePath);
    if (configFile.open(QIODevice::WriteOnly)) {
        configFile.write(jsonDoc.toJson());
        configFile.close();
        QMessageBox::information(this, "成功", "创建json成功");
    } else {
        QMessageBox::critical(this, "错误", "发生了错误--无法创建json文件，请检查权限或警用json");
        startVibration();
    }
}
void MainWindow::writeYaml()
{
    QFile file(ui->path->text());
    QString editedContent = ui->yamledit->toPlainText();
    if(editedContent.isEmpty()){
        startVibration();
        QMessageBox::critical(this, "错误", "发生了错误--输入框为空，请点击重置yaml");
        return;
    }

    // 打开文件，以文本写入模式
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // 创建QTextStream对象
        QTextStream out(&file);

        // 写入YAML内容
        out << editedContent;

        // 关闭文件
        file.close();

        QMessageBox::information(this, "成功", "写入yaml成功");
    }
    else
    {
        QMessageBox::critical(this, "错误", "发生了错误--无法写入yaml文件，请检查权限或点击“打开”检查yaml文件是否存在");
        startVibration();
    }
}

void MainWindow::writeJson(const QString& filePath, QString& a, QString& b){
    if(nojson==true){
        return;
    }
    QJsonObject defaultParams;
    defaultParams["path"] = a;
    defaultParams["Tunnel-ID"] = b;

    // 将默认参数保存到 JSON 文件
    QJsonDocument jsonDoc(defaultParams);

    QFile configFile(filePath);
    if (configFile.open(QIODevice::WriteOnly)) {
        configFile.write(jsonDoc.toJson());
        configFile.close();
        QMessageBox::information(this, "成功", "保存json成功");
    } else {
        QMessageBox::critical(this, "错误", "发生了错误--无法创建json文件，请检查权限或警用json");
        startVibration();
    }
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(1230, 600);  // 你想要的最小大小
    setMaximumSize(1230, 600);
        QPixmap originalPixmap(":/new/prefix1/CF_logomark.png");  // 替换为你的图片路径
    QSize newSize(240,240);  // 设置新的大小
    QPixmap scaledPixmap = originalPixmap.scaled(newSize);


    // 将图片设置到 QLabel 中
    ui->label_5->setPixmap(scaledPixmap);
    QString folderPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "//.cloudflared//";



    QString homeDirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    ui->path->setText(homeDirPath + "/.cloudflared/config.yaml");
    QFile readFile(QCoreApplication::applicationDirPath() + "/config.json");
    if (readFile.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = readFile.readAll();
        QJsonDocument jsonDocRead = QJsonDocument::fromJson(jsonData);

        // 检查 JSON 是否有效
        if (!jsonDocRead.isNull() && jsonDocRead.isObject()) {
            QJsonObject readParams = jsonDocRead.object();

            // 获取参数 x 的值
            QString path = readParams["path"].toString();
            QString TunnelID = readParams["Tunnel-ID"].toString();
            if (!path.isEmpty()){
                ui->path->setText(path);
                ui->TunnelID->setText(TunnelID);
            }else{
                ui->TunnelID->setText(TunnelID);
            }
        } else {
        }
        readFile.close();
    } else {
        if (readFile.exists()) {
            QMessageBox::critical(this, "错误-加载配置", "发生了错误--无法打开json文件，请检查权限检查json文件是否存在");
        } else {
            QMessageBox::critical(this, "错误-加载配置", "发生了错误--无法打开json文件，即将创建新json文件");
            createJson(QCoreApplication::applicationDirPath() + "/config.json");
        }

    }
    QDir folderDir(folderPath);

    // 获取文件夹中的所有文件
    QStringList files = folderDir.entryList(QStringList("*.json"), QDir::Files);

    QStringList fileNamesWithoutExtension;
    foreach (const QString &fileName, files) {
        QFileInfo fileInfo(fileName);
        QString baseName = fileInfo.baseName(); // 获取文件名，包含扩展名
        QString fileNameWithoutExtension = baseName.left(baseName.lastIndexOf('.')); // 去除扩展名
        fileNamesWithoutExtension << fileNameWithoutExtension;
    }
    QMessageBox::information(this, "多个文件", "检测到一个或多个文件: " + fileNamesWithoutExtension.join(", ") + "接下来请选择其中一个作为启动Tunnel ID");
    bool ok;
    QString userInput = QInputDialog::getText(this, "输入值", "请输入选择的选项:", QLineEdit::Normal, "", &ok);

    bool ok2;
    int index = userInput.toInt(&ok2);
    index -=1;

    if (ok2 && index >= 0 && index < fileNamesWithoutExtension.size()) {
        selectedFileName = fileNamesWithoutExtension.at(index);
        QMessageBox::information(this, "输入值", "您使用的Tunnel ID是 " + selectedFileName);
        ui->TunnelID->setText(selectedFileName);
        // 执行你的逻辑
    } else {
        QMessageBox::information(this, "输入值", "输入无效");
    }
    checkForUpdates();
}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::startVibration()
{
    // 创建一个水平方向的属性动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100);  // 设置震动的持续时间
    animation->setLoopCount(3);   // 设置震动的次数

    // 获取当前窗口位置
    int startX = this->geometry().x();
    int startY = this->geometry().y();

    // 设置震动范围
    int vibrationRange = 10;

    // 设置动画的关键帧
    animation->setKeyValueAt(0, QRect(startX, startY, width(), height()));
    animation->setKeyValueAt(0.2, QRect(startX + vibrationRange, startY, width(), height()));
    animation->setKeyValueAt(0.4, QRect(startX - vibrationRange, startY, width(), height()));
    animation->setKeyValueAt(0.6, QRect(startX, startY + vibrationRange, width(), height()));
    animation->setKeyValueAt(0.8, QRect(startX, startY - vibrationRange, width(), height()));
    animation->setKeyValueAt(1, QRect(startX, startY, width(), height()));

    // 开始动画
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::on_openyaml_clicked()
{
    QFile fileyaml(ui->path->text());//读取文本框中yaml文件的位置
    if (!fileyaml.exists()) {
        QString homeDirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        if(!selectedFileName.isEmpty()){
            yamlContent =   "# Tunnel UUID，就是同目录下的json文件的文件名\n"
                            "tunnel: "+selectedFileName+"\n"
                            "# 鉴权文件路径，注意根据自己的路径修改，下面的例子是win的路径\n"
                            "credentials-file: "+homeDirPath + "/.cloudflared/config.yaml\n"
                            "protocol: h2mux\n\n"
                            "# 服务配置\n"
                            "ingress:\n"
                            "  # hostname是刚才添加DNS记录时指定的域名\n"
                            "  - hostname: home.xxxxx.cn\n"
                            "    # service是需要暴露的服务，比如这里反代了5244端口\n"
                            "    service: http://127.0.0.1:8888\n"
                            "  - hostname: home-desktop.xxxxx.cn\n"
                            "    # 这里的示例是windows 远程桌面服务\n"
                            "    service: rdp://localhost:3389\n"
                            "  # 最后记得添加一个默认404\n"
                            "  - service: http_status:404";
            createYaml(homeDirPath + "/.cloudflared/config.yaml",yamlContent);
        }else{
            createYaml(homeDirPath + "/.cloudflared/config.yaml",yamlContent);
        }


    }
    if (!fileyaml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading";
        startVibration();
        return;
    }

    QTextStream in(&fileyaml);
    QString fileContent = in.readAll();

    // 关闭文件
    fileyaml.close();
    ui->yamledit->setPlainText(fileContent);
}


void MainWindow::on_saveyaml_clicked()
{
    QString aa=ui->path->text();
    QString bb=ui->TunnelID->text();
    writeJson(QCoreApplication::applicationDirPath() + "/config.json",aa,bb);
    writeYaml();

    QStringList arguments;
    arguments << "/K" << "cloudflared tunnel ingress validate";

    QProcess *myProcess = new QProcess(this);
    QString program = "cmd.exe";
    myProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->startupInfo->dwFlags |= STARTF_USESHOWWINDOW;
        args->startupInfo->wShowWindow = SW_SHOWNORMAL; // 将窗口显示状态设置为正常大小

        args->flags |= CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    });

    myProcess->start(program, arguments);

    if (!myProcess->waitForStarted()) {
        QMessageBox::critical(this, "错误-打开cmd", "发生了错误--无法打开cmd");
    }




}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSize currentSize = w.size();

    return a.exec();
}

void MainWindow::restartApplication()
{
    // 执行重启程序的操作

    // 获取应用程序路径
    QString applicationPath = QCoreApplication::applicationFilePath();

    // 启动新的进程以重新运行应用程序
    QProcess::startDetached(applicationPath);

    // 退出当前实例
    QCoreApplication::quit();
}



void MainWindow::on_res_clicked()
{

    QString homeDirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    if(!selectedFileName.isEmpty()){
        yamlContent =   "# Tunnel UUID，就是同目录下的json文件的文件名\n"
                      "tunnel: "+selectedFileName+"\n"
                                           "# 鉴权文件路径，注意根据自己的路径修改，下面的例子是win的路径\n"
                                           "credentials-file: "+homeDirPath + "/.cloudflared/"+selectedFileName+".json\n"
                                      "protocol: h2mux\n\n"
                                      "# 服务配置\n"
                                      "ingress:\n"
                                      "  # hostname是刚才添加DNS记录时指定的域名\n"
                                      "  - hostname: home.xxxxx.cn\n"
                                      "    # service是需要暴露的服务，比如这里反代了5244端口\n"
                                      "    service: http://127.0.0.1:8888\n"
                                      "  - hostname: home-desktop.xxxxx.cn\n"
                                      "    # 这里的示例是windows 远程桌面服务\n"
                                      "    service: rdp://localhost:3389\n"
                                      "  # 最后记得添加一个默认404\n"
                                      "  - service: http_status:404";
        createYaml(homeDirPath + "/.cloudflared/config.yaml",yamlContent);
    }else{
        createYaml(homeDirPath + "/.cloudflared/config.yaml",yamlContent);
    }
    ui->yamledit->setPlainText(yamlContent);
}


void MainWindow::on_open_clicked()
{
    void on_saveyaml_clicked();
    QString aa=ui->path->text();
    QString bb=ui->TunnelID->text();
    QStringList arguments;
    arguments << "/K" << "cloudflared tunnel --config " + aa + " run " + bb;

    QProcess *myProcess = new QProcess(this);
    QString program = "cmd.exe";
    myProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->startupInfo->dwFlags |= STARTF_USESHOWWINDOW;
        args->startupInfo->wShowWindow = SW_SHOWNORMAL; // 将窗口显示状态设置为正常大小

        args->flags |= CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    });

    myProcess->start(program, arguments);

    if (!myProcess->waitForStarted()) {
        QMessageBox::critical(this, "错误-打开cmd", "发生了错误--无法打开cmd");
    }

}




void MainWindow::on_login_clicked()
{
    QStringList arguments;
    arguments << "/K" << "cloudflared tunnel login";

    QProcess *myProcess = new QProcess(this);
    QString program = "cmd.exe";
    myProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->startupInfo->dwFlags |= STARTF_USESHOWWINDOW;
        args->startupInfo->wShowWindow = SW_SHOWNORMAL; // 将窗口显示状态设置为正常大小

        args->flags |= CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    });

    myProcess->start(program, arguments);

    if (!myProcess->waitForStarted()) {
        QMessageBox::critical(this, "错误-打开cmd", "发生了错误--无法打开cmd");
    }
}

void MainWindow::on_createTunnel_clicked()
{
    QString Tunnelname=ui->Tunnelname->text();
    ui->dnsname->setText(Tunnelname);

    QStringList arguments;
    arguments << "/K" << "cloudflared tunnel create "+Tunnelname;

    QProcess *myProcess = new QProcess(this);
    QString program = "cmd.exe";
    myProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->startupInfo->dwFlags |= STARTF_USESHOWWINDOW;
        args->startupInfo->wShowWindow = SW_SHOWNORMAL; // 将窗口显示状态设置为正常大小

        args->flags |= CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    });

    myProcess->start(program, arguments);

    if (!myProcess->waitForStarted()) {
        QMessageBox::critical(this, "错误-打开cmd", "发生了错误--无法打开cmd");
    }
}


void MainWindow::on_download_clicked()
{

    QString url = "https://one.dash.cloudflare.com/";  // 替换为你想要打开的网址
    QDesktopServices::openUrl(QUrl(url));
}



void MainWindow::checkForUpdates()//ai辅助:)
{
    ui->about->setText("开发者:"+githubUsername+"\n程序仓库:"+githubRepo+"\n当前版本:"+currentVersion);
    QNetworkAccessManager manager;
    QUrl apiUrl("https://github.com/MeProm/Cloudflare-UI-program");
    QNetworkRequest request(apiUrl);

    QNetworkReply* reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

            if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();
                QJsonValue tagName = jsonObj["tag_name"];

                if (tagName.isString()) {
                    QString latestVersion = tagName.toString();

                    if (latestVersion != currentVersion) {
                        qDebug() << "New version available: " << latestVersion;
                        // 在这里可以弹出提示用户更新的窗口或执行其他操作
                    } else {
                        qDebug() << "No new version available.";
                    }
                }
            }
        } else {
            qDebug() << "Error checking for updates: " << reply->errorString();
        }

        reply->deleteLater();
    });
}

void MainWindow::on_setdns_clicked()
{
    QString dnsname =ui->dnsname->text();
    QString dnsdomain =ui->domain->text();
    QStringList arguments;
    arguments << "/K" << "cloudflared tunnel route dns "+dnsname + " " + dnsdomain;

    QProcess *myProcess = new QProcess(this);
    QString program = "cmd.exe";
    myProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->startupInfo->dwFlags |= STARTF_USESHOWWINDOW;
        args->startupInfo->wShowWindow = SW_SHOWNORMAL; // 将窗口显示状态设置为正常大小

        args->flags |= CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    });

    myProcess->start(program, arguments);

    if (!myProcess->waitForStarted()) {
        QMessageBox::critical(this, "错误-打开cmd", "发生了错误--无法打开cmd");
    }

}




\



void MainWindow::on_github_download_clicked()
{
    QString url = "https://github.com/MeProm/Cloudflare-UI-program";  // 替换为你想要打开的网址
    QDesktopServices::openUrl(QUrl(url));
}


void MainWindow::on_cloud_download_clicked()
{
    QString url = "https://github.com/cloudflare/cloudflared/releases/";  // 替换为你想要打开的网址
    QDesktopServices::openUrl(QUrl(url));
}


void MainWindow::on_pushButton_clicked()
{
    restartApplication();
}


void MainWindow::on_del_clicked()
{
    QMessageBox::information(this, "删除", "建议先去官网看一下自己所用的Tunnel-ID，再删除多余的json");
    QString directoryPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "//.cloudflared//"; // 指定目录路径

    // 使用QDesktopServices打开文件资源管理器
    QDesktopServices::openUrl(QUrl("file:///" + directoryPath));

}

