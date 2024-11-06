#include "mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QRegularExpression>
#include <QInputDialog>
#include <QToolBar>
#include <QFontDialog>
#include <QColorDialog>
#include <QLabel>
#include <QTextCursor>
#include <QRegularExpression>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : currentFilePath(""),QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 初始化状态栏标签
    lengthLabel = new QLabel("length: 0");
    linesLabel = new QLabel("lines: 1");
    positionLabel = new QLabel("Pos: 0");
    statusLabel = new QLabel("Ln: 1, Col: 1");

    // 添加标签到状态栏
    statusBar()->addWidget(lengthLabel);
    statusBar()->addWidget(linesLabel);
    statusBar()->addWidget(statusLabel);
    statusBar()->addWidget(positionLabel);

    // 创建替换对话框并连接信号
    replaceDialog = new ReplaceDialog(this);
    connect(replaceDialog, &ReplaceDialog::findNext, this, &MainWindow::findNext);
    connect(replaceDialog, &ReplaceDialog::replace, this, &MainWindow::replace);

    findDialog=new FindDialog(this); // 创建查找对话框
    connect(findDialog, &FindDialog::findNext, this, &MainWindow::findNext);

    createActions();

    // 初始化定时器
    autoSaveTimer = new QTimer(this);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::autoSave);
    autoSaveTimer->start(5000); // 每5分钟（300000毫秒）自动保存一次
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createActions() {
    connect(ui->action_N_Ctrl_N, &QAction::triggered, this, &MainWindow::newFile);//新建
    connect(ui->action_O_Ctrl_O, &QAction::triggered, this, &MainWindow::openFile);//打开
    connect(ui->action_S_Ctrl_S, &QAction::triggered, this, &MainWindow::saveFile);//保存
    connect(ui->action_T_Ctrl_X, &QAction::triggered, this, &MainWindow::cutText);//剪切
    connect(ui->action_C_Ctrl_C, &QAction::triggered, this, &MainWindow::copyText);//复制
    connect(ui->action_V, &QAction::triggered, this, &MainWindow::pasteText);//黏贴
    connect(ui->action_U_Ctrl_Z, &QAction::triggered, this, &MainWindow::undoText);//撤销
    connect(ui->action_Ctrl_Shift_Z, &QAction::triggered, this, &MainWindow::redoText);//恢复
    connect(ui->action_F, &QAction::triggered, this, &MainWindow::findTextShow);//查找
    connect(ui->action_R, &QAction::triggered, this, &MainWindow::replaceText);//替换
    connect(ui->action_A_3, &QAction::triggered, this, &MainWindow::about);//关于
    connect(ui->action_A, &QAction::triggered, this, &MainWindow::saveAs); // 另存为
    connect(ui->action_A_2, &QAction::triggered, this, &MainWindow::selectAll); // 全选
    connect(ui->action_X, &QAction::triggered, this, &MainWindow::exitApp); // 退出
    connect(ui->action_T, &QAction::triggered, this, &MainWindow::toggleToolBar); // 切换工具栏
    connect(ui->action_S, &QAction::triggered, this, &MainWindow::toggleStatusBar); // 切换状态栏
    connect(ui->action_W, &QAction::triggered, this, &MainWindow::toggleWordWrap); // 自动换行
    connect(ui->action_F_2, &QAction::triggered, this, &MainWindow::changeFontSize); // 设置字体大小
    connect(ui->action_4, &QAction::triggered, this, &MainWindow::changeFontColor); // 更改字体颜色
    connect(ui->action_5, &QAction::triggered, this, &MainWindow::changeFontBackground); // 更改字体背景色
    connect(ui->action_6, &QAction::triggered, this, &MainWindow::changeEditorBackground); // 更改编辑器背景色
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::updateStatusBar);
    connect(ui->plainTextEdit, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);

}

// 创建新文件，将编辑器内容清空
void MainWindow::newFile() {
    ui->plainTextEdit->clear();
    currentFilePath.clear(); // 清空文件路径
    updateStatusBar(); // 更新状态栏
}

// 打开现有文件并显示内容
void MainWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(this, "打开文件");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            ui->plainTextEdit->setPlainText(in.readAll());
            file.close();
            currentFilePath = filename; // 更新文件路径
            setWindowTitle(filename);
            updateStatusBar(); // 更新状态栏
        } else {
            QMessageBox::warning(this, "错误", "无法打开文件");
        }
    }
}

