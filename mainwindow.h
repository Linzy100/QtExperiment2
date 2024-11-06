#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabel.h"
#include "finddialog.h"
#include "replacedialog.h"
#include <QTimer>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel *lengthLabel;      // 显示文本长度
    QLabel *linesLabel;       // 显示行数
    QLabel *positionLabel;    // 显示光标位置
    QLabel *statusLabel;      // 显示当前行和列
    QString currentFilePath; // 当前文件路径


private slots:
    void newFile();
    void openFile();
    void saveFile();
    void cutText();
    void copyText();
    void pasteText();
    void undoText();
    void redoText();
    void findTextShow();
    void about();
    void saveAs();         // 另存为功能
    void selectAll();      // 全选功能
    void toggleToolBar();  // 显示/隐藏工具栏
    void toggleStatusBar(); // 显示/隐藏状态栏
    void toggleWordWrap();      // 打开/关闭自动换行
    void changeFontSize();        // 设置字体大小
    void changeFontColor();       // 更改字体颜色
    void changeFontBackground();   // 更改字体背景色
    void changeEditorBackground(); // 更改编辑器背景色
    void updateStatusBar();
    void exitApp();
    void replaceText();              // 打开替换对话框
    void findNext(const QString &searchText, bool matchCase, bool searchUp);
    void replace(const QString &searchText, const QString &replaceText, bool matchCase); // 实际替换功能
    void autoSave(); // 新增自动保存槽

private:
    Ui::MainWindow *ui;
    FindDialog *findDialog; // 查找对话框指针
    ReplaceDialog *replaceDialog; // 替换对话框指针
    QTimer *autoSaveTimer; // 新增定时器
    void createActions();
    void createMenus();
    void createToolBars();
};

#endif // MAINWINDOW_H
/*#ifndef MAINWINDOW_H
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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
*/
