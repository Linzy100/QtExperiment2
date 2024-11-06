#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog {
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

    QString getSearchText() const;      // 获取查找文本
    QString getReplaceText() const;     // 获取替换文本
    bool isMatchCase() const;           // 是否区分大小写
    bool isSearchUp() const;            // 查找方向是否向上

signals:
    void findNext(const QString &searchText, bool matchCase, bool searchUp);
    void replace(const QString &searchText, const QString &replaceText, bool matchCase);

private slots:
    void onFindNextButtonClicked();      // 查找下一个按钮槽
    void onReplaceButtonClicked();       // 替换按钮槽
    void onReplaceAllButtonClicked();    // 全部替换按钮槽

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H

