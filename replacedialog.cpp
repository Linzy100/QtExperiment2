#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog) {
    ui->setupUi(this);

    // 连接按钮信号
    connect(ui->pushButton, &QPushButton::clicked, this, &ReplaceDialog::onFindNextButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &ReplaceDialog::onReplaceButtonClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &ReplaceDialog::onReplaceAllButtonClicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &ReplaceDialog::reject); // 取消按钮
}

ReplaceDialog::~ReplaceDialog() {
    delete ui;
}

QString ReplaceDialog::getSearchText() const {
    return ui->lineEdit->text(); // 获取查找文本
}

QString ReplaceDialog::getReplaceText() const {
    return ui->lineEdit_2->text(); // 获取替换文本
}

bool ReplaceDialog::isMatchCase() const {
    return ui->checkBox->isChecked(); // 是否区分大小写
}

bool ReplaceDialog::isSearchUp() const {
    return ui->radioButton->isChecked(); // 是否选择向上
}

void ReplaceDialog::onFindNextButtonClicked() {
    emit findNext(getSearchText(), isMatchCase(), isSearchUp());
}

void ReplaceDialog::onReplaceButtonClicked() {
    emit replace(getSearchText(), getReplaceText(), isMatchCase());
}

void ReplaceDialog::onReplaceAllButtonClicked() {
    // In a full implementation, you would loop through the text and replace all occurrences
    emit replace(getSearchText(), getReplaceText(), isMatchCase());
}