// 保存当前文件内容
void MainWindow::saveFile() {
    if (!currentFilePath.isEmpty()) {
        // 保存到已存在的文件
        QFile file(currentFilePath);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out << ui->plainTextEdit->toPlainText();
            file.close();
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件");
        }
    } else {
        // 另存为
        saveAs();
    }
}

// 另存为功能
void MainWindow::saveAs() {
    QString filename = QFileDialog::getSaveFileName(this, "另存为");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out << ui->plainTextEdit->toPlainText();
            file.close();
            currentFilePath = filename; // 更新当前文件路径
            setWindowTitle(filename);
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件");
        }
    }
}


// 剪切选中的文本
void MainWindow::cutText() {
    if (ui->plainTextEdit->textCursor().hasSelection()) { // 检查是否有选中内容
        ui->plainTextEdit->cut();
    } else {
        QMessageBox::information(this, "提示", "没有选中的文本可以剪切。");
    }
}

// 复制选中的文本
void MainWindow::copyText() {
    if (ui->plainTextEdit->textCursor().hasSelection()) { // 检查是否有选中内容
        ui->plainTextEdit->copy();
    } else {
        QMessageBox::information(this, "提示", "没有选中的文本可以复制。");
    }
}

// 粘贴文本到光标位置
void MainWindow::pasteText() {
    ui->plainTextEdit->paste();
}

// 撤销上一个操作
void MainWindow::undoText() {
    ui->plainTextEdit->undo();
}

// 重做上一个操作
void MainWindow::redoText() {
    ui->plainTextEdit->redo();
}

// 查找文本功能
void MainWindow::findTextShow()
{
    findDialog->show();
}

void MainWindow::findNext(const QString &searchText, bool matchCase, bool searchUp) {
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    QString documentText = ui->plainTextEdit->toPlainText(); // 获取编辑器中的文本

    // 设置正则表达式选项
    QRegularExpression::PatternOptions options;
    options = matchCase ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption; // 区分大小写

    // 创建正则表达式
    QRegularExpression regex(searchText, options);

    if (searchUp) { // 向上查找
        if (cursor.position() == 0) {
            QMessageBox::information(this, "提示", "已经到达顶部，没有更多匹配项。");
            return;
        }

        QString textBeforeCursor = documentText.left(cursor.position()); // 获取光标之前的文本
        QRegularExpressionMatch match = regex.match(textBeforeCursor);

        if (match.hasMatch()) {
            int startPos = match.capturedStart(); // 获取匹配的开始位置
            int length = match.capturedLength(); // 获取匹配的长度
            cursor.setPosition(startPos); // 将光标移动到匹配的开始位置
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length); // 选中匹配项

            ui->plainTextEdit->setTextCursor(cursor); // 更新文本编辑器中的光标
            ui->plainTextEdit->ensureCursorVisible(); // 确保光标可见
        } else {
            QMessageBox::information(this, "提示", "未找到匹配项。");
        }
    } else { // 向下查找
        QRegularExpressionMatch match = regex.match(documentText, cursor.position());
        if (match.hasMatch()) {
            int startPos = match.capturedStart(); // 获取匹配的开始位置
            int length = match.capturedLength(); // 获取匹配的长度
            cursor.setPosition(startPos); // 将光标移动到匹配的开始位置
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length); // 选中匹配项

            ui->plainTextEdit->setTextCursor(cursor); // 更新文本编辑器中的光标
            ui->plainTextEdit->ensureCursorVisible(); // 确保光标可见
        } else {
            QMessageBox::information(this, "提示", "未找到匹配项。");
        }
    }
}


// 替换文本功能
void MainWindow::replaceText() {
    replaceDialog->show(); // 显示替换对话框
}

void MainWindow::replace(const QString &searchText, const QString &replaceText, bool matchCase) {
    // 获取当前光标
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    QString documentText = ui->plainTextEdit->toPlainText(); // 获取编辑器中的文本

    // 设置正则表达式选项
    QRegularExpression::PatternOptions options;
    options = matchCase ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption; // 区分大小写

    QRegularExpression regex(searchText, options);

    if (cursor.hasSelection()) {
        // 用户选中了文本，直接用选中内容替换
        cursor.insertText(replaceText);
    } else {
        // 从光标位置查找并替换
        cursor.beginEditBlock();
        int startPosition = 0; // 从文档开始位置
        while (true) {
            // 查找下一个匹配项
            cursor = ui->plainTextEdit->textCursor(); // 重置光标
            cursor.setPosition(startPosition); // 设置光标到上次匹配的起始位置

            // 在搜索区域中查找
            QRegularExpressionMatch match = regex.match(documentText, startPosition);
            if (match.hasMatch()) {
                // 找到匹配项，并替换其内容
                startPosition = match.capturedStart(); // 获取当前匹配的开始位置
                cursor.setPosition(startPosition);
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, match.capturedLength());
                cursor.insertText(replaceText); // 插入替换文本
                startPosition += replaceText.length(); // 更新下次查找起始位置
            } else {
                break; // 没有匹配，退出循环
            }
        }
        cursor.endEditBlock();

        // 更新文本框内容，重置光标
        ui->plainTextEdit->setTextCursor(cursor);
        ui->plainTextEdit->moveCursor(QTextCursor::Start); // 可选，移动光标到起始位置
    }
}


