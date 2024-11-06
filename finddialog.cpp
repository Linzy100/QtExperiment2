#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog) {
    ui->setupUi(this);

    // 连接按钮点击信号
    connect(ui->pushButton, &QPushButton::clicked, this, &FindDialog::onFindNextButtonClicked); // 查找下一个
    connect(ui->pushButton_2, &QPushButton::clicked, this, &FindDialog::reject); // 取消
}

FindDialog::~FindDialog() {
    delete ui;
}

QString FindDialog::getSearchText() const {
    return ui->lineEdit->text(); // 获取输入框中的查找文本
}

bool FindDialog::isSearchUp() const {
    return ui->radioButton->isChecked(); // 是否选择“向上”
}

bool FindDialog::isMatchCase() const {
    return ui->checkBox->isChecked(); // 是否勾选“区分大小写”
}

void FindDialog::onFindNextButtonClicked() {
    emit findNext(getSearchText(), isMatchCase(), isSearchUp());
}


