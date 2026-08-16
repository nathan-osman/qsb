/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

#include "categorydialog.hpp"

CategoryDialog::CategoryDialog(const QString &name, QWidget *parent)
    : QDialog(parent)
    , mName(name)
{
    setWindowTitle("Edit Category");
    resize(400, 150);

    QLineEdit *lineEditName = new QLineEdit;
    lineEditName->setText(name);
    connect(lineEditName, &QLineEdit::textChanged, this, [this](const QString &text) {
        mName = text;
    });

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Name:", lineEditName);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch(1);
    mainLayout->addWidget(buttonBox);
}