// 显示关于信息
void MainWindow::about() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec(); // 显示模态对话框
}

// 全选功能
void MainWindow::selectAll() {
    ui->plainTextEdit->selectAll(); // 选中所有文本
}

// 退出功能
void MainWindow::exitApp() {
    QApplication::quit(); // 退出应用程序
}

// 切换工具栏的显示状态
void MainWindow::toggleToolBar() {
    ui->toolBar->setVisible(!ui->toolBar->isVisible());
}

// 切换状态栏的显示状态
void MainWindow::toggleStatusBar() {
    ui->statusbar->setVisible(!ui->statusbar->isVisible());
}

// 打开/关闭自动换行功能
void MainWindow::toggleWordWrap() {
    // 检查当前的换行模式
    if (ui->plainTextEdit->lineWrapMode() == QPlainTextEdit::WidgetWidth) {
        // 如果当前是换行状态，则设置为不换行
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->action_W->setChecked(false); // 假设action_W是用于自动换行
    } else {
        // 设置为换行状态
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->action_W->setChecked(true); // 更新状态为被选中
    }
}

// 设置字体大小功能
void MainWindow::changeFontSize() {
    bool ok;
    int fontSize = QInputDialog::getInt(this, "设置字体大小", "字体大小:", 12, 1, 100, 1, &ok);
    if (ok) {
        QFont font = ui->plainTextEdit->font(); // 获取当前字体
        font.setPointSize(fontSize); // 设置新的字体大小
        ui->plainTextEdit->setFont(font); // 应用字体到文本编辑器
    }
}

// 更改字体颜色功能
void MainWindow::changeFontColor() {
    QColor color = QColorDialog::getColor(Qt::black, this, "选择字体颜色");
    if (color.isValid()) {
        QPalette p = ui->plainTextEdit->palette();
        p.setColor(QPalette::Text, color);
        ui->plainTextEdit->setPalette(p);
    }
}

// 更改字体背景色功能
void MainWindow::changeFontBackground() {
    QColor color = QColorDialog::getColor(Qt::white, this, "选择字体背景颜色");
    if (color.isValid()) {
        // 当文本被选中时，应用背景色
        QTextCursor cursor = ui->plainTextEdit->textCursor();
        if (!cursor.hasSelection()) {
            cursor.select(QTextCursor::Document); // 如果没有选择，则选中全部
        }
        QTextCharFormat format;
        format.setBackground(color);
        cursor.mergeCharFormat(format);
        ui->plainTextEdit->setTextCursor(cursor); // 重新设置光标
    }
}

// 更改编辑器背景颜色功能
void MainWindow::changeEditorBackground() {
    QColor color = QColorDialog::getColor(Qt::white, this, "选择编辑器背景颜色");
    if (color.isValid()) {
        QPalette p = ui->plainTextEdit->palette();
        p.setColor(QPalette::Base, color);
        ui->plainTextEdit->setPalette(p);
    }
}

void MainWindow::updateStatusBar() {
    QString text = ui->plainTextEdit->toPlainText();
    lengthLabel->setText(QString("length: %1").arg(text.length()));
    linesLabel->setText(QString("lines: %1").arg(text.count('\n') + 1)); // 计算行数

    QTextCursor cursor = ui->plainTextEdit->textCursor();
    positionLabel->setText(QString("Pos: %1").arg(cursor.position()));
    statusLabel->setText(QString("Ln: %1, Col: %2").arg(cursor.blockNumber() + 1).arg(cursor.columnNumber() + 1));
}

// 自动保存方法
void MainWindow::autoSave() {
    if (!currentFilePath.isEmpty()) {
        saveFile(); // 调用现有保存方法
    }
}
