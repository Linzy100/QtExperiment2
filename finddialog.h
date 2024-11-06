#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog {
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

    QString getSearchText() const;      // 获取查找文本
    bool isMatchCase() const;           // 是否区分大小写
    bool isSearchUp() const;            // 查找方向是否向上

signals:
    void findNext(const QString &searchText, bool matchCase, bool searchUp);

private slots:
    void onFindNextButtonClicked();      // 查找下一个按钮槽

private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H



