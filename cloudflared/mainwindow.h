#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 创建 QFile 实例


private slots:
    void on_openyaml_clicked();

    void startVibration();

    void on_saveyaml_clicked();

    void createYaml(const QString& filePath, const QString& Content);

    void createJson(const QString& filePath);

    void writeJson(const QString& filePath, QString& a, QString& b);

    bool writeYaml();

    void on_checkBox_clicked(bool checked);

    void on_res_clicked();

    void on_open_clicked();

    void checkForUpdates();

    void on_setdns_clicked();

    void on_createTunnel_clicked();

    void on_download_clicked();

    void on_login_clicked();

    void on_github_download_clicked();

    void on_cloud_download_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